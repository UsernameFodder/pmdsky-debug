//! Validating the substantive contents of `resymgen` YAML files. Implements the `check` command.

use std::borrow::Borrow;
use std::cmp;
use std::collections::{BTreeMap, BTreeSet, HashMap, HashSet};
use std::error::Error;
use std::fmt::{self, Display, Formatter};
use std::fs::File;
use std::io::{self, Write};
use std::iter;
use std::path::{Path, PathBuf};
use std::rc::Rc;

use syn::{self, Ident};
use termcolor::{Color, ColorChoice, ColorSpec, StandardStream, WriteColor};

use super::data_formats::symgen_yml::bounds::{self, BoundViolation};
use super::data_formats::symgen_yml::{
    Block, MaybeVersionDep, OrdString, Subregion, SymGen, Symbol, Uint, Version, VersionDep,
};
use super::util::MultiFileError;

/// Naming conventions for symbol names.
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub enum NamingConvention {
    /// Symbol names should be valid identifiers (in accordance with Rust syntax).
    /// This condition implicitly applies to all other variants.
    Identifier,
    /// snake_case
    SnakeCase,
    /// SCREAMING_SNAKE_CASE
    ScreamingSnakeCase,
    /// camelCase
    CamelCase,
    /// PascalCase
    PascalCase,
    /// camel_Snake_Case (camelCase_Then_PascalCase)
    CamelSnakeCase,
    /// underscoreSeparated_camelCase
    UnderscoreSeparatedCamelCase,
    /// Pascal_Snake_Case (UnderscoreSeparated_PascalCase)
    PascalSnakeCase,
    /// flatcase
    FlatCase,
    /// UPPERFLATCASE
    UpperFlatCase,
}

impl NamingConvention {
    /// Checks if a name is valid under a given naming convention
    fn check(&self, name: &str) -> bool {
        if let Self::Identifier = self {
        } else {
            // All other conventions are contingent on valid identifiers everything goes through
            // the Identifier check first.
            if !Self::Identifier.check(name) {
                return false;
            }
        }
        match self {
            Self::Identifier => {
                // Makes sure there's no leading/trailing whitespace.
                // The Rust parser actually handles that fine, but we don't want to.
                if let Some(c) = name.chars().next() {
                    if c.is_whitespace() {
                        return false;
                    }
                }
                if let Some(c) = name.chars().next_back() {
                    if c.is_whitespace() {
                        return false;
                    }
                }

                // This checks for whether a name is a valid identifier in Rust; see
                // https://stackoverflow.com/questions/58937136/in-a-procedural-macro-how-can-i-check-if-a-string-is-a-valid-variable-name-and
                // This is a bit lazy and not quite correct for symbol names (e.g., it'll treat Rust
                // keywords as invalid), but it's a sane standard to follow that includes non-ascii
                // identifiers (https://rust-lang.github.io/rfcs/2457-non-ascii-idents.html), which
                // technically is supported by gcc as well. We could also enforce the standard
                // "valid ascii identifier" rules (i.e., [A-Za-z_][A-Za-z0-9_]*), but this is simpler
                // and handles Unicode, and the downsides are probably fine? Who's going to be naming
                // their symbols "fn" and "impl" anyway?
                syn::parse_str::<Ident>(name).is_ok()
            }
            // Note: !is_uppercase() is less restrictive than is_lowercase()
            Self::SnakeCase => name.chars().all(|c| !c.is_uppercase()),
            // Note: !is_lowercase() is less restrictive than is_uppercase()
            Self::ScreamingSnakeCase => name.chars().all(|c| !c.is_lowercase()),
            Self::CamelCase => {
                // The first character must not be uppercase
                if let Some(c) = name.chars().next() {
                    if c.is_uppercase() {
                        return false;
                    }
                }
                NamingConvention::camel_family_check(name)
            }
            Self::PascalCase => {
                // The first character must not be lowercase
                if let Some(c) = name.chars().next() {
                    if c.is_lowercase() {
                        return false;
                    }
                }
                NamingConvention::camel_family_check(name)
            }
            Self::CamelSnakeCase => {
                let mut parts_iter = name.split('_');
                if let Some(first_part) = parts_iter.next() {
                    if !Self::CamelCase.check(first_part) {
                        return false;
                    }
                }
                parts_iter.all(|part| Self::PascalCase.check(part))
            }
            Self::UnderscoreSeparatedCamelCase => {
                name.split('_').all(|part| Self::CamelCase.check(part))
            }
            Self::PascalSnakeCase => name.split('_').all(|part| Self::PascalCase.check(part)),
            // Note: !is_uppercase() is less restrictive than is_lowercase()
            Self::FlatCase => name.chars().all(|c| !c.is_uppercase() && c != '_'),
            // Note: !is_lowercase() is less restrictive than is_uppercase()
            Self::UpperFlatCase => name.chars().all(|c| !c.is_lowercase() && c != '_'),
        }
    }

    /// Checks if a name is valid under a given set of naming conventions
    fn check_multiple(convs: &BTreeSet<NamingConvention>, name: &str) -> bool {
        convs.iter().any(|conv| conv.check(name))
    }

    // Includes camelCase and PascalCase
    fn camel_family_check(name: &str) -> bool {
        let mut consecutive_uppercase = 0;
        for c in name.chars() {
            // No underscores allowed
            if c == '_' {
                return false;
            }
            // No more than two consecutive characters can be uppercase
            // (this allows for one-letter words like "A")
            if c.is_uppercase() {
                consecutive_uppercase += 1;
            } else {
                consecutive_uppercase = 0;
            }
            if consecutive_uppercase > 2 {
                return false;
            }
        }
        true
    }
}

/// Checks that can be run on `resymgen` YAML symbol tables.
#[derive(Debug, Clone)]
pub enum Check {
    /// All addresses and lengths (for both blocks and symbols) must be explicitly listed by version.
    ExplicitVersions,
    /// Any version used within a version-dependent address or length, or within a subregion
    /// version list, must appear in the version list of the parent block.
    CompleteVersionList,
    /// YAML maps (key-value pair lists) must have at least one entry.
    NonEmptyMaps,
    /// Symbol and subregion names within a block must be unique.
    UniqueSymbols,
    /// Symbol names must be unique within a block and all its subregions.
    UniqueSymbolsAcrossSubregions,
    /// Symbols and subregions must fall within the address range of the parent block.
    InBoundsSymbols,
    /// For a given block and version, function symbols must not overlap with each other, and
    /// subregions must not overlap with other subregions, function symbols, or data symbols.
    NoOverlap,
    /// Function symbol names must adhere to the specified set of [`NamingConvention`]s.
    FunctionNames(BTreeSet<NamingConvention>),
    /// Data symbol names must adhere to the specified set of [`NamingConvention`]s.
    DataNames(BTreeSet<NamingConvention>),
}

