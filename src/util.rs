use std::error::Error;
use std::fmt::{self, Display, Formatter};
use std::fs;
use std::path::Path;

use tempfile::{NamedTempFile, PersistError};

use super::data_formats::symgen_yml::{IntFormat, SymGen};

/// Encapsulates a collection of similar errors for different files.
#[derive(Debug)]
pub struct MultiFileError {
    pub base_msg: String,
    pub errors: Vec<(String, Box<dyn Error>)>,
}

impl Error for MultiFileError {}

impl Display for MultiFileError {
    fn fmt(&self, f: &mut Formatter) -> fmt::Result {
        write!(f, "{} [{} file(s)]", self.base_msg, self.errors.len())?;
        for (filename, err) in self.errors.iter() {
            write!(f, "\n{}: {}", filename, err)?;
        }
        Ok(())
    }
}

/// Persist the temporary file at the target path.
///
/// This wraps `NamedTempFile::persist()` with fallback to manual copying.
/// On some systems, the normal `persist()` might not always work. For example, on Unix
/// systems `persist()` ultimately relies on `std::fs::rename()`:
/// https://github.com/Stebalien/tempfile/blob/66aa57f7d3a38234fcd393077366b61d36171e42/src/file/imp/unix.rs#L131
/// which will fail if the tempfile and target path are not on the same filesystem:
/// https://doc.rust-lang.org/std/fs/fn.rename.html. Since `/tmp` (the normal return value of
/// `std::env::temp_dir()` on Linux) can sometimes be mounted on a separate filesystem, this is
/// a fairly common case.
///
/// If the normal `persist()` method fails, try manually copying the tempfile to the destination.
/// Unlike `std::fs::rename()`, `std::fs::copy()` works even across filesystems.
pub fn persist_named_temp_file_safe<P: AsRef<Path>>(
    file: NamedTempFile,
    new_path: P,
) -> Result<(), PersistError> {
    if let Err(e) = file.persist(&new_path) {
        let file = NamedTempFile::from(e); // Grab the file handle again
        if let Err(io_err) = fs::copy(file.path(), new_path) {
            return Err(PersistError {
                error: io_err,
                file,
            });
        }
    }
    Ok(())
}

/// Recursively write a [`SymGen`] and all its subregions to files, starting with the top-level
/// file path specified by `top_path`, and using the given `int_format`.
pub fn symgen_write_recursive<P: AsRef<Path>>(
    symgen: &SymGen,
    top_path: P,
    int_format: IntFormat,
) -> Result<(), Box<dyn Error>> {
    for cursor in symgen.cursor(top_path.as_ref()).btraverse() {
        // Write to a tempfile first, then replace the old one atomically.
        let output_file = NamedTempFile::new()?;
        cursor.symgen().write(&output_file, int_format)?;
        persist_named_temp_file_safe(output_file, cursor.path())?;
    }
    Ok(())
}
