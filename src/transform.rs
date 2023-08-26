//! Data transformations involving the `resymgen` YAML format and other formats. Implements the
//! `gen` and `merge` commands.

use std::borrow::Cow;
use std::collections::BTreeSet;
use std::convert::AsRef;
use std::error::Error;
use std::fs::{self, File};
use std::path::{Path, PathBuf};

use tempfile::NamedTempFile;

use super::data_formats::symgen_yml::{IntFormat, LoadParams, Sort, Subregion, SymGen, Symbol};
use super::data_formats::{Generate, InFormat, OutFormat};
use super::util;

/// Forms the output file path from the base, version, and format.
fn output_file_name(base: &Path, version: &str, format: &OutFormat) -> PathBuf {
    let output_stem = match base.file_stem() {
        Some(s) => {
            let mut stem = s.to_os_string();
            if !version.is_empty() {
                // Append the version as a suffix if it exists
                stem.push("_");
                stem.push(version);
            }
            stem
        }
        None => {
            // path is empty or invalid; fall back to just the version
            if !version.is_empty() {
                version.into()
            } else {
                // no path or version; use the format extension as the output stem...
                format.extension().into()
            }
        }
    };
    base.with_file_name(output_stem)
        .with_extension(format.extension())
}

/// Generates symbol tables from a given SymGen struct for multiple different formats/versions.
fn generate_symbols<P: AsRef<Path>>(
    symgen: &SymGen,
    formats: &[OutFormat],
    versions: &[&str],
    output_base: P,
) -> Result<(), Box<dyn Error>> {
    for fmt in formats.iter() {
        for version in versions.iter() {
            // Write to a tempfile first, then persist atomically.
            let output_file = output_file_name(output_base.as_ref(), version, fmt);
            let f_gen = NamedTempFile::new()?;
            fmt.generate(&f_gen, symgen, version)?;
            // Make sure the parent directory exists first
            if let Some(parent) = output_file.parent() {
                fs::create_dir_all(parent)?;
            }
            util::persist_named_temp_file_safe(f_gen, output_file)?;
        }
    }
    Ok(())
}

/// Gets a list of all version names within a SymGen.
/// 1. If a version list is explicitly specified by blocks, use those.
/// 2. If a block does not explicitly specify a version list, infer it
/// based on the addresses it contains.
/// 3. If blocks with symbols exist but none has an explicit version, return
/// a vector containing a single empty string ("").
fn all_version_names(symgen: &SymGen) -> Vec<&str> {
    let mut vers = BTreeSet::new();
    let mut symgen_has_symbols: bool = false;
    let mut versions_inferred_from_symbols: bool = false;
    for b in symgen.blocks() {
        if let Some(v) = &b.versions {
            // Explicit version list
            for name in v.iter().map(|v| v.name()) {
                vers.insert(name);
            }
        } else {
            // Inferred version list
            for name in b.address.versions().map(|v| v.name()) {
                vers.insert(name);
            }
            for s in b.iter() {
                symgen_has_symbols = true;
                for name in s.address.versions().map(|v| v.name()) {
                    versions_inferred_from_symbols = true;
                    vers.insert(name);
                }
            }
        }
    }
    if symgen_has_symbols && vers.is_empty() {
        // Special case: symbols exist but all are Common
        return vec![""];
    } else if versions_inferred_from_symbols && !vers.is_empty() {
        // XXX: A non-empty version list was inferred from symbol contents. Unfortunately, this
        // won't actually give the desired generation output for blocks without an explicit
        // version list, since the version-realization step during generation won't work properly
        // without one. Warn the user of this, and tell them to use the --complete-version-list
        // check to enforce an explicit version list on all blocks.
        eprintln!(
            "Warning: Detected one or more blocks containing by-version symbol addresses, but \
            without an explicit version list. Proceeding with inferred version list, but the \
            generated output might be incomplete. Use `check --complete-version-list` to enforce \
            explicit version lists on all blocks."
        );
    }
    vers.into_iter().collect()
}

