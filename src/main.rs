#[macro_use]
extern crate clap;

use std::convert::AsRef;
use std::error::Error;
use std::io::{self, Write};
use std::path::Path;
use std::process;

use clap::{App, AppSettings, Arg, ArgSettings, SubCommand};
use termcolor::{Color, ColorChoice, ColorSpec, StandardStream, WriteColor};

use resymgen::{self, MultiFileError};

fn int_format(write_as_decimal: bool) -> resymgen::IntFormat {
    if write_as_decimal {
        resymgen::IntFormat::Decimal
    } else {
        resymgen::IntFormat::Hexadecimal
    }
}

const SUPPORTED_NAMING_CONVENTIONS: [&str; 10] = [
    "identifier",
    "snake_case",
    "SCREAMING_SNAKE_CASE",
    "camelCase",
    "PascalCase",
    "camel_Snake_Case",
    "underscoreSeparated_camelCase",
    "Pascal_Snake_Case",
    "flatcase",
    "UPPERFLATCASE",
];

// name is assumed to be in SUPPORTED_NAMING_CONVENTIONS (case-insensitive)
fn naming_convention(name: &str) -> resymgen::NamingConvention {
    match name.to_ascii_lowercase().as_ref() {
        "identifier" => resymgen::NamingConvention::Identifier,
        "snake_case" => resymgen::NamingConvention::SnakeCase,
        "screaming_snake_case" => resymgen::NamingConvention::ScreamingSnakeCase,
        "camelcase" => resymgen::NamingConvention::CamelCase,
        "pascalcase" => resymgen::NamingConvention::PascalCase,
        "camel_snake_case" => resymgen::NamingConvention::CamelSnakeCase,
        "underscoreseparated_camelcase" => resymgen::NamingConvention::UnderscoreSeparatedCamelCase,
        "pascal_snake_case" => resymgen::NamingConvention::PascalSnakeCase,
        "flatcase" => resymgen::NamingConvention::FlatCase,
        "upperflatcase" => resymgen::NamingConvention::UpperFlatCase,
        _ => panic!("Unsupported naming convention '{}'", name), // control should never reach this point
    }
}

const SUPPORTED_SYMBOL_TYPES: [&str; 2] = ["function", "data"];

