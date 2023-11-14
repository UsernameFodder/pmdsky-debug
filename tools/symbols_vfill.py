#!/usr/bin/env python3

"""
`symbols_vfill.py` is a command line utility for filling in missing function
addresses in the pmdsky-debug symbol tables, for addresses that are known in
some game versions (e.g., NA, EU) but not in others.

When run, the program will modify the YAML symbol tables in-place (unless
dry-run mode is enabled). It is recommended that you commit any symbol table
changes that you are working on BEFORE running `symbols_vfill.py`. This will
allow you to inspect filled addresses with `git diff` (or similar), and will
also prevent you from losing your changes if something somehow goes wrong.

This program is conservative in the symbol addresses it fills in, and tries
hard to avoid adding incorrect addresses caused by spurious matches between
binary files. It uses the following strategies to this end:
    - Only filling in addresses for function symbols, not data symbols. Data
      symbols are very difficult to match automatically because the meaning
      of data symbols is largely context-dependent. This means that the
      contents of a data symbol are not necessarily unique: two different
      symbols might point to exactly the same binary data, and yet might have
      different semantic meanings depending on the context.
    - Skipping symbols with multiple addresses. These are difficult to match
      automatically for similar reasons to data symbols, even for functions.
    - Only filling in addresses where EXACTLY one match (based on assembly
      instructions) is found. If multiple matches are found, there is
      significant ambiguity as to whether all the matches actually correspond
      to the same function (e.g., veneers for different functions can
      have exactly matching assembly). If a function symbol's length is not
      known, symbols_vfill.py makes a guess for the length, and adaptively
      adjusts it until it leads to exactly one match (if possible).
    - Imposing a minimum instruction count on matches based on adaptive symbol
      length inference. By default, the inferred length will always be at least
      the length of 4 instructions.
    - By default (can be disabled), discarding inferred addresses that are out
      of order relative to existing addresses of neighboring functions.

Note that this program has a "fast mode", which can be enabled with the --fast
flag. The only thing this changes is the number of times the formatter is run.
As the name suggests, fast mode runs faster, and has no differences upon
successful completion of the program. However, this may leave the symbol table
files in a bad state if the program is terminated prematurely (such as from a
user interrupt).

This program requires cargo to be installed and available in the runtime
environment so that `resymgen` can be run.

Example usage:

python3 symbols_vfill.py -f \
    --dir-na </path/to/EoS_NA_unpacked_dir> \
    --dir-eu </path/to/EoS_EU_unpacked_dir>

python3 symbols_vfill.py -f --dry-run \
    --dir-na </path/to/EoS_NA_unpacked_dir> \
    --dir-eu </path/to/EoS_EU_unpacked_dir>
"""

import argparse
from pathlib import Path
import re
import subprocess
import sys
from typing import Dict, Generator, Iterable, List, NamedTuple, Optional, Tuple, Union
import yaml

import arm5find
import offsets
from resymgen import resymgen


class DependentVersion:
    def __init__(self, display_name: str, binary_suffix: str, version_suffix: str):
        self.display_name = display_name
        self.binary_suffix = binary_suffix
        self.version_suffix = version_suffix

    def __str__(self) -> str:
        return self.display_name

    def match_binary(self, bin_name: str) -> bool:
        return bin_name.endswith(self.binary_suffix)

    def convert_binary(self, bin_name: str) -> str:
        return bin_name + self.binary_suffix

    def match_version(self, version: str) -> bool:
        return version.endswith(self.version_suffix)

    def convert_version(self, version: str) -> str:
        return version + self.version_suffix


DEPENDENT_VERSIONS = [
    DependentVersion("ITCM", ".itcm", "-ITCM"),
    DependentVersion("WRAM", ".wram", "-WRAM"),
    DependentVersion("RAM", ".ram", "-RAM"),
]
REAL_BINARY_NAMES = [
    b
    for b in offsets.BINARY_NAMES
    if not any(v.match_binary(b) for v in DEPENDENT_VERSIONS)
]


