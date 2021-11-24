use std::fmt::{self, Display, Formatter};
use std::{error, io, result, string};

use serde_yaml;

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

pub type Result<T> = result::Result<T, Error>;
