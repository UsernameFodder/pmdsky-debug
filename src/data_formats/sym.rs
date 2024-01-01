//! The SYM (.sym) format.
//!
//! The SYM format is a simple symbol table format. Each symbol is listed on a separate line, and
//! consists of a memory address (as a raw, 8-character hexadecimal number) and a name separated
//! by spaces. Symbols are always listed by their primary name; any aliases are ignored.
//!
//! # Example
//! ```csv
//! 02000000 main
//! 02400000 function1
//! 02FFFFFF SOME_DATA
//! ```

use std::error::Error;
use std::io::Write;

use csv::WriterBuilder;
use serde::{Serialize, Serializer};

use super::symgen_yml::{Generate, SymGen, Uint};

/// Generator for the .sym format.
pub struct SymFormatter {}

fn serialize_as_hex8<S>(x: &Uint, s: S) -> Result<S::Ok, S::Error>
where
    S: Serializer,
{
    // If x is larger than u32::MAX, it'll just be printed with as many characters as needed.
    // SYM only supports 32-bit numbers by virtue of requiring 8-character hex offsets,
    // so there isn't really a well defined way to handle this case ¯\_(ツ)_/¯.
    s.serialize_str(&format!("{:08X}", x))
}

#[derive(Debug, Serialize)]
struct Entry<'a> {
    #[serde(serialize_with = "serialize_as_hex8")]
    address: Uint,
    name: &'a str,
}

impl Generate for SymFormatter {
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
        for s in symgen.symbols_realized(version) {
            // NOTE: aliases are ignored here on purpose; only write the primary name
            wtr.serialize(Entry {
                address: s.address,
                name: s.name,
            })?;
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
                      - 0x2001FFF
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
        let f = SymFormatter {};
        assert_eq!(
            f.generate_str(&symgen, "v1").expect("generate failed"),
            "02000000 fn1\n02001FFF fn2\n02002000 fn2\n02003000 SOME_DATA\n"
        );
        assert_eq!(
            f.generate_str(&symgen, "v2").expect("generate failed"),
            "02002000 fn1\n02003000 fn2\n02004000 SOME_DATA\n"
        );
    }

    #[test]
    fn test_generate_64bit() {
        let symgen = SymGen::read(
            r"
            main:
              address: 0x2000000
              length: 0x10000000000
              functions:
                - name: fn1
                  address: 0x100000000
                - name: fn2
                  address: 0x2000000
                - name: fn3
                  address: 0xFFFFFFFFFF
              data: []
            "
            .as_bytes(),
        )
        .expect("Read failed");

        let f = SymFormatter {};
        // The format makes no official specification on how 64-bit numbers should behave,
        // but resymgen should still deal with them in a consistent manner.
        assert_eq!(
            f.generate_str(&symgen, "").expect("generate failed"),
            "100000000 fn1\n02000000 fn2\nFFFFFFFFFF fn3\n"
        );
    }
}
