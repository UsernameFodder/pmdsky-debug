//! This module contains structs and methods for reading, writing, and manipulating different
//! symbol data formats.
//!
//! The code for each data format is separated into its own module, including the `resymgen` YAML
//! format itself (the [`symgen_yml`] module).

pub mod ghidra;
pub mod ghidra_csv;
pub mod json;
pub mod sym;
pub mod symgen_yml;

use std::error::Error;
use std::fs::File;
use std::io::{Read, Write};
use std::path::Path;

use ghidra::GhidraFormatter;
use ghidra_csv::CsvLoader;
use json::JsonFormatter;
use sym::SymFormatter;
pub use symgen_yml::Generate;
use symgen_yml::{Load, LoadParams, Subregion, SymGen, Symbol};

// `OutFormat` is like a poor man's version of trait objects for Generate. Real trait objects don't
// work because `Generate` isn't object-safe (generate() is generic), so we can't use dynamic
// polymorphism and need to define an enum instead for static polymorphism (the idea is to be able
// to make an array of `Generate` objects).

/// Output formats that can be generated from the [`resymgen` YAML] format
/// (all types that are [`Generate`]).
///
/// [`resymgen` YAML]: symgen_yml
#[derive(Debug, Clone, Copy)]
pub enum OutFormat {
    /// [`ghidra`] format
    Ghidra,
    /// [`sym`] format
    Sym,
    /// [`json`] format
    Json,
}

// Technically this makes it redundant to impl Generate for the individual formatters, but I think
// the trait still adds clarity, even though it doesn't add utility :)
impl Generate for OutFormat {
    fn generate<W: Write>(
        &self,
        writer: W,
        symgen: &SymGen,
        version: &str,
    ) -> Result<(), Box<dyn Error>> {
        match self {
            Self::Ghidra => GhidraFormatter {}.generate(writer, symgen, version),
            Self::Sym => SymFormatter {}.generate(writer, symgen, version),
            Self::Json => JsonFormatter {}.generate(writer, symgen, version),
        }
    }
}

impl OutFormat {
    /// Returns the [`OutFormat`] corresponding to the given `name`, if there is one.
    pub fn from(name: &str) -> Option<Self> {
        match name {
            "ghidra" => Some(Self::Ghidra),
            "sym" => Some(Self::Sym),
            "json" => Some(Self::Json),
            _ => None,
        }
    }
    /// Returns the file extension associated with the [`OutFormat`].
    pub fn extension(&self) -> String {
        match self {
            Self::Ghidra => String::from("ghidra"),
            Self::Sym => String::from("sym"),
            Self::Json => String::from("json"),
        }
    }
    /// Returns an [`Iterator`] over all [`OutFormat`] variants.
    pub fn all() -> impl Iterator<Item = OutFormat> {
        [Self::Ghidra, Self::Sym, Self::Json].iter().copied()
    }
}

/// Input formats that can be merged into a symbol table in the [`resymgen` YAML] format
/// (all types that are [`Load`]).
///
/// [`resymgen` YAML]: symgen_yml
#[derive(Debug, Clone, Copy)]
pub enum InFormat {
    /// [`resymgen` YAML] format.
    ///
    /// [`resymgen` YAML]: symgen_yml
    Yaml,
    /// A specific [CSV] format exported from Ghidra projects.
    ///
    /// [CSV]: ghidra_csv
    Csv,
}

impl InFormat {
    /// Returns the [`InFormat`] corresponding to the given `name`, if there is one.
    pub fn from(name: &str) -> Option<Self> {
        match name {
            "yml" => Some(Self::Yaml),
            "csv" => Some(Self::Csv),
            _ => None,
        }
    }
    /// Returns the file extension associated with the [`InFormat`].
    pub fn extension(&self) -> String {
        match self {
            Self::Yaml => String::from("yml"),
            Self::Csv => String::from("csv"),
        }
    }
    /// Returns an [`Iterator`] over all [`InFormat`] variants.
    pub fn all() -> impl Iterator<Item = InFormat> {
        [Self::Yaml, Self::Csv].iter().copied()
    }

    /// Reads data from `rdr` in the format specified by the [`InFormat`], and merges it into
    /// `symgen` using the options specified in `params`.
    ///
    /// If a `file_name` is provided, it may be used for subregion resolution, depending on the
    /// [`InFormat`].
    pub fn merge<R, P>(
        &self,
        symgen: &mut SymGen,
        rdr: R,
        file_name: Option<P>,
        params: &LoadParams,
    ) -> Result<Vec<Symbol>, Box<dyn Error>>
    where
        R: Read,
        P: AsRef<Path>,
    {
        let unmerged = match self {
            Self::Yaml => {
                let mut other = SymGen::read_no_init(rdr)?;
                if let Some(file_name) = file_name {
                    other
                        .resolve_subregions(Subregion::subregion_dir(file_name.as_ref()), |p| {
                            File::open(p)
                        })?;
                }
                symgen.merge_symgen(&other)?;
                Vec::new()
            }
            Self::Csv => symgen.merge_symbols(CsvLoader::load(rdr, params)?)?,
        };
        Ok(unmerged)
    }
}
