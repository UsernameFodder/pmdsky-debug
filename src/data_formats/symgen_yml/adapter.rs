use std::error::Error;
use std::io::{Read, Write};

use super::symgen::{SymGen, Symbol};

pub trait Generate {
    fn generate<W: Write>(
        &self,
        writer: W,
        symgen: &SymGen,
        version: &str,
    ) -> Result<(), Box<dyn Error>>;

    fn generate_str(&self, symgen: &SymGen, version: &str) -> Result<String, Box<dyn Error>> {
        let mut bytes = Vec::<u8>::new();
        self.generate(&mut bytes, symgen, version)?;
        Ok(String::from_utf8(bytes)?)
    }
}

#[derive(Debug, Clone, Copy, PartialEq, Eq, Hash)]
pub enum SymbolType {
    Function,
    Data,
}

#[derive(Debug, Clone, PartialEq, Eq)]
pub struct AddSymbol {
    pub symbol: Symbol,
    pub stype: SymbolType,
    pub block_name: Option<String>,
}

#[derive(Clone)]
pub struct LoadParams {
    pub default_block_name: Option<String>,
    pub default_symbol_type: Option<SymbolType>,
    pub default_version_name: Option<String>,
}

// For merge functionality
pub trait Load {
    type Source: Iterator<Item = AddSymbol>;

    fn load<R: Read>(rdr: R, params: &LoadParams) -> Result<Self::Source, Box<dyn Error>>;
}
