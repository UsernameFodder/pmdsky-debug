/// formatting resymgen YAML files
use std::error::Error;
use std::fs::{self, File};
use std::io::{self, Write};
use std::path::Path;

use similar::TextDiff;
use tempfile::NamedTempFile;
use termcolor::{Color, ColorChoice, ColorSpec, StandardStream, WriteColor};

use super::data_formats::symgen_yml::{IntFormat, SymGen};

/// Formats a given input file.
pub fn format_file<P: AsRef<Path>>(
    input_file: P,
    int_format: IntFormat,
) -> Result<(), Box<dyn Error>> {
    let contents = {
        let f = File::open(input_file.as_ref())?;
        SymGen::read_sorted(&f)?
    };
    // Write to a tempfile first, then replace the old one atomically.
    let f_new = NamedTempFile::new()?;
    contents.write(&f_new, int_format)?;
    f_new.persist(input_file.as_ref())?;
    Ok(())
}

/// Checks the format of a given input file. On success, returns true.
/// On format-check failure, returns false and prints a diff.
pub fn format_check_file<P: AsRef<Path>>(
    input_file: P,
    int_format: IntFormat,
) -> Result<bool, Box<dyn Error>> {
    // Need to read to string in order to diff
    let text = fs::read_to_string(input_file.as_ref())?;
    let contents = SymGen::read_sorted(text.as_bytes())?;
    let formatted_text = contents.write_to_str(int_format)?;
    if text == formatted_text {
        Ok(true)
    } else {
        print_format_diff(
            &text,
            &formatted_text,
            &format!("{}", input_file.as_ref().display()),
        )?;
        Ok(false)
    }
}

fn print_format_diff(old: &str, new: &str, title: &str) -> io::Result<()> {
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
            if i > 1 && line.starts_with("+") {
                stderr.set_color(color.set_fg(Some(Color::Green)))?;
                writeln!(&mut stderr, "{}", line)?;
            } else if i > 1 && line.starts_with("-") {
                stderr.set_color(color.set_fg(Some(Color::Red)))?;
                writeln!(&mut stderr, "{}", line)?;
            } else if line.starts_with("@@") {
                // Only color the stuff between the first two instances of "@@"
                stderr.set_color(color.set_fg(Some(Color::Cyan)))?;
                let end = match line[2..].find("@@") {
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
    if let Err(e) = stderr.reset() {
        Err(e)
    } else {
        res
    }
}
