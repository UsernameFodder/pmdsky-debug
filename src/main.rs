#[macro_use]
extern crate clap;

use std::error::Error;

use clap::{App, AppSettings, Arg, SubCommand};

use resymgen;

fn int_format(write_as_decimal: bool) -> resymgen::IntFormat {
    if write_as_decimal {
        resymgen::IntFormat::Decimal
    } else {
        resymgen::IntFormat::Hexadecimal
    }
}

fn run_resymgen() -> Result<(), Box<dyn Error>> {
    let matches = App::new(crate_name!())
        .version(crate_version!())
        .author("UsernameFodder")
        .about("Generates symbol tables for reverse engineering applications from a YAML specification.")
        .setting(AppSettings::ArgRequiredElseHelp)
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
        .get_matches();

    match matches.subcommand_name() {
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
