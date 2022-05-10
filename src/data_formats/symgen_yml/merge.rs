//! Merging data into `resymgen` YAML structs.
//!
//! This is a separate, mostly private module because (for efficiency) raw merge operations leave
//! internal initialization/ordering in inconsistent state, which needs to be corrected explicitly
//! through reinitialization. However, the publicly exported utilities are safe.

use std::borrow::Cow;
use std::collections::HashMap;
use std::error::Error;
use std::fmt::{self, Debug, Display, Formatter};

use super::adapter::{AddSymbol, SymbolType};
use super::bounds;
use super::error::MergeError;
use super::symgen::*;
use super::types::*;

/// A conflict encountered while merging.
#[derive(Debug)]
pub struct MergeConflict {
    /// Description of the parent object being merged into.
    parent_desc: String,
    /// Description of the object being merged.
    other_desc: String,
    /// Nested [`MergeConflict`], if the conflict arises from more primitive inner structures.
    inner: Option<Box<MergeConflict>>,
}

impl MergeConflict {
    fn new<S: ToString>(parent_desc: S, other_desc: S) -> Self {
        Self {
            parent_desc: parent_desc.to_string(),
            other_desc: other_desc.to_string(),
            inner: None,
        }
    }

    /// Wraps a [`Result<T, MergeConflict>`] in another [`MergeConflict`] with a description
    /// `desc`, if an [`Err`].
    fn wrap<T, S: ToString>(res: Result<T, MergeConflict>, desc: S) -> Result<T, MergeConflict> {
        match res {
            Ok(x) => Ok(x),
            Err(inner) => Err(MergeConflict {
                parent_desc: desc.to_string(),
                other_desc: desc.to_string(),
                inner: Some(Box::new(inner)),
            }),
        }
    }
}

impl Error for MergeConflict {}

impl Display for MergeConflict {
    fn fmt(&self, f: &mut Formatter) -> fmt::Result {
        match &self.inner {
            None => write!(
                f,
                "could not merge '{}' with '{}'",
                self.parent_desc, self.other_desc
            ),
            Some(inner) => {
                if self.parent_desc == self.other_desc {
                    write!(f, "{}: {}", self.parent_desc, inner)
                } else {
                    write!(f, "({}, {}): {}", self.parent_desc, self.other_desc, inner)
                }
            }
        }
    }
}

/// An error encountered when attempting to merge into a [`Block`] that doesn't exist in the
/// parent `SymGen`.
#[derive(Debug)]
pub struct MissingBlock {
    block_name: String,
}

impl Error for MissingBlock {}

impl Display for MissingBlock {
    fn fmt(&self, f: &mut Formatter) -> fmt::Result {
        write!(f, "no block with name \"{}\"", self.block_name)
    }
}

/// An error encountered when there is ambiguity in which [`Block`] to merge an [`AddSymbol`] into,
/// i.e. there are multiple different [`Block`]s that could work.
#[derive(Debug)]
pub struct BlockInferenceError {
    symbol_name: String,
    /// List of names of all blocks that an [`AddSymbol`] could be merged into.
    matching_blocks: Vec<String>,
}

impl Error for BlockInferenceError {}

impl Display for BlockInferenceError {
    fn fmt(&self, f: &mut Formatter) -> fmt::Result {
        if self.matching_blocks.is_empty() {
            write!(
                f,
                "no matching blocks found for symbol \"{}\"",
                self.symbol_name
            )
        } else {
            write!(
                f,
                "more than one matching block found for symbol \"{}\": {}",
                self.symbol_name,
                self.matching_blocks.join(", ")
            )
        }
    }
}

/// A type that can be merged with another instance of the same type.
trait Merge {
    /// Merge `other` into `self`.
    fn merge(&mut self, other: &Self) -> Result<(), MergeConflict>;
}

impl Merge for Uint {
    fn merge(&mut self, other: &Self) -> Result<(), MergeConflict> {
        if self == other {
            Ok(())
        } else {
            Err(MergeConflict::new(
                format!("{:#X}", self),
                format!("{:#X}", other),
            ))
        }
    }
}

impl Merge for Linkable {
    fn merge(&mut self, other: &Self) -> Result<(), MergeConflict> {
        if let Self::Single(x) = self {
            if let Self::Single(y) = other {
                if x != y {
                    // Might as well save a separate push since we already know the outcome
                    *self = Self::Multiple(vec![*x, *y]);
                }
                return Ok(());
            }
            *self = Self::Multiple(vec![*x]);
        }

        if let Self::Multiple(v) = self {
            for y in other.iter() {
                if !v.contains(y) {
                    v.push(*y);
                }
            }
            Ok(())
        } else {
            // This should never happen
            panic!("Merging into Linkable::Single?")
        }
    }
}

