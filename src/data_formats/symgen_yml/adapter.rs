//! Adapter APIs to integrate the `resymgen` YAML format with other data formats
//! (through the `gen` and `merge` commands).

use std::error::Error;
use std::io::{Read, Write};

use super::symgen::{SymGen, Symbol};

/// `Generate` implementers can convert a [`SymGen`] into a different data format.
pub trait Generate {
    /// Write the contents of `symgen` for `version` to `writer` in the desired format.
    fn generate<W: Write>(
        &self,
        writer: W,
        symgen: &SymGen,
        version: &str,
    ) -> Result<(), Box<dyn Error>>;

    /// Write the contents of `symgen` for `version` to a [`String`].
    fn generate_str(&self, symgen: &SymGen, version: &str) -> Result<String, Box<dyn Error>> {
        let mut bytes = Vec::<u8>::new();
        self.generate(&mut bytes, symgen, version)?;
        Ok(String::from_utf8(bytes)?)
    }
}

/// Types of symbols within a [`SymGen`].
#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum SymbolType {
    Function,
    Data,
}

/// A [`Symbol`] tagged with metadata to be added into a [`SymGen`].
#[derive(Debug, Clone, PartialEq, Eq)]
pub struct AddSymbol {
    pub symbol: Symbol,
    pub stype: SymbolType,
    pub block_name: Option<String>,
}

/// Parameters to control how data in a foreign format is merged into a [`SymGen`].
#[derive(Clone)]
pub struct LoadParams {
    /// Name of the block in which to place a symbol if none is present.
    pub default_block_name: Option<String>,
    /// Default symbol type to assign to a symbol if none is present.
    pub default_symbol_type: Option<SymbolType>,
    /// Default version name to assign to a symbol if none is present.
    pub default_version_name: Option<String>,
}

/// `Load` implementers can read the contents of a [`Read`] type and produce a stream of
/// [`AddSymbol`]s, which are used for merging foreign formats into a [`SymGen`].
pub trait Load {
    type Source: Iterator<Item = AddSymbol>;

    /// Load data in some format from `rdr` and return an iterator of [`AddSymbol`]s based on
    /// the options specified in `params`.
    fn load<R: Read>(rdr: R, params: &LoadParams) -> Result<Self::Source, Box<dyn Error>>;
}
