#!/bin/sh

# Convenience script that runs the checks and formatters for both the C headers and symbol tables.
# Expects the working directory to be the repo root.

set -e

make -C headers
make -C headers format
make -C headers symbol-check

cargo run --release -- check -r -Vvbomu -d screaming_snake_case -f pascal_snake_case -f snake_case symbols/*.yml
cargo run --release -- fmt -r symbols/*.yml

