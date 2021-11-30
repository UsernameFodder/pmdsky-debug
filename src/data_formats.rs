/// reading/writing different symbol data formats.
pub mod ghidra;
pub mod sym;
pub mod symgen_yml;

use std::error::Error;
use std::io::Write;

use ghidra::GhidraFormatter;
use sym::SymFormatter;
pub use symgen_yml::Generate;
use symgen_yml::SymGen;

/// This is like a poor man's version of trait objects for Generate. Real trait objects don't work
/// because Generate isn't object-safe (generate() is generic), so we can't use dynamic polymorphism
/// and need to define an enum instead for static polymorphism (the idea is to be able to make an
/// array of Generate objects).
#[derive(Debug, Clone, Copy)]
pub enum OutFormat {
    Ghidra,
    Sym,
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
        }
    }
}

impl OutFormat {
    pub fn from(name: &str) -> Option<Self> {
        match name {
            "ghidra" => Some(Self::Ghidra),
            "sym" => Some(Self::Sym),
            _ => None,
        }
    }
    pub fn extension(&self) -> String {
        match self {
            Self::Ghidra => String::from("ghidra"),
            Self::Sym => String::from("sym"),
        }
    }
    pub fn all() -> impl Iterator<Item = OutFormat> {
        [Self::Ghidra, Self::Sym].iter().copied()
    }
}
