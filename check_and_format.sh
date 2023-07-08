#!/bin/sh

set -e

pushd headers
make
make format
make symbol-check
popd

cargo run -- check -r -Vvbomu -d screaming_snake_case -f pascalcase -f snake_case symbols/**.yml
cargo run -- fmt -r symbols/**.yml