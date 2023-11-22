# Contributing to `pmdsky-debug`
Contributions are welcome, no matter how small! This can be new symbols, new type definitions, changes to `resymgen`, bug fixes, documentation, etc. Make sure to add yourself to [`CREDITS.txt`](../CREDITS.txt) if you're a new contributor!

You can contribute by opening a pull request. If you've never done this before, [this tutorial](https://github.com/firstcontributions/first-contributions/blob/main/README.md) is a friendly introduction to the process. You can also follow GitHub's official [quickstart guide](https://docs.github.com/en/get-started/quickstart/contributing-to-projects), if you prefer.

This repository is designed to make contributing simple while maintaining quality. No setup is required to contribute to the symbol tables and C headers beyond a normal text editor and the ability to create pull requests on GitHub. When you open or update a pull request, the relevant GitHub Actions jobs will automatically test your changes (and run the formatter for you if necessary). Changes to `resymgen` require the standard Rust toolchain.

## Symbols (functions and data)
See the [`symbols/` README](../symbols/README.md#contributing) for more information on adding new symbols.

**NOTE:** If you're adding symbols, you might also want to add [new types](#types-structs-enums-function-signatures-global-declarations) to accompany them. This is optional but recommended, especially if you're adding functions. Providing function signatures in the C headers allows you to strictly specify function parameters and their types. Providing global variable declarations in the C headers allows you to strictly specify data structure types.

## Types (structs, enums, function signatures, global declarations)
See the [`headers/` README](../headers/README.md#contributing) for more information on adding new types.

## Incomplete information that needs more research
It's common in reverse engineering to have only a vague idea about how something works. For example, you might have a suspicion about what a function does without being certain about it. Partial or incomplete information is still better than nothing, so feel free to contribute symbols and types anyway (just note the uncertainty in the symbol description and/or as a comment in the C headers). Alternatively, if you feel that more understanding is necessary to make a proper contribution, you can also [file a new issue](https://github.com/UsernameFodder/pmdsky-debug/issues) using the "Research topic" issue template.

## `resymgen`
If you want to contribute to `resymgen` (e.g., adding support for more data formats), you'll need to [install Rust](https://www.rust-lang.org/tools/install). The `resymgen` package is a standard [Cargo](https://doc.rust-lang.org/cargo/) project (so build with `cargo build`, run tests with `cargo test`, etc.). You can view the rendered library docs on [Docs.rs](https://docs.rs/resymgen/latest/resymgen/).

## Documentation
If you see an error in the existing docs, feel free to make a pull request to correct it. If you want to add a new guide, tutorial, reference, etc., please add it to the [`docs/`](.) directory.
