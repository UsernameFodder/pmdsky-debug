#!/usr/bin/env python3

# Script to annotate function and data declarations in the C headers with
# docstrings generated from their corresponding symbol table entries, if
# present.
#
# By default, this script edits header files in-place. It's recommended that
# you commit any header file changes before running this script.

from abc import ABC
import argparse
import fnmatch
import os
import re
import subprocess
import textwrap
from typing import Dict, Iterator, Optional, Set, Tuple
import yaml

from symbol_check import (
    FunctionList,
    DataList,
    HeaderSymbolList,
    ROOT_DIR,
)


class Formatter(ABC):
    def format_docstring(self, text: str) -> str:
        raise NotImplementedError

    def format_file(self, filename: str):
        raise NotImplementedError

    def sanitize_comment(self, comment: str) -> str:
        # Break up any end-of-comment delimiters in the raw comment text
        return comment.replace("*/", "* /")


class ClangFormatFormatter(Formatter):
    def __init__(self):
        # Check for clang-format in the runtime environment
        subprocess.run(["clang-format", "--version"], capture_output=True, check=True)

    def format_docstring(self, text: str) -> str:
        docstring = "/**\n"
        for line in text.splitlines():
            docstring += f" * {self.sanitize_comment(line)}\n"
        docstring += " */\n"
        return docstring

    def format_file(self, filename: str):
        subprocess.run(["clang-format", "-i", filename], check=True)


class TextWrapFormatter(Formatter):
    """Fallback formatter if clang-format isn't found in the environment"""

    def __init__(self):
        self.linewidth = 100
        try:
            clang_format_file = os.path.join(ROOT_DIR, "headers", ".clang-format")
            with open(clang_format_file, "r") as f:
                clang_format_options = yaml.safe_load(f)
            self.linewidth = int(clang_format_options["ColumnLimit"])
        except Exception:
            pass

    def format_docstring(self, text: str) -> str:
        prefix = " * "
        # Text wrapping needs to be done line-by-line to properly preserve
        # empty lines
        docstring = "/**\n"
        for line in text.splitlines():
            wrapped_lines = textwrap.fill(
                self.sanitize_comment(line),
                self.linewidth,
                initial_indent=prefix,
                subsequent_indent=prefix,
                replace_whitespace=False,
                break_long_words=False,
            )
            # the initial_indent and subsequent_indent parameters don't work
            # with empty lines, so add the bare prefix directly in this case
            docstring += wrapped_lines if wrapped_lines else prefix.rstrip()
            docstring += "\n"
        docstring += " */\n"
        return docstring

    def format_file(self, filename: str):
        return


class HeaderDocstringAdder:
    PREAMBLE_LINE = "// THIS DOCSTRING WAS GENERATED AUTOMATICALLY\n"
    CPP_COMMENT_RE = re.compile(r"\s*//")

    def __init__(
        self,
        symbol_list: HeaderSymbolList,
        *,
        extension: str = "",
        formatter: Optional[Formatter] = None,
    ):
        self.slist = symbol_list
        self.in_extension = ""
        self.out_extension = extension
        self.formatter = formatter if formatter is not None else TextWrapFormatter()
        # Parse symbol descriptions from the YAML symbol table
        with open(symbol_list.symbol_file, "r") as f:
            self.symbol_descriptions: Dict[str, str] = {
                symbol["name"]: symbol["description"]
                for block in yaml.safe_load(f).values()
                for symbol in block[symbol_list.SYMBOL_LIST_KEY]
                if "description" in symbol
            }

    def input_header_file(self):
        return self.slist.header_file + self.in_extension

    def output_header_file(self):
        return self.slist.header_file + self.out_extension

    def commit_header_file(self):
        self.formatter.format_file(self.output_header_file())
        self.in_extension = self.out_extension

    def load_symbol_names(self) -> Set[str]:
        return set(self.slist.names_from_c_header(self.input_header_file()))

    @staticmethod
    def _input_header_lines(lines) -> Iterator[Tuple[str, bool, bool]]:
        in_docstring = False
        in_c_style_comment = False
        for line in lines:
            in_cpp_style_comment = False
            if line == HeaderDocstringAdder.PREAMBLE_LINE:
                in_docstring = True
                in_cpp_style_comment = True
                in_c_style_comment = False
            else:
                if HeaderDocstringAdder.CPP_COMMENT_RE.match(line):
                    in_cpp_style_comment = True
                elif not in_c_style_comment and "/*" in line:
                    in_c_style_comment = True

            in_comment = in_c_style_comment or in_cpp_style_comment
            if not in_comment:
                in_docstring = False

            yield line, in_comment, in_docstring

            if in_c_style_comment and "*/" in line:
                in_c_style_comment = False

    def get_docstring(self, symbol: str) -> Optional[str]:
        if symbol not in self.symbol_descriptions:
            return None
        return self.formatter.format_docstring(self.symbol_descriptions[symbol])

    def add_docstrings(self):
        symbol_names = self.load_symbol_names()
        with open(self.input_header_file(), "r") as f:
            lines = f.readlines()
        with open(self.output_header_file(), "w") as f:
            for line, in_comment, in_docstring in self._input_header_lines(lines):
                if in_docstring:
                    # Skip over docstrings
                    continue
                if not in_comment:
                    match = self.slist.NAME_REGEX.search(line)
                    if match:
                        symbol = match[1]
                        if symbol in symbol_names:  # safety check
                            # Insert a docstring before writing this line
                            # NOTE: This assumes the symbol name is on the
                            # first line of the symbol declaration
                            docstring = self.get_docstring(symbol)
                            if docstring is not None:
                                f.write(HeaderDocstringAdder.PREAMBLE_LINE)
                                f.write(docstring)
                f.write(line)
        self.commit_header_file()


def add_header_docstrings(
    symbol_list_cls: HeaderSymbolList,
    extension: str,
    *,
    formatter: Optional[Formatter] = None,
    filter: Optional[str] = None,
    verbose: bool = False,
):
    for header_file in symbol_list_cls.headers():
        if filter is not None and not fnmatch.fnmatch(header_file, filter):
            continue
        try:
            adder = HeaderDocstringAdder(
                symbol_list_cls(header_file), extension=extension, formatter=formatter
            )
        except ValueError:
            # File doesn't correspond to a symbol file; skip
            continue

        adder.add_docstrings()
        if verbose:
            print(f"Added docstrings to {header_file}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Add Javadoc-style docstrings to the C headers using the symbol tables"
    )
    parser.add_argument(
        "-e",
        "--extension",
        default="",
        help="file extension for the docstring-annotated output files; will modify in-place if omitted or empty",
    )
    parser.add_argument(
        "-f",
        "--filter",
        help="Unix filename path filter for header files to process",
    )
    parser.add_argument("-v", "--verbose", action="store_true", help="verbose output")
    args = parser.parse_args()

    # Prefer formatting with clang-format if possible
    try:
        formatter = ClangFormatFormatter()
        if args.verbose:
            print("using clang-format for formatting")
    except Exception:
        formatter = TextWrapFormatter()
        if args.verbose:
            print(
                "clang-format not found; "
                + f"using textwrap for formatting (linewidth={formatter.linewidth})"
            )

    add_header_docstrings(
        FunctionList,
        args.extension,
        formatter=formatter,
        filter=args.filter,
        verbose=args.verbose,
    )
    add_header_docstrings(
        DataList,
        args.extension,
        formatter=formatter,
        filter=args.filter,
        verbose=args.verbose,
    )
