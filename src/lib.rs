//! # Working with the `resymgen` YAML format
//!
//! This crate contains utilities for working with the YAML configuration files of the `resymgen`
//! (**r**everse **e**ngineering **sym**bol table **gen**erator) command line tool.
//!
//! The top-level library functions implement the subcommands of `resymgen`. That is, they take
//! input files containing symbol data, process the file contents based on specified configuration
//! parameters, and generate output related to the symbol data in the form of console output or
//! files.
//!
//! The [`data_formats`] module defines structures and methods related to parsing and manipulating
//! raw symbol data in various formats.

mod checks;
pub mod data_formats;
mod formatting;
mod transform;
mod util;

pub use checks::*;
pub use data_formats::symgen_yml::{IntFormat, LoadParams, SymbolType};
pub use data_formats::{InFormat, OutFormat};
pub use formatting::*;
pub use transform::*;
pub use util::*;