// stype is assumed to be in SUPPORTED_SYMBOL_TYPES
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
                .about("Generates one or more symbol tables from a resymgen YAML file and its subregion files")
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
                    Arg::with_name("sort")
                        .help("Within each symbol category (functions, data), generate symbols in order by address")
                        .short("s")
                        .long("sort"),
                    Arg::with_name("output directory")
                        .help("Output directory")
                        .takes_value(true)
                        .short("o")
                        .long("output-dir")
                        .default_value("out")
                        .required(true),
                    Arg::with_name("input")
                        .help("Input resymgen YAML file name(s)")
                        .required(true)
                        .multiple(true)
                        .index(1),
                ]),
        )
        .subcommand(
            SubCommand::with_name("fmt")
                .about("Formats a resymgen YAML file")
                .args(&[
                    Arg::with_name("recursive")
                        .help("Recursively format the given file and its subregion files")
                        .short("r")
                        .long("recursive"),
                    Arg::with_name("check")
                        .help("Run in 'check' mode. If the input is improperly formatted, exit with 1 and print a diff.")
                        .short("c")
                        .long("check"),
                    Arg::with_name("decimal")
                        .help("Write integers in decimal format. By default integers are written as hexadecimal.")
                        .short("d")
                        .long("decimal"),
                    Arg::with_name("input")
                        .help("Input resymgen YAML file name(s)")
                        .required(true)
                        .multiple(true)
                        .index(1),
                ]),
        )
        .subcommand(
            SubCommand::with_name("check")
                .about("Validates the contents of a resymgen YAML file")
                .args(&[
                    Arg::with_name("recursive")
                        .help("Recursively validate the given file and its subregion files")
                        .short("r")
                        .long("recursive"),
                    Arg::with_name("explicit versions")
                        .help("Require versions to be explicitly specified in maps")
                        .short("v")
                        .long("explicit-versions"),
                    Arg::with_name("complete version list")
                        .help("Require versions used in maps to appear exactly once in the top-level version list. If the --recursive option is specified, also require version lists to contain all versions used in subregions.")
                        .short("V")
                        .long("complete-version-list"),
                    Arg::with_name("nonempty maps")
                        .help("Require maps to have at least one entry")
                        .short("m")
                        .long("nonempty-maps"),
                    Arg::with_name("unique symbols")
                        .help("Require symbol names to be unique within a block. If the --recursive option is specified, require symbol names to be unique within a block and its subregions, and require subregion names to be unique within a block.")
                        .short("u")
                        .long("unique-symbols"),
                    Arg::with_name("in-bounds symbols")
                        .help("Require symbols to be within specified per-version block bounds. If the --recursive option is specified, also require subregions to be within the per-version bounds of their parent blocks.")
                        .short("b")
                        .long("in-bounds-symbols"),
                    Arg::with_name("no overlap")
                        .help("Disallow per-version overlap between functions within a block. If the --recursive option is specified, also disallow per-version overlap between a subregion and any other subregion, function, or data within a block.")
                        .short("o")
                        .long("no-overlap"),
                    Arg::with_name("function names")
                        .help("Enforce a naming convention for function symbols. This option can be specified multiple times with different values to enforce that at least one of the specified naming conventions applies for each symbol. Note that all conventions implicitly enforce valid identifiers.")
                        .takes_value(true)
                        .short("f")
                        .long("function-names")
                        .multiple(true)
                        .number_of_values(1)
                        .set(ArgSettings::CaseInsensitive)
                        .possible_values(&SUPPORTED_NAMING_CONVENTIONS),
                    Arg::with_name("data names")
                        .help("Enforce a naming convention for data symbols. This option can be specified multiple times with different values to enforce that at least one of the specified naming conventions applies for each symbol. Note that all conventions implicitly enforce valid identifiers.")
                        .takes_value(true)
                        .short("d")
                        .long("data-names")
                        .multiple(true)
                        .number_of_values(1)
                        .set(ArgSettings::CaseInsensitive)
                        .possible_values(&SUPPORTED_NAMING_CONVENTIONS),
                    Arg::with_name("input")
                        .help("Input resymgen YAML file name(s)")
                        .required(true)
                        .multiple(true)
                        .index(1),
                ]),
        )
        .subcommand(
            SubCommand::with_name("merge")
                .about("Merge one or more data files into a resymgen YAML file and its subregion files")
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

            let input_files = matches.values_of("input").unwrap();
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
            let sort_output = matches.is_present("sort");

            let mut errors = Vec::with_capacity(input_files.len());
            for input_file in input_files {
                let run_gen = || -> Result<(), Box<dyn Error>> {
                    let input_file_stem = Path::new(input_file)
                        .file_stem()
                        .ok_or("Empty input file name")?;
                    let output_base = Path::new(output_dir).join(input_file_stem);
                    resymgen::generate_symbol_tables(
                        input_file,
                        output_formats.clone(),
                        output_versions.clone(),
                        sort_output,
                        output_base,
                    )?;
                    Ok(())
                };
                if let Err(e) = run_gen() {
                    errors.push((input_file.to_string(), e));
                }
            }
            if errors.is_empty() {
                Ok(())
            } else {
                Err(MultiFileError {
                    base_msg: "Failed to generate symbols".to_string(),
                    errors,
                }
                .into())
            }
        }
        Some("fmt") => {
            let matches = matches.subcommand_matches("fmt").unwrap();

            let input_files = matches.values_of("input").unwrap();
            let recursive = matches.is_present("recursive");
            let iformat = int_format(matches.is_present("decimal"));
            if matches.is_present("check") {
                let mut errors = Vec::with_capacity(input_files.len());
                let mut failed = false;
                for input_file in input_files {
                    match resymgen::format_check_file(input_file, recursive, iformat) {
                        Ok(success) => {
                            if !success {
                                println!();
                                failed = true;
                            }
                        }
                        Err(e) => errors.push((input_file.to_string(), e)),
                    };
                }
                if !errors.is_empty() {
                    return Err(MultiFileError {
                        base_msg: "Could not complete format check".to_string(),
                        errors,
                    }
                    .into());
                }
                if failed {
                    return Err("Formatting issues detected.".into());
                }
            } else {
                let mut errors = Vec::with_capacity(input_files.len());
                for input_file in input_files {
                    if let Err(e) = resymgen::format_file(input_file, recursive, iformat) {
                        errors.push((input_file.to_string(), e));
                    }
                }
                if !errors.is_empty() {
                    return Err(MultiFileError {
                        base_msg: "Formatting failed".to_string(),
                        errors,
                    }
                    .into());
                }
            }
            Ok(())
        }
        Some("check") => {
            let matches = matches.subcommand_matches("check").unwrap();

            let input_files = matches.values_of("input").unwrap();
            let recursive = matches.is_present("recursive");

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
            if matches.is_present("no overlap") {
                checks.push(resymgen::Check::NoOverlap);
            }
            if let Some(convs) = matches.values_of("function names") {
                checks.push(resymgen::Check::FunctionNames(
                    convs.map(naming_convention).collect(),
                ));
            }
            if let Some(convs) = matches.values_of("data names") {
                checks.push(resymgen::Check::DataNames(
                    convs.map(naming_convention).collect(),
                ));
            }
            // This one handles multiple files internally so that check result printing
            // can be merged appropriately
            if !resymgen::run_and_print_checks(input_files.collect::<Vec<_>>(), &checks, recursive)?
            {
                return Err("Checks did not pass".into());
            }
            Ok(())
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
                symgen_file,
                &input_files,
                input_format,
                &merge_params,
                iformat,
            )?;
            if fix_formatting {
                resymgen::format_file(symgen_file, true, iformat)?;
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
                                .map(|s| s.name.clone())
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
                return Err(e.into());
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
    process::exit(match run_resymgen() {
        Ok(_) => 0,
        Err(err) => {
            let mut stderr = StandardStream::stderr(ColorChoice::Always);
            let mut print_err = || -> io::Result<()> {
                // Print the "ERROR" in red to be eye-catching
                let mut color = ColorSpec::new();
                stderr.set_color(color.set_fg(Some(Color::Red)))?;
                write!(&mut stderr, "ERROR")?;
                stderr.reset()?;
                writeln!(&mut stderr, ": {}", err)?;
                Ok(())
            };
            if let Err(e) = print_err() {
                // Normal eprintln the IO error if print_err fails for some reason
                eprintln!("Internal error: {}", e);
            }
            // Always try to clean up color settings before returning
            if let Err(e) = stderr.reset() {
                eprintln!("Internal error: {}", e);
            }
            1
        }
    });
}