impl<T> Merge for VersionDep<T>
where
    T: Merge + Debug + Clone + 'static,
{
    fn merge(&mut self, other: &Self) -> Result<(), MergeConflict> {
        // Map version names to the actual Version keys
        let mut versions_by_name: HashMap<_, _> = self
            .versions()
            .map(|v| (v.name().to_owned(), v.clone()))
            .collect();
        for (v, x) in other.iter() {
            match versions_by_name.get(v.name()) {
                // This version key already exists (by name), so try to merge the inner value
                Some(vers) => {
                    MergeConflict::wrap(self.get_mut_native(vers).unwrap().merge(x), v.name())?
                }
                None => {
                    // This version key is new, so insert it
                    self.insert_native(v.clone(), x.clone());
                    versions_by_name.insert(v.name().to_owned(), v.clone());
                }
            }
        }
        Ok(())
    }
}

impl<T> Merge for MaybeVersionDep<T>
where
    T: Merge + PartialEq + Debug + Clone + 'static,
{
    fn merge(&mut self, other: &Self) -> Result<(), MergeConflict> {
        match self {
            Self::Common(x) => {
                match other {
                    Self::Common(y) => {
                        // Directly pass up the error without wrapping since this is Common
                        x.merge(y)?;
                    }
                    Self::ByVersion(other_vals) => {
                        let mut values = other_vals.values();
                        let first = values.next();
                        // If other is empty, there's nothing to do
                        if let Some(y) = first {
                            if values.all(|x| x == y) {
                                // If all the by-version values are actually equal,
                                // we can just keep self as Common to preserve generality.
                                // All we need to do here is merge the shared value from other,
                                // as if it were Common.
                                x.merge(y)?;
                            } else {
                                // If the versions are actually different, swap self/other
                                // and call into the ByVersion <- Common code path.
                                let mut new_self = other.clone();
                                new_self.merge(self)?;
                                *self = new_self;
                            }
                        }
                    }
                }
            }
            Self::ByVersion(vals) => {
                match other {
                    Self::Common(y) => {
                        // This behavior is slightly asymmetric. For Common <- ByVersion we try to
                        // preserve self as Common if other doesn't have any version differences.
                        // Here (ByVersion <- Common), since self is already versioned, never
                        // convert back to Common, and always just realize other before merging.
                        for (vers, x) in vals.iter_mut() {
                            MergeConflict::wrap(x.merge(y), vers.name())?;
                        }
                        // The merge was successful, but since version inference was involved,
                        // print a warning. This case shouldn't be common for merges anyway.
                        eprintln!(
                            "Warning: merging by-version values with common (unversioned) values \
                                expands the common values by inference, and might cause some \
                                implicit versions to be lost."
                        );
                    }
                    Self::ByVersion(other_vals) => {
                        // Directly pass up the error without wrapping since VersionDep already
                        // handles version keys on merge failure.
                        vals.merge(other_vals)?;
                    }
                }
            }
        }
        Ok(())
    }
}

/// Returns a possibly truncated substring of `s`. If truncated, the "..." suffix will be appended
/// to indicate continuation.
fn truncate(s: &str) -> String {
    const MAX_LENGTH: usize = 100;
    if s.chars().count() <= MAX_LENGTH {
        s.to_string()
    } else {
        format!(
            "{}...",
            s.chars()
                .take(MAX_LENGTH)
                .fold(String::with_capacity(MAX_LENGTH), |mut acc, c| {
                    acc.push(c);
                    acc
                })
        )
    }
}

impl Merge for Symbol {
    fn merge(&mut self, other: &Self) -> Result<(), MergeConflict> {
        if self.name != other.name {
            return Err(MergeConflict::new(&self.name, &other.name));
        }
        if let Some(other_desc) = &other.description {
            match &mut self.description {
                Some(self_desc) => {
                    if self_desc != other_desc {
                        return MergeConflict::wrap(
                            Err(MergeConflict::new(
                                truncate(self_desc),
                                truncate(other_desc),
                            )),
                            "description",
                        );
                    }
                }
                None => self.description = Some(other_desc.clone()),
            };
        }
        MergeConflict::wrap(self.address.merge(&other.address), "address")?;
        if let Some(other_len) = &other.length {
            match &mut self.length {
                None => self.length = Some(other_len.clone()),
                Some(len) => MergeConflict::wrap(len.merge(other_len), "length")?,
            };
        }
        Ok(())
    }
}