class SymbolTable:
    """A symbol table from pmdsky-debug"""

    SYMBOL_DIR: Path = Path(__file__).resolve().parent.parent / "symbols"

    def __init__(self, arg: Union[str, Path]):
        if isinstance(arg, Path):
            self.path = arg
            return
        binary: str = arg
        if binary not in REAL_BINARY_NAMES:
            raise ValueError(f'Invalid binary: "{binary}"')
        if binary.startswith("overlay"):
            # overlay0 -> overlay00, etc.
            binary = f"overlay{int(binary.lstrip('overlay')):02}"
        self.path = SymbolTable.SYMBOL_DIR / f"{binary}.yml"

    def walk(self) -> Generator["SymbolTable", None, None]:
        yield self
        subregion_dir = self.path.parents[0] / Path(self.path.stem)
        if subregion_dir.is_dir():
            for p in sorted(subregion_dir.iterdir()):
                if p.is_file():
                    yield from SymbolTable(p).walk()

    @staticmethod
    def fmt(files: Union[str, List[str]]):
        try:
            # Use resymgen for formatting rather than relying on pyyaml
            resymgen.fmt(files, capture_output=True, check=True)
        except subprocess.CalledProcessError as e:
            print(e.stderr.decode(), file=sys.stderr)
            raise

    def read(self) -> dict:
        with self.path.open("r") as f:
            return yaml.safe_load(f)

    def write(self, symbols: dict, *, skip_formatting: bool = False):
        with self.path.open("w") as f:
            yaml.safe_dump(symbols, f)

        if not skip_formatting:
            # It's slower to do formatting here, one file at a time rather than
            # formatting everything at once at the end of the program, but this
            # gives better atomicity if the program terminates prematurely.
            SymbolTable.fmt(str(self.path))


def find_binary_files(dirname: str, binaries: Iterable[str]) -> Dict[str, str]:
    """Locate the files corresponding to the given binaries within a directory.

    Args:
        dirname (str): path of the directory to search
        binaries (Iterable[str]): collection of binaries to search for

    Returns:
        Dict[str, str]: mapping from binaries to located file paths
    """
    binary_files: Dict[str, str] = {}
    binary_set = set(binaries)
    for fpath in Path(dirname).glob("**/*.bin"):
        name = fpath.name.rstrip(".bin")
        if name.startswith("overlay"):
            # overlay_0000 or overlay0000 -> overlay0, etc.
            name = f"overlay{int(name.lstrip('overlay').lstrip('_'))}"
        if name in binary_set:
            binary_files[name] = str(fpath)
    return binary_files


class FillCounter:
    """Counters for printed summary statistics"""

    def __init__(
        self, filled: int = 0, unfilled: int = 0, discarded: int = 0, skipped: int = 0
    ):
        self.filled = filled
        self.unfilled = unfilled
        self.discarded = discarded
        self.skipped = skipped

    def __iadd__(self, other: "FillCounter") -> "FillCounter":
        self.filled += other.filled
        self.unfilled += other.unfilled
        self.discarded += other.discarded
        self.skipped += other.skipped
        return self

    def __add__(self, other: "FillCounter") -> "FillCounter":
        s = FillCounter(self.filled, self.unfilled, self.discarded, self.skipped)
        s += other
        return s

    def summary(self, indent=2):
        print(f"{' ' * indent}{self.filled} addresses filled")
        print(f"{' ' * indent}{self.unfilled} filling failures")
        print(f"{' ' * indent}{self.discarded} filling discards")
        print(f"{' ' * indent}{self.skipped} symbols skipped")


class AddressBounds(NamedTuple):
    """Bounds for inferred symbol addresses."""

    lower: Optional[int] = None
    upper: Optional[int] = None

    def __str__(self) -> str:
        lower_str = f"0x{self.lower:X}" if self.lower is not None else str(None)
        upper_str = f"0x{self.upper:X}" if self.upper is not None else str(None)
        return f"({lower_str}, {upper_str})"


