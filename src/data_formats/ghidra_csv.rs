//! A simple CSV format that can be exported directly from a Ghidra project.
//!
//! Files of this format can be obtained by exporting a project's symbol table to CSV using the
//! "Export Columns to CSV..." option, with only the "Name", "Location", and "Type" columns
//! selected, and appropriate symbol type filters applied. It consists of a header row (which is
//! always "Name","Location","Type"), followed by rows containing one symbol each. As the header
//! suggests, each data row consists of a name, memory address (as hexadecimal), and symbol type
//! ("Function" or "Data Label"), separated by commas. All values are double-quoted strings.
//!
//! # Example
//! ```csv
//! "Name","Location","Type"
//! "main","02000000","Function"
//! "function1","02400000","Function"
//! "SOME_DATA","02ffffff","Data Label"
//! ```

use std::error::Error;
use std::io::Read;
use std::vec::IntoIter;

use csv::{self, ReaderBuilder};
use serde::{de, Deserialize, Deserializer};

use super::symgen_yml::{AddSymbol, Load, LoadParams, MaybeVersionDep, Symbol, SymbolType, Uint};

fn deserialize_hex<'de, D>(deserializer: D) -> Result<Uint, D::Error>
where
    D: Deserializer<'de>,
{
    let hex_str: &str = Deserialize::deserialize(deserializer)?;
    Uint::from_str_radix(hex_str, 16).map_err(de::Error::custom)
}

fn deserialize_symbol_type<'de, D>(deserializer: D) -> Result<Option<SymbolType>, D::Error>
where
    D: Deserializer<'de>,
{
    let type_str: &str = Deserialize::deserialize(deserializer)?;
    Ok(match type_str {
        "Function" => Some(SymbolType::Function),
        "Data Label" => Some(SymbolType::Data),
        _ => None,
    })
}

#[derive(Debug, Deserialize)]
struct RawEntry {
    #[serde(rename = "Name")]
    name: String,
    #[serde(rename = "Location")]
    #[serde(deserialize_with = "deserialize_hex")]
    location: Uint,
    #[serde(rename = "Type")]
    #[serde(deserialize_with = "deserialize_symbol_type")]
    stype: Option<SymbolType>, // None means an unknown/unsupported symbol type
}

#[derive(Debug)]
struct Entry {
    name: String,
    location: Uint,
    stype: SymbolType,
}

/// Loader for a specific CSV format exported from Ghidra projects.
pub struct CsvLoader {
    entries: IntoIter<Entry>,
    params: LoadParams,
}

impl CsvLoader {
    fn read<R: Read>(rdr: R) -> Result<Vec<Entry>, csv::Error> {
        let mut csv_rdr = ReaderBuilder::new()
            .double_quote(false)
            .escape(Some(b'\\'))
            .from_reader(rdr);
        let mut symbols = Vec::new();
        for result in csv_rdr.deserialize() {
            let mut raw: RawEntry = result?;
            if let Some(stype) = raw.stype.take() {
                symbols.push(Entry {
                    name: raw.name,
                    location: raw.location,
                    stype,
                })
            }
        }
        Ok(symbols)
    }
}

impl Iterator for CsvLoader {
    type Item = AddSymbol;

    fn next(&mut self) -> Option<Self::Item> {
        self.entries.next().map(|entry| AddSymbol {
            symbol: Symbol {
                name: entry.name,
                aliases: None,
                address: match &self.params.default_version_name {
                    Some(vers) => MaybeVersionDep::ByVersion(
                        [(vers.as_str().into(), entry.location.into())].into(),
                    ),
                    None => MaybeVersionDep::Common(entry.location.into()),
                },
                length: None,
                description: None,
            },
            stype: entry.stype,
            block_name: self.params.default_block_name.clone(),
        })
    }
}

impl Load for CsvLoader {
    type Source = Self;

    fn load<R: Read>(rdr: R, params: &LoadParams) -> Result<Self::Source, Box<dyn Error>> {
        Ok(Self {
            entries: Self::read(rdr)?.into_iter(),
            params: params.clone(),
        })
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    fn get_test_csv() -> String {
        String::from(
            r#""Name","Location","Type"
"fn1","2000000","Function"
"SOME_DATA","2010000","Data Label""#,
        )
    }

    #[test]
    fn test_load_no_params() {
        let contents = get_test_csv();
        let result = CsvLoader::load(
            contents.as_bytes(),
            &LoadParams {
                default_block_name: None,
                default_symbol_type: None,
                default_version_name: None,
            },
        );
        assert!(result.is_ok());
        let mut iter = result.unwrap();
        assert_eq!(
            iter.next(),
            Some(AddSymbol {
                symbol: Symbol {
                    name: "fn1".to_string(),
                    aliases: None,
                    address: MaybeVersionDep::Common(0x2000000.into()),
                    length: None,
                    description: None,
                },
                stype: SymbolType::Function,
                block_name: None,
            })
        );
        assert_eq!(
            iter.next(),
            Some(AddSymbol {
                symbol: Symbol {
                    name: "SOME_DATA".to_string(),
                    aliases: None,
                    address: MaybeVersionDep::Common(0x2010000.into()),
                    length: None,
                    description: None,
                },
                stype: SymbolType::Data,
                block_name: None,
            })
        );
        assert_eq!(iter.next(), None);
    }

    #[test]
    fn test_load_with_params() {
        let contents = get_test_csv();
        let result = CsvLoader::load(
            contents.as_bytes(),
            &LoadParams {
                default_block_name: Some("main".to_string()),
                default_symbol_type: None,
                default_version_name: Some("v1".to_string()),
            },
        );
        assert!(result.is_ok());
        let mut iter = result.unwrap();
        assert_eq!(
            iter.next(),
            Some(AddSymbol {
                symbol: Symbol {
                    name: "fn1".to_string(),
                    aliases: None,
                    address: MaybeVersionDep::ByVersion(
                        [(("v1", 0).into(), 0x2000000.into())].into()
                    ),
                    length: None,
                    description: None,
                },
                stype: SymbolType::Function,
                block_name: Some("main".to_string()),
            })
        );
        assert_eq!(
            iter.next(),
            Some(AddSymbol {
                symbol: Symbol {
                    name: "SOME_DATA".to_string(),
                    aliases: None,
                    address: MaybeVersionDep::ByVersion(
                        [(("v1", 0).into(), 0x2010000.into())].into()
                    ),
                    length: None,
                    description: None,
                },
                stype: SymbolType::Data,
                block_name: Some("main".to_string()),
            })
        );
        assert_eq!(iter.next(), None);
    }
}
