#!/usr/bin/env python3

"""
`new_symbol_header_check.py` is a command line utility for checking whether
symbols newly added to the pmdsky-debug symbol tables over a Git revision range
are also present in the C headers. Only primary names are considered, not
aliases.

Note that the Git revision range determines what symbols count as "new", but
that any new symbols are always checked against the current revision of the C
headers.

This script combines functionality from `symdiff.py` and
`headers/symbol_check.py`.

Example usage:

python3 new_symbol_header_check.py
python3 new_symbol_header_check.py HEAD~5 HEAD~2 --
    </path/to/arm9.yml> </path/to/overlay29.yml>
"""

import argparse
from pathlib import Path
from types import ModuleType
from typing import Optional

import ansi
import symdiff


def import_from_path(module_path: Path) -> ModuleType:
    import importlib.util
    import sys

    module_name = module_path.stem
    spec = importlib.util.spec_from_file_location(module_name, module_path)
    if spec is None:
        raise FileNotFoundError(module_path)
    if spec.loader is None:
        raise RuntimeError(f"no loader for spec from {module_path}")
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    sys.modules[module_name] = module  # make subsequent import statements work
    return module


SYMBOL_CHECK_PY = Path(__file__).resolve().parent.parent / "headers" / "symbol_check.py"
import_from_path(SYMBOL_CHECK_PY)
import symbol_check


def get_primary_symbols(table: symdiff.SymbolTable) -> list[str]:
    """Get all symbols in a given a SymbolTable, excluding aliases."""
    return [
        s.name
        for block in table.blocks.values()
        for l in [block.functions, block.data]
        for s in l
    ]


def get_header_symbols(symbol_path: Path) -> set[str]:
    """Get all symbols in the header files corresponding to a symbol file."""
    header_symbols = set()
    for symbol_list in [symbol_check.FunctionList, symbol_check.DataList]:
        header_path = symbol_list.get_header_file(str(symbol_path))
        if header_path is None:
            continue
        subregion_dir = Path(header_path).with_suffix("")
        subregion_paths = list(symbol_list.headers(str(subregion_dir)))
        for header_file in [header_path] + subregion_paths:
            try:
                slist = symbol_list(header_file)
                header_symbols.update(slist.names_from_header_file())
            except (FileNotFoundError, ValueError):
                # File doesn't correspond to a header/symbol file; skip
                pass
    return header_symbols


def print_help_text():
    ansi.print(
        ansi.BOLD + ansi.YELLOW,
        "CAUTION: Detected new symbols that have been added to the symbol tables"
        " without corresponding prototypes in the C headers!",
    )
    ansi.print(
        ansi.YELLOW,
        "Please note that header prototypes may be omitted if the signature or type"
        " is unknown. If this is intentional, this notice can be ignored. However,"
        " if this is unintentional, and the function signatures or data types are"
        " known, consider adding prototypes to the C headers!",
    )
    print()


def check_symbol_headers(
    path: Path, base: str, target: Optional[str], *, show_help_text: bool = False
) -> bool:
    old_table = symdiff.SymbolTable(path, revision=base)
    new_table = symdiff.SymbolTable(path, revision=target)
    if not old_table.valid and not new_table.valid:
        return True

    old_symbols_set = set(get_primary_symbols(old_table))
    new_symbols = get_primary_symbols(new_table)
    # Maintain symbol order for reporting determinism
    added_symbols = [s for s in new_symbols if s not in old_symbols_set]
    if not added_symbols:
        return True

    # Additions detected, check headers
    header_symbols = get_header_symbols(path)
    # Maintain symbol order for reporting determinism
    missing_symbols = [s for s in added_symbols if s not in header_symbols]
    if not missing_symbols:
        return True

    if show_help_text:
        print_help_text()
    ansi.print(
        ansi.BOLD + ansi.CYAN,
        "New symbols without header prototypes added to "
        + f"{path.resolve().relative_to(symdiff.SYMBOL_DIR)}:",
    )
    for missing in missing_symbols:
        ansi.print(ansi.CYAN, f"  - {missing}")
    print()
    return False


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Check C headers for newly added symbols in the"
        + " pmdsky-debug symbol tables.",
    )
    parser.add_argument(
        "-w",
        "--warn",
        action="store_true",
        help="print an extended warning message for missing headers",
    )
    args = symdiff.symdiff_parse_args(parser)

    missing_headers = False
    for path in args.path:
        missing_headers |= not check_symbol_headers(
            path,
            args.base,
            args.target,
            show_help_text=args.warn and not missing_headers,
        )
    if missing_headers:
        raise SystemExit(1)
