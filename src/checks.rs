//! Validating the substantive contents of `resymgen` YAML files. Implements the `check` command.

use std::cmp;
use std::collections::{BTreeMap, HashSet};
use std::error::Error;
use std::fmt::{self, Display, Formatter};
use std::fs::File;
use std::io::{self, Write};
use std::path::Path;

use syn::{self, Ident};
use termcolor::{Color, ColorChoice, ColorSpec, StandardStream, WriteColor};

use super::data_formats::symgen_yml::bounds;
use super::data_formats::symgen_yml::{
    Block, MaybeVersionDep, OrdString, SymGen, Symbol, Uint, Version, VersionDep,
};
use super::util::MultiFileError;

/// Naming conventions for symbol names.
#[derive(Debug, Clone, Copy)]
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
                if let Some(c) = name.chars().rev().next() {
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
        }
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
#[derive(Debug, Clone, Copy)]
pub enum Check {
    /// All addresses and lengths (for both blocks and symbols) must be explicitly listed by version.
    ExplicitVersions,
    /// Any version used within a version-dependent address or length must appear in the version
    /// list of the parent block.
    CompleteVersionList,
    /// YAML maps (key-value pair lists) must have at least one entry.
    NonEmptyMaps,
    /// Symbol names within a block must be unique.
    UniqueSymbols,
    /// Symbols must fall within the address range of the parent block.
    InBoundsSymbols,
    /// Function symbols must not overlap with each other for a given block and version.
    NoFunctionOverlap,
    /// Function symbol names must adhere to the specified [`NamingConvention`].
    FunctionNames(NamingConvention),
    /// Data symbol names must adhere to the specified [`NamingConvention`].
    DataNames(NamingConvention),
}

/// The result of a [Check] run on `resymgen` YAML symbol tables.
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
            Self::InBoundsSymbols => self.result(check_in_bounds_symbols(symgen)),
            Self::NoFunctionOverlap => self.result(check_no_function_overlap(symgen)),
            Self::FunctionNames(conv) => self.result(check_function_names(symgen, *conv)),
            Self::DataNames(conv) => self.result(check_data_names(symgen, *conv)),
        }
    }
    fn result(&self, raw_result: Result<(), String>) -> CheckResult {
        CheckResult {
            check: *self,
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

/// Runs a simple boolean check on all address/length fields. The generic <'a> is the lifetime of
/// the [`SymGen`] object to check, and allows the checker context to hold references to a block if
/// needed.
trait SimpleAddrLenChecker<'a> {
    fn init_context(
        &mut self,
        _block_name: &'a OrdString,
        _block: &'a Block,
    ) -> Result<(), String> {
        Ok(())
    }
    fn check_val<T>(&self, val: &'a MaybeVersionDep<T>) -> bool;
    fn error_stem<T>(&self, val: &'a MaybeVersionDep<T>) -> String;

    fn check_symgen(&mut self, symgen: &'a SymGen) -> Result<(), String> {
        for (bname, b) in symgen.iter() {
            self.init_context(bname, b)?;

            assert_check(self.check_val(&b.address), || {
                format!(
                    "block \"{}\": address {}",
                    bname,
                    self.error_stem(&b.address)
                )
            })?;
            assert_check(self.check_val(&b.length), || {
                format!("block \"{}\": length {}", bname, self.error_stem(&b.length))
            })?;
            for s in b.iter() {
                assert_check(self.check_val(&s.address), || {
                    format!(
                        "block \"{}\", symbol \"{}\": address {}",
                        bname,
                        s.name,
                        self.error_stem(&s.address)
                    )
                })?;
                if let Some(l) = &s.length {
                    assert_check(self.check_val(l), || {
                        format!(
                            "block \"{}\", symbol \"{}\": length {}",
                            bname,
                            s.name,
                            self.error_stem(l)
                        )
                    })?;
                }
            }
        }
        Ok(())
    }
}

fn check_explicit_versions(symgen: &SymGen) -> Result<(), String> {
    struct ExplicitVersionChecker {}
    impl SimpleAddrLenChecker<'_> for ExplicitVersionChecker {
        fn check_val<T>(&self, val: &MaybeVersionDep<T>) -> bool {
            !val.is_common()
        }
        fn error_stem<T>(&self, _val: &MaybeVersionDep<T>) -> String {
            "has no version".to_string()
        }
    }

    let mut c = ExplicitVersionChecker {};
    c.check_symgen(symgen)
}

fn check_complete_version_list(symgen: &SymGen) -> Result<(), String> {
    struct CompleteVersionListChecker<'a> {
        block_versions: HashSet<&'a Version>,
    }
    impl<'a> SimpleAddrLenChecker<'a> for CompleteVersionListChecker<'a> {
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
        fn check_val<T>(&self, val: &MaybeVersionDep<T>) -> bool {
            val.versions()
                .collect::<HashSet<_>>()
                .is_subset(&self.block_versions)
        }
        fn error_stem<T>(&self, val: &MaybeVersionDep<T>) -> String {
            let mut vers_diff: Vec<String> = val
                .versions()
                .collect::<HashSet<_>>()
                .difference(&self.block_versions)
                .map(|v| v.name().to_string())
                .collect();
            vers_diff.sort();
            format!("contains unknown versions: {:?}", vers_diff)
        }
    }

    let mut c = CompleteVersionListChecker {
        block_versions: HashSet::new(),
    };
    c.check_symgen(symgen)
}