def calc_symbol_addr_bounds(symbol_list: List[dict]) -> List[Dict[str, AddressBounds]]:
    """Calculate bounds for inferred addresses of each symbol in the list.

    Bounds are per-version, and are based on the addresses of immediate
    neighbors of a symbol.

    Args:
        symbol_list (List[dict]): resymgen symbol list

    Returns:
        List[Dict[str, AddressBounds]]: List of per-version address bounds
            corresponding to each symbol in the input list
    """

    bounds_list: List[Dict[str, AddressBounds]] = [{} for _ in symbol_list]

    last_addr_by_vers = {}
    # First pass, iterate forward to establish lower bounds
    for symbol, bounds in zip(symbol_list, bounds_list):
        for vers, addr in last_addr_by_vers.items():
            # Use the first address if there are multiple, since that's the
            # expected list sorting key
            if isinstance(addr, list):
                if not addr:
                    continue
                addr = addr[0]
            bounds[vers] = AddressBounds(lower=addr)
        last_addr_by_vers.update(symbol["address"])

    # Second pass, iterate backwards to establish upper bounds. Also
    # simultaneously perform validation; if upper < lower, it means the
    # existing symbol list had an out-of-order jump, so force no bounds.
    last_addr_by_vers = {}
    for symbol, bounds in zip(reversed(symbol_list), reversed(bounds_list)):
        for vers, addr in last_addr_by_vers.items():
            # Use the first address if there are multiple, since that's the
            # expected list sorting key
            if isinstance(addr, list):
                if not addr:
                    continue
                addr = addr[0]

            if vers not in bounds:
                bounds[vers] = AddressBounds(upper=addr)
            elif bounds[vers].lower <= addr:
                bounds[vers] = bounds[vers]._replace(upper=addr)
            else:
                # Out of order jump; delete the bound
                del bounds[vers]
        last_addr_by_vers.update(symbol["address"])

    return bounds_list