/// Generates symbol tables from a given `input_file` for multiple different `output_formats` and
/// `output_versions`.
///
/// Output is written to filepaths based on `output_base`. Both `output_formats` and
/// `output_versions` default to all formats/versions if `None`. If `sort_output` is true, the
/// function and data sections of the output symbol tables will each be sorted by symbol address.
///
/// # Examples
/// ```ignore
/// generate_symbol_tables(
///     "/path/to/symbols.yml",
///     Some([OutFormat::Ghidra]),
///     Some("v1"),
///     false,
///     "/path/to/out/symbols",
/// )
/// .expect("failed to generate symbol tables");
/// ```
pub fn generate_symbol_tables<'v, I, F, V, O>(
    input_file: I,
    output_formats: Option<F>,
    output_versions: Option<V>,
    sort_output: bool,
    output_base: O,
) -> Result<(), Box<dyn Error>>
where
    I: AsRef<Path>,
    F: AsRef<[OutFormat]>,
    V: AsRef<[&'v str]>,
    O: AsRef<Path>,
{
    let input_file = input_file.as_ref();
    let mut contents = {
        let file = File::open(input_file)?;
        SymGen::read(&file)?
    };
    contents.resolve_subregions(Subregion::subregion_dir(input_file), |p| File::open(p))?;
    contents.collapse_subregions();
    if sort_output {
        contents.sort();
    }

    let formats = match &output_formats {
        Some(f) => Cow::Borrowed(f.as_ref()),
        None => Cow::Owned(OutFormat::all().collect::<Vec<_>>()),
    };
    let versions = match &output_versions {
        Some(v) => Cow::Borrowed(v.as_ref()),
        None => Cow::Owned(all_version_names(&contents)),
    };

    generate_symbols(&contents, &formats, &versions, output_base)
}

/// Merges symbols from a collection of `input_files` of the format `input_format` into a given
/// `symgen_file`.
///
/// Additional configuration is specified with `merge_params`. Integers are written in `int_format`.
///
/// # Examples
/// ```ignore
/// let params = LoadParams {
///     default_block_name: None,
///     default_symbol_type: None,
///     default_version_name: Some("v1".into()),
/// };
/// merge_symbols(
///     "/path/to/symbols.yml",
///     ["/path/to/input.csv"],
///     InFormat::Csv,
///     &params,
///     IntFormat::Hexadecimal,
/// )
/// .expect("failed to merge symbols");
/// ```
pub fn merge_symbols<P, P2, I>(
    symgen_file: P,
    input_files: I,
    input_format: InFormat,
    merge_params: &LoadParams,
    int_format: IntFormat,
) -> Result<Vec<Vec<Symbol>>, Box<dyn Error>>
where
    P: AsRef<Path>,
    P2: AsRef<Path>,
    I: AsRef<[P2]>,
{
    let symgen_file = symgen_file.as_ref();
    let mut contents = {
        let file = File::open(symgen_file)?;
        SymGen::read(&file)?
    };
    contents.resolve_subregions(Subregion::subregion_dir(symgen_file), |p| File::open(p))?;

    let mut unmerged_symbols = Vec::with_capacity(input_files.as_ref().len());
    for input_name in input_files.as_ref() {
        let input = File::open(input_name)?;
        unmerged_symbols.push(input_format.merge(
            &mut contents,
            input,
            Some(input_name),
            merge_params,
        )?);
    }

    util::symgen_write_recursive(&contents, symgen_file, int_format)?;
    Ok(unmerged_symbols)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_output_file_name() {
        let cases = [
            (("out/", "NA", OutFormat::Ghidra), "out_NA.ghidra"),
            (("out/name", "EU", OutFormat::Sym), "out/name_EU.sym"),
            (
                ("/foo/bar/baz.yml", "JP", OutFormat::Sym),
                "/foo/bar/baz_JP.sym",
            ),
            (("", "NA", OutFormat::Sym), "NA.sym"),
            (("out/", "", OutFormat::Ghidra), "out.ghidra"),
            (("", "", OutFormat::Sym), "sym.sym"),
        ];
        for ((base, version, format), exp) in cases {
            assert_eq!(
                output_file_name(&Path::new(base), version, &format),
                Path::new(exp)
            );
        }
    }

    #[test]
    fn test_all_version_names() {
        let s = SymGen::read(
            r"
            main:
              versions:
                - v1
                - v3
              address: 0x2000000
              length: 0x1000
              functions: []
              data: []
            other:
              versions:
                - v2
              address: 0x2100000
              length: 0x1000
              functions: []
              data: []
            "
            .as_bytes(),
        )
        .expect("Read failed");

        assert_eq!(all_version_names(&s), vec!["v1", "v2", "v3"]);
    }

    #[test]
    fn test_all_version_names_inferred() {
        let s = SymGen::read(
            r"
            main:
              address:
                v0: 0x2000000
              length: 0x1000
              functions:
                - name: fn1
                  address:
                    v1: 0x2000000
                    v3: 0x2000000
              data: []
            other:
              address: 0x2100000
              length: 0x1000
              functions: []
              data:
                - name: data1
                  address:
                    v2: 0x2100000
            "
            .as_bytes(),
        )
        .expect("Read failed");

        assert_eq!(all_version_names(&s), vec!["v0", "v1", "v2", "v3"]);
    }

    #[test]
    fn test_all_version_names_common() {
        let s = SymGen::read(
            r"
            main:
              address: 0x2000000
              length: 0x1000
              functions:
                - name: fn1
                  address: 0x2000000
              data: []
            other:
              address: 0x2100000
              length: 0x1000
              functions: []
              data:
                - name: data1
                  address: 0x2100000
            "
            .as_bytes(),
        )
        .expect("Read failed");

        assert_eq!(all_version_names(&s), vec![""]);
    }

    #[test]
    fn test_all_version_names_empty() {
        let s = SymGen::read(
            r"
            main:
              address: 0x2000000
              length: 0x1000
              functions: []
              data: []
            "
            .as_bytes(),
        )
        .expect("Read failed");

        assert_eq!(all_version_names(&s), Vec::<&str>::new());
    }
}