impl Merge for SymbolList {
    fn merge(&mut self, other: &Self) -> Result<(), MergeConflict> {
        let mut name_to_idx: HashMap<_, _> = self
            .iter()
            .enumerate()
            .map(|(i, s)| (s.name.clone(), i))
            .collect();
        for symbol in other.iter() {
            match name_to_idx.get(&symbol.name) {
                Some(&i) => unsafe {
                    // Never out of bounds since it comes from the list, which never shrinks
                    MergeConflict::wrap(self.get_unchecked_mut(i).merge(symbol), &symbol.name)?;
                },
                None => {
                    self.push(symbol.clone());
                    name_to_idx.insert(symbol.name.clone(), self.len() - 1);
                }
            }
        }
        Ok(())
    }
}

impl Merge for Block {
    fn merge(&mut self, other: &Self) -> Result<(), MergeConflict> {
        if let Some(other_desc) = &other.description {
            match &mut self.description {
                Some(self_desc) => {
                    if self_desc != other_desc {
                        return MergeConflict::wrap(
                            Err(MergeConflict::new(
                                truncate(self_desc),
                                truncate(other_desc),
                            )),
                            "description",
                        );
                    }
                }
                None => self.description = Some(other_desc.clone()),
            };
        }

        if let Some(versions) = &self.versions {
            // If other has fields that might have different versions than self, make sure to
            // expand self to be ByVersion as well, so that self's versions can't be inadvertently
            // lost while merging. However, it's not as easy to do this generally for the
            // SymbolLists, so these might lead to warning messages if self's symbols have Common
            // fields and the user is relying on version inference.
            if self.versions != other.versions {
                self.address.expand_versions(versions);
            }
            if self.versions != other.versions {
                self.length.expand_versions(versions);
            }
        }

        if let Some(other_versions) = &other.versions {
            match &mut self.versions {
                Some(vers) => {
                    for ov in other_versions {
                        if !vers.iter().any(|v| v.name() == ov.name()) {
                            // New version; add it to self
                            vers.push(ov.clone());
                        }
                    }
                }
                None => self.versions = Some(other_versions.clone()),
            }
        }

        let other_address;
        let other_length;
        let mut other_functions = Cow::Borrowed(&other.functions);
        let mut other_data = Cow::Borrowed(&other.data);
        if let Some(other_versions) = &other.versions {
            // Realize any Common values in other using other.versions
            other_address = Cow::Owned(MaybeVersionDep::ByVersion(
                other.address.by_version(other_versions),
            ));
            other_length = Cow::Owned(MaybeVersionDep::ByVersion(
                other.length.by_version(other_versions),
            ));
            other_functions.to_mut().expand_versions(other_versions);
            other_data.to_mut().expand_versions(other_versions);
        } else if let Some(versions) = &self.versions {
            // If other doesn't have a version list, try using self.versions as a fallback
            other_address = Cow::Owned(MaybeVersionDep::ByVersion(
                other.address.by_version(versions),
            ));
            other_length = Cow::Owned(MaybeVersionDep::ByVersion(
                other.length.by_version(versions),
            ));
            other_functions.to_mut().expand_versions(versions);
            other_data.to_mut().expand_versions(versions);
        } else {
            // No other options; use other's fields as is
            // If other doesn't have a version list, try using self.versions as a fallback
            other_address = Cow::Borrowed(&other.address);
            other_length = Cow::Borrowed(&other.length);
        }
        MergeConflict::wrap(self.address.merge(&other_address), "address")?;
        MergeConflict::wrap(self.length.merge(&other_length), "length")?;

        MergeConflict::wrap(self.functions.merge(&other_functions), "functions")?;
        MergeConflict::wrap(self.data.merge(&other_data), "data")?;
        Ok(())
    }
}

impl Merge for SymGen {
    fn merge(&mut self, other: &Self) -> Result<(), MergeConflict> {
        for (bname, block) in other.iter() {
            // Ensure the block exists
            match self.block_key(&bname.val).cloned() {
                Some(bkey) => {
                    MergeConflict::wrap(
                        self.get_mut(&bkey)
                            .map(|b| b.merge(block))
                            .unwrap_or(Ok(())),
                        bname,
                    )?;
                }
                None => {
                    self.insert(bname.clone(), block.clone());
                }
            }
        }
        // Reinit because merging can introduce new OrdStrings/Versions
        self.init();
        Ok(())
    }
}

/// For [`SymbolList`] lookup and insertion, managed by a cache that stores
/// index by block name, by symbol type, by symbol name.
struct SymbolManager(HashMap<String, HashMap<SymbolType, HashMap<String, usize>>>);

