use std::error::Error;
use std::io::Write;

use super::symgen::SymGen;

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