def function_fill_versions(
    function: dict,
    file_contents_cache: Dict[str, bytes],
    file_by_version: Dict[str, str],
    bin_name: str,
    *,
    min_instr_count: int = 4,
    addr_bounds: Optional[Dict[str, AddressBounds]] = None,
    verbosity: int = 0,
    dry_run: bool = False,
) -> FillCounter:
    """Fill in missing addresses for a single function symbol.

    Args:
        function (dict): resymgen function symbol, can be mutated
        file_contents_cache (Dict[str, bytes]): binary file contents by game
            version, can be mutated
        file_by_version (Dict[str, str]): binary file paths by game version
        bin_name (str): short name of the binary containing the function
        min_instr_count (int, optional): minimum instruction count for adaptive
            length search. Defaults to 4.
        addr_bounds (Optional[Dict[str, AddressBounds]], optional): per-version
            bounds on inferred addresses. Defaults to None.
        verbosity (int, optional): verbosity (0-4). Defaults to 0.
        dry_run (bool, optional): enable dry run mode. Defaults to False.

    Returns:
        FillCounter: statistics from the filling process
    """

    def report(message: str, level: int = 0):
        if verbosity >= level:
            print(message)

    def debug(message: str, level: int = 4):
        report(f"DEBUG: {message}", level)

    if min_instr_count <= 0:
        raise ValueError("minimum instruction count must be positive")

    versions = set(file_by_version.keys())
    dep_versions = [
        d
        for d in DEPENDENT_VERSIONS
        if any(d.match_version(v) for v in function["address"].keys())
    ]
    known = versions & set(function["address"].keys())
    if not known or len(known) == len(versions):
        # This address is totally unknown (should be impossible
        # according to the symbol table spec), or known for all
        # versions already
        report(f"[{bin_name}] {function['name']}: skipped, no addresses to fill", 3)
        # This doesn't count as "skipped". It's more like "not considered"
        return FillCounter()

    if any(type(addr) == list for addr in function["address"].values()):
        # If a function appears in more than one place, it's going
        # to be very difficult to search for it in other binaries
        # automatically without a risk of false positives, so
        # filling these in should be done manually
        report(
            f"[{bin_name}] {function['name']}: skipped, has multiple addresses",
            2,
        )
        return FillCounter(skipped=1)

    counter = FillCounter()

    # We know the address in some versions, but not all, so we can try to fill
    # in the missing ones
    missing = [v for v in file_by_version if v not in known]
    known = [v for v in file_by_version if v in known]
    length: Optional[int] = None
    try:
        # Prefer a version with both address and length
        known_length = versions & set(function.get("length", {}).keys())
        src_vers = next(v for v in known if v in known_length)
        length = function["length"][src_vers]
    except StopIteration:
        # If that doesn't exist, just take the first version with a known address
        src_vers = known[0]

    # Convert from absolute address to relative file offset before searching
    relative = offsets.convert_offsets(
        src_vers, [bin_name], [function["address"][src_vers]]
    )[0].get_mapped()[0]
    # Try to fill in each missing version. Use an adaptive function length if
    # it's unknown
    adaptive_length = length is None
    if length is None:
        # Start with a reasonable guess for length of 8 assembly instructions.
        # Clamp to the minimum instruction count
        length = max(8, min_instr_count) * arm5find.AsmSegment.INSTRUCTION_SIZE
    for dst_vers in missing:
        log_prefix = f"[{bin_name}, {dst_vers}] {function['name']}: "

        if dst_vers not in file_contents_cache:
            # Read the binary file for the first time and cache it
            with open(file_by_version[dst_vers], "rb") as f:
                file_contents_cache[dst_vers] = f.read()
        contents = file_contents_cache[dst_vers]

        # Search for a single match. If there are multiple simultaneous
        # matches, the search was too permissive and the results don't count
        match: Optional[re.Match] = None

        def single_search(fn_len: int) -> List[re.Match]:
            segment = arm5find.AsmSegment(relative, fn_len)
            with open(file_by_version[src_vers], "rb") as f:
                regex = segment.regex(f)
                return list(regex.finditer(contents))

        search_results = single_search(length)
        if adaptive_length:
            # Adaptively grow or shrink the length until the search returns
            # exactly one match. Use multiples of the instruction size
            instr = length // arm5find.AsmSegment.INSTRUCTION_SIZE
            prev_instr = instr  # for coarse search
            next_instr = instr  # for coarse search
            instr_bounds: Optional[list] = None  # for fine search
            if len(search_results) != 1:
                debug(
                    f"{log_prefix}starting adaptive length search"
                    + f" (min instruction count = {min_instr_count})"
                )
            while len(search_results) != 1:
                debug(
                    f"{log_prefix}adaptive length: 0x{length:X}"
                    + f" -> {len(search_results)} matches"
                )
                if instr_bounds is None:
                    # Coarse search. We don't have any idea of how big the
                    # function is, so vary the instruction count geometrically
                    # to narrow it down
                    if len(search_results) == 0:
                        # No search results, shrink the length so the search is
                        # more permissive
                        next_instr = instr // 2
                        # If we're overshooting the minimum instruction count,
                        # clamp
                        if next_instr < min_instr_count and instr > min_instr_count:
                            next_instr = min_instr_count
                    else:
                        # More than one search result, grow the length so the
                        # search is more restrictive
                        next_instr = instr * 2

                    # If we reach the lower limit on instruction count or we
                    # end up backtracking in size, transition from coarse search
                    # to fine search
                    if (
                        next_instr < min_instr_count
                        or (next_instr - instr) * (prev_instr - instr) > 0
                    ):
                        debug(f"{log_prefix}transitioning to fine search")
                        # instr_bounds is [inclusive, exclusive)
                        instr_bounds = [
                            min(prev_instr, instr) + 1,
                            max(prev_instr, instr),
                        ]
                        if instr_bounds[0] >= instr_bounds[1]:
                            # Nothing to do a fine search on
                            break
                        # seed fine search mode with the first midpoint
                        next_instr = (instr_bounds[0] + instr_bounds[1]) // 2
                        # fallthrough to search
                    prev_instr = instr
                    instr = next_instr
                else:
                    # Fine search (post-seeding). We have an upper and lower
                    # bound for the instruction count, so do a binary search
                    # within this range
                    if len(search_results) == 0:
                        # No search results, shrink the length so the search is
                        # more permissive
                        instr_bounds[1] = instr
                    else:
                        # More than one search result, grow the length so the
                        # search is more restrictive
                        instr_bounds[0] = instr + 1
                    if instr_bounds[0] >= instr_bounds[1]:
                        # Nothing left to try; binary search finished
                        break
                    instr = (instr_bounds[0] + instr_bounds[1]) // 2

                length = instr * arm5find.AsmSegment.INSTRUCTION_SIZE
                search_results = single_search(length)
            debug(f"{log_prefix}inferred length 0x{length:X}")
        if len(search_results) == 1:
            match = search_results[0]

        if match is not None:
            # Convert back to absolute address
            match_addr = offsets.convert_offsets(dst_vers, [bin_name], [match.start()])[
                0
            ].get_mapped()[0]

            if addr_bounds is not None and dst_vers in addr_bounds:
                # Check that the inferred address falls in the expected bounds
                bounds = addr_bounds[dst_vers]
                debug(f"{log_prefix}verifying against address bounds {bounds}")
                if not (
                    (bounds.lower is None or bounds.lower < match_addr)
                    and (bounds.upper is None or match_addr < bounds.upper)
                ):
                    report(
                        f"{log_prefix}discarding out-of-order inferred address 0x{match_addr:X}",
                        1,
                    )
                    counter.discarded += 1
                    continue

            # If the original symbol has any dependent addresses
            # (ITCM/WRAM/RAM), also try to add the corresponding addresses for
            # the target version
            dep_addrs: List[Tuple[DependentVersion, int]] = []
            dep_mismatch = False
            for dep_vers in dep_versions:
                # Convert to the dependent version's absolute address
                dep_mappings = offsets.convert_offsets(
                    dst_vers, [dep_vers.convert_binary(bin_name)], [match.start()]
                )
                # Do the check regardless of whether or not we end up adding
                if not dep_mappings:
                    report(
                        f"{log_prefix}discarding non-{dep_vers} inferred "
                        + f"address 0x{match_addr:X} for {dep_vers} function "
                        + "symbol",
                        1,
                    )
                    counter.discarded += 1
                    dep_mismatch = True
                    break
                # Don't override an existing address for this version
                if dep_vers.convert_version(dst_vers) not in function["address"]:
                    # Successful mapping
                    dep_addrs.append((dep_vers, dep_mappings[0].get_mapped()[0]))
            if dep_mismatch:
                continue

            report_str = f"{log_prefix}found address 0x{match_addr:X}"
            if dep_addrs:
                dep_str = ", ".join([f"{v}: 0x{a:X}" for v, a in dep_addrs])
                report_str += f" ({dep_str})"
            report(report_str, 0 if dry_run else 2)
            if not dry_run:
                function["address"][dst_vers] = match_addr
                for dep_vers, dep_addr in dep_addrs:
                    function["address"][dep_vers.convert_version(dst_vers)] = dep_addr
            # Dependent addresses don't count towards the total filled counter
            counter.filled += 1
        else:
            report(f"{log_prefix}no match found", 1)
            counter.unfilled += 1

    return counter


