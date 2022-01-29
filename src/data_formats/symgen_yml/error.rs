//! Error types for this module.

use std::fmt::{self, Display, Formatter};
use std::{error, io, result, string};

use serde_yaml;

use super::merge::{BlockInferenceError, MergeConflict, MissingBlock};

/// Error encountered during processing a [`SymGen`].
///
/// [`SymGen`]: super::SymGen
#[derive(Debug)]
pub enum Error {
    Yaml(serde_yaml::Error),
    Io(io::Error),
    FromUtf8(string::FromUtf8Error),
}

impl error::Error for Error {}

impl Display for Error {
    fn fmt(&self, f: &mut Formatter) -> fmt::Result {
        match self {
            // YAML errors are probably the most common error a user will encounter,
            // so it's worth labeling it explicitly.
            Self::Yaml(e) => write!(f, "YAML: {}", e),
            Self::Io(e) => write!(f, "{}", e),
            Self::FromUtf8(e) => write!(f, "{}", e),
        }
    }
}

/// `Result` from processing a [`SymGen`].
///
/// [`SymGen`]: super::SymGen
pub type Result<T> = result::Result<T, Error>;

/// Error encountered while merging into a [`SymGen`].
///
/// [`SymGen`]: super::SymGen
#[derive(Debug)]
pub enum MergeError {
    Conflict(MergeConflict),
    MissingBlock(MissingBlock),
    BlockInference(BlockInferenceError),
}

impl error::Error for MergeError {}

impl Display for MergeError {
    fn fmt(&self, f: &mut Formatter) -> fmt::Result {
        match self {
            Self::Conflict(e) => write!(f, "{}", e),
            Self::MissingBlock(e) => write!(f, "{}", e),
            Self::BlockInference(e) => write!(f, "{}", e),
        }
    }
}
