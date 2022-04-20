//! Data transformations involving the `resymgen` YAML format and other formats. Implements the
//! `gen` and `merge` commands.

use std::borrow::Cow;
use std::collections::BTreeSet;
use std::convert::AsRef;
use std::error::Error;
use std::fs::{self, File};
use std::path::{Path, PathBuf};

use tempfile::NamedTempFile;

use super::data_formats::symgen_yml::{IntFormat, LoadParams, SymGen, Symbol};
use super::data_formats::{Generate, InFormat, OutFormat};
use super::util;

/// Forms the output file path from the base, version, and format.
fn output_file_name(base: &Path, version: &str, format: &OutFormat) -> PathBuf {
    let output_stem = match base.file_stem() {
        Some(s) => {
            let mut stem = s.to_os_string();
            stem.push("_");
            stem.push(version);
            stem
        }
        None => version.into(), // path is empty or invalid; fall back to just the version
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

/// Gets a list of all version names explicitly specified by blocks within a SymGen.
fn all_version_names(symgen: &SymGen) -> Vec<&str> {
    let mut vers = BTreeSet::new();
    for b in symgen.blocks() {
        if let Some(v) = &b.versions {
            for name in v.iter().map(|v| v.name()) {
                vers.insert(name);
            }
        }
    }
    vers.into_iter().collect()
}

/// Generates symbol tables from a given `input_file` for multiple different `output_formats` and
/// `output_versions`.
///
/// Output is written to filepaths based on `output_base`. Both `output_formats` and
/// `output_versions` default to all formats/versions if `None`.
///
/// # Examples
/// ```ignore
/// generate_symbol_tables(
///     "/path/to/symbols.yml",
///     Some([OutFormat::Ghidra]),
///     Some("v1"),
///     "/path/to/out/symbols",
/// )
/// .expect("failed to generate symbol tables");
/// ```
pub fn generate_symbol_tables<'v, I, F, V, O>(
    input_file: I,
    output_formats: Option<F>,
    output_versions: Option<V>,
    output_base: O,
) -> Result<(), Box<dyn Error>>
where
    I: AsRef<Path>,
    F: AsRef<[OutFormat]>,
    V: AsRef<[&'v str]>,
    O: AsRef<Path>,
{
    let file = File::open(input_file)?;
    let contents = SymGen::read(&file)?;

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

    let mut unmerged_symbols = Vec::with_capacity(input_files.as_ref().len());
    for input_name in input_files.as_ref() {
        let input = File::open(input_name)?;
        unmerged_symbols.push(input_format.merge(&mut contents, input, merge_params)?);
    }

    let output_file = NamedTempFile::new()?;
    contents.write(&output_file, int_format)?;
    util::persist_named_temp_file_safe(output_file, symgen_file)?;
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