def symbols_fill_versions(
    binaries: Dict[str, Dict[str, str]],
    *,
    min_instr_count: int = 4,
    respect_sort_order: bool = False,
    verbosity: int = 0,
    dry_run: bool = False,
    fast_mode: bool = False,
) -> Dict[str, FillCounter]:
    """Fill in addresses for missing versions within the symbol tables.

    Args:
        binaries (Dict[str, Dict[str, str]]): binary file paths by binary short
            name (outer key) and game version (inner key)
        min_instr_count (int, optional): minimum instruction count for adaptive
            length search. Defaults to 4.
        respect_sort_order(bool, optional): respect the sort ordering of
            existing symbols when filling new addresses. Defaults to False.
        verbosity (int, optional): verbosity (0-4). Defaults to 0.
        dry_run (bool, optional): enable dry run mode. Defaults to False.
        fast_mode (bool, optional): enable fast mode. Defaults to False.

    Returns:
        Dict[str, FillCounter]: statistics from the filling process, by binary
    """
    # Counters by binary for reporting
    counters: Dict[str, FillCounter] = {}
    files_to_format: List[str] = []  # Only used in fast mode

    for bin_name, file_by_version in binaries.items():
        # Keep a cache of file contents by version to avoid loading them many times
        binary_contents: Dict[str, bytes] = {}

        counters[bin_name] = FillCounter()

        # Fill symbols in all subregion files
        for symbol_table in SymbolTable(bin_name).walk():
            symbol_contents = symbol_table.read()
            table_counter = FillCounter()
            for block in symbol_contents.values():
                # Data symbols are pretty much impossible to match generally
                # without a risk of false positives, because the same raw data
                # could pretty easily be used in multiple different contexts
                # (which we would consider to be different symbols). So, only
                # try to fill in function addresses.
                functions = block["functions"]

                # If we need to respect sort order, build a list of by-version
                # lower/upper bounds from the existing symbol addresses
                functions_addr_bounds = None
                if respect_sort_order:
                    functions_addr_bounds = calc_symbol_addr_bounds(functions)

                for i, function in enumerate(functions):
                    addr_bounds = None
                    if functions_addr_bounds is not None:
                        addr_bounds = functions_addr_bounds[i]
                    table_counter += function_fill_versions(
                        function,
                        binary_contents,
                        file_by_version,
                        bin_name,
                        min_instr_count=min_instr_count,
                        addr_bounds=addr_bounds,
                        verbosity=verbosity,
                        dry_run=dry_run,
                    )

            # The symbol table is modified iff the filled counter is positive
            if not dry_run and table_counter.filled > 0:
                symbol_table.write(symbol_contents, skip_formatting=fast_mode)
                if fast_mode:
                    # We'll need to run the formatter on this later
                    files_to_format.append(str(symbol_table.path))
            counters[bin_name] += table_counter

    if files_to_format:
        SymbolTable.fmt(files_to_format)

    return counters


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Fill in missing version-dependent addresses in the pmdsky-debug symbol tables."
    )
    for v in offsets.BINARIES:
        parser.add_argument(
            f"--dir-{v.lower()}",
            help=f"data directory for unpacked EoS ({v}) ROM",
        )
    parser.add_argument(
        "-b",
        "--binary",
        choices=REAL_BINARY_NAMES,
        action="append",
        help="EoS binary",
    )
    parser.add_argument(
        "-i",
        "--min-instr-count",
        type=int,
        default=4,
        help="minimum instruction count for adaptive symbol lengths",
    )
    parser.add_argument(
        "-S",
        "--ignore-sort-order",
        action="store_true",
        default=False,
        help="allow inferred addresses that violate the existing address"
        + "ordering of existing symbols for the corresponding version",
    )
    parser.add_argument(
        "-v", "--verbose", action="count", default=0, help="verbosity level"
    )
    parser.add_argument("-n", "--dry-run", action="store_true", help="dry run")
    parser.add_argument(
        "-f",
        "--fast",
        action="store_true",
        help=f"{Path(__file__).name} in fast mode will run faster,"
        + " but might leave files in a bad state upon premature termination",
    )
    args = parser.parse_args()

    if not args.binary:
        args.binary = REAL_BINARY_NAMES

    data_dirs = {
        vers: getattr(args, f"dir_{vers.lower()}")
        for vers in offsets.BINARIES
        if getattr(args, f"dir_{vers.lower()}") is not None
    }
    if len(data_dirs) < 2:
        raise SystemExit("Data directories must be provided for at least 2 versions")

    # Outer key is binary name, inner key is version, inner value is file path.
    files_by_version: Dict[str, Dict[str, str]] = {name: {} for name in args.binary}
    for vers, data_dir in data_dirs.items():
        files = find_binary_files(data_dir, args.binary)
        if len(files) < len(args.binary):
            missing = sorted(set(args.binary) - set(files))
            raise SystemExit(f"Missing binaries from {data_dir}: {', '.join(missing)}")
        for name, fpath in files.items():
            files_by_version[name][vers] = fpath

    if args.dry_run:
        print("*** DRY RUN ***")
    counters = symbols_fill_versions(
        files_by_version,
        min_instr_count=args.min_instr_count,
        respect_sort_order=not args.ignore_sort_order,
        verbosity=args.verbose,
        dry_run=args.dry_run,
        fast_mode=args.fast,
    )
    total_counter = FillCounter()
    for counter in counters.values():
        total_counter += counter

    # Print a combined summary
    print()
    print("Summary:")
    print(f"  {len(counters)} symbol table(s) scanned")
    total_counter.summary()

    if args.verbose >= 1 and len(counters) > 0:
        # Print a breakdown by file
        print()
        print(f"Breakdown by symbol table:")
        for name, counter in counters.items():
            print(f"  {name}:")
            counter.summary(indent=4)
