#[macro_use]
extern crate clap;

use std::convert::AsRef;
use std::error::Error;
use std::io::{self, Write};
use std::path::Path;

use clap::{App, AppSettings, Arg, ArgSettings, SubCommand};
use termcolor::{Color, ColorChoice, ColorSpec, StandardStream, WriteColor};

use resymgen;

fn int_format(write_as_decimal: bool) -> resymgen::IntFormat {
    if write_as_decimal {
        resymgen::IntFormat::Decimal
    } else {
        resymgen::IntFormat::Hexadecimal
    }
}

const SUPPORTED_NAMING_CONVENTIONS: [&str; 5] = [
    "identifier",
    "snake_case",
    "SCREAMING_SNAKE_CASE",
    "camelCase",
    "PascalCase",
];

/// name is assumed to be in SUPPORTED_NAMING_CONVENTIONS (case-insensitive)
fn naming_convention(name: &str) -> resymgen::NamingConvention {
    match name.to_ascii_lowercase().as_ref() {
        "identifier" => resymgen::NamingConvention::Identifier,
        "snake_case" => resymgen::NamingConvention::SnakeCase,
        "screaming_snake_case" => resymgen::NamingConvention::ScreamingSnakeCase,
        "camelcase" => resymgen::NamingConvention::CamelCase,
        "pascalcase" => resymgen::NamingConvention::PascalCase,
        _ => panic!("Unsupported naming convention '{}'", name), // control should never reach this point
    }
}

const SUPPORTED_SYMBOL_TYPES: [&str; 2] = ["function", "data"];

/// stype is assumed to be in SUPPORTED_SYMBOL_TYPES
fn symbol_type(stype: &str) -> resymgen::SymbolType {
    match stype {
        "function" => resymgen::SymbolType::Function,
        "data" => resymgen::SymbolType::Data,
        _ => panic!("Unsupported symbol type '{}'", stype), // control should never reach this point
    }
}