impl SymbolManager {
    fn new() -> Self {
        Self(HashMap::new())
    }

    /// Gets a mutable reference to a [`Symbol`] from within `slist`, with the given
    /// cache key (`block_name`, `symbol_type`, `symbol_name`), if present in `slist`.
    /// If the symbol is present but not within the [`SymbolManager`] cache, it will be
    /// added the cache for future lookups.
    fn get<'s>(
        &mut self,
        slist: &'s mut SymbolList,
        block_name: &str,
        symbol_type: &SymbolType,
        symbol_name: &str,
    ) -> Option<&'s mut Symbol> {
        let idx = if let Some(imap) = self.0.get(block_name).and_then(|m| m.get(symbol_type)) {
            imap.get(symbol_name).copied()
        } else {
            let imap = slist.iter().enumerate().fold(
                HashMap::with_capacity(slist.len()),
                |mut m, (i, x)| {
                    // Give the first appearance of a symbol name precedence
                    if !m.contains_key(&x.name) {
                        m.insert(x.name.clone(), i);
                    }
                    m
                },
            );
            let i = imap.get(symbol_name).copied();
            self.0
                .entry(block_name.to_owned())
                .or_insert_with(HashMap::new)
                .entry(*symbol_type)
                .or_insert(imap);
            i
        };
        idx.map(move |i| unsafe { slist.get_unchecked_mut(i) })
    }

    /// Appends `symbol` onto `slist`, and update the [`SymbolManager`] cache with a given
    /// `block_name` and `symbol_type`.
    fn insert(
        &mut self,
        slist: &mut SymbolList,
        block_name: &str,
        symbol_type: &SymbolType,
        symbol: Symbol,
    ) {
        if let Some(imap) = self
            .0
            .get_mut(block_name)
            .and_then(|m| m.get_mut(symbol_type))
        {
            imap.insert(symbol.name.clone(), slist.len());
        }
        slist.push(symbol);
    }
}