/// The result of a [`Check`] run on `resymgen` YAML symbol tables.
#[derive(Debug)]
pub struct CheckResult {
    pub check: Check,
    pub succeeded: bool,
    pub details: Option<String>,
}

impl Check {
    fn run(&self, symgen: &SymGen) -> CheckResult {
        match self {
            Self::ExplicitVersions => self.result(check_explicit_versions(symgen)),
            Self::CompleteVersionList => self.result(check_complete_version_list(symgen)),
            Self::NonEmptyMaps => self.result(check_nonempty_maps(symgen)),
            Self::UniqueSymbols => self.result(check_unique_symbols(symgen)),
            Self::UniqueSymbolsAcrossSubregions => {
                self.result(check_unique_symbols_across_subregions(symgen))
            }
            Self::InBoundsSymbols => self.result(check_in_bounds_symbols(symgen)),
            Self::NoOverlap => self.result(check_no_overlap(symgen)),
            Self::FunctionNames(convs) => self.result(check_function_names(symgen, convs)),
            Self::DataNames(convs) => self.result(check_data_names(symgen, convs)),
        }
    }
    fn result(&self, raw_result: Result<(), String>) -> CheckResult {
        CheckResult {
            check: self.clone(), // cloning a Check is expected to be reasonably cheap
            succeeded: raw_result.is_ok(),
            details: raw_result.err(),
        }
    }
}

impl Display for Check {
    fn fmt(&self, f: &mut Formatter) -> fmt::Result {
        write!(f, "{:?}", self)
    }
}

/// Assert a predicate, and return an error if the predicate is false.
fn assert_check<F, S>(pred: bool, err_msg: F) -> Result<(), String>
where
    F: FnOnce() -> S,
    S: ToString,
{
    if pred {
        Ok(())
    } else {
        Err(err_msg().to_string())
    }
}

/// Runs a simple boolean check on all address/length fields, and optionally all subregion blocks.
/// The generic <'a> is the lifetime of the [`SymGen`] object to check, and allows the checker
/// context to hold references to a block if needed.
trait SimpleBlockContentsChecker<'a> {
    fn init_context(
        &mut self,
        _block_name: &'a OrdString,
        _block: &'a Block,
    ) -> Result<(), String> {
        Ok(())
    }
    fn should_check_subblocks() -> bool {
        false
    }
    fn check_subblock(&self, _subblock: &'a Block) -> Result<(), String> {
        Ok(())
    }
    fn check_val<T>(&self, val: &'a MaybeVersionDep<T>) -> Result<(), String>;

    fn check_symgen(&mut self, symgen: &'a SymGen) -> Result<(), String> {
        for (bname, b) in symgen.iter() {
            self.init_context(bname, b)?;

            if let Some(err_stem) = self.check_val(&b.address).err() {
                return Err(format!("block \"{}\": address {}", bname, err_stem,));
            }
            if let Some(err_stem) = self.check_val(&b.length).err() {
                return Err(format!("block \"{}\": length {}", bname, err_stem));
            }
            for s in b.iter() {
                if let Some(err_stem) = self.check_val(&s.address).err() {
                    return Err(format!(
                        "block \"{}\", symbol \"{}\": address {}",
                        bname, s.name, err_stem
                    ));
                }
                if let Some(l) = &s.length {
                    if let Some(err_stem) = self.check_val(l).err() {
                        return Err(format!(
                            "block \"{}\", symbol \"{}\": length {}",
                            bname, s.name, err_stem
                        ));
                    }
                }
            }

            if Self::should_check_subblocks() {
                // Use paths relative to the root symgen
                for subblock in b.cursor(&bname.val, Path::new("")).subblocks() {
                    if let Some(err_stem) = self.check_subblock(subblock.block()).err() {
                        return Err(format!(
                            "block \"{}\": subregion block \"{}::{}\" {}",
                            bname,
                            subblock.path().display(),
                            subblock.name(),
                            err_stem
                        ));
                    }
                }
            }
        }
        Ok(())
    }
}

fn check_explicit_versions(symgen: &SymGen) -> Result<(), String> {
    struct ExplicitVersionChecker {}
    impl SimpleBlockContentsChecker<'_> for ExplicitVersionChecker {
        fn check_val<T>(&self, val: &MaybeVersionDep<T>) -> Result<(), String> {
            if val.is_common() {
                Err("has no version".to_string())
            } else {
                Ok(())
            }
        }
    }

    let mut c = ExplicitVersionChecker {};
    c.check_symgen(symgen)
}

fn check_complete_version_list(symgen: &SymGen) -> Result<(), String> {
    struct CompleteVersionListChecker<'a> {
        block_versions: HashSet<&'a Version>,
    }
    impl<'a> CompleteVersionListChecker<'a> {
        fn err_stem(&self, versions: &HashSet<&Version>) -> String {
            let mut vers_diff: Vec<String> = versions
                .difference(&self.block_versions)
                .map(|v| v.name().to_string())
                .collect();
            vers_diff.sort();
            format!("contains unknown versions: [{}]", vers_diff.join(", "))
        }
        fn check(&self, versions: &HashSet<&Version>) -> Result<(), String> {
            if versions.is_subset(&self.block_versions) {
                Ok(())
            } else {
                Err(self.err_stem(versions))
            }
        }
    }
    impl<'a> SimpleBlockContentsChecker<'a> for CompleteVersionListChecker<'a> {
        fn init_context(
            &mut self,
            block_name: &'a OrdString,
            block: &'a Block,
        ) -> Result<(), String> {
            self.block_versions.clear();
            if let Some(v) = &block.versions {
                let versions: HashSet<&Version> = v.iter().collect();
                assert_check(versions.len() == v.len(), || {
                    format!("block \"{}\": version list contains duplicates", block_name)
                })?;
                self.block_versions = versions;
            }
            Ok(())
        }
        fn should_check_subblocks() -> bool {
            true
        }
        fn check_subblock(&self, subblock: &Block) -> Result<(), String> {
            if let Some(versions) = &subblock.versions {
                self.check(
                    &versions
                        .iter()
                        .map(|v| {
                            // Need to map versions to the parent block's ordinal space since
                            // the subblock has its own, possibly incompatible ordinal space.
                            if let Some(native_v) = self
                                .block_versions
                                .iter()
                                .find(|parent_v| parent_v.name() == v.name())
                            {
                                native_v
                            } else {
                                v
                            }
                        })
                        .collect::<HashSet<_>>(),
                )
            } else {
                Ok(())
            }
        }
        fn check_val<T>(&self, val: &MaybeVersionDep<T>) -> Result<(), String> {
            self.check(&val.versions().collect::<HashSet<_>>())
        }
    }

    let mut c = CompleteVersionListChecker {
        block_versions: HashSet::new(),
    };
    c.check_symgen(symgen)
}

