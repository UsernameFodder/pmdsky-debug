#[macro_use]
extern crate clap;

use std::convert::AsRef;
use std::error::Error;
use std::path::Path;

use clap::{App, AppSettings, Arg, ArgSettings, SubCommand};

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

fn run_resymgen() -> Result<(), Box<dyn Error>> {
    let gen_formats: Vec<_> = resymgen::OutFormat::all().map(|f| f.extension()).collect();

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