impl SymGen {
    /// Merges `other` into `self`.
    pub fn merge_symgen(&mut self, other: &Self) -> Result<(), MergeError> {
        self.merge(other).map_err(MergeError::Conflict)
    }
    /// Merges `other` into `self`.
    ///
    /// Returns a `Vec<Symbol>` containing symbols that were not successfully merged if no
    /// fatal error was encountered, or a [`MergeError`] if a fatal error was encountered.
    pub fn merge_symbols<I>(&mut self, other: I) -> Result<Vec<Symbol>, MergeError>
    where
        I: Iterator<Item = AddSymbol>,
    {
        let mut unmerged_symbols = Vec::new();
        let mut sym_manager = SymbolManager::new();
        for to_add in other {
            let (bname, block) = match &to_add.block_name {
                Some(name) => match self.block_key(name) {
                    // A block name was explicitly specified, so retrieve it
                    Some(bkey) => {
                        let key = bkey.clone();
                        (name, self.get_mut(&key).unwrap())
                    }
                    None => {
                        return Err(MergeError::MissingBlock(MissingBlock {
                            block_name: name.clone(),
                        }))
                    }
                },
                None => {
                    // No block name; try to infer it based on the symbol address
                    let mut first_match: Option<_> = None;
                    // Set this vector only if there's more than one match. Collect all the
                    // matching block names in this case.
                    let mut all_matching_blocks: Option<Vec<String>> = None;
                    for (bname, block) in self.iter_mut() {
                        if bounds::block_contains(block, &to_add.symbol) {
                            match first_match.as_ref() {
                                None => first_match = Some((&bname.val, block)),
                                Some(m) => {
                                    let mut new = all_matching_blocks
                                        .take()
                                        .unwrap_or_else(|| vec![m.0.clone()]);
                                    new.push(bname.val.clone());
                                    all_matching_blocks = Some(new);
                                }
                            }
                        }
                    }

                    match first_match {
                        Some(inferred) => {
                            if let Some(matching_blocks) = all_matching_blocks {
                                // It's ambiguous which block to merge into, so error out
                                return Err(MergeError::BlockInference(BlockInferenceError {
                                    symbol_name: to_add.symbol.name,
                                    matching_blocks,
                                }));
                            }
                            inferred
                        }
                        None => {
                            // This symbol doesn't fit in any of the blocks, so skip it
                            unmerged_symbols.push(to_add.symbol);
                            continue;
                        }
                    }
                }
            };

            let slist = match to_add.stype {
                SymbolType::Function => &mut block.functions,
                SymbolType::Data => &mut block.data,
            };
            let sym = sym_manager.get(slist, bname, &to_add.stype, &to_add.symbol.name);
            match sym {
                Some(s) => {
                    let to_merge = if let Some(vers) = &block.versions {
                        let mut cpy = to_add.symbol.clone();
                        cpy.expand_versions(vers);
                        Cow::Owned(cpy)
                    } else {
                        Cow::Borrowed(&to_add.symbol)
                    };
                    s.merge(&to_merge).map_err(MergeError::Conflict)?;
                }
                None => sym_manager.insert(slist, bname, &to_add.stype, to_add.symbol.clone()),
            };
        }
        // Reinit because merging can introduce new OrdStrings/Versions
        self.init();
        Ok(unmerged_symbols)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_merge_uint() {
        let mut x: Uint = 1;
        assert!(x.merge(&1).is_ok());
        assert_eq!(x, 1);
        assert!(x.merge(&2).is_err());
    }

    #[test]
    fn test_merge_linkable() {
        let mut x = Linkable::from([1, 2, 3]);
        assert!(x.merge(&[5, 3, 4].into()).is_ok());
        assert_eq!(x, [1, 2, 3, 5, 4].into());
    }

    #[test]
    fn test_merge_version_dep() {
        let mut x = VersionDep::from([("v1".into(), 1), ("v2".into(), 2)]);
        assert!(x
            .merge(&[("v1".into(), 1), ("v3".into(), 3)].into())
            .is_ok());
        assert_eq!(
            &x,
            &[("v1".into(), 1), ("v2".into(), 2), ("v3".into(), 3)].into()
        );
        assert!(x.merge(&[("v1".into(), 2)].into()).is_err())
    }

    #[test]
    fn test_merge_version_dep_same_version_different_ordinal() {
        let mut x = VersionDep::from([(("v1", 0).into(), 1), (("v2", 1).into(), 2)]);
        assert!(x
            .merge(&[(("v1", 2).into(), 1), (("v3", 1).into(), 3)].into())
            .is_ok());
        assert_eq!(
            &x,
            &[
                (("v1", 0).into(), 1),
                (("v2", 1).into(), 2),
                (("v3", 1).into(), 3)
            ]
            .into()
        );
    }

    #[test]
    fn test_merge_maybe_version_dep_common() {
        let mut x = MaybeVersionDep::Common(1);
        assert!(x.merge(&MaybeVersionDep::Common(1)).is_ok());
        assert_eq!(&x, &MaybeVersionDep::Common(1));
    }

    #[test]
    fn test_merge_maybe_version_dep_by_version() {
        let mut x = MaybeVersionDep::ByVersion([("v1".into(), 1), ("v2".into(), 2)].into());
        assert!(x
            .merge(&MaybeVersionDep::ByVersion(
                [("v1".into(), 1), ("v3".into(), 3)].into()
            ))
            .is_ok());
        assert_eq!(
            &x,
            &MaybeVersionDep::ByVersion(
                [("v1".into(), 1), ("v2".into(), 2), ("v3".into(), 3)].into()
            )
        );
    }

    #[test]
    fn test_merge_maybe_version_dep_common_into_by_version() {
        let mut x = MaybeVersionDep::ByVersion([("v1".into(), 1), ("v2".into(), 1)].into());
        assert!(x.merge(&MaybeVersionDep::Common(1)).is_ok());
        assert_eq!(
            &x,
            &MaybeVersionDep::ByVersion([("v1".into(), 1), ("v2".into(), 1)].into())
        );

        let mut y = MaybeVersionDep::ByVersion([("v1".into(), 1), ("v2".into(), 2)].into());
        assert!(y.merge(&MaybeVersionDep::Common(1)).is_err());

        let mut z: MaybeVersionDep<Linkable> = MaybeVersionDep::ByVersion(
            [("v1".into(), [1].into()), ("v2".into(), [2].into())].into(),
        );
        assert!(z.merge(&MaybeVersionDep::Common([3].into())).is_ok());
        assert_eq!(
            &z,
            &MaybeVersionDep::ByVersion(
                [("v1".into(), [1, 3].into()), ("v2".into(), [2, 3].into())].into()
            )
        );
    }

    #[test]
    fn test_merge_maybe_version_dep_by_version_into_common() {
        let mut x = MaybeVersionDep::Common(1);
        assert!(x
            .merge(&MaybeVersionDep::ByVersion(
                [("v1".into(), 1), ("v2".into(), 1)].into()
            ))
            .is_ok());
        assert_eq!(&x, &MaybeVersionDep::Common(1));

        let mut y = MaybeVersionDep::Common(1);
        assert!(y
            .merge(&MaybeVersionDep::ByVersion(
                [("v1".into(), 1), ("v2".into(), 2)].into()
            ))
            .is_err());

        let mut z: MaybeVersionDep<Linkable> = MaybeVersionDep::Common([3].into());
        assert!(z
            .merge(&MaybeVersionDep::ByVersion(
                [("v1".into(), [1].into()), ("v2".into(), [2].into())].into(),
            ))
            .is_ok());
        assert_eq!(
            &z,
            &MaybeVersionDep::ByVersion(
                [("v1".into(), [1, 3].into()), ("v2".into(), [2, 3].into())].into()
            )
        );
    }

    #[test]
    fn test_merge_symbol() {
        let mut x = Symbol {
            name: "function".to_string(),
            address: MaybeVersionDep::ByVersion(
                [("v1".into(), 1.into()), ("v2".into(), 2.into())].into(),
            ),
            length: None,
            description: None,
        };
        assert!(x
            .merge(&Symbol {
                name: "function".to_string(),
                address: MaybeVersionDep::ByVersion([("v3".into(), 3.into())].into()),
                length: Some(MaybeVersionDep::Common(5)),
                description: Some("desc".to_string()),
            })
            .is_ok());
        assert_eq!(
            &x,
            &Symbol {
                name: "function".to_string(),
                address: MaybeVersionDep::ByVersion(
                    [
                        ("v1".into(), 1.into()),
                        ("v2".into(), 2.into()),
                        ("v3".into(), 3.into())
                    ]
                    .into()
                ),
                length: Some(MaybeVersionDep::Common(5)),
                description: Some("desc".to_string()),
            }
        );

        assert!(x
            .merge(&Symbol {
                name: "function".to_string(),
                address: MaybeVersionDep::ByVersion([("v3".into(), 3.into())].into()),
                length: None,
                description: Some("other desc".to_string()),
            })
            .is_err())
    }

    #[test]
    fn test_merge_symbol_list() {
        let mut x = SymbolList::from([
            Symbol {
                name: "function1".to_string(),
                address: MaybeVersionDep::Common(1.into()),
                length: None,
                description: None,
            },
            Symbol {
                name: "function2".to_string(),
                address: MaybeVersionDep::Common(2.into()),
                length: None,
                description: None,
            },
        ]);
        assert!(x
            .merge(&SymbolList::from([
                Symbol {
                    name: "function3".to_string(),
                    address: MaybeVersionDep::Common(3.into()),
                    length: None,
                    description: None,
                },
                Symbol {
                    name: "function2".to_string(),
                    address: MaybeVersionDep::Common(4.into()),
                    length: None,
                    description: Some("desc".to_string()),
                },
            ]))
            .is_ok());
        assert_eq!(
            &x,
            &SymbolList::from([
                Symbol {
                    name: "function1".to_string(),
                    address: MaybeVersionDep::Common(1.into()),
                    length: None,
                    description: None,
                },
                Symbol {
                    name: "function2".to_string(),
                    address: MaybeVersionDep::Common([2, 4].into()),
                    length: None,
                    description: Some("desc".to_string()),
                },
                Symbol {
                    name: "function3".to_string(),
                    address: MaybeVersionDep::Common(3.into()),
                    length: None,
                    description: None,
                },
            ])
        );
    }

    #[test]
    fn test_merge_block() {
        let mut x = Block {
            versions: Some(vec!["v1".into()]),
            address: MaybeVersionDep::ByVersion([("v1".into(), 1)].into()),
            length: MaybeVersionDep::ByVersion([("v1".into(), 10)].into()),
            description: None,
            functions: [Symbol {
                name: "function1".to_string(),
                address: MaybeVersionDep::ByVersion([("v1".into(), 1.into())].into()),
                length: None,
                description: None,
            }]
            .into(),
            data: [].into(),
        };
        assert!(x
            .merge(&Block {
                versions: Some(vec!["v2".into(), "v1".into()]),
                address: MaybeVersionDep::ByVersion([("v2".into(), 2)].into()),
                length: MaybeVersionDep::ByVersion([("v1".into(), 10)].into()),
                description: Some("desc".to_string()),
                functions: [Symbol {
                    name: "function2".to_string(),
                    address: MaybeVersionDep::ByVersion([("v2".into(), 1.into())].into()),
                    length: None,
                    description: None,
                }]
                .into(),
                data: [Symbol {
                    name: "data".to_string(),
                    address: MaybeVersionDep::ByVersion([("v1".into(), 1.into())].into()),
                    length: None,
                    description: None,
                }]
                .into()
            })
            .is_ok());
        assert_eq!(
            &x,
            &Block {
                versions: Some(vec!["v1".into(), "v2".into()]),
                address: MaybeVersionDep::ByVersion([("v1".into(), 1), ("v2".into(), 2)].into()),
                length: MaybeVersionDep::ByVersion([("v1".into(), 10)].into()),
                description: Some("desc".to_string()),
                functions: [
                    Symbol {
                        name: "function1".to_string(),
                        address: MaybeVersionDep::ByVersion([("v1".into(), 1.into())].into()),
                        length: None,
                        description: None,
                    },
                    Symbol {
                        name: "function2".to_string(),
                        address: MaybeVersionDep::ByVersion([("v2".into(), 1.into())].into()),
                        length: None,
                        description: None,
                    },
                ]
                .into(),
                data: [Symbol {
                    name: "data".to_string(),
                    address: MaybeVersionDep::ByVersion([("v1".into(), 1.into())].into()),
                    length: None,
                    description: None,
                }]
                .into()
            }
        );
    }

    #[test]
    fn test_merge_block_expand_versions() {
        let mut x = Block {
            versions: Some(vec!["v1".into()]),
            address: MaybeVersionDep::Common(1),
            length: MaybeVersionDep::Common(10),
            description: None,
            functions: [Symbol {
                name: "function1".to_string(),
                address: MaybeVersionDep::ByVersion([("v1".into(), 1.into())].into()),
                length: None,
                description: None,
            }]
            .into(),
            data: [].into(),
        };
        assert!(x
            .merge(&Block {
                versions: Some(vec!["v2".into()]),
                address: MaybeVersionDep::Common(2),
                length: MaybeVersionDep::Common(3),
                description: None,
                functions: [
                    Symbol {
                        name: "function1".to_string(),
                        address: MaybeVersionDep::Common(1.into()),
                        length: None,
                        description: None,
                    },
                    Symbol {
                        name: "function2".to_string(),
                        address: MaybeVersionDep::Common(1.into()),
                        length: None,
                        description: None,
                    },
                ]
                .into(),
                data: [Symbol {
                    name: "data".to_string(),
                    address: MaybeVersionDep::Common(1.into()),
                    length: None,
                    description: None,
                }]
                .into()
            })
            .is_ok());
        assert_eq!(
            &x,
            &Block {
                versions: Some(vec!["v1".into(), "v2".into()]),
                address: MaybeVersionDep::ByVersion([("v1".into(), 1), ("v2".into(), 2)].into()),
                length: MaybeVersionDep::ByVersion([("v1".into(), 10), ("v2".into(), 3)].into()),
                description: None,
                functions: [
                    Symbol {
                        name: "function1".to_string(),
                        address: MaybeVersionDep::ByVersion(
                            [("v1".into(), 1.into()), ("v2".into(), 1.into())].into()
                        ),
                        length: None,
                        description: None,
                    },
                    Symbol {
                        name: "function2".to_string(),
                        address: MaybeVersionDep::ByVersion([("v2".into(), 1.into())].into()),
                        length: None,
                        description: None,
                    },
                ]
                .into(),
                data: [Symbol {
                    name: "data".to_string(),
                    address: MaybeVersionDep::ByVersion([("v2".into(), 1.into())].into()),
                    length: None,
                    description: None,
                }]
                .into()
            }
        );
    }

    fn get_simple_symgen() -> SymGen {
        SymGen::read(
            r#"
            main:
              versions:
                - v1
              address:
                v1: 0x2000000
              length:
                v1: 0x100000
              description: foo
              functions:
                - name: fn1
                  address:
                    v1: 0x2001000
                  description: bar
              data: []
            "#
            .as_bytes(),
        )
        .unwrap()
    }

    #[test]
    fn test_merge_symgen() {
        let mut x = get_simple_symgen();
        assert!(x
            .merge(
                &SymGen::read(
                    r#"
                    main:
                      versions:
                        - v1
                        - v2
                      address:
                        v1: 0x2000000
                        v2: 0x2000000
                      length:
                        v1: 0x100000
                        v2: 0x100004
                      description: foo
                      functions:
                        - name: fn1
                          address:
                            v1: 0x2001000
                            v2: 0x2002000
                          length:
                            v1: 0x1000
                            v2: 0x1000
                        - name: fn2
                          address:
                            v1:
                              - 0x2002000
                              - 0x2003000
                            v2: 0x2003000
                          description: baz
                      data:
                        - name: SOME_DATA
                          address:
                            v1: 0x2000000
                            v2: 0x2000000
                          length:
                            v1: 0x1000
                            v2: 0x2000
                          description: foo bar baz
                    other:
                      address: 0x2100000
                      length: 0x100000
                      functions:
                        - name: fn3
                          address: 0x2100000
                      data: []
                    "#
                    .as_bytes(),
                )
                .expect("Read failed")
            )
            .is_ok());
        assert_eq!(
            &x,
            &SymGen::read(
                r#"
                main:
                  versions:
                    - v1
                    - v2
                  address:
                    v1: 0x2000000
                    v2: 0x2000000
                  length:
                    v1: 0x100000
                    v2: 0x100004
                  description: foo
                  functions:
                    - name: fn1
                      address:
                        v1: 0x2001000
                        v2: 0x2002000
                      length:
                        v1: 0x1000
                        v2: 0x1000
                      description: bar
                    - name: fn2
                      address:
                        v1:
                          - 0x2002000
                          - 0x2003000
                        v2: 0x2003000
                      description: baz
                  data:
                    - name: SOME_DATA
                      address:
                        v1: 0x2000000
                        v2: 0x2000000
                      length:
                        v1: 0x1000
                        v2: 0x2000
                      description: foo bar baz
                other:
                  address: 0x2100000
                  length: 0x100000
                  functions:
                    - name: fn3
                      address: 0x2100000
                  data: []
                "#
                .as_bytes(),
            )
            .expect("Read failed")
        );
    }

    // Returns initial SymGen, AddSymbol list, and expected SymGen after merge
    fn get_merge_symbols_data() -> (SymGen, Vec<AddSymbol>, SymGen) {
        (
            get_simple_symgen(),
            vec![
                AddSymbol {
                    symbol: Symbol {
                        name: "fn1".to_string(),
                        address: MaybeVersionDep::ByVersion(
                            [("v1".into(), 0x2002000.into())].into(),
                        ),
                        length: None,
                        description: None,
                    },
                    stype: SymbolType::Function,
                    block_name: Some("main".to_string()),
                },
                AddSymbol {
                    symbol: Symbol {
                        name: "SOME_DATA".to_string(),
                        address: MaybeVersionDep::ByVersion(
                            [("v1".into(), 0x2003000.into())].into(),
                        ),
                        length: None,
                        description: None,
                    },
                    stype: SymbolType::Data,
                    block_name: Some("main".to_string()),
                },
            ],
            SymGen::read(
                r#"
                main:
                  versions:
                    - v1
                  address:
                    v1: 0x2000000
                  length:
                    v1: 0x100000
                  description: foo
                  functions:
                    - name: fn1
                      address:
                        v1:
                          - 0x2001000
                          - 0x2002000
                      description: bar
                  data:
                    - name: SOME_DATA
                      address:
                        v1: 0x2003000
                "#
                .as_bytes(),
            )
            .expect("Read failed"),
        )
    }

    #[test]
    fn test_merge_symbols_from_iter() {
        let (mut x, add_symbols, expected) = get_merge_symbols_data();
        let res = x.merge_symbols(Box::new(add_symbols.into_iter()));
        assert!(res.is_ok());
        assert!(res.unwrap().is_empty());
        assert_eq!(&x, &expected);
    }

    #[test]
    fn test_merge_symbols_from_iter_with_block_inference() {
        let (mut x, mut add_symbols, expected) = get_merge_symbols_data();
        // Remove block names to force inference
        for s in add_symbols.iter_mut() {
            s.block_name = None;
        }
        // Add a symbol for which block inference will fail
        let unmerged_symbol = Symbol {
            name: "fn3".to_string(),
            address: MaybeVersionDep::ByVersion([("v1".into(), 0x2200000.into())].into()),
            length: None,
            description: None,
        };
        add_symbols.push(AddSymbol {
            symbol: unmerged_symbol.clone(),
            stype: SymbolType::Function,
            block_name: None,
        });

        let res = x.merge_symbols(Box::new(add_symbols.into_iter()));
        assert!(res.is_ok());
        assert_eq!(res.unwrap(), vec![unmerged_symbol.clone()]);
        assert_eq!(&x, &expected);
    }

    #[test]
    fn test_merge_symbols_from_iter_block_inference_error() {
        let (mut x, mut add_symbols, _) = get_merge_symbols_data();
        // Clone the existing block under a new name, so that inference becomes ambiguous
        let block = x.blocks().next().unwrap().clone();
        x.insert(("other", 1).into(), block);
        // Remove block names to force inference
        for s in add_symbols.iter_mut() {
            s.block_name = None;
        }
        assert!(x.merge_symbols(Box::new(add_symbols.into_iter())).is_err());
    }
}