fn check_nonempty_maps(symgen: &SymGen) -> Result<(), String> {
    struct NonEmptyMapChecker {}
    impl SimpleBlockContentsChecker<'_> for NonEmptyMapChecker {
        fn check_val<T>(&self, val: &MaybeVersionDep<T>) -> Result<(), String> {
            if val.is_empty() {
                Err("is empty".to_string())
            } else {
                Ok(())
            }
        }
    }

    let mut c = NonEmptyMapChecker {};
    c.check_symgen(symgen)
}

fn check_unique_symbols(symgen: &SymGen) -> Result<(), String> {
    let mut duplicate_names: BTreeMap<&OrdString, HashSet<&str>> = BTreeMap::new();
    let mut duplicate_subregions: BTreeMap<&OrdString, HashSet<&Path>> = BTreeMap::new();
    for (bname, b) in symgen.iter() {
        let mut names: HashSet<&str> = HashSet::new();
        for name in b.iter().flat_map(|s| s.iter_names()) {
            if !names.insert(name) {
                duplicate_names.entry(bname).or_default().insert(name);
            }
        }
        if let Some(subregions) = &b.subregions {
            let mut snames: HashSet<&Path> = HashSet::new();
            for sname in subregions.iter().map(|s| &s.name) {
                if !snames.insert(sname) {
                    duplicate_subregions.entry(bname).or_default().insert(sname);
                }
            }
        }
    }
    assert_check(
        duplicate_names.is_empty() && duplicate_subregions.is_empty(),
        || {
            let mut components = Vec::with_capacity(2);
            if !duplicate_names.is_empty() {
                components.push(format!(
                    "Found duplicate symbol names:\n{}",
                    duplicate_names
                        .into_iter()
                        .map(|(bname, names)| {
                            let mut names: Vec<_> = names.into_iter().collect();
                            names.sort_unstable();
                            format!("- block \"{}\": [{}]", bname, names.join(", "))
                        })
                        .collect::<Vec<_>>()
                        .join("\n")
                ));
            }
            if !duplicate_subregions.is_empty() {
                components.push(format!(
                    "Found duplicate subregion names:\n{}",
                    duplicate_subregions
                        .into_iter()
                        .map(|(bname, subregions)| {
                            let mut subregions: Vec<_> = subregions
                                .into_iter()
                                .map(|p| p.to_string_lossy())
                                .collect();
                            subregions.sort();
                            format!("- block \"{}\": [{}]", bname, subregions.join(", "))
                        })
                        .collect::<Vec<_>>()
                        .join("\n")
                ));
            }
            components.join("\n")
        },
    )
}

