use std::error::Error;
use std::io::Write;

use csv::WriterBuilder;
use serde::{Serialize, Serializer};

use super::symgen_yml::{Generate, SymGen, Uint};

pub struct SymFormatter {}

fn serialize_as_hex8<S>(x: &Uint, s: S) -> Result<S::Ok, S::Error>
where
    S: Serializer,
{
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
}
