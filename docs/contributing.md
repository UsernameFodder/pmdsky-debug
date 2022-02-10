# Contributing to `pmdsky-debug`
Contributions are welcome, no matter how small! This can be new symbols, new type definitions, changes to `resymgen`, bug fixes, documentation, etc. Make sure add yourself to [`CREDITS.txt`](../CREDITS.txt) if you're a new contributor!

This repository is designed to make contributing simple while maintaining quality. No setup is required to contribute to the symbol tables and C headers beyond a normal text editor and the ability to create pull requests on GitHub. When you open or update a pull request, the relevant GitHub Actions jobs will automatically test your changes (and run the formatter for you if necessary). Changes to `resymgen` require the standard Rust toolchain.

## Symbols (functions and data)
See the [`symbols/` README](../symbols/README.md#contributing) for more information on adding new symbols.

**NOTE:** If you're adding symbols, you might also want to add [new types](#types-structs-enums-function-signatures) to accompany them. This is optional but recommended, especially if you're adding functions. Providing function signatures in the C headers allows you to strictly specify function parameters and their types.

## Types (structs, enums, function signatures)
See the [`headers/` README](../headers/README.md#contributing) for more information on adding new types.

## `resymgen`
If you want to contribute to `resymgen` (e.g., adding support for more data formats), you'll need to [install Rust](https://www.rust-lang.org/tools/install). The `resymgen` package is a standard [Cargo](https://doc.rust-lang.org/cargo/) project (so build with `cargo build`, run tests with `cargo test`, etc.). You can view the rendered library docs on [Docs.rs](https://docs.rs/resymgen/latest/resymgen/).

## Documentation
If you see an error in the existing docs, feel free to make a pull request to correct it. If you want to add a new guide, tutorial, reference, etc., please add it to the [`docs/`](.) directory.