fn check_unique_symbols_across_subregions(symgen: &SymGen) -> Result<(), String> {
    #[derive(PartialEq, Eq, Hash)]
    struct RcPathKey(Rc<PathBuf>);
    impl Borrow<Path> for RcPathKey {
        fn borrow(&self) -> &Path {
            self.0.as_path()
        }
    }
    struct PathCache(HashSet<RcPathKey>);
    impl PathCache {
        fn new() -> Self {
            Self(HashSet::new())
        }
        fn get(&mut self, p: &Path) -> Rc<PathBuf> {
            if let Some(RcPathKey(existing_path)) = self.0.get(p) {
                Rc::clone(existing_path)
            } else {
                let new_path = Rc::new(p.to_owned());
                self.0.insert(RcPathKey(Rc::clone(&new_path)));
                new_path
            }
        }
    }
    type SymbolLocations<'s> = HashMap<&'s str, Vec<(Rc<PathBuf>, &'s str)>>;

    let mut duplicate_symbols: BTreeMap<&str, SymbolLocations> = BTreeMap::new();

    // Use paths relative to the root symgen
    for cursor in symgen.cursor(Path::new("")).blocks() {
        let mut all_symbols: SymbolLocations = HashMap::new();
        let mut path_cache = PathCache::new();
        let bname = cursor.name();

        for block in cursor.dtraverse() {
            let path = path_cache.get(block.path());
            let symbols: HashSet<&str> =
                block.block().iter().flat_map(|s| s.iter_names()).collect();
            for symbol in symbols {
                all_symbols
                    .entry(symbol)
                    .or_default()
                    .push((Rc::clone(&path), block.name()));
            }
        }
        let duplicates: SymbolLocations = all_symbols
            .into_iter()
            .filter(|(_, loc)| loc.len() > 1)
            .collect();
        if !duplicates.is_empty() {
            duplicate_symbols.insert(bname, duplicates);
        }
    }

    assert_check(duplicate_symbols.is_empty(), || {
        format!(
            "Found duplicate symbol names:\n{}",
            duplicate_symbols
                .into_iter()
                .map(|(bname, symbol_locations)| {
                    let mut repeated_symbols: Vec<_> = symbol_locations
                        .into_iter()
                        .map(|(symbol, locations)| {
                            format!(
                                "  - \"{}\" repeated in: [{}]",
                                symbol,
                                locations
                                    .into_iter()
                                    .map(|(path, block)| {
                                        if path.components().next().is_none() {
                                            block.to_string()
                                        } else {
                                            format!("{}::{}", path.display(), block)
                                        }
                                    })
                                    .collect::<Vec<_>>()
                                    .join(", ")
                            )
                        })
                        .collect();
                    repeated_symbols.sort();
                    format!("- block \"{}\":\n{}", bname, repeated_symbols.join("\n"))
                })
                .collect::<Vec<_>>()
                .join("\n")
        )
    })
}

fn check_in_bounds_symbols(symgen: &SymGen) -> Result<(), String> {
    fn range_str((addr, opt_len): (Uint, Option<Uint>)) -> String {
        match opt_len {
            Some(len) => format!("{:#X}..{:#X}", addr, addr + len),
            None => format!("{:#X}", addr),
        }
    }
    fn violation_str(violation: BoundViolation, bname: &OrdString, identifier: String) -> String {
        if let Some(vers) = &violation.version {
            format!(
                "block \"{}\" [{}]: {} at {} is outside of block bounds {}",
                bname,
                vers,
                identifier,
                range_str(violation.extent),
                range_str(violation.bound),
            )
        } else {
            format!(
                "block \"{}\": {} at {} is outside of block bounds {}",
                bname,
                identifier,
                range_str(violation.extent),
                range_str(violation.bound),
            )
        }
    }

    for (bname, b) in symgen.iter() {
        let bounds = b.extent();
        for s in b.iter() {
            if let Some(violation) = bounds::symbol_in_bounds(&bounds, s, &b.versions) {
                return Err(violation_str(
                    violation,
                    bname,
                    format!("symbol \"{}\"", s.name),
                ));
            }
        }
        for subblock in b.cursor(&bname.val, Path::new("")).subblocks() {
            if let Some(violation) = bounds::block_in_bounds(&bounds, subblock.block()) {
                return Err(violation_str(
                    violation,
                    bname,
                    format!(
                        "subregion block \"{}::{}\"",
                        subblock.path().display(),
                        subblock.name()
                    ),
                ));
            }
        }
    }
    Ok(())
}

fn check_no_overlap(symgen: &SymGen) -> Result<(), String> {
    type Extent = (Uint, Uint);
    struct ExtentsByVersion<'a> {
        versioned: Option<VersionDep<Vec<(Extent, &'a str)>>>,
        unversioned: Option<Vec<(Extent, &'a str)>>,
    }
    impl<'a> ExtentsByVersion<'a> {
        fn new() -> Self {
            Self {
                versioned: None,
                unversioned: None,
            }
        }
        fn get_ext_endpoints(addr: Uint, len: Option<Uint>) -> Extent {
            // Every object is considered to have a length of at least 1
            (addr, addr + cmp::max(1, len.unwrap_or(1)))
        }
        fn append(&mut self, vers: Option<Version>, ext: Extent, name: &'a str) {
            match vers {
                None => match &mut self.unversioned {
                    None => {
                        self.unversioned = Some(vec![(ext, name)]);
                    }
                    Some(exts) => {
                        exts.push((ext, name));
                    }
                },
                Some(vers) => match &mut self.versioned {
                    None => {
                        self.versioned = Some([(vers, vec![(ext, name)])].into());
                    }
                    Some(exts) => {
                        exts.entry_native(vers)
                            .and_modify(|e| e.push((ext, name)))
                            .or_insert_with(|| vec![(ext, name)]);
                    }
                },
            }
        }
        fn append_symbol(&mut self, symbol: &'a Symbol, versions: Option<&[Version]>) {
            match symbol.extents(versions) {
                MaybeVersionDep::ByVersion(s_exts) => {
                    for (vers, (addrs, len)) in s_exts.iter() {
                        for &addr in addrs.iter() {
                            self.append(
                                Some(vers.clone()),
                                Self::get_ext_endpoints(addr, *len),
                                &symbol.name,
                            )
                        }
                    }
                }
                MaybeVersionDep::Common((addrs, len)) => {
                    // Version expansion wasn't possible, assume unversioned
                    for &addr in addrs.iter() {
                        self.append(None, Self::get_ext_endpoints(addr, len), &symbol.name)
                    }
                }
            }
        }
        fn append_block(&mut self, bname: &'a str, block: &'a Block) {
            match block.extent() {
                MaybeVersionDep::ByVersion(exts) => {
                    for (vers, &(addr, len)) in exts.iter() {
                        // Need to map versions to the internal ordinal space since foreign blocks
                        // can each have their own, possibly incompatible ordinal space.
                        let native_vers = self
                            .versioned
                            .as_ref()
                            .and_then(|versioned| versioned.find_native_version(vers))
                            .unwrap_or(vers)
                            .clone();
                        self.append(Some(native_vers), Self::get_ext_endpoints(addr, len), bname)
                    }
                }
                MaybeVersionDep::Common((addr, len)) => {
                    // Version expansion wasn't possible, assume unversioned
                    self.append(None, Self::get_ext_endpoints(addr, len), bname)
                }
            }
        }
        fn check_exts_for_self_overlap(
            exts: &mut [(Extent, &str)],
            ext_type: &str,
        ) -> Result<(), String> {
            exts.sort_unstable();
            for pair in exts.windows(2) {
                let ((start1, end1), name1) = pair[0];
                let ((start2, end2), name2) = pair[1];
                assert_check(start2 >= end1, || {
                    format!(
                        "overlapping {} \"{}\" ({:#X}-{:#X}) and \"{}\" ({:#X}-{:#X})",
                        ext_type,
                        name1,
                        start1,
                        end1 - 1,
                        name2,
                        start2,
                        end2 - 1
                    )
                })?;
            }
            Ok(())
        }
        fn check_for_self_overlap(&mut self, bname: &str, ext_type: &str) -> Result<(), String> {
            if let Some(exts_by_vers) = self.versioned.as_mut() {
                for (vers, exts) in exts_by_vers.iter_mut() {
                    if let Some(err_stem) = Self::check_exts_for_self_overlap(exts, ext_type).err()
                    {
                        return Err(format!("block \"{}\" [{}]: {}", bname, vers, err_stem));
                    }
                }
            }
            if let Some(exts) = self.unversioned.as_mut() {
                if let Some(err_stem) = Self::check_exts_for_self_overlap(exts, ext_type).err() {
                    return Err(format!("block \"{}\": {}", bname, err_stem));
                }
            }
            Ok(())
        }
        fn check_exts_for_mutual_overlap(
            exts1: &mut [(Extent, &str)],
            exts2: &mut [(Extent, &str)],
            ext_type1: &str,
            ext_type2: &str,
        ) -> Result<(), String> {
            exts1.sort_unstable();
            exts2.sort_unstable();

            let (mut iter1, mut iter2) = (exts1.iter(), exts2.iter());
            let (mut next1, mut next2) = (iter1.next(), iter2.next());
            while let (Some(val1), Some(val2)) = (next1, next2) {
                let ((start1, end1), name1) = val1;
                let ((start2, end2), name2) = val2;
                if start2 >= end1 {
                    next1 = iter1.next();
                } else if start1 >= end2 {
                    next2 = iter2.next();
                } else {
                    return Err(format!(
                        "{} \"{}\" ({:#X}-{:#X}) overlaps with {} \"{}\" ({:#X}-{:#X})",
                        ext_type2,
                        name2,
                        start2,
                        end2 - 1,
                        ext_type1,
                        name1,
                        start1,
                        end1 - 1,
                    ));
                }
            }
            Ok(())
        }
        fn check_for_overlap_with(
            &mut self,
            other: &mut ExtentsByVersion,
            bname: &str,
            ext_type_self: &str,
            ext_type_other: &str,
        ) -> Result<(), String> {
            if let (Some(exts_by_vers), Some(other_exts_by_vers)) =
                (self.versioned.as_mut(), other.versioned.as_mut())
            {
                for (vers, exts) in exts_by_vers.iter_mut() {
                    // Need to use get_mut() since self and other could have different
                    // version ordinal spaces
                    if let Some(other_exts) = other_exts_by_vers.get_mut(vers) {
                        if let Some(err_stem) = Self::check_exts_for_mutual_overlap(
                            exts,
                            other_exts,
                            ext_type_self,
                            ext_type_other,
                        )
                        .err()
                        {
                            return Err(format!("block \"{}\" [{}]: {}", bname, vers, err_stem));
                        }
                    }
                }
            }
            if let (Some(exts), Some(other_exts)) =
                (self.unversioned.as_mut(), other.unversioned.as_mut())
            {
                if let Some(err_stem) = Self::check_exts_for_mutual_overlap(
                    exts,
                    other_exts,
                    ext_type_self,
                    ext_type_other,
                )
                .err()
                {
                    return Err(format!("block \"{}\": {}", bname, err_stem));
                }
            }

            Ok(())
        }
    }

    for (bname, block) in symgen.iter() {
        // Common expansion will be done using the version list if present. If there's no version
        // list, any Common values will only be checked for overlap with other Common values.
        // It really isn't reasonable to expect better inference for what versions a Common value
        // represents because the obvious meaning (any version mentioned by at least one symbol in
        // the list) is sort of overkill to compute (it would require an entire pass over the
        // symbols to construct an "inferred version list"). So it's entirely reasonable to assume
        // that Common values will be treated as having "no version" if no version list is present.
        // If this is undesired behavior, the user should run the full-version-list check first to
        // make sure Common values have a well-defined set to realize to.
        let versions = block.versions.as_deref();
        // ((inclusive start, exclusive end), symbol name)
        let mut extents_by_vers = ExtentsByVersion::new();

        // Gather all function extents
        for s in block.functions.iter() {
            extents_by_vers.append_symbol(s, versions);
        }

        // Compare function extents among themselves for overlaps
        extents_by_vers.check_for_self_overlap(&bname.val, "functions")?;

        let cursor = block.cursor(&bname.val, Path::new(""));
        if cursor.has_subregions() {
            // Gather all data extents
            for s in block.data.iter() {
                extents_by_vers.append_symbol(s, versions);
            }

            // Gather all subregion extents
            let mut subregion_extents_by_vers = ExtentsByVersion::new();
            for subblock in cursor.subblocks() {
                // For subregions, each subblock has its own version list, so use that for Common
                // expansion rather than the parent block's version list
                subregion_extents_by_vers.append_block(subblock.name(), subblock.block());
            }

            // Compare subregion extents among themselves for overlaps
            subregion_extents_by_vers.check_for_self_overlap(&bname.val, "subregions")?;
            // Compare subregion extents with function/data extents for overlaps
            subregion_extents_by_vers.check_for_overlap_with(
                &mut extents_by_vers,
                &bname.val,
                "subregion",
                "symbol",
            )?;
        }
    }
    Ok(())
}

fn symbols_name_check<'s, F, I>(
    symgen: &'s SymGen,
    convs: &BTreeSet<NamingConvention>,
    block_iter: F,
    symbol_type: &str,
) -> Result<(), String>
where
    F: Fn(&'s Block) -> I,
    I: Iterator<Item = &'s Symbol>,
{
    let mut bad_names: BTreeMap<&OrdString, HashSet<&str>> = BTreeMap::new();
    for (bname, b) in symgen.iter() {
        for s in block_iter(b) {
            if !NamingConvention::check_multiple(convs, &s.name) {
                bad_names.entry(bname).or_default().insert(&s.name);
            }
            for a in s.iter_aliases() {
                if !NamingConvention::check_multiple(convs, a) {
                    bad_names.entry(bname).or_default().insert(a);
                }
            }
        }
    }
    assert_check(bad_names.is_empty(), || {
        format!(
            "Found invalid {} names:\n{}",
            symbol_type,
            bad_names
                .into_iter()
                .map(|(bname, names)| {
                    let mut names: Vec<_> = names.into_iter().collect();
                    names.sort_unstable();
                    format!("- block \"{}\": [{}]", bname, names.join(", "))
                })
                .collect::<Vec<_>>()
                .join("\n")
        )
    })
}

fn check_function_names(symgen: &SymGen, convs: &BTreeSet<NamingConvention>) -> Result<(), String> {
    symbols_name_check(symgen, convs, |b: &Block| b.functions.iter(), "function")
}

fn check_data_names(symgen: &SymGen, convs: &BTreeSet<NamingConvention>) -> Result<(), String> {
    symbols_name_check(symgen, convs, |b: &Block| b.data.iter(), "data")
}

/// Validates a given `input_file` under the specified `checks`.
///
/// In `recursive` mode, subregion files are also validated.
///
/// Returns a `Vec<(PathBuf, CheckResult)>` with the results of all checks on all the files
/// validated, if all checks were run without encountering any fatal errors.
///
/// # Examples
/// ```ignore
/// let results = run_checks(
///     "/path/to/symbols.yml",
///     &[
///         Check::ExplicitVersions,
///         Check::FunctionNames([NamingConvention::SnakeCase].into()),
///     ],
///     true,
/// )
/// .expect("Fatal error occurred");
/// ```
pub fn run_checks<P: AsRef<Path>>(
    input_file: P,
    checks: &[Check],
    recursive: bool,
) -> Result<Vec<(PathBuf, CheckResult)>, Box<dyn Error>> {
    /// For returning either a [`Once`] iterator or an [`Empty`] iterator, while still allowing
    /// static dispatch.
    enum OnceOrEmpty<T> {
        Once(iter::Once<T>),
        Empty(iter::Empty<T>),
    }
    impl<T> Iterator for OnceOrEmpty<T> {
        type Item = T;

        fn next(&mut self) -> Option<Self::Item> {
            match self {
                Self::Once(o) => o.next(),
                Self::Empty(e) => e.next(),
            }
        }
    }

    let input_file = input_file.as_ref();
    let mut contents = {
        let f = File::open(input_file)?;
        SymGen::read(&f)?
    };
    if recursive {
        contents.resolve_subregions(Subregion::subregion_dir(input_file), |p| File::open(p))?;
    }
    Ok(checks
        .iter()
        .flat_map(|chk| {
            let check_results = contents
                .cursor(input_file)
                .dtraverse()
                .map(move |cursor| (cursor.path().to_owned(), chk.run(cursor.symgen())));
            if let (Check::UniqueSymbols, true) =
                (chk, contents.cursor(input_file).has_subregions())
            {
                // Recursive UniqueSymbols is a special case.
                // Add a cross-subregion uniqueness check that spans all subregions
                check_results.chain(OnceOrEmpty::Once(iter::once((
                    input_file.to_owned(),
                    Check::UniqueSymbolsAcrossSubregions.run(&contents),
                ))))
            } else {
                check_results.chain(OnceOrEmpty::Empty(iter::empty()))
            }
        })
        .collect())
}

/// Prints check results similar to `cargo test` output.
fn print_report(results: &[(PathBuf, CheckResult)]) -> io::Result<()> {
    let mut stdout = StandardStream::stdout(ColorChoice::Always);
    let mut print_colored_report = || -> io::Result<()> {
        let mut color = ColorSpec::new();

        // Results list
        for (name, r) in results {
            stdout.reset()?;
            write!(&mut stdout, "check {}::{} ... ", name.display(), r.check)?;
            if r.succeeded {
                stdout.set_color(color.set_fg(Some(Color::Green)))?;
                writeln!(&mut stdout, "ok")?;
            } else {
                stdout.set_color(color.set_fg(Some(Color::Red)))?;
                writeln!(&mut stdout, "FAILED")?;
            }
        }

        stdout.reset()?;
        writeln!(&mut stdout)?;
        let n_failed = results.iter().filter(|(_, r)| !r.succeeded).count();
        let n_passed = results.len() - n_failed;

        // Failure details
        if n_failed > 0 {
            writeln!(&mut stdout, "failures:")?;
            writeln!(&mut stdout)?;
            for (name, r) in results.iter().filter(|(_, r)| !r.succeeded) {
                writeln!(&mut stdout, "---- [{}] {} ----", name.display(), r.check)?;
                if let Some(msg) = &r.details {
                    writeln!(&mut stdout, "{}", msg)?;
                }
                writeln!(&mut stdout)?;
            }
        }

        // Results summary
        write!(&mut stdout, "check result: ")?;
        if n_failed == 0 {
            stdout.set_color(color.set_fg(Some(Color::Green)))?;
            write!(&mut stdout, "ok")?;
        } else {
            stdout.set_color(color.set_fg(Some(Color::Red)))?;
            write!(&mut stdout, "FAILED")?;
        }
        stdout.reset()?;
        writeln!(&mut stdout, ". {} passed; {} failed", n_passed, n_failed)?;

        Ok(())
    };
    let res = print_colored_report();
    // Always try to clean up color settings before returning
    stdout.reset()?; // throw away Ok() output
    res
}

/// Validates a given set of `input_files` under the specified `checks`, and prints a summary of
/// the results.
///
/// In `recursive` mode, subregion files of the given input files are also validated.
///
/// If all checks were run without encountering a fatal error, returns `true` if all checks passed
/// and `false` otherwise.
///
/// # Examples
/// ```ignore
/// let passed = run_and_print_checks(
///     ["/path/to/symbols.yml", "/path/to/other_symbols.yml"],
///     &[
///         Check::ExplicitVersions,
///         Check::FunctionNames([NamingConvention::SnakeCase].into()),
///     ],
///     true,
/// )
/// .expect("Fatal error occurred");
/// ```
pub fn run_and_print_checks<I, P>(
    input_files: I,
    checks: &[Check],
    recursive: bool,
) -> Result<bool, Box<dyn Error>>
where
    P: AsRef<Path>,
    I: AsRef<[P]>,
{
    let input_files = input_files.as_ref();
    // At least this many check results, but there could be more in recursive mode
    let mut results = Vec::with_capacity(input_files.len() * checks.len());
    let mut errors = Vec::with_capacity(input_files.len());
    for input_file in input_files {
        match run_checks(input_file, checks, recursive) {
            Ok(result) => results.extend(result.into_iter()),
            Err(e) => errors.push((input_file.as_ref().to_string_lossy().into_owned(), e)),
        }
    }

    // Best-effort: print what we have, even if some checks errored
    print_report(&results)?;

    if !errors.is_empty() {
        return Err(MultiFileError {
            base_msg: "Could not complete checks".to_string(),
            errors,
        }
        .into());
    }
    Ok(results.iter().all(|(_, r)| r.succeeded))
}

#[cfg(test)]
mod tests {
    use super::super::data_formats::symgen_yml::test_utils;
    use super::*;

    #[cfg(test)]
    mod naming_convention_tests {
        use super::*;

        fn run_name_checks<const M: usize, const N: usize>(
            conv: NamingConvention,
            good: [&str; M],
            bad: [&str; N],
        ) {
            for name in good.iter() {
                assert!(conv.check(name));
            }
            for name in bad.iter() {
                assert!(!conv.check(name));
            }
        }

        #[test]
        fn test_identifier() {
            run_name_checks(
                NamingConvention::Identifier,
                ["some_function", "àéïõç"],
                [" f", "f ", "f f", "f-f", "1abc"],
            )
        }

        #[test]
        fn test_snake_case() {
            run_name_checks(
                NamingConvention::SnakeCase,
                ["snake_case", "_case", "snake", "snake_", "with_number1"],
                ["SCREAMING_SNAKE", "camelCase", "PascalCase", "Caps"],
            )
        }

        #[test]
        fn test_screaming_snake_case() {
            run_name_checks(
                NamingConvention::ScreamingSnakeCase,
                [
                    "SCREAMING_SNAKE",
                    "_SNAKE",
                    "SNAKE",
                    "SCREAMING_",
                    "WITH_NUMBER1",
                ],
                ["snake_case", "camelCase", "PascalCase", "Caps", "lower"],
            )
        }

        #[test]
        fn test_camel_case() {
            run_name_checks(
                NamingConvention::CamelCase,
                ["camelCase", "camel", "withAWord", "withANumber1"],
                ["snake_case", "SCREAMING_SNAKE", "PascalCase", "Caps"],
            )
        }

        #[test]
        fn test_pascal_case() {
            run_name_checks(
                NamingConvention::PascalCase,
                ["PascalCase", "Pascal", "WithAWord", "WithANumber1"],
                ["snake_case", "SCREAMING_SNAKE", "camelCase", "lower"],
            )
        }

        #[test]
        fn test_camel_snake_case() {
            run_name_checks(
                NamingConvention::CamelSnakeCase,
                [
                    "camelCase",
                    "camel",
                    "withAWord",
                    "withANumber1",
                    "camelPart_PascalPart",
                    "camelPart1_PascalPart2_PascalPart3",
                ],
                [
                    "snake_case",
                    "SCREAMING_SNAKE",
                    "PascalCase",
                    "Caps",
                    "camelPart_camelPart",
                    "PascalPart_camelPart",
                ],
            )
        }

        #[test]
        fn test_underscore_separated_camel_case() {
            run_name_checks(
                NamingConvention::UnderscoreSeparatedCamelCase,
                [
                    "camelCase",
                    "snake_case",
                    "camel",
                    "withAWord",
                    "withANumber1",
                    "camelPart_camelPart",
                    "camelPart1_camelPart2_camelPart3",
                ],
                [
                    "SCREAMING_SNAKE",
                    "PascalCase",
                    "Caps",
                    "camelPart_PascalPart",
                    "PascalPart_camelPart",
                ],
            )
        }

        #[test]
        fn test_pascal_snake_case() {
            run_name_checks(
                NamingConvention::PascalSnakeCase,
                [
                    "PascalCase",
                    "Pascal",
                    "WithAWord",
                    "WithANumber1",
                    "PascalPart_PascalPart",
                    "PascalPart1_PascalPart2_PascalPart3",
                ],
                [
                    "snake_case",
                    "SCREAMING_SNAKE",
                    "camelCase",
                    "lower",
                    "camelPart_PascalPart",
                    "PascalPart_camelPart",
                ],
            )
        }

        #[test]
        fn test_flatcase() {
            run_name_checks(
                NamingConvention::FlatCase,
                ["flatcase", "withanumber1"],
                [
                    "snake_case",
                    "SCREAMING_SNAKE",
                    "camelCase",
                    "Caps",
                    "UPPER",
                ],
            )
        }

        #[test]
        fn test_upper_flatcase() {
            run_name_checks(
                NamingConvention::UpperFlatCase,
                ["UPPERFLATCASE", "WITHANUMBER1"],
                [
                    "snake_case",
                    "SCREAMING_SNAKE",
                    "camelCase",
                    "Caps",
                    "lower",
                ],
            )
        }

        #[test]
        fn test_snake_or_pascal_case() {
            let convs = BTreeSet::from([NamingConvention::SnakeCase, NamingConvention::PascalCase]);
            assert!(NamingConvention::check_multiple(&convs, "snake_case"));
            assert!(NamingConvention::check_multiple(&convs, "PascalCase"));
            assert!(!NamingConvention::check_multiple(
                &convs,
                "mixed_snake_PascalCase"
            ));
        }
    }

    fn get_test_symgen() -> SymGen {
        SymGen::read(
            r"
            main:
              versions:
                - v1
                - v2
              address:
                v1: 0x2000000
                v2: 0x2000000
              length:
                v1: 0x100000
                v2: 0x100000
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
                      - 0x2000000
                      - 0x2002000
                    v2: 0x2004000
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
            "
            .as_bytes(),
        )
        .expect("Read failed")
    }

    fn get_test_symgen_with_subregions() -> SymGen {
        test_utils::get_symgen_with_subregions(
            r"
            main:
              versions:
                - v1
                - v2
                - v3
              address:
                v1: 0x2000000
                v2: 0x2000000
                v3: 0x2000000
              length:
                v1: 0x100000
                v2: 0x100000
                v3: 0x100000
              subregions:
                - sub1.yml
                - sub2.yml
              functions: []
              data: []
            ",
            &[
                (
                    "sub1.yml",
                    r"
                    sub1:
                      versions:
                        - v3
                        - v1
                      address:
                        v3: 0x2000000
                        v1: 0x2000000
                      length:
                        v3: 0x100
                        v1: 0x100
                      functions:
                        - name: sub1_fn
                          address:
                            v3: 0x2000000
                            v1: 0x2000000
                      data: []
                    ",
                ),
                (
                    "sub2.yml",
                    r"
                    sub2:
                      versions:
                        - v2
                      address:
                        v2: 0x20FFF00
                      length:
                        v2: 0x100
                      functions: []
                      data:
                        - name: sub2_data
                          address:
                            v2: 0x20FFF80
                          length:
                            v2: 0x80
                    ",
                ),
            ],
        )
    }

    fn get_main_block(symgen: &mut SymGen) -> &mut Block {
        symgen
            .get_mut(&symgen.block_key("main").expect("No block \"main\"").clone())
            .unwrap()
    }

    fn get_subregion_block(symgen: &mut SymGen, i: usize) -> &mut Block {
        let subregion = get_main_block(symgen)
            .subregions
            .as_mut()
            .expect("No subregions")
            .get_mut(i)
            .expect("Invalid index")
            .contents
            .as_mut()
            .expect("Subregion has no contents");
        subregion
            .blocks_mut()
            .next()
            .expect("Subregion has no blocks")
    }

    #[test]
    fn test_explicit_versions() {
        let mut symgen = get_test_symgen();
        assert!(check_explicit_versions(&symgen).is_ok());

        let block = get_main_block(&mut symgen);
        // Change the block address to use an implicit version
        block.address = MaybeVersionDep::Common(0x2000000);
        assert!(check_explicit_versions(&symgen).is_err());
    }

    #[test]
    fn test_complete_version_list() {
        let mut symgen = get_test_symgen();
        assert!(check_complete_version_list(&symgen).is_ok());

        let block = get_main_block(&mut symgen);
        // Delete the block version list
        block.versions = None;
        assert!(check_complete_version_list(&symgen).is_err());
    }

    #[test]
    fn test_complete_version_list_with_subregions() {
        let mut symgen = get_test_symgen_with_subregions();
        assert!(check_complete_version_list(&symgen).is_ok());

        let block = get_main_block(&mut symgen);
        // Delete one of the versions
        block.versions.as_mut().unwrap().pop();
        assert!(check_complete_version_list(&symgen).is_err());
    }

    #[test]
    fn test_unique_symbols() {
        let mut symgen = get_test_symgen();
        assert!(check_unique_symbols(&symgen).is_ok());

        let block = get_main_block(&mut symgen);
        // Copy the function symbols to the data symbols so they clash
        block.data = block.functions.clone();
        assert!(check_unique_symbols(&symgen).is_err());
    }

    #[test]
    fn test_unique_symbols_with_aliases() {
        let mut symgen = get_test_symgen();
        assert!(check_unique_symbols(&symgen).is_ok());

        let block = get_main_block(&mut symgen);
        // Give one function an alias that clashes with another symbol
        block
            .functions
            .get_mut(0)
            .expect("symgen has no functions")
            .aliases = Some(vec![block.functions[1].name.clone()]);
        assert!(check_unique_symbols(&symgen).is_err());

        let mut symgen = get_test_symgen();
        let block = get_main_block(&mut symgen);
        // Give functions clashing aliases
        block
            .functions
            .get_mut(0)
            .expect("symgen has no functions")
            .aliases = Some(vec!["alias".to_string()]);
        block
            .functions
            .get_mut(1)
            .expect("symgen doesn't have two functions")
            .aliases = Some(vec!["alias".to_string()]);
        assert!(check_unique_symbols(&symgen).is_err());
    }

    #[test]
    fn test_unique_subregions() {
        let mut symgen = get_test_symgen_with_subregions();
        assert!(check_unique_symbols(&symgen).is_ok());

        let block = get_main_block(&mut symgen);
        // Add a duplicate subregion
        let subregion = Subregion {
            name: block.subregions.as_ref().unwrap()[0].name.clone(),
            contents: None,
        };
        block.subregions.as_mut().unwrap().push(subregion);
        assert!(check_unique_symbols(&symgen).is_err());
    }

    #[test]
    fn test_unique_symbols_across_subregions() {
        let mut symgen = get_test_symgen_with_subregions();
        assert!(check_unique_symbols_across_subregions(&symgen).is_ok());

        // Insert a copy of a subregion's symbol into the main block
        let symbol = get_subregion_block(&mut symgen, 0)
            .functions
            .iter()
            .next()
            .expect("subregion block has no functions")
            .clone();
        let block = get_main_block(&mut symgen);
        block.functions.push(symbol);
        assert!(check_unique_symbols_across_subregions(&symgen).is_err());
    }

    #[test]
    fn test_unique_symbols_with_aliases_across_subregions() {
        let mut symgen = get_test_symgen_with_subregions();
        assert!(check_unique_symbols_across_subregions(&symgen).is_ok());

        // Insert an aliased copy of a subregion's symbol into the main block
        let mut symbol = get_subregion_block(&mut symgen, 0)
            .functions
            .iter()
            .next()
            .expect("subregion block has no functions")
            .clone();
        symbol.aliases = Some(vec![symbol.name]);
        symbol.name = "aliased_fn".to_string();
        let block = get_main_block(&mut symgen);
        block.functions.push(symbol);
        assert!(check_unique_symbols_across_subregions(&symgen).is_err());
    }

    #[test]
    fn test_in_bounds_symbols() {
        let mut symgen = get_test_symgen();
        assert!(check_in_bounds_symbols(&symgen).is_ok());

        let block = get_main_block(&mut symgen);
        // Set the block length to 0 so the symbols end up out of bounds
        for l in block.length.values_mut() {
            *l = 0;
        }
        assert!(check_in_bounds_symbols(&symgen).is_err());
    }

    #[test]
    fn test_in_bounds_subregions() {
        let mut symgen = get_test_symgen_with_subregions();
        assert!(check_in_bounds_symbols(&symgen).is_ok());

        let block = get_main_block(&mut symgen);
        // Shrink the main block so the sub2 subregion ends up out of bounds
        *block.length.get_mut(Some(&"v2".into())).unwrap() -= 0x80;
        assert!(check_in_bounds_symbols(&symgen).is_err());
    }

    #[test]
    fn test_no_overlap() {
        let mut symgen = get_test_symgen();
        assert!(check_no_overlap(&symgen).is_ok());

        let block = get_main_block(&mut symgen);
        // Swap the address of the second function to match the first, causing an overlap
        let function = block
            .functions
            .iter()
            .next()
            .expect("symgen has no functions")
            .clone();
        let mut overlapping = block
            .functions
            .iter()
            .next()
            .expect("symgen does not have two functions")
            .clone();
        let addr = overlapping
            .address
            .get_mut_native(block.version("v1"))
            .unwrap();
        *addr = function
            .address
            .get_native(block.version("v1"))
            .unwrap()
            .clone();
        block.functions = [function, overlapping].into();
        assert!(check_no_overlap(&symgen).is_err());
    }

    #[test]
    fn test_no_overlap_common_with_version_list() {
        let mut symgen = SymGen::read(
            r"
            main:
              versions:
                - v1
                - v2
              address: 0x2000000
              length: 0x100000
              functions:
                - name: fn1
                  address: 0x2001000
                  length: 0x1000
                - name: fn2
                  address:
                    v1:
                      - 0x2000000
                      - 0x2002000
                    v2: 0x2004000
              data: []
        "
            .as_bytes(),
        )
        .expect("Read failed");

        assert!(check_no_overlap(&symgen).is_ok());

        let block = get_main_block(&mut symgen);
        // Swap the address of the first function to match one of the versions in the second,
        // causing an overlap
        let mut function = block
            .functions
            .iter()
            .next()
            .expect("symgen has no functions")
            .clone();
        let overlapping = block
            .functions
            .iter()
            .next()
            .expect("symgen does not have two functions")
            .clone();
        let addr = function.address.get_mut_native(None).unwrap();
        *addr = overlapping
            .address
            .get_native(block.version("v1"))
            .unwrap()
            .clone();
        block.functions = [function, overlapping].into();
        assert!(check_no_overlap(&symgen).is_err());
    }

    #[test]
    fn test_no_overlap_common_no_version_list() {
        let mut symgen = SymGen::read(
            r"
            main:
              address: 0x2000000
              length: 0x100000
              functions:
                - name: fn1
                  address: 0x2001000
                  length: 0x1000
                - name: fn2
                  address:
                    - 0x2000000
                    - 0x2002000
              data: []
        "
            .as_bytes(),
        )
        .expect("Read failed");

        assert!(check_no_overlap(&symgen).is_ok());

        let block = get_main_block(&mut symgen);
        // Swap the address of the second function to match the first, causing an overlap
        let function = block
            .functions
            .iter()
            .next()
            .expect("symgen has no functions")
            .clone();
        let mut overlapping = block
            .functions
            .iter()
            .next()
            .expect("symgen does not have two functions")
            .clone();
        let addr = overlapping.address.get_mut_native(None).unwrap();
        *addr = function.address.get_native(None).unwrap().clone();
        block.functions = [function, overlapping].into();
        assert!(check_no_overlap(&symgen).is_err());
    }

    #[test]
    fn test_no_overlap_with_subregions() {
        let mut symgen = get_test_symgen_with_subregions();
        assert!(check_no_overlap(&symgen).is_ok());

        // Add a symbol to the main block that overlaps with a subregion symbol
        let address = *get_subregion_block(&mut symgen, 0)
            .address
            .get(Some(&"v1".into()))
            .unwrap();
        let block = get_main_block(&mut symgen);
        block.functions.push(Symbol {
            name: String::from("main_fn"),
            aliases: None,
            address: MaybeVersionDep::ByVersion([("v1".into(), [address].into())].into()),
            length: None,
            description: None,
        });
        assert!(check_no_overlap(&symgen).is_err());
    }

    #[test]
    fn test_symbols_name_check() {
        let mut symgen = get_test_symgen();
        assert!(check_function_names(&symgen, &[NamingConvention::SnakeCase].into()).is_ok());
        assert!(check_data_names(&symgen, &[NamingConvention::ScreamingSnakeCase].into()).is_ok());

        let block = get_main_block(&mut symgen);
        // Set the function to have the wrong case
        block
            .functions
            .get_mut(0)
            .expect("symgen has no functions")
            .name = "PascalCase".to_string();
        assert!(check_function_names(&symgen, &[NamingConvention::SnakeCase].into()).is_err());
        // Loosen the check to accept the wrong case
        assert!(check_function_names(
            &symgen,
            &[NamingConvention::SnakeCase, NamingConvention::PascalCase].into()
        )
        .is_ok());
        // Make sure that if all case conventions don't match, the check still fails
        assert!(check_function_names(
            &symgen,
            &[NamingConvention::SnakeCase, NamingConvention::CamelCase].into()
        )
        .is_err());

        // reborrow
        let block = get_main_block(&mut symgen);
        // Set the data to have the wrong case
        block.data.get_mut(0).expect("symgen has no data").name = "snake_case".to_string();
        assert!(check_data_names(&symgen, &[NamingConvention::ScreamingSnakeCase].into()).is_err());
    }

    #[test]
    fn test_symbols_alias_name_check() {
        let mut symgen = get_test_symgen();
        assert!(check_function_names(&symgen, &[NamingConvention::SnakeCase].into()).is_ok());
        assert!(check_data_names(&symgen, &[NamingConvention::ScreamingSnakeCase].into()).is_ok());

        let block = get_main_block(&mut symgen);
        // Set the function to have an alias with the wrong case
        block
            .functions
            .get_mut(0)
            .expect("symgen has no functions")
            .aliases = Some(vec!["PascalCase".to_string()]);
        assert!(check_function_names(&symgen, &[NamingConvention::SnakeCase].into()).is_err());

        // reborrow
        let block = get_main_block(&mut symgen);
        // Set the data to have an alias with the wrong case
        block.data.get_mut(0).expect("symgen has no data").aliases =
            Some(vec!["snake_case".to_string()]);
        assert!(check_data_names(&symgen, &[NamingConvention::ScreamingSnakeCase].into()).is_err());
    }
}
