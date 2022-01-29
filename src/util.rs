use std::error::Error;
use std::fmt::{self, Display, Formatter};

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
