//! Merging data into `resymgen` YAML structs.
//!
//! This is a separate, mostly private module because (for efficiency) raw merge operations leave
//! internal initialization/ordering in inconsistent state, which needs to be corrected explicitly
//! through reinitialization. However, the publicly exported utilities are safe.

use std::borrow::Cow;
use std::collections::HashMap;
use std::error::Error;
use std::fmt::{self, Debug, Display, Formatter};
use std::ops::Deref;
use std::path::{Path, PathBuf};

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
        if !self.iter_names().any(|n| n == other.name) {
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
        // Slight optimization since most merges probably won't involve aliases
        if self.name != other.name || other.aliases.is_some() {
            let mut aliases = self.aliases.clone().unwrap_or_default();
            for alias in other.iter_names().filter(|&n| n != self.name) {
                // We don't expect more than a few aliases, so a linear search is probably going
                // to be faster than a using a set
                if !aliases.iter().any(|a| a == alias) {
                    aliases.push(alias.to_owned());
                }
            }
            if !aliases.is_empty() {
                self.aliases = Some(aliases);
            }
        }
        Ok(())
    }
}

impl Merge for SymbolList {
    fn merge(&mut self, other: &Self) -> Result<(), MergeConflict> {
        let mut name_to_idx: HashMap<_, _> = self
            .iter()
            .enumerate()
            // look for matches on all symbol names, including aliases
            .flat_map(|(i, s)| s.iter_names().map(move |n| (n.to_owned(), i)))
            .collect();
        for symbol in other.iter() {
            match name_to_idx.get(&symbol.name) {
                Some(&i) => unsafe {
                    // # Safety
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

        // Merge subregions
        if let Some(other_subregions) = &other.subregions {
            if let Some(subregions) = &mut self.subregions {
                // Try to match subregions between blocks
                for other_sub in other_subregions {
                    if let Some(sub) = subregions.iter_mut().find(|s| s.name == other_sub.name) {
                        // Found matching subregions; merge them
                        sub.merge(other_sub)?;
                    } else {
                        // No matching subregions; just append to the subregion list
                        subregions.push(other_sub.clone());
                    }
                }
            } else {
                // This block has no subregions; just steal the other's subregions
                self.subregions = Some(other_subregions.clone());
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

type IndexMap = HashMap<String, usize>;

/// Subsidiary type to [`SymbolManager`] for interacting with a particular [`SymbolList`].
struct SymbolListManager<'m, 's> {
    imap: &'m mut IndexMap,
    slist: &'s mut SymbolList,
}

impl<'m, 's> SymbolListManager<'m, 's> {
    /// Gets a mutable reference to a [`Symbol`] from within the managed [`SymbolList`] with a name
    /// matching the given `symbol`, if present. Otherwise, appends `symbol` onto `slist`, update
    /// the manager cache, and return `None`.
    fn get_or_insert(self, symbol: &Symbol) -> Option<&'s mut Symbol> {
        if let Some(idx) = self.imap.get(&symbol.name).copied() {
            // # Safety
            // The only way to get a `SymbolListManager` instance is by calling
            // `SymbolManager::manage_list()`, which requires that a given cache key (and thus
            // `imap`) must always correspond to the same `slist`. Since `imap` is constructed to
            // be an index map into `slist`, this means that any value within `imap` should be a
            // valid index, so we can skip the check.
            return Some(unsafe { self.slist.get_unchecked_mut(idx) });
        }
        let new_idx = self.slist.len();
        self.imap.insert(symbol.name.clone(), new_idx);
        for alias in symbol.iter_aliases() {
            // The new symbol's aliases could still overlap with some other name, even if its
            // primary name doesn't, so we still need to check this
            if !self.imap.contains_key(alias) {
                self.imap.insert(alias.to_owned(), new_idx);
            }
        }
        self.slist.push(symbol.clone());
        None
    }
}

type BySymbolListId<T> = HashMap<Option<PathBuf>, HashMap<String, HashMap<SymbolType, T>>>;

/// For [`SymbolList`] lookup and insertion, managed by a cache that stores
/// index by subregion path, by block name, by symbol type, by symbol name.
struct SymbolManager(BySymbolListId<IndexMap>);

impl SymbolManager {
    fn new() -> Self {
        Self(HashMap::new())
    }

    /// Put the given [`SymbolList`] under the management of the [`SymbolManager`], and return a
    /// [`SymbolListManager`] through which to interact with the [`SymbolList`],
    ///
    /// If this [`SymbolList`] has not been seen by the manager yet, its entries will be indexed
    /// by symbol name for future cache lookups.
    ///
    /// # Safety
    /// `slist` must always be the same for a given (`subregion_path`, `block_name`, `symbol_type`).
    unsafe fn manage_list<'s>(
        &mut self,
        slist: &'s mut SymbolList,
        subregion_path: &Option<PathBuf>,
        block_name: &str,
        symbol_type: &SymbolType,
    ) -> SymbolListManager<'_, 's> {
        let imap_ref: &mut IndexMap = if let Some(imap) = self
            .0
            .get_mut(subregion_path)
            .and_then(|m| m.get_mut(block_name))
            .and_then(|m| m.get_mut(symbol_type))
        {
            imap
        } else {
            let name_iter = slist.iter().enumerate().map(|(i, s)| (i, s.name.deref()));
            let alias_iter = slist
                .iter()
                .enumerate()
                .flat_map(|(i, s)| s.iter_aliases().map(move |a| (i, a)));
            let imap = name_iter.chain(alias_iter).fold(
                HashMap::with_capacity(slist.len()),
                |mut m, (i, x)| {
                    // Give the first appearance of a symbol name precedence, and give primary
                    // names precedence over aliases
                    if !m.contains_key(x) {
                        m.insert(x.to_owned(), i);
                    }
                    m
                },
            );
            self.0
                .entry(subregion_path.clone())
                .or_default()
                .entry(block_name.to_owned())
                .or_default()
                .entry(*symbol_type)
                .or_insert(imap)
        };
        SymbolListManager {
            // # Safety
            // imap_ref will always be a mutable reference to some value in self.0. Since we need
            // to return this reference (with the same lifetime as self), it forces the mutable
            // borrows against self.0 in both arms of the if-else clause to have the self lifetime,
            // which causes the borrow checker to complain because the mutable references "overlap",
            // even though they're mutually exclusive. To silence the borrow checker, just cast
            // imap_ref to a pointer and back to a reference.
            imap: unsafe { &mut *(imap_ref as *mut IndexMap) },
            slist,
        }
    }
}

/// A type that can be intrinsically associated with a single block name.
trait BlockMatch {
    type Raw;

    fn new(raw: Self::Raw) -> Self;
    fn raw(self) -> Self::Raw;
    fn block_name(&self) -> String;
}

/// A collection of match results from block inference. Matches are only valid if there aren't more
/// than one of them.
enum BlockMatches<T> {
    None,
    One(T),
    Many(Vec<String>),
}

impl<T: BlockMatch> BlockMatches<T> {
    /// Add a new [`BlockMatch`] to the collection.
    fn add(&mut self, raw: T::Raw) {
        let m = T::new(raw);
        match self {
            Self::None => *self = Self::One(m),
            Self::One(first) => *self = Self::Many(vec![first.block_name(), m.block_name()]),
            Self::Many(all_matches) => all_matches.push(m.block_name()),
        }
    }
    /// Resolve the collection into a [`BlockMatch`] if one exists, or an error if there were
    /// multiple matches.
    fn resolve(self, symbol_name: &str) -> Result<Option<T::Raw>, MergeError> {
        match self {
            // This symbol doesn't fit in any of the blocks
            Self::None => Ok(None),
            // This symbol fits in exactly one block
            Self::One(m) => Ok(Some(m.raw())),
            // It's ambiguous which block to merge into, so error out
            Self::Many(all_matches) => Err(MergeError::BlockInference(BlockInferenceError {
                symbol_name: symbol_name.to_owned(),
                matching_blocks: all_matches,
            })),
        }
    }
}

/// Match result from block inference when merging symbols.
struct InferBlockMatch<'n, 'b, P>((Option<P>, &'n String, &'b mut Block));

impl<'n, 'b, P> BlockMatch for InferBlockMatch<'n, 'b, P>
where
    P: AsRef<Path>,
{
    type Raw = (Option<P>, &'n String, &'b mut Block);

    fn new(raw: Self::Raw) -> Self {
        Self(raw)
    }
    fn raw(self) -> Self::Raw {
        self.0
    }
    fn block_name(&self) -> String {
        match &self.0 .0 {
            Some(p) => format!("{}::{}", p.as_ref().display(), self.0 .1),
            None => self.0 .1.clone(),
        }
    }
}

type BlockAssignment<'n, 'b> = (Option<PathBuf>, &'n String, &'b mut Block);

impl SymGen {
    /// Merges `other` into `self`.
    pub fn merge_symgen(&mut self, other: &Self) -> Result<(), MergeError> {
        self.merge(other).map_err(MergeError::Conflict)
    }
    /// Determine which [`Block`], if any, the given [`AddSymbol`] should be merged into.
    ///
    /// The assigned [`Block`] may be either a top-level one in the [`SymGen`] or a subsidiary
    /// [`Block`] within a resolved [`Subregion`].
    fn assign_block<'b, 's, 'n>(
        &'b mut self,
        to_add: &'s AddSymbol,
        subregion_path: Option<&Path>,
    ) -> Result<Option<BlockAssignment<'n, 'b>>, MergeError>
    where
        'b: 'n,
        's: 'n,
    {
        let (bname, block) = if let (Some(name), None) = (&to_add.block_name, subregion_path) {
            // Not in subregion and block name was explicitly specified, so retrieve it
            match self.block_key(name) {
                Some(bkey) => {
                    let key = bkey.clone();
                    (name, self.get_mut(&key).unwrap())
                }
                None => {
                    return Err(MergeError::MissingBlock(MissingBlock {
                        block_name: name.clone(),
                    }))
                }
            }
        } else {
            // In subregion or no block name, so try to infer the block based on the symbol address
            let mut block_matches: BlockMatches<InferBlockMatch<_>> = BlockMatches::None;
            for (bname, block) in self.iter_mut() {
                if bounds::block_contains_symbol(block, &to_add.symbol) {
                    block_matches.add((subregion_path, &bname.val, block));
                }
            }
            if let Some(assignment) = block_matches.resolve(&to_add.symbol.name)? {
                // The subregion path was only needed for error reporting
                (assignment.1, assignment.2)
            } else {
                return Ok(None);
            }
        };

        // Search through subregions in the selected block for a match, and assign the matching
        // subregion block instead, if one exists.
        if let Some(subregions) = &mut block.subregions {
            let mut block_matches: BlockMatches<InferBlockMatch<_>> = BlockMatches::None;
            for subregion in subregions {
                if let Some(symgen) = &mut subregion.contents {
                    let sub_path = if let Some(p) = subregion_path {
                        Cow::Owned(Subregion::subregion_dir(p).join(&subregion.name))
                    } else {
                        Cow::Borrowed(&subregion.name)
                    };
                    if let Some(assignment) = symgen.assign_block(to_add, Some(&sub_path))? {
                        block_matches.add(assignment);
                    }
                }
            }
            if let Some(block_match) = block_matches.resolve(&to_add.symbol.name)? {
                // # Safety
                // block_match contains direct references into a block in a subregion, which itself
                // is contained within block.subregions. This means that if we try to return it
                // directly, the compiler will infer the lifetimes of the references within
                // block_match to be 'n and 'b (for the block name and block, respectively), which
                // means that the parent borrow to &mut block.subregions must also satisfy the same
                // lifetime constraints. However, since 'n and 'b come from the function inputs,
                // they outlive the scope of this code block and extend to the end of the function.
                // Since there's a code path that falls through to return a mutable reference to
                // the top-level block, the borrow checker will complain that it aliases with the
                // long-lived mutable reference to block.subregions, even though this is clearly
                // nonsense.
                //
                // To silence the borrow checker, convert the references within block_match into
                // raw pointers, then back to references again. Then return a newly constructed
                // tuple with these new references. This severs the lifetime inference chain
                // between the return value and &mut block.subregions at the beginning of the
                // scope, allowing &mut block.subregions to be assigned a lifetime local to the
                // scope.
                let (bname_ref, block_ref) = unsafe {
                    (
                        &*(block_match.1 as *const String),
                        &mut *(block_match.2 as *mut Block),
                    )
                };
                return Ok(Some((block_match.0, bname_ref, block_ref)));
            }
        }

        // Assign the matching top-level block
        Ok(Some((subregion_path.map(|p| p.to_owned()), bname, block)))
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
            let assignment = self.assign_block(&to_add, None)?;
            let (sub_path, bname, block) = match assignment {
                Some((sub_path, bname, block)) => (sub_path, bname, block),
                None => {
                    unmerged_symbols.push(to_add.symbol);
                    continue;
                }
            };

            let slist = match to_add.stype {
                SymbolType::Function => &mut block.functions,
                SymbolType::Data => &mut block.data,
            };
            let list_manager = unsafe {
                // # Safety
                // `assignment` (`sub_path`, `bname`, `block` -> `slist`) always come from
                // `to_add` via `self.assign_block()`, which always returns references into self.
                // This means that for a given (`sub_path`, `bname, `to_add.stype`), `slist` will
                // always be the same list. This satisfies the invariant of `manage_list`.
                sym_manager.manage_list(slist, &sub_path, bname, &to_add.stype)
            };
            if let Some(sym) = list_manager.get_or_insert(&to_add.symbol) {
                let to_merge = if let Some(vers) = &block.versions {
                    let mut cpy = to_add.symbol.clone();
                    cpy.expand_versions(vers);
                    Cow::Owned(cpy)
                } else {
                    Cow::Borrowed(&to_add.symbol)
                };
                sym.merge(&to_merge).map_err(MergeError::Conflict)?;
            }
        }
        // Reinit because merging can introduce new OrdStrings/Versions
        self.init();
        Ok(unmerged_symbols)
    }
}

impl Merge for Subregion {
    fn merge(&mut self, other: &Self) -> Result<(), MergeConflict> {
        if self.name != other.name {
            return Err(MergeConflict::new(
                self.name.display(),
                other.name.display(),
            ));
        }
        if let Some(contents) = &mut self.contents {
            if let Some(other_contents) = &other.contents {
                // Both subregions have contents; merge them
                contents.merge(other_contents)?;
            }
        } else {
            // No contents; copy over the other's
            *self = other.clone();
        }
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::super::symgen::test_utils;
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
            aliases: None,
            address: MaybeVersionDep::ByVersion(
                [("v1".into(), 1.into()), ("v2".into(), 2.into())].into(),
            ),
            length: None,
            description: None,
        };
        assert!(x
            .merge(&Symbol {
                name: "function".to_string(),
                aliases: None,
                address: MaybeVersionDep::ByVersion([("v3".into(), 3.into())].into()),
                length: Some(MaybeVersionDep::Common(5)),
                description: Some("desc".to_string()),
            })
            .is_ok());
        assert_eq!(
            &x,
            &Symbol {
                name: "function".to_string(),
                aliases: None,
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
                aliases: None,
                address: MaybeVersionDep::ByVersion([("v3".into(), 3.into())].into()),
                length: None,
                description: Some("other desc".to_string()),
            })
            .is_err())
    }

    #[test]
    fn test_merge_symbol_aliases() {
        let mut x = Symbol {
            name: "function".to_string(),
            aliases: Some(vec!["function_alias1".to_string()]),
            address: MaybeVersionDep::Common(1.into()),
            length: None,
            description: None,
        };
        assert!(x
            .merge(&Symbol {
                name: "function".to_string(),
                aliases: None,
                address: MaybeVersionDep::Common(1.into()),
                length: None,
                description: None,
            })
            .is_ok());
        assert!(x
            .merge(&Symbol {
                name: "function".to_string(),
                aliases: Some(vec!["function_alias1".to_string()]), // should be deduped
                address: MaybeVersionDep::Common(1.into()),
                length: None,
                description: None,
            })
            .is_ok());
        assert!(x
            .merge(&Symbol {
                name: "function_alias1".to_string(),
                aliases: Some(vec!["function_alias2".to_string(), "function".to_string()]),
                address: MaybeVersionDep::Common(1.into()),
                length: Some(MaybeVersionDep::Common(5)),
                description: Some("desc".to_string()),
            })
            .is_ok());
        assert_eq!(
            &x,
            &Symbol {
                name: "function".to_string(),
                aliases: Some(vec![
                    "function_alias1".to_string(),
                    "function_alias2".to_string()
                ]),
                address: MaybeVersionDep::Common(1.into()),
                length: Some(MaybeVersionDep::Common(5)),
                description: Some("desc".to_string()),
            }
        );

        assert!(x
            .merge(&Symbol {
                name: "function_alias3".to_string(),
                aliases: Some(vec!["function".to_string()]),
                address: MaybeVersionDep::Common(1.into()),
                length: None,
                description: None,
            })
            .is_err())
    }

    #[test]
    fn test_merge_symbol_list() {
        let mut x = SymbolList::from([
            Symbol {
                name: "function1".to_string(),
                aliases: None,
                address: MaybeVersionDep::Common(1.into()),
                length: None,
                description: None,
            },
            Symbol {
                name: "function2".to_string(),
                aliases: Some(vec!["function2_alias".to_string()]),
                address: MaybeVersionDep::Common(2.into()),
                length: None,
                description: None,
            },
        ]);
        assert!(x
            .merge(&SymbolList::from([
                Symbol {
                    name: "function3".to_string(),
                    aliases: Some(vec!["function3_alias".to_string()]),
                    address: MaybeVersionDep::Common(3.into()),
                    length: None,
                    description: None,
                },
                Symbol {
                    name: "function2".to_string(),
                    aliases: None,
                    address: MaybeVersionDep::Common(4.into()),
                    length: None,
                    description: Some("desc".to_string()),
                },
                Symbol {
                    name: "function2_alias".to_string(),
                    aliases: Some(vec!["function2_alias2".to_string()]),
                    address: MaybeVersionDep::Common(5.into()),
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
                    aliases: None,
                    address: MaybeVersionDep::Common(1.into()),
                    length: None,
                    description: None,
                },
                Symbol {
                    name: "function2".to_string(),
                    aliases: Some(vec![
                        "function2_alias".to_string(),
                        "function2_alias2".to_string()
                    ]),
                    address: MaybeVersionDep::Common([2, 4, 5].into()),
                    length: None,
                    description: Some("desc".to_string()),
                },
                Symbol {
                    name: "function3".to_string(),
                    aliases: Some(vec!["function3_alias".to_string()]),
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
            subregions: None,
            functions: [Symbol {
                name: "function1".to_string(),
                aliases: None,
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
                subregions: None,
                functions: [Symbol {
                    name: "function2".to_string(),
                    aliases: None,
                    address: MaybeVersionDep::ByVersion([("v2".into(), 1.into())].into()),
                    length: None,
                    description: None,
                }]
                .into(),
                data: [Symbol {
                    name: "data".to_string(),
                    aliases: None,
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
                subregions: None,
                functions: [
                    Symbol {
                        name: "function1".to_string(),
                        aliases: None,
                        address: MaybeVersionDep::ByVersion([("v1".into(), 1.into())].into()),
                        length: None,
                        description: None,
                    },
                    Symbol {
                        name: "function2".to_string(),
                        aliases: None,
                        address: MaybeVersionDep::ByVersion([("v2".into(), 1.into())].into()),
                        length: None,
                        description: None,
                    },
                ]
                .into(),
                data: [Symbol {
                    name: "data".to_string(),
                    aliases: None,
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
            subregions: None,
            functions: [Symbol {
                name: "function1".to_string(),
                aliases: None,
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
                subregions: None,
                functions: [
                    Symbol {
                        name: "function1".to_string(),
                        aliases: None,
                        address: MaybeVersionDep::Common(1.into()),
                        length: None,
                        description: None,
                    },
                    Symbol {
                        name: "function2".to_string(),
                        aliases: None,
                        address: MaybeVersionDep::Common(1.into()),
                        length: None,
                        description: None,
                    },
                ]
                .into(),
                data: [Symbol {
                    name: "data".to_string(),
                    aliases: None,
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
                subregions: None,
                functions: [
                    Symbol {
                        name: "function1".to_string(),
                        aliases: None,
                        address: MaybeVersionDep::ByVersion(
                            [("v1".into(), 1.into()), ("v2".into(), 1.into())].into()
                        ),
                        length: None,
                        description: None,
                    },
                    Symbol {
                        name: "function2".to_string(),
                        aliases: None,
                        address: MaybeVersionDep::ByVersion([("v2".into(), 1.into())].into()),
                        length: None,
                        description: None,
                    },
                ]
                .into(),
                data: [Symbol {
                    name: "data".to_string(),
                    aliases: None,
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
                  aliases:
                    - fn1_alias
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
                        - name: fn1_alias
                          address:
                            v3: 0x2003000
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
                      aliases:
                        - fn1_alias
                      address:
                        v1: 0x2001000
                        v2: 0x2002000
                        v3: 0x2003000
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

    #[test]
    fn test_merge_symgen_with_subregions() {
        // The subregion handling code is in Block not SymGen, but it's easier to construct a
        // SymGen, and the code path for SymGen should call into the Block code anyway.
        let sub1_text = r#"sub1:
            address: 0x10
            length: 0x10
            functions:
              - name: sub1_fn
                address: 0x10
            data: []
            "#;
        let top_level_sub3_text = r#"sub3:
            address: 0x30
            length: 0x10
            functions: []
            data: []
            "#;
        let sub4_text = r#"sub4:
            address: 0x2C
            length: 0x4
            functions:
              - name: sub4_fn
                address: 0x2C
            data: []
            "#;

        let mut symgen = test_utils::get_symgen_with_subregions(
            r#"main:
            address: 0x0
            length: 0x100
            subregions:
              - sub1.yml
              - sub2.yml
            functions:
              - name: fn1
                address: 0x0
            data:
              - name: data1
                address: 0x50
                length: 0x4
            "#,
            &[
                ("sub1.yml", sub1_text),
                (
                    "sub2.yml",
                    r#"sub2:
                    address: 0x20
                    length: 0x10
                    subregions:
                      - sub3.yml
                    functions:
                      - name: sub2_fn1
                        address: 0x20
                    data: []
                    "#,
                ),
                (
                    "sub2/sub3.yml",
                    r#"sub3:
                    address: 0x28
                    length: 0x4
                    functions:
                      - name: sub3_fn1
                        address: 0x28
                    data: []
                    "#,
                ),
            ],
        );
        let other = test_utils::get_symgen_with_subregions(
            r#"main:
            address: 0x0
            length: 0x100
            subregions:
              - sub2.yml
              - sub3.yml
            functions:
              - name: fn2
                address: 0x4
            data:
              - name: data2
                address: 0x54
                length: 0x4
            "#,
            &[
                (
                    "sub2.yml",
                    r#"sub2:
                    address: 0x20
                    length: 0x10
                    subregions:
                      - sub3.yml
                      - sub4.yml
                    functions:
                      - name: sub2_fn2
                        address: 0x24
                    data: []
                    "#,
                ),
                (
                    "sub2/sub3.yml",
                    r#"sub3:
                    address: 0x28
                    length: 0x4
                    functions:
                      - name: sub3_fn2
                        address: 0x2A
                    data: []
                    "#,
                ),
                ("sub2/sub4.yml", sub4_text),
                ("sub3.yml", top_level_sub3_text),
            ],
        );
        let expected = test_utils::get_symgen_with_subregions(
            r#"main:
            address: 0x0
            length: 0x100
            subregions:
              - sub1.yml
              - sub2.yml
              - sub3.yml
            functions:
              - name: fn1
                address: 0x0
              - name: fn2
                address: 0x4
            data:
              - name: data1
                address: 0x50
                length: 0x4
              - name: data2
                address: 0x54
                length: 0x4
            "#,
            &[
                ("sub1.yml", sub1_text),
                (
                    "sub2.yml",
                    r#"sub2:
                    address: 0x20
                    length: 0x10
                    subregions:
                      - sub3.yml
                      - sub4.yml
                    functions:
                      - name: sub2_fn1
                        address: 0x20
                      - name: sub2_fn2
                        address: 0x24
                    data: []
                    "#,
                ),
                (
                    "sub2/sub3.yml",
                    r#"sub3:
                    address: 0x28
                    length: 0x4
                    functions:
                      - name: sub3_fn1
                        address: 0x28
                      - name: sub3_fn2
                        address: 0x2A
                    data: []
                    "#,
                ),
                ("sub2/sub4.yml", sub4_text),
                ("sub3.yml", top_level_sub3_text),
            ],
        );
        let res = symgen.merge(&other);
        assert!(res.is_ok());
        assert_eq!(&symgen, &expected);
    }

    // Returns initial SymGen, AddSymbol list, and expected SymGen after merge
    fn get_merge_symbols_data() -> (SymGen, Vec<AddSymbol>, SymGen) {
        (
            get_simple_symgen(),
            vec![
                AddSymbol {
                    symbol: Symbol {
                        name: "fn1".to_string(),
                        aliases: None,
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
                        name: "fn1_alias".to_string(),
                        aliases: Some(vec!["fn1_alias2".to_string()]),
                        address: MaybeVersionDep::ByVersion(
                            [("v1".into(), 0x2003000.into())].into(),
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
                        aliases: None,
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
                      aliases:
                        - fn1_alias
                        - fn1_alias2
                      address:
                        v1:
                          - 0x2001000
                          - 0x2002000
                          - 0x2003000
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
            aliases: None,
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

    fn get_merge_target_with_subregions() -> SymGen {
        test_utils::get_symgen_with_subregions(
            r#"main:
            address: 0x0
            length: 0x100
            subregions:
              - sub1.yml
              - sub2.yml
            functions: []
            data: []
            "#,
            &[
                (
                    "sub1.yml",
                    r#"sub1:
                    address: 0x0
                    length: 0x50
                    functions: []
                    data: []
                    "#,
                ),
                (
                    "sub2.yml",
                    r#"sub2:
                    address: 0x40
                    length: 0x40
                    subregions:
                      - sub3.yml
                    functions: []
                    data: []
                    "#,
                ),
                (
                    "sub2/sub3.yml",
                    r#"sub3:
                    address: 0x60
                    length: 0x20
                    functions: []
                    data: []
                    "#,
                ),
            ],
        )
    }

    #[test]
    fn test_merge_symbols_from_iter_with_subregions() {
        let mut x = get_merge_target_with_subregions();
        let unmerged_symbol = Symbol {
            name: "unmerged".to_string(),
            aliases: None,
            address: MaybeVersionDep::Common(0x100.into()),
            length: None,
            description: None,
        };
        let add_symbols = vec![
            AddSymbol {
                symbol: Symbol {
                    name: "main_fn".to_string(),
                    aliases: None,
                    address: MaybeVersionDep::Common(0x80.into()),
                    length: None,
                    description: None,
                },
                stype: SymbolType::Function,
                block_name: None,
            },
            AddSymbol {
                symbol: Symbol {
                    name: "sub1_data".to_string(),
                    aliases: None,
                    address: MaybeVersionDep::Common(0x0.into()),
                    length: Some(MaybeVersionDep::Common(0x4)),
                    description: None,
                },
                stype: SymbolType::Data,
                block_name: None,
            },
            AddSymbol {
                symbol: Symbol {
                    name: "sub2_fn".to_string(),
                    aliases: None,
                    address: MaybeVersionDep::Common(0x50.into()),
                    length: None,
                    description: None,
                },
                stype: SymbolType::Function,
                block_name: None,
            },
            AddSymbol {
                symbol: Symbol {
                    name: "sub3_data".to_string(),
                    aliases: None,
                    address: MaybeVersionDep::Common(0x60.into()),
                    length: Some(MaybeVersionDep::Common(0x4)),
                    description: None,
                },
                stype: SymbolType::Data,
                // Make sure providing the top-level block name doesn't mess anything up
                block_name: Some("main".to_string()),
            },
            AddSymbol {
                symbol: Symbol {
                    name: "sub3_fn".to_string(),
                    aliases: None,
                    address: MaybeVersionDep::Common(0x64.into()),
                    length: None,
                    description: None,
                },
                stype: SymbolType::Function,
                block_name: None,
            },
            AddSymbol {
                // Make sure unmerged symbols are still treated properly
                symbol: unmerged_symbol.clone(),
                stype: SymbolType::Function,
                block_name: None,
            },
        ];
        let expected = test_utils::get_symgen_with_subregions(
            r#"main:
            address: 0x0
            length: 0x100
            subregions:
              - sub1.yml
              - sub2.yml
            functions:
              - name: main_fn
                address: 0x80
            data: []
            "#,
            &[
                (
                    "sub1.yml",
                    r#"sub1:
                    address: 0x0
                    length: 0x50
                    functions: []
                    data:
                      - name: sub1_data
                        address: 0x0
                        length: 0x4
                    "#,
                ),
                (
                    "sub2.yml",
                    r#"sub2:
                    address: 0x40
                    length: 0x40
                    subregions:
                      - sub3.yml
                    functions:
                      - name: sub2_fn
                        address: 0x50
                    data: []
                    "#,
                ),
                (
                    "sub2/sub3.yml",
                    r#"sub3:
                    address: 0x60
                    length: 0x20
                    functions:
                      - name: sub3_fn
                        address: 0x64
                    data:
                      - name: sub3_data
                        address: 0x60
                        length: 0x4
                    "#,
                ),
            ],
        );

        let res = x.merge_symbols(Box::new(add_symbols.into_iter()));
        assert!(res.is_ok());
        assert_eq!(res.unwrap(), vec![unmerged_symbol.clone()]);
        assert_eq!(&x, &expected);
    }

    #[test]
    fn test_merge_symbols_from_iter_with_subregions_inference_error() {
        let mut x = get_merge_target_with_subregions();
        assert!(x
            .merge_symbols(Box::new(
                vec![AddSymbol {
                    symbol: Symbol {
                        name: "fn1".to_string(),
                        aliases: None,
                        address: MaybeVersionDep::Common(0x40.into()), // Fits in both sub1 and sub2
                        length: None,
                        description: None,
                    },
                    stype: SymbolType::Function,
                    block_name: None,
                }]
                .into_iter()
            ))
            .is_err());
    }
}
