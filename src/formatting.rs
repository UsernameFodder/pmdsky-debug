//! Formatting `resymgen` YAML files. Implements the `fmt` command.

use std::error::Error;
use std::fmt::Display;
use std::fs::{self, File};
use std::io::{self, Write};
use std::path::Path;

use similar::TextDiff;
use termcolor::{Color, ColorChoice, ColorSpec, StandardStream, WriteColor};

use super::data_formats::symgen_yml::{IntFormat, Sort, Subregion, SymGen};
use super::util;

/// Formats a given `input_file` using the given `int_format`.
///
/// In `recursive` mode, subregion files are also formatted.
///
/// # Examples
/// ```ignore
/// format_file("/path/to/symbols.yml", false, IntFormat::Hexadecimal).expect("Format failed");
/// ```
pub fn format_file<P: AsRef<Path>>(
    input_file: P,
    recursive: bool,
    int_format: IntFormat,
) -> Result<(), Box<dyn Error>> {
    let input_file = input_file.as_ref();
    let mut contents = {
        let f = File::open(input_file)?;
        SymGen::read(&f)?
    };
    if recursive {
        contents.resolve_subregions(Subregion::subregion_dir(input_file), |p| File::open(p))?;
    }
    contents.sort();
    util::symgen_write_recursive(&contents, input_file, int_format)
}

/// Checks the format of a given `input_file`, subject to the given `int_format`.
///
/// In `recursive` mode, subregion files are also checked.
///
/// On success, returns `true`. On failure, returns `false` and prints a diff.
///
/// # Examples
/// ```ignore
/// let succeeded = format_check_file("/path/to/symbols.yml", false, IntFormat::Hexadecimal)
///     .expect("Format check failed");
/// ```
pub fn format_check_file<P: AsRef<Path>>(
    input_file: P,
    recursive: bool,
    int_format: IntFormat,
) -> Result<bool, Box<dyn Error>> {
    let input_file = input_file.as_ref();
    let mut contents = {
        let f = File::open(input_file)?;
        SymGen::read(&f)?
    };
    if recursive {
        contents.resolve_subregions(Subregion::subregion_dir(input_file), |p| File::open(p))?;
    }
    contents.sort();

    let mut success = true;
    // Depth-first traversal is more intuitive for reporting formatting issues
    for cursor in contents.cursor(input_file).dtraverse() {
        // It's unfortunate we're reading the same file twice, but it's simpler than trying to
        // resolve subregions manually, and less memory intensive than caching. If this ever
        // becomes a performance issue, it can be optimized.
        let text = fs::read_to_string(cursor.path())?;
        let formatted_text = cursor.symgen().write_to_str(int_format)?;
        if text != formatted_text {
            print_format_diff(&text, &formatted_text, cursor.path().display())?;
            // Keep going to check any other subregion files, but fail the check as a whole
            success = false;
        }
    }
    Ok(success)
}

/// Prints a diff between a file and its formatted version in unified diff format.
/// The title is printed as part of the diff header.
fn print_format_diff<D: Display>(old: &str, new: &str, title: D) -> io::Result<()> {
    let mut stderr = StandardStream::stderr(ColorChoice::Always);
    let mut print_colored_diff = || -> io::Result<()> {
        let diff = TextDiff::from_lines(old, new)
            .unified_diff()
            .header(&format!("[cur] {}", title), &format!("[fmt] {}", title))
            .to_string();
        let mut color = ColorSpec::new();
        // Manually add pretty colors to the diff output (based on `git diff`) :)
        for (i, line) in diff.lines().enumerate() {
            // Don't color the first 2 lines from the header
            if i > 1 && line.starts_with('+') {
                stderr.set_color(color.set_fg(Some(Color::Green)))?;
                let trimmed = line.trim_end();
                if trimmed.len() < line.len() {
                    // Trailing whitespace on added lines should be colored red
                    write!(&mut stderr, "{}", trimmed)?;
                    stderr.set_color(color.set_bg(Some(Color::Red)))?;
                    write!(&mut stderr, "{}", &line[trimmed.len()..])?;
                    color.clear();
                    stderr.reset()?;
                    writeln!(&mut stderr)?;
                } else {
                    writeln!(&mut stderr, "{}", line)?;
                }
            } else if i > 1 && line.starts_with('-') {
                stderr.set_color(color.set_fg(Some(Color::Red)))?;
                writeln!(&mut stderr, "{}", line)?;
            } else if let Some(stripped_line) = line.strip_prefix("@@") {
                // Only color the stuff between the first two instances of "@@"
                stderr.set_color(color.set_fg(Some(Color::Cyan)))?;
                let end = match stripped_line.find("@@") {
                    Some(end_shifted) => {
                        end_shifted + 4 // + 2 for the shift, + 2 to move past the found "@@"
                    }
                    None => line.len(), // Color the whole line
                };
                write!(&mut stderr, "{}", &line[..end])?;
                stderr.reset()?;
                writeln!(&mut stderr, "{}", &line[end..])?;
            } else {
                stderr.reset()?;
                writeln!(&mut stderr, "{}", line)?;
            }
        }
        Ok(())
    };
    let res = print_colored_diff();
    // Always try to clean up color settings before returning
    stderr.reset()?; // throw away Ok() output
    res
}
