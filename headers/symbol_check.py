#!/usr/bin/env python3

# Script to check that all function and data symbol names present in the C
# headers are also present in the symbol tables.

from abc import ABC
import argparse
import difflib
import os
import re
from typing import Dict, Generator, List, Optional
import yaml

ROOT_DIR = os.path.relpath(os.path.join(os.path.dirname(__file__), ".."))
SYMBOLS_DIR = os.path.join(ROOT_DIR, "symbols")


class HeaderSymbolList(ABC):
    """
    A list of all symbols of some type defined for a specific binary file,
    including the C headers and the symbol tables.
    """

    HEADERS_DIR: str  # Path to directory containing the C headers
    SYMBOL_LIST_KEY: str  # YAML key of the corresponding symbol list
    # Regex for finding symbol names within C headers, with one capturing group
    # for the symbol name
    NAME_REGEX: re.Pattern

    header_file: str
    symbol_file: str
    # Caches to avoid doing unnecessary file I/O
    cached_header_names: Optional[List[str]]
    cached_symbol_names: Optional[List[str]]

    def __init__(self, header_file: str):
        assert hasattr(self, "HEADERS_DIR")
        assert hasattr(self, "SYMBOL_LIST_KEY")
        assert hasattr(self, "NAME_REGEX")

        self.cached_header_names = None
        self.cached_symbol_names = None
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

    @staticmethod
    def header_file_strip_comments(contents: str) -> str:
        # Remove "//" comments
        contents = re.sub(r"//[^\n]*\n", "\n", contents)
        # Remove "/* */" comments
        contents = re.sub(r"/\*.*?\*/", "", contents, flags=re.DOTALL)
        return contents

    @classmethod
    def names_from_c_header(cls, fpath: str) -> List[str]:
        with open(fpath, "r") as f:
            return cls.NAME_REGEX.findall(cls.header_file_strip_comments(f.read()))

    def names_from_header_file(self) -> List[str]:
        if self.cached_header_names is None:
            self.cached_header_names = self.names_from_c_header(self.header_file)
        return list(self.cached_header_names)

    def names_from_symbol_file(self) -> List[str]:
        if self.cached_symbol_names is None:
            with open(self.symbol_file, "r") as f:
                self.cached_symbol_names = [
                    symbol["name"]
                    for block in yaml.safe_load(f).values()
                    for symbol in block[self.SYMBOL_LIST_KEY]
                ]
        return list(self.cached_symbol_names)

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

    def order_diff(self) -> List[str]:
        """
        Return a diff between the symbol tables and the C headers, including only
        the symbols that are in both.
        """
        symbol_names = self.names_from_symbol_file()
        header_names = self.names_from_header_file()

        common_symbols = set(symbol_names) & set(header_names)
        symbol_order = [name for name in symbol_names if name in common_symbols]
        header_order = [name for name in header_names if name in common_symbols]
        return list(
            difflib.unified_diff(
                header_order,
                symbol_order,
                fromfile=f"[old] {self.header_file}",
                tofile=f"[new] {self.header_file} (matches {self.symbol_file})",
                n=1,
                lineterm="",
            )
        )


class FunctionList(HeaderSymbolList):
    HEADERS_DIR = os.path.join(ROOT_DIR, "headers", "functions")
    SYMBOL_LIST_KEY = "functions"
    # Look for words preceding an open parenthesis. Not perfect, but
    # good enough. This should work for everything except functions
    # with function pointer parameters, but function pointer parameters
    # should really be typedef'd for readability anyway.
    NAME_REGEX = re.compile(r"\b(\w+)\s*\(")


class DataList(HeaderSymbolList):
    HEADERS_DIR = os.path.join(ROOT_DIR, "headers", "data")
    SYMBOL_LIST_KEY = "data"
    # Look for words preceding a semicolon, with optional closing
    # parentheses and/or pairs of square brackets in between. This
    # should work for most things, like:
    # - normal variables `int x;`
    # - arrays `int x[10];`
    # - array pointers `int (*x)[10]`
    # It won't work for function pointers `int (*f)(int, int)`, but
    # these should really be typedef'd for readability anyway.
    NAME_REGEX = re.compile(r"\b(\w+)(?:\s*(?:\)|\[\s*\w+\s*\]))*\s*;")


def run_symbol_check(
    symbol_list: HeaderSymbolList,
    symbol_type_str: str,
    find_extra_symbols: bool = False,
    check_order: bool = False,
    verbose: bool = False,
) -> bool:
    passed = True
    for header_file in symbol_list.headers():
        try:
            slist = symbol_list(header_file)
            missing = slist.missing_symbols()
            extra = slist.extra_symbols() if find_extra_symbols else []
            order_diff = slist.order_diff() if check_order else []
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
                print()
            if extra:
                print(f"{slist}: found {len(extra)} untyped symbols in symbol tables")
                print(
                    f"The following {symbol_type_str} are present in"
                    + f" {slist.symbol_file} but missing from"
                    + f" {slist.header_file}:"
                )
                for symbol_name in extra:
                    print(f"    - {symbol_name}")
                print()
            if order_diff:
                passed = False
                print(
                    f"{slist}: found ordering discrepancies between"
                    + f" {symbol_type_str} in the C headers and symbol tables."
                )
                print(
                    "To match the symbol tables, make the following changes"
                    + " to the C headers:"
                )
                print("\n".join(order_diff))
                print()
            if not missing and not extra and not order_diff and verbose:
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
    parser.add_argument(
        "-s",
        "--sort-order",
        action="store_true",
        help="check sort order in the C headers based on symbol table order",
    )
    parser.add_argument("-v", "--verbose", action="store_true", help="verbose output")
    args = parser.parse_args()

    functions_passed = run_symbol_check(
        FunctionList,
        "functions",
        find_extra_symbols=args.extra_symbols,
        check_order=args.sort_order,
        verbose=args.verbose,
    )
    data_passed = run_symbol_check(
        DataList,
        "data symbols",
        find_extra_symbols=args.extra_symbols,
        check_order=args.sort_order,
        verbose=args.verbose,
    )
    if not functions_passed or not data_passed:
        raise SystemExit(1)