fn check_nonempty_maps(symgen: &SymGen) -> Result<(), String> {
    struct NonEmptyMapChecker {}
    impl SimpleAddrLenChecker<'_> for NonEmptyMapChecker {
        fn check_val<T>(&self, val: &MaybeVersionDep<T>) -> bool {
            val.len() > 0
        }
        fn error_stem<T>(&self, _val: &MaybeVersionDep<T>) -> String {
            "is empty".to_string()
        }
    }

    let mut c = NonEmptyMapChecker {};
    c.check_symgen(symgen)
}

fn check_unique_symbols(symgen: &SymGen) -> Result<(), String> {
    let mut duplicate_names: BTreeMap<&OrdString, HashSet<&str>> = BTreeMap::new();
    for (bname, b) in symgen.iter() {
        let mut names: HashSet<&str> = HashSet::new();
        for name in b.iter().map(|s| &s.name) {
            if !names.insert(name) {
                duplicate_names.entry(bname).or_default().insert(name);
            }
        }
    }
    assert_check(duplicate_names.is_empty(), || {
        format!(
            "Found duplicate symbol names:\n{}",
            duplicate_names
                .iter()
                .map(|(bname, names)| {
                    let mut names: Vec<_> = names.iter().collect();
                    names.sort();
                    format!("- block \"{}\": {:?}", bname, names)
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

    for (bname, b) in symgen.iter() {
        let bounds = b.extent();
        for s in b.iter() {
            if let Some(violation) = bounds::symbol_in_bounds(&bounds, s, &b.versions) {
                return Err(if let Some(vers) = &violation.version {
                    format!(
                        "block \"{}\" [{}]: symbol \"{}\" at {} is outside of block bounds {}",
                        bname,
                        vers,
                        s.name,
                        range_str(violation.extent),
                        range_str(violation.bound),
                    )
                } else {
                    format!(
                        "block \"{}\": symbol \"{}\" at {} is outside of block bounds {}",
                        bname,
                        s.name,
                        range_str(violation.extent),
                        range_str(violation.bound),
                    )
                });
            }
        }
    }
    Ok(())
}

fn check_no_function_overlap(symgen: &SymGen) -> Result<(), String> {
    type Extent = (Uint, Uint);

    fn append_ext<'a>(
        extents: &mut Option<VersionDep<Vec<(Extent, &'a str)>>>,
        vers: Version,
        ext: Extent,
        name: &'a str,
    ) {
        match extents {
            None => {
                *extents = Some([(vers, vec![(ext, name)])].into());
            }
            Some(exts) => {
                exts.entry(vers)
                    .and_modify(|e| e.push((ext, name)))
                    .or_insert_with(|| vec![(ext, name)]);
            }
        }
    }

    for (bname, b) in symgen.iter() {
        // If there's no version list, use an empty list for Common expansion. It really isn't
        // reasonable to expect better inference for what versions a Common value represents
        // because the obvious meaning (any version mentioned by at least one symbol in the list)
        // is sort of overkill to compute (it would require an entire pass over the symbols to
        // construct an "inferred version list"). So it's entirely reasonable to assume that
        // Common values will be ignored when no version list is present. If this is undesired
        // behavior, the user should run the full-version-list check first to make sure Common
        // values have a well-defined set to realize to.
        let versions = b.versions.as_deref().unwrap_or(&[]);
        // ((inclusive start, exclusive end), symbol name)
        let mut extents_by_vers: Option<VersionDep<Vec<(Extent, &str)>>> = None;

        // Gather all function extents
        for s in b.functions.iter() {
            if let MaybeVersionDep::ByVersion(s_exts) = s.extents(Some(versions)) {
                for (vers, (addrs, len)) in s_exts.iter() {
                    for addr in addrs.iter() {
                        // Every function is considered to have a length of at least 1
                        append_ext(
                            &mut extents_by_vers,
                            vers.clone(),
                            (*addr, *addr + cmp::max(1, len.unwrap_or(1))),
                            &s.name,
                        )
                    }
                }
            } else {
                // This should never happen because versions is never None
                panic!(
                    "Symbol::extents({:?}) resolved to Common for {:?}",
                    versions, s
                )
            }
        }

        // Compare adjacent extents for overlaps
        if let Some(mut exts_by_vers) = extents_by_vers {
            for (vers, exts) in exts_by_vers.iter_mut() {
                exts.sort_unstable();
                for pair in exts.windows(2) {
                    let ((start1, end1), name1) = pair[0];
                    let ((start2, end2), name2) = pair[1];
                    assert_check(start2 >= end1, || {
                        format!("block \"{}\" [{}]: overlapping functions \"{}\" ({:#X}-{:#X}) and \"{}\" ({:#X}-{:#X})", bname, vers, name1, start1, end1-1, name2, start2, end2-1)
                    })?;
                }
            }
        }
    }
    Ok(())
}

fn symbols_name_check<'s, F, I>(
    symgen: &'s SymGen,
    conv: NamingConvention,
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
            if !conv.check(&s.name) {
                bad_names.entry(bname).or_default().insert(&s.name);
            }
        }
    }
    assert_check(bad_names.is_empty(), || {
        format!(
            "Found invalid {} names:\n{}",
            symbol_type,
            bad_names
                .iter()
                .map(|(bname, names)| {
                    let mut names: Vec<_> = names.iter().collect();
                    names.sort();
                    format!("- block \"{}\": {:?}", bname, names)
                })
                .collect::<Vec<_>>()
                .join("\n")
        )
    })
}

fn check_function_names(symgen: &SymGen, conv: NamingConvention) -> Result<(), String> {
    symbols_name_check(symgen, conv, |b: &Block| b.functions.iter(), "function")
}

fn check_data_names(symgen: &SymGen, conv: NamingConvention) -> Result<(), String> {
    symbols_name_check(symgen, conv, |b: &Block| b.data.iter(), "data")
}

/// Validates a given `input_file` under the specified `checks`.
///
/// Returns a [`Vec<CheckResult>`] corresponding to `checks` if all checks were run without
/// encountering any fatal errors.
///
/// # Examples
/// ```ignore
/// let results = run_checks(
///     "/path/to/symbols.yml",
///     &[
///         Check::ExplicitVersions,
///         Check::FunctionNames(NamingConvention::SnakeCase),
///     ],
/// )
/// .expect("Fatal error occurred");
/// ```
pub fn run_checks<P: AsRef<Path>>(
    input_file: P,
    checks: &[Check],
) -> Result<Vec<CheckResult>, Box<dyn Error>> {
    let f = File::open(input_file)?;
    let contents = SymGen::read(&f)?;
    Ok(checks.iter().map(|chk| chk.run(&contents)).collect())
}

/// Prints check results similar to `cargo test` output.
fn print_report(results: &[(String, CheckResult)]) -> io::Result<()> {
    let mut stdout = StandardStream::stdout(ColorChoice::Always);
    let mut print_colored_report = || -> io::Result<()> {
        let mut color = ColorSpec::new();

        // Results list
        for (name, r) in results {
            stdout.reset()?;
            write!(&mut stdout, "check {}::{} ... ", name, r.check)?;
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
                writeln!(&mut stdout, "---- [{}] {} ----", name, r.check)?;
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
    if let Err(e) = stdout.reset() {
        Err(e)
    } else {
        res
    }
}

/// Validates a given set of `input_files` under the specified `checks`, and prints a summary of
/// the results.
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
///         Check::FunctionNames(NamingConvention::SnakeCase),
///     ],
/// )
/// .expect("Fatal error occurred");
/// ```
pub fn run_and_print_checks<I, P>(input_files: I, checks: &[Check]) -> Result<bool, Box<dyn Error>>
where
    P: AsRef<Path>,
    I: AsRef<[P]>,
{
    let input_files = input_files.as_ref();
    let mut results = Vec::with_capacity(input_files.len() * checks.len());
    let mut errors = Vec::with_capacity(input_files.len());
    for input_file in input_files {
        match run_checks(input_file, checks) {
            Ok(result) => results.extend(
                result
                    .into_iter()
                    .map(|r| (input_file.as_ref().to_string_lossy().into_owned(), r)),
            ),
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
    use super::*;

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

    fn get_main_block(symgen: &mut SymGen) -> &mut Block {
        symgen
            .get_mut(&symgen.block_key("main").expect("No block \"main\"").clone())
            .unwrap()
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
    fn test_complete_versions_list() {
        let mut symgen = get_test_symgen();
        assert!(check_complete_version_list(&symgen).is_ok());

        let block = get_main_block(&mut symgen);
        // Delete the block version list
        block.versions = None;
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
    fn test_no_function_overlap() {
        let mut symgen = get_test_symgen();
        assert!(check_no_function_overlap(&symgen).is_ok());

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
        let addr = overlapping.address.get_mut(block.version("v1")).unwrap();
        *addr = function.address.get(block.version("v1")).unwrap().clone();
        block.functions = [function, overlapping].into();
        assert!(check_no_function_overlap(&symgen).is_err());
    }

    #[test]
    fn test_symbols_name_check() {
        let mut symgen = get_test_symgen();
        assert!(check_function_names(&symgen, NamingConvention::SnakeCase).is_ok());
        assert!(check_data_names(&symgen, NamingConvention::ScreamingSnakeCase).is_ok());

        let block = get_main_block(&mut symgen);
        // Set the function to have the wrong case
        block
            .functions
            .get_mut(0)
            .expect("symgen has no functions")
            .name = "PascalCase".to_string();
        assert!(check_function_names(&symgen, NamingConvention::SnakeCase).is_err());

        // reborrow
        let block = get_main_block(&mut symgen);
        // Set the data to have the wrong case
        block.data.get_mut(0).expect("symgen has no data").name = "snake_case".to_string();
        assert!(check_data_names(&symgen, NamingConvention::ScreamingSnakeCase).is_err());
    }
}
