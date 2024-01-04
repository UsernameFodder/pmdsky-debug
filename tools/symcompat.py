#!/usr/bin/env python3

"""
`symcompat.py` is a command line utility for checking symbol name compatibility
across Git revisions of the pmdsky-debug symbol tables. A name compatibility
issue is defined as a symbol name in the base revision that is no longer
present (either as a primary name or an alias) in the target revision.

This script is largely based on `symdiff.py`, and reuses much of the same
parsing logic. It also relies on the diffing algorithm when inferring symbol
renames.

Example usage:

python3 symcompat.py
python3 symcompat.py HEAD~5 HEAD~2 -- </path/to/arm9.yml> </path/to/overlay29.yml>
"""

import argparse
from pathlib import Path
from typing import List, Optional

import ansi
import symdiff


def get_symbols(table: symdiff.SymbolTable) -> List[str]:
    """Get all symbols in a given a SymbolTable, including aliases."""
    return [
        name
        for block in table.blocks.values()
        for l in [block.functions, block.data]
        for s in l
        for name in [s.name] + s.aliases
    ]


def print_help_text():
    ansi.print(
        ansi.BOLD + ansi.YELLOW,
        "CAUTION: Detected symbols that have been removed or renamed without aliasing!",
    )
    ansi.print(
        ansi.YELLOW,
        "Please note that other projects may depend on these symbols, and removing"
        " them may cause compatibility issues. If this is intentional, this notice can"
        " be ignored. However, if this is unintentional, and symbols are being"
        " renamed, consider adding the old symbol names as aliases, like so:",
    )
    print("     - name: NewSymbolName")
    ansi.print(ansi.GREEN, "+      aliases:")
    # NOTE: For GitHub Actions, each line must be printed separately, see
    # https://github.com/orgs/community/discussions/40864
    ansi.print(ansi.GREEN, "+        - OldSymbolName")
    print()


def check_symbol_compatibility(
    path: Path, base: str, target: Optional[str], *, show_help_text: bool = False
) -> bool:
    old_table = symdiff.SymbolTable(path, revision=base)
    new_table = symdiff.SymbolTable(path, revision=target)
    if not old_table.valid and not new_table.valid:
        return True

    old_symbols = get_symbols(old_table)
    new_symbols_set = set(get_symbols(new_table))
    # Maintain symbol order for reporting determinism
    removed_symbols = [s for s in old_symbols if s not in new_symbols_set]
    if not removed_symbols:
        return True

    # Deletions detected
    if show_help_text:
        print_help_text()
    # Try to infer renames if possible
    diff = new_table.diff(old_table)
    renames = {
        d.path.name: d.new_path.name
        for d in diff.modified
        if d.new_path is not None and d.path.name != d.new_path.name
    }
    ansi.print(
        ansi.BOLD + ansi.CYAN,
        f"Removed symbols from {path.resolve().relative_to(symdiff.SYMBOL_DIR)}:",
    )
    for removed in removed_symbols:
        line = f"  - {removed}"
        renamed_to = renames.get(removed)
        if renamed_to is not None:
            line += f" (renamed to {renamed_to}?)"
        ansi.print(ansi.CYAN, line)
    print()
    return False


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Check symbol name compatibility across Git revisions of"
        + " the pmdsky-debug symbol tables.",
    )
    parser.add_argument(
        "-w",
        "--warn",
        action="store_true",
        help="print an extended warning message for compatibility issues",
    )
    args = symdiff.symdiff_parse_args(parser)

    compat_issues = False
    for path in args.path:
        compat_issues |= not check_symbol_compatibility(
            path,
            args.base,
            args.target,
            show_help_text=args.warn and not compat_issues,
        )
    if compat_issues:
        raise SystemExit(1)