fn run_resymgen() -> Result<(), Box<dyn Error>> {
    let gen_formats: Vec<_> = resymgen::OutFormat::all().map(|f| f.extension()).collect();
    let merge_formats: Vec<_> = resymgen::InFormat::all().map(|f| f.extension()).collect();

    let matches = App::new(crate_name!())
        .version(crate_version!())
        .author("UsernameFodder")
        .about("Generates symbol tables for reverse engineering applications from a YAML specification.")
        .setting(AppSettings::ArgRequiredElseHelp)
        .subcommand(
            SubCommand::with_name("gen")
                .about("Generates one or more symbol tables from a resymgen YAML file")
                .args(&[
                    Arg::with_name("format")
                        .help("Symbol table output format")
                        .takes_value(true)
                        .short("f")
                        .long("format")
                        .multiple(true)
                        .number_of_values(1)
                        .possible_values(&gen_formats.iter().map(|f| f.as_ref()).collect::<Vec<_>>()),
                    Arg::with_name("binary version")
                        .help("Version of the binary to generate a symbol table for")
                        .takes_value(true)
                        .short("v")
                        .long("binary-version")
                        .multiple(true)
                        .number_of_values(1),
                    Arg::with_name("output directory")
                        .help("Output directory")
                        .takes_value(true)
                        .short("o")
                        .long("output-dir")
                        .default_value("out")
                        .required(true),
                    Arg::with_name("input")
                        .help("Input resymgen YAML file name")
                        .required(true)
                        .index(1),
                ]),
        )
        .subcommand(
            SubCommand::with_name("fmt")
                .about("Formats a resymgen YAML file")
                .args(&[
                    Arg::with_name("check")
                        .help("Run in 'check' mode. If the input is improperly formatted, exit with 1 and print a diff.")
                        .short("c")
                        .long("check"),
                    Arg::with_name("decimal")
                        .help("Write integers in decimal format. By default integers are written as hexadecimal.")
                        .short("d")
                        .long("decimal"),
                    Arg::with_name("input")
                        .help("Input file name")
                        .required(true)
                        .index(1),
                ]),
        )
        .subcommand(
            SubCommand::with_name("check")
                .about("Validates the contents of a resymgen YAML file")
                .args(&[
                    Arg::with_name("explicit versions")
                        .help("Require versions to be explicitly specified in maps")
                        .short("v")
                        .long("explicit-versions"),
                    Arg::with_name("complete version list")
                        .help("Require versions used in maps to appear exactly once in the top-level version list")
                        .short("V")
                        .long("complete-version-list"),
                    Arg::with_name("nonempty maps")
                        .help("Require maps to have at least one entry")
                        .short("m")
                        .long("nonempty-maps"),
                    Arg::with_name("unique symbols")
                        .help("Require symbol names to be unique within a block")
                        .short("u")
                        .long("unique-symbols"),
                    Arg::with_name("in-bounds symbols")
                        .help("Require symbols to be within specified per-version block bounds")
                        .short("b")
                        .long("in-bounds-symbols"),
                    Arg::with_name("no function overlap")
                        .help("Disallow per-version overlap between functions within a block")
                        .short("o")
                        .long("no-function-overlap"),
                    Arg::with_name("function names")
                        .help("Enforce a naming convention for function symbols. Note that all conventions implicitly enforce valid identifiers.")
                        .takes_value(true)
                        .short("f")
                        .long("function-names")
                        .set(ArgSettings::CaseInsensitive)
                        .possible_values(&SUPPORTED_NAMING_CONVENTIONS),
                    Arg::with_name("data names")
                        .help("Enforce a naming convention for data symbols. Note that all conventions implicitly enforce valid identifiers.")
                        .takes_value(true)
                        .short("d")
                        .long("data-names")
                        .set(ArgSettings::CaseInsensitive)
                        .possible_values(&SUPPORTED_NAMING_CONVENTIONS),
                    Arg::with_name("input")
                        .help("Input file name")
                        .required(true)
                        .index(1),
                ]),
        )
        .subcommand(
            SubCommand::with_name("merge")
                .about("Merge one or more data files into a resymgen YAML file")
                .args(&[
                    Arg::with_name("format")
                        .help("Input data format")
                        .takes_value(true)
                        .short("f")
                        .long("input-format")
                        .possible_values(&merge_formats.iter().map(|f| f.as_ref()).collect::<Vec<_>>())
                        .default_value("yml")
                        .required(true),
                    Arg::with_name("symbol type")
                        .help("Default symbol type to assume if input data symbols is unlabeled")
                        .takes_value(true)
                        .short("s")
                        .long("symbol-type")
                        .possible_values(&SUPPORTED_SYMBOL_TYPES),
                    Arg::with_name("binary version")
                        .help("Default binary version to assume if input data version is unlabeled")
                        .takes_value(true)
                        .short("v")
                        .long("binary-version"),
                    Arg::with_name("block")
                        .help("Default block to assume if input data blocks are unlabeled")
                        .takes_value(true)
                        .short("b")
                        .long("block"),
                    Arg::with_name("decimal")
                        .help("Write integers in decimal format. By default integers are written as hexadecimal.")
                        .short("d")
                        .long("decimal"),
                    Arg::with_name("fix formatting")
                        .help("Run the formatter on the final resymgen YAML file after the merge.")
                        .short("x")
                        .long("fix-formatting"),
                    Arg::with_name("input")
                        .help("input data file")
                        .required(true)
                        .takes_value(true)
                        .short("i")
                        .long("input")
                        .multiple(true)
                        .number_of_values(1),
                    Arg::with_name("symgen file")
                        .help("resymgen YAML file to modify")
                        .required(true)
                        .index(1),
                ]),
        )
        .get_matches();

    match matches.subcommand_name() {
        Some("gen") => {
            let matches = matches.subcommand_matches("gen").unwrap();

            let input_file = matches.value_of("input").unwrap();
            let output_dir = matches.value_of("output directory").unwrap();
            let output_formats = match matches.values_of("format") {
                Some(v) => Some(
                    v.map(|name| {
                        resymgen::OutFormat::from(name)
                            .ok_or_else(|| format!("Invalid output format: '{}'", name))
                    })
                    .collect::<Result<Vec<_>, _>>()?,
                ),
                None => None,
            };
            let output_versions: Option<Vec<_>> =
                matches.values_of("binary version").map(|v| v.collect());

            let input_file_stem = Path::new(input_file)
                .file_stem()
                .ok_or("Empty input file name")?;
            let output_base = Path::new(output_dir).join(input_file_stem);
            resymgen::generate_symbol_tables(
                input_file,
                output_formats,
                output_versions,
                output_base,
            )
        }
        Some("fmt") => {
            let matches = matches.subcommand_matches("fmt").unwrap();

            let input_file = matches.value_of("input").unwrap();
            let iformat = int_format(matches.is_present("decimal"));
            if matches.is_present("check") {
                if !resymgen::format_check_file(input_file, iformat)? {
                    Err("Formatting issues detected.")?;
                }
                Ok(())
            } else {
                resymgen::format_file(input_file, iformat)
            }
        }
        Some("check") => {
            let matches = matches.subcommand_matches("check").unwrap();

            let input_file = matches.value_of("input").unwrap();

            let mut checks = Vec::new();
            if matches.is_present("explicit versions") {
                checks.push(resymgen::Check::ExplicitVersions);
            }
            if matches.is_present("complete version list") {
                checks.push(resymgen::Check::CompleteVersionList);
            }
            if matches.is_present("nonempty maps") {
                checks.push(resymgen::Check::NonEmptyMaps);
            }
            if matches.is_present("unique symbols") {
                checks.push(resymgen::Check::UniqueSymbols);
            }
            if matches.is_present("in-bounds symbols") {
                checks.push(resymgen::Check::InBoundsSymbols);
            }
            if matches.is_present("no function overlap") {
                checks.push(resymgen::Check::NoFunctionOverlap);
            }
            if let Some(conv) = matches.value_of("function names") {
                checks.push(resymgen::Check::FunctionNames(naming_convention(conv)));
            }
            if let Some(conv) = matches.value_of("data names") {
                checks.push(resymgen::Check::DataNames(naming_convention(conv)));
            }
            resymgen::run_and_print_checks(input_file, &checks)
        }
        Some("merge") => {
            let matches = matches.subcommand_matches("merge").unwrap();

            let input_files: Vec<_> = matches.values_of("input").unwrap().collect();
            let symgen_file = matches.value_of("symgen file").unwrap();
            let input_format_name = matches.value_of("format").unwrap();
            let input_format = resymgen::InFormat::from(input_format_name)
                .ok_or_else(|| format!("Invalid input format: '{}'", input_format_name))?;
            let merge_params = resymgen::LoadParams {
                default_block_name: matches.value_of("block").map(String::from),
                default_symbol_type: matches.value_of("symbol type").map(symbol_type),
                default_version_name: matches.value_of("binary version").map(String::from),
            };
            let iformat = int_format(matches.is_present("decimal"));
            let fix_formatting = matches.is_present("fix formatting");
            let unmerged_symbols = resymgen::merge_symbols(
                &symgen_file,
                &input_files,
                input_format,
                &merge_params,
                iformat,
            )?;
            if fix_formatting {
                resymgen::format_file(&symgen_file, iformat)?;
            }

            // Print the unmerged symbols from each file, with terminal colors
            let mut stdout = StandardStream::stdout(ColorChoice::Always);
            let mut print_unmerged_colored = || -> io::Result<()> {
                let mut color = ColorSpec::new();
                for (fname, unmerged) in input_files.iter().zip(unmerged_symbols.iter()) {
                    if !unmerged.is_empty() {
                        stdout.set_color(color.set_fg(Some(Color::Yellow)))?;
                        write!(&mut stdout, "* Unmerged symbols from \"{}\":", fname)?;
                        stdout.reset()?;
                        writeln!(
                            &mut stdout,
                            " {}",
                            unmerged
                                .iter()
                                .map(|s| format!("{}", s.name))
                                .collect::<Vec<_>>()
                                .join(", ")
                        )?;
                    }
                }
                stdout.reset()?;
                Ok(())
            };
            let res = print_unmerged_colored();
            // Always try to clean up color settings before returning
            if let Err(e) = stdout.reset() {
                Err(e)?;
            } else {
                res?;
            }

            Ok(())
        }
        Some(s) => panic!("Subcommand '{}' not implemented", s), // control should never reach this point
        _ => panic!("Missing subcommand"), // control should never reach this point
    }
}

fn main() {
    if let Err(e) = run_resymgen() {
        // Display the error nicely rather than in debug format
        println!("Error: {}", e);
    }
}
