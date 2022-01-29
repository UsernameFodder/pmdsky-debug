//! The `resymgen` YAML format and associated APIs to integrate with other data formats.
//!
//! [`SymGen`] represents an entire `resymgen` YAML file. Through this type and its methods,
//! `resymgen` files can be read from files, manipulated in various ways, and written to files.
//!
//! A `resymgen` YAML file consists of one or more named [`Block`]s. Each [`Block`] is tagged with
//! some metadata, and contains two lists of [`Symbol`]s, one for functions and one for data.
//! A [`Symbol`] represents one or more memory regions containing an identifiable chunk of
//! instructions or data.
//!
//! # Example
//! ```yml
//! main:
//!   versions:
//!     - v1
//!     - v2
//!   address:
//!     v1: 0x2000000
//!     v2: 0x2010000
//!   length:
//!     v1: 0x100000
//!     v2: 0x100000
//!   description: The main memory region
//!   functions:
//!     - name: function1
//!       address:
//!         v1: 0x2001000
//!         v2: 0x2012000
//!       description: |-
//!         multi
//!         line
//!         description
//!     - name: function2
//!       address:
//!         v1:
//!           - 0x2002000
//!           - 0x2003000
//!         v2: 0x2013000
//!       description: simple description
//!   data:
//!     - name: SOME_DATA
//!       address:
//!         v1: 0x2000000
//!         v2: 0x2010000
//!       length:
//!         v1: 0x1000
//!         v2: 0x1600
//! other:
//!   address: 0x2400000
//!   length: 0x100000
//!   functions: []
//!   data:
//!     - name: OTHER_DATA
//!       address: 0x2400000
//! ```

mod adapter;
mod error;
mod merge;
mod symgen;
mod types;

pub mod bounds;

pub use adapter::*;
pub use error::*;
pub use symgen::*;
pub use types::{Linkable, MaybeVersionDep, OrdString, OrderMap, Sort, Uint, Version, VersionDep};
