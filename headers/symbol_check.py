#!/usr/bin/env python3

# Script to check that all function names present in the C headers are also
# present in the symbol tables.

from abc import ABC, abstractmethod
import argparse
import os
import re
from typing import Dict, Generator, List, Optional
import yaml

ROOT_DIR = os.path.relpath(os.path.join(os.path.dirname(__file__), ".."))
SYMBOLS_DIR = os.path.join(ROOT_DIR, "symbols")


def get_headers(header_dir: str) -> Generator[str, None, None]:
    for root, dirs, files in os.walk(header_dir):
        dirs.sort()  # Ensure subdirectories are visited in sorted order
        for f in sorted(files):
            if f.endswith(".h"):
                yield os.path.join(root, f)


class HeaderSymbolList(ABC):
    """
    A list of all symbols of some type defined for a specific binary file,
    including the C headers and the symbol tables.
    """

    HEADERS_DIR: str  # Path to directory containing the C headers
    SYMBOL_LIST_KEY: str  # YAML key of the corresponding symbol list

    header_file: str
    symbol_file: str

    def __init__(self, header_file: str):
        assert hasattr(self, "HEADERS_DIR")
        assert hasattr(self, "SYMBOL_LIST_KEY")

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

    @classmethod
    def file_stem(cls, header_file: str) -> str:
        return os.path.splitext(os.path.relpath(header_file, start=cls.HEADERS_DIR))[0]

    @classmethod
    def headers(cls) -> Generator[str, None, None]:
        for root, dirs, files in os.walk(cls.HEADERS_DIR):
            dirs.sort()  # Ensure subdirectories are visited in sorted order
            for f in sorted(files):
                if f.endswith(".h"):
                    yield os.path.join(root, f)

    @classmethod
    def get_symbol_file(cls, header_file: str) -> Optional[str]:
        """
        Get the symbol file name that corresponds to the given header file.
        """
        fname = os.path.join(SYMBOLS_DIR, cls.file_stem(header_file) + ".yml")
        return fname if os.path.isfile(fname) else None

    @abstractmethod
    def names_from_header_file(self) -> List[str]:
        return []

    def names_from_symbol_file(self) -> List[str]:
        with open(self.symbol_file, "r") as f:
            return [
                symbol["name"]
                for block in yaml.safe_load(f).values()
                for symbol in block[self.SYMBOL_LIST_KEY]
            ]

    def missing_symbols(self) -> List[str]:
        """
        Find symbols that are in the C headers but not in the symbol tables.
        """
        return list(
            set(self.names_from_header_file()) - set(self.names_from_symbol_file())
        )

    def extra_symbols(self) -> List[str]:
        """
        Find symbols that are in the symbol tables but not in the C headers.
        """
        return list(
            set(self.names_from_symbol_file()) - set(self.names_from_header_file())
        )


class FunctionList(HeaderSymbolList):
    HEADERS_DIR = os.path.join(ROOT_DIR, "headers", "functions")
    SYMBOL_LIST_KEY = "functions"

    def names_from_header_file(self) -> List[str]:
        with open(self.header_file, "r") as f:
            # Look for words preceding an open parenthesis. Not perfect, but
            # good enough. This should work for everything except functions
            # with function pointer parameters, but function pointer parameters
            # should really be typedef'd for readability anyway.
            return re.findall(r"\b(\w+)\s*\(", f.read())


class DataList(HeaderSymbolList):
    HEADERS_DIR = os.path.join(ROOT_DIR, "headers", "data")
    SYMBOL_LIST_KEY = "data"

    def names_from_header_file(self) -> List[str]:
        with open(self.header_file, "r") as f:
            # Look for words preceding a semicolon, with optional closing
            # parentheses and/or pairs of square brackets in between. This
            # should work for most things, like:
            # - normal variables `int x;`
            # - arrays `int x[10];`
            # - array pointers `int (*x)[10]`
            # It won't work for function pointers `int (*f)(int, int)`, but
            # these should really be typedef'd for readability anyway.
            return re.findall(r"\b(\w+)(?:\s*(?:\)|\[\s*\w+\s*\]))*\s*;", f.read())


def run_symbol_check(
    symbol_list: HeaderSymbolList,
    symbol_type_str: str,
    find_extra_symbols: bool = False,
    verbose: bool = False,
) -> bool:
    passed = True
    for header_file in symbol_list.headers():
        try:
            slist = symbol_list(header_file)
            missing = slist.missing_symbols()
            extra = slist.extra_symbols() if find_extra_symbols else []
            if missing:
                passed = False
                print(
                    f"{slist}: found {len(missing)} discrepancies between"
                    + " C headers and symbol tables."
                )
                print(
                    f"The following {symbol_type_str} are present in"
                    + f" {slist.header_file} but missing from"
                    + f" {slist.symbol_file}:"
                )
                for symbol_name in missing:
                    print(f"    - {symbol_name}")
            if extra:
                print(f"{slist}: found {len(extra)} untyped symbols in symbol tables")
                print(
                    f"The following {symbol_type_str} are present in"
                    + f" {slist.symbol_file} but missing from"
                    + f" {slist.header_file}:"
                )
                for symbol_name in extra:
                    print(f"    - {symbol_name}")
            if not missing and not extra and verbose:
                print(f"Checked {header_file}")
        except ValueError:
            # File doesn't correspond to a symbol file; skip
            pass
    return passed


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Cross-check the C headers with the symbol tables"
    )
    parser.add_argument(
        "-e",
        "--extra-symbols",
        action="store_true",
        help="print extra symbols present in the symbol tables but not in the C headers",
    )
    parser.add_argument("-v", "--verbose", action="store_true", help="verbose output")
    args = parser.parse_args()

    functions_passed = run_symbol_check(
        FunctionList,
        "functions",
        find_extra_symbols=args.extra_symbols,
        verbose=args.verbose,
    )
    data_passed = run_symbol_check(
        DataList,
        "data symbols",
        find_extra_symbols=args.extra_symbols,
        verbose=args.verbose,
    )
    if not functions_passed or not data_passed:
        raise SystemExit(1)
