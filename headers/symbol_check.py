#!/usr/bin/env python3

# Script to check that all function names present in the C headers are also
# present in the symbol tables.

import os
import re
from typing import Dict, Generator, List, Optional
import yaml

ROOT_DIR = os.path.relpath(os.path.join(os.path.dirname(__file__), ".."))
HEADERS_FUNCTIONS_DIR = os.path.join(ROOT_DIR, "headers", "functions")
SYMBOLS_DIR = os.path.join(ROOT_DIR, "symbols")


def get_function_headers() -> Generator[str, None, None]:
    for root, dirs, files in os.walk(HEADERS_FUNCTIONS_DIR):
        dirs.sort()  # Ensure subdirectories are visited in sorted order
        for f in sorted(files):
            if f.endswith(".h"):
                yield os.path.join(root, f)


class FunctionList:
    """
    A list of all functions defined for a specific binary file, including the
    C headers and the symbol tables.
    """

    header_file: str
    symbol_file: str

    def __init__(self, header_file: str):
        self.header_file = header_file
        symbol_file = self.get_symbol_file(self.header_file)
        if symbol_file is not None:
            self.symbol_file = symbol_file
        else:
            raise ValueError(
                f"Header file '{header_file}' has no corresponding symbol file"
            )

    def __str__(self) -> str:
        return self.file_stem(self.header_file)

    @staticmethod
    def file_stem(header_file: str) -> str:
        return os.path.splitext(
            os.path.relpath(header_file, start=HEADERS_FUNCTIONS_DIR)
        )[0]

    @staticmethod
    def get_symbol_file(header_file: str) -> Optional[str]:
        """
        Get the symbol file name that corresponds to the given header file.
        """
        fname = os.path.join(SYMBOLS_DIR, FunctionList.file_stem(header_file) + ".yml")
        return fname if os.path.isfile(fname) else None

    def functions_from_header_file(self) -> List[str]:
        with open(self.header_file, "r") as f:
            return re.findall(r"\b(\w+)\s*\(", f.read())

    def functions_from_symbol_file(self) -> List[str]:
        with open(self.symbol_file, "r") as f:
            return [
                symbol["name"]
                for block in yaml.safe_load(f).values()
                for symbol in block["functions"]
            ]

    def missing_functions(self) -> List[str]:
        """
        Find functions that are in the C headers but not in the symbol tables.
        """
        return list(
            set(self.functions_from_header_file())
            - set(self.functions_from_symbol_file())
        )


def run_symbol_check() -> bool:
    passed = True
    for header_file in get_function_headers():
        try:
            flist = FunctionList(header_file)
            missing = flist.missing_functions()
            if missing:
                passed = False
                print(
                    f"{flist}: found {len(missing)} discrepancies between"
                    + " C headers and symbol tables."
                )
                print(
                    f"The following functions are present in"
                    + f" {flist.header_file} but missing from"
                    + f" {flist.symbol_file}:"
                )
                for symbol_name in missing:
                    print(f"    - {symbol_name}")
        except ValueError:
            # File doesn't correspond to a symbol file; skip
            pass
    return passed


if __name__ == "__main__":
    if not run_symbol_check():
        raise SystemExit(1)
