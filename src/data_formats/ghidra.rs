//! A Ghidra-compatible symbol table format (.ghidra).
//!
//! This format is the symbol table format read by the `ImportSymbolsScript.py` Ghidra script.
//! Each symbol is listed on a separate line, and consists of a name, a memory address (as raw
//! hexadecimal), and a letter identifying the symbol type ('f' for functions and 'l' for labels).
//! If a symbol has aliases, each alias is listed on a separate line.
//!
//! # Example
//! ```csv
//! main 2000000 f
//! function1 2400000 f
//! SOME_DATA 2FFFFFF l
//! ```

use std::error::Error;
use std::io::Write;

use csv::WriterBuilder;
use serde::{Serialize, Serializer};

use super::symgen_yml::{Generate, SymGen, Uint};

/// Generator for the .ghidra format.
pub struct GhidraFormatter {}

#[derive(Debug)]
enum SymbolType {
    Function,
    Label,
}

impl Serialize for SymbolType {
    fn serialize<S>(&self, serializer: S) -> Result<S::Ok, S::Error>
    where
        S: Serializer,
    {
        match self {
            Self::Function => serializer.serialize_char('f'),
            Self::Label => serializer.serialize_char('l'),
        }
    }
}

fn serialize_as_hex<S>(x: &Uint, s: S) -> Result<S::Ok, S::Error>
where
    S: Serializer,
{
    s.serialize_str(&format!("{:X}", x))
}

#[derive(Debug, Serialize)]
struct Entry<'a> {
    name: &'a str,
    #[serde(serialize_with = "serialize_as_hex")]
    address: Uint,
    stype: SymbolType,
}

impl Generate for GhidraFormatter {
    fn generate<W: Write>(
        &self,
        writer: W,
        symgen: &SymGen,
        version: &str,
    ) -> Result<(), Box<dyn Error>> {
        let mut wtr = WriterBuilder::new()
            .delimiter(b' ')
            .has_headers(false)
            .from_writer(writer);
        for f in symgen.functions_realized(version) {
            wtr.serialize(Entry {
                name: f.name,
                address: f.address,
                stype: SymbolType::Function,
            })?;
            if let Some(aliases) = f.aliases {
                for alias in aliases {
                    wtr.serialize(Entry {
                        name: alias,
                        address: f.address,
                        stype: SymbolType::Function,
                    })?;
                }
            }
        }
        for d in symgen.data_realized(version) {
            wtr.serialize(Entry {
                name: d.name,
                address: d.address,
                stype: SymbolType::Label,
            })?;
            if let Some(aliases) = d.aliases {
                for alias in aliases {
                    wtr.serialize(Entry {
                        name: alias,
                        address: d.address,
                        stype: SymbolType::Label,
                    })?;
                }
            }
        }
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

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
                  aliases:
                    - fn1_alias
                  address:
                    v1: 0x2000000
                    v2: 0x2002000
                  length:
                    v1: 0x1000
                    v2: 0x1000
                  description: bar
                - name: fn2
                  address:
                    v1:
                      - 0x2001000
                      - 0x2002000
                    v2: 0x2003000
                  description: baz
              data:
                - name: SOME_DATA
                  address:
                    v1: 0x2003000
                    v2: 0x2004000
                  length:
                    v1: 0x1000
                    v2: 0x2000
                  description: foo bar baz
        "
            .as_bytes(),
        )
        .expect("Read failed")
    }

    #[test]
    fn test_generate() {
        let symgen = get_test_symgen();
        let f = GhidraFormatter {};
        assert_eq!(
            f.generate_str(&symgen, "v1").expect("generate failed"),
            "fn1 2000000 f\nfn1_alias 2000000 f\nfn2 2001000 f\nfn2 2002000 f\nSOME_DATA 2003000 l\n"
        );
        assert_eq!(
            f.generate_str(&symgen, "v2").expect("generate failed"),
            "fn1 2002000 f\nfn1_alias 2002000 f\nfn2 2003000 f\nSOME_DATA 2004000 l\n"
        );
    }
}
