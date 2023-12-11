#!/usr/bin/env python3

"""
`xmapcorrelate.py` is a command line utility for correlating identical symbols
across different ARMv5 binaries annotated with xMAP files.

Example usage:

python3 xmapcorrelate.py \
    </path/to/arm9-src.bin> </path/to/arm9-src.xMAP> .main \
    </path/to/arm9-target.bin>

python3 xmapcorrelate.py \
    </path/to/arm9-src.bin> </path/to/arm9-src.xMAP> .main \
    </path/to/arm9-target.bin> </path/to/arm9-target.xMAP>

python3 xmapcorrelate.py -v -t atd -D -f '(sub|ov[0-9]+)_[A-F0-9]{8}'
    </path/to/arm9-src.bin> </path/to/arm9-src.xMAP> .arm9 \
    </path/to/arm9-target.bin> </path/to/arm9-target.xMAP> .main
"""

import argparse
import bisect
from enum import Enum
import os
import re
import sys
from typing import Dict, Iterator, List, NamedTuple, Optional, Set, Tuple, Union

import arm5find


class SegmentDataType(Enum):
    DATA = "$d"
    ARM = "$a"
    THUMB = "$t"

    def is_asm(self) -> bool:
        return self == SegmentDataType.ARM or self == SegmentDataType.THUMB


class SymbolSegment(NamedTuple):
    """An address range covering a subset of bytes for a symbol."""

    data_type: SegmentDataType
    address: int
    length: int

    def end(self) -> int:
        return self.address + self.length


class MapSymbol(NamedTuple):
    """A symbol from an xMAP file."""

    name: str
    source: str
    # A symbol may have more than one segment of data. E.g., functions may
    # have an ARM code segment followed by a data pool segment.
    segments: List[SymbolSegment]

    def address(self) -> int:
        if not self.segments:
            raise RuntimeError("no segments")
        return self.segments[0].address

    def data_type(self) -> SegmentDataType:
        if not self.segments:
            raise RuntimeError("no segments")
        return self.segments[0].data_type

    def length(self) -> int:
        return sum(segment.length for segment in self.segments)

    def end(self) -> int:
        return self.address() + self.length()

    def format(self, include_addr: bool = True, include_length: bool = False) -> str:
        """Return a pretty-print string for this MapSymbol."""
        s = self.name
        if include_addr:
            s += f" @ 0x{self.address():X}"
        if include_length:
            s += f" / {self.length()} bytes"
        if self.source:
            s += f" [{self.source}]"
        return s


class XMapParser:
    def __init__(self, debug: bool):
        self.debug = debug

    class MapSymbolBuilder:
        def __init__(
            self,
            name: str,
            source: str,
            address: int,
            length: int,
            *,
            max_alignment: int = 4,
        ):
            self.symbol = MapSymbol(name, source, [])
            self.address = address
            self.length = length
            self.max_alignment = max_alignment
            self._cursor = address
            self.ticks_since_last_add = 0

        def __str__(self) -> str:
            return f"{self.symbol.name} @ 0x{self.address:X} [{self.symbol.source}]"

        def tick(self) -> int:
            self.ticks_since_last_add += 1
            return self.ticks_since_last_add

        def max_align(self, x: int) -> int:
            return (
                (x + self.max_alignment - 1) // self.max_alignment
            ) * self.max_alignment

        def add(self, address: int, data_type: SegmentDataType) -> Optional[MapSymbol]:
            if address < self._cursor:
                raise ValueError("symbol segment addresses cannot decrease")
            # Allow the address to go past the symbol's end address only up to any
            # padding that may be generated due to the maximum possible alignment
            if address > self.max_align(self.address + self.length):
                raise ValueError("symbol segment out of symbol bounds")
            self.ticks_since_last_add = 0
            # ignore empty segments
            if address != self._cursor:
                self.symbol.segments.append(
                    SymbolSegment(
                        data_type,
                        self._cursor,
                        # Ensure the segment doesn't go out of the symbol bounds.
                        # Truncating the segment will cause a gap between the
                        # segment end and the cursor, but it's okay because if
                        # we're truncating, we'll be returning the finalized symbol
                        # immediately anyway.
                        min(address, self.address + self.length) - self._cursor,
                    )
                )
                self._cursor = address
            if self._cursor >= self.address + self.length:
                return self.symbol

    def parse(self, fname: str, section: str) -> Tuple[int, List[MapSymbol]]:
        """Parse a section of an xMAP file for symbol mappings.

        Args:
            fname (str): xMAP file name
            section (str): name of the section of the xMAP file to parse

        Returns:
            Tuple[int, List[MapSymbol]]: (load address, list of symbols)
        """

        def debug(message):
            if self.debug:
                print(f"DEBUG: {message}")

        def debug_line(message, i, line):
            debug(f"line {i+1}: {line}")
            debug(f"    {message}")

        section = section.strip()
        load_addr = 0
        symbols: List[MapSymbol] = []
        with open(fname, "r") as f:
            debug(f"parsing {fname}")
            current_section: Optional[str] = None
            current_data_type = SegmentDataType.DATA
            current_symbol: Optional[XMapParser.MapSymbolBuilder] = None
            found_section = False
            found_load_addr = False
            in_exception_table = False
            for i, line in enumerate(f.readlines()):
                line = line.strip()

                if current_symbol is not None and current_symbol.tick() > 1:
                    # we went a line without processing the current symbol,
                    # so discard it as incomplete
                    debug_line(
                        "*** incomplete symbol "
                        + f"[{len(current_symbol.symbol.segments)} segment(s)]: "
                        + str(current_symbol),
                        i,
                        line,
                    )
                    current_symbol = None

                if current_section is None:
                    if not line:
                        continue
                    if not line.startswith("#"):
                        raise RuntimeError(
                            f"line {i+1}: unexpected content without parent section"
                        )
                    name = line[1:].strip()
                    if not name:
                        continue
                    # begin section
                    current_section = name
                    debug_line(f"start section: {current_section}", i, line)
                    continue

                if not line:
                    # end section
                    if found_section:
                        # already processed the target section, no need to go further
                        break
                    current_section = None
                    continue

                if current_section != section:
                    # non-target section
                    continue

                # section contents
                found_section = True

                if not found_load_addr and line.endswith("START (linker command file)"):
                    # section load address
                    load_addr = int(line.split()[0][len("#>") :], 16)
                    found_load_addr = True
                    debug_line(f"load address: 0x{load_addr:X}", i, line)
                if line.startswith("#") or in_exception_table:
                    # some other annotation
                    if line.endswith("_ETABLE_START (linker command file)"):
                        # special case: skip over the exception table, which is
                        # interleaved with entries but uses a header format that isn't
                        # parseable in the same way
                        in_exception_table = True
                        debug_line(f"start exception table", i, line)
                    elif in_exception_table and line.endswith(
                        "_ETABLE_END (linker command file)"
                    ):
                        in_exception_table = False
                        debug_line(f"end exception table", i, line)
                    continue

                # address map entries
                try:
                    # name and source can potentially have spaces in them, so they need
                    # more careful parsing
                    address, length, section_type, name_and_source = line.split(
                        maxsplit=3
                    )
                    address = int(address, 16)
                    length = int(length, 16)

                    # pull off the name from the end by finding the open parenthesis
                    # corresponding to the rightmost closing parenthesis
                    if name_and_source[-1] != ")":
                        raise ValueError
                    paren_search_end = -1
                    paren_count = 1
                    while paren_count > 0:
                        open_paren = name_and_source.rindex("(", 0, paren_search_end)
                        paren_count -= 1
                        paren_count += name_and_source.count(
                            ")", open_paren + 1, paren_search_end
                        )
                        paren_search_end = open_paren
                    name = name_and_source[:open_paren].strip()
                    source = name_and_source[open_paren + 1 : -1].strip()
                except ValueError:
                    debug_line("*** unexpected line format", i, line)
                    raise

                new_data_type: Optional[SegmentDataType] = None
                try:
                    # set data type for subsequent lines (but defer setting it
                    # so we still use the previous data type when adding the
                    # current segment to an existing symbol)
                    new_data_type = SegmentDataType(name)
                except ValueError:
                    pass

                # symbol
                if current_symbol:
                    # add to an existing symbol
                    try:
                        finalized = current_symbol.add(address, current_data_type)
                    except ValueError as e:
                        debug_line(
                            f"*** ignoring symbol {current_symbol}: failed to add "
                            + f"address 0x{address:X}: {e}",
                            i,
                            line,
                        )
                        # clear this symbol
                        current_symbol = None
                    if current_symbol:
                        if finalized:
                            if finalized.segments:
                                debug_line(
                                    f"symbol [{len(finalized.segments)} segment(s)]: "
                                    + finalized.format(),
                                    i,
                                    line,
                                )
                                symbols.append(finalized)
                            else:
                                debug_line(
                                    f"*** empty symbol: {current_symbol}", i, line
                                )
                            current_symbol = None
                        elif source != current_symbol.symbol.source:
                            # this line was part of the symbol, check that source matches
                            raise RuntimeError(
                                f"line {i+1}: {current_symbol}: segment source "
                                + f'mismatch: "{source}"'
                            )
                # Don't include zero-width symbols, or start-of-binary-section markers
                if (
                    current_symbol is None
                    and new_data_type is None
                    and length != 0
                    and name != section_type
                ):
                    current_symbol = XMapParser.MapSymbolBuilder(
                        name, source, address, length
                    )
                if new_data_type:
                    current_data_type = new_data_type
        return load_addr, symbols


class MappedBinary:
    """A binary annotated with symbols from an xMAP file."""

    def __str__(self) -> str:
        s = f"{self.binary_name} ({len(self.contents)} bytes)"
        if self.xmap_name:
            s += f", {self.xmap_name}[{self.xmap_section} @ 0x{self.load_address:X}] ({len(self.symbols)} symbols)"
        return s

    def __init__(
        self,
        binary_fname: str,
        xmap_fname: Optional[str] = None,
        xmap_section: Optional[str] = None,
        *,
        debug: bool = False,
    ):
        """Initialize a new MappedBinary.

        Args:
            binary_fname (str): binary file name
            xmap_fname (Optional[str], optional): xMAP file name. Defaults to None.
            xmap_section (Optional[str], optional): xMAP section name. Defaults to None.

        Raises:
            ValueError: if xmap_fname is provided but xmap_section is not
        """
        self.binary_name = os.path.basename(binary_fname)
        self.xmap_name = os.path.basename(xmap_fname) if xmap_fname else None
        self.xmap_section = xmap_section

        with open(binary_fname, "rb") as f:
            self.contents: bytes = f.read()

        if xmap_fname is not None:
            if xmap_section is None:
                raise ValueError("unspecified section in xmap file")
            self.load_address, self.symbols = XMapParser(debug).parse(
                xmap_fname, xmap_section
            )
        else:
            self.load_address = 0
            self.symbols = []

        self._symbol_addresses = [symbol.address() for symbol in self.symbols]
        self.addr_map = {symbol.address(): symbol for symbol in self.symbols}

    def segment_bytes(self, segment: SymbolSegment) -> bytes:
        addr = self.get_relative(segment.address)
        return self.contents[addr : addr + segment.length]

    def segment_regex(
        self,
        segment: SymbolSegment,
        *,
        ignore_ldr_str_offset: bool = False,
        ignore_b_offset: bool = False,
    ) -> re.Pattern:
        """Get a search regex for the binary data of a given segment."""
        SEARCH_TYPE = {
            SegmentDataType.DATA: (arm5find.DataSegment, {}),
            SegmentDataType.ARM: (
                arm5find.AsmSegment,
                {
                    "ignore_ldr_str_offset": ignore_ldr_str_offset,
                    "ignore_b_offset": ignore_b_offset,
                },
            ),
            # XXX: no support for thumb asm yet, so match bytes exactly like with data
            SegmentDataType.THUMB: (arm5find.DataSegment, {}),
        }
        search_type = SEARCH_TYPE[segment.data_type]
        return search_type[0](self.get_relative(segment.address), segment.length).regex(
            self.contents, **search_type[1]
        )

    def symbol_regexes(
        self,
        symbol: MapSymbol,
        *,
        ignore_ldr_str_offset: bool = False,
        ignore_b_offset: bool = False,
    ) -> Iterator[Tuple[SymbolSegment, re.Pattern]]:
        """Get search regexes for the binary data of a given symbol."""
        return (
            (
                seg,
                self.segment_regex(
                    seg,
                    ignore_ldr_str_offset=ignore_ldr_str_offset,
                    ignore_b_offset=ignore_b_offset,
                ),
            )
            for seg in symbol.segments
        )

    def is_veneer(self, symbol: MapSymbol) -> bool:
        """Checks if a given symbol is a veneer function."""
        if len(symbol.segments) != 2:
            return False
        asm, data = symbol.segments
        return (
            data.data_type == SegmentDataType.DATA
            and data.length == 4
            and asm.data_type == SegmentDataType.ARM
            and asm.length == 8
            # ldr r12,[data]
            # bx r12
            and self.segment_bytes(asm) == b"\x00\xc0\x9f\xe5\x1c\xff\x2f\xe1"
        )

    def symbol_at(self, addr: int) -> Optional[MapSymbol]:
        """Get the symbol starting at a given address, if one exists."""
        return self.addr_map.get(addr)

    def symbol_occupying(self, addr: int) -> Optional[MapSymbol]:
        """Get the symbol that occupies a given address, if one exists."""
        # fast path
        at = self.symbol_at(addr)
        if at is not None:
            return at

        left_neighbor = bisect.bisect_right(self._symbol_addresses, addr) - 1
        if left_neighbor >= 0:
            symbol = self.symbols[left_neighbor]
            # Checking address directly allows 0-length symbols to be matched
            if addr < symbol.end() or addr == symbol.address():
                return symbol
        return None

    def get_relative(self, abs_addr: int) -> int:
        """Convert an absolute address to a relative address."""
        return abs_addr - self.load_address

    def get_absolute(self, rel_addr: int) -> int:
        """Convert a relative address to an absolute address."""
        return rel_addr + self.load_address


class CorrelatedSymbol(NamedTuple):
    """Target symbol correlated to a source symbol."""

    address: int  # start (absolute) address of the symbol in the target
    length: int  # length of the symbol
    # A correlated symbol may not always align perfectly with a pre-existing
    # target symbol; symbol will be populated by whatever target symbol occupies
    # address
    symbol: Optional[MapSymbol]
    data_pool_mismatch: bool

    def length_mismatch(self) -> bool:
        return self.symbol is not None and self.length != self.symbol.length()

    def format(self) -> str:
        """Return a pretty-print string for this CorrelatedSymbol."""
        if self.symbol is None:
            s = f"address 0x{self.address:X}"
        elif self.address != self.symbol.address():
            s = f"address 0x{self.address:X} (in {self.symbol.format(include_addr=False)})"
        else:
            s = self.symbol.format()

        mismatches = []
        if self.length_mismatch():
            mismatches.append(
                f"length mismatch: {self.length} != {self.symbol.length()}"
            )
        if self.data_pool_mismatch:
            mismatches.append("data pool mismatch")
        if mismatches:
            s += f" ({', '.join(mismatches)})"

        return s


def xmap_correlate(
    source: MappedBinary,
    target: MappedBinary,
    *,
    no_veneers: bool = False,
    match_data_pools: bool = True,
    match_length: bool = True,
    ignore_ldr_str_offset: bool = False,
    ignore_b_offset: bool = False,
    min_length: int = 0,
    source_symbol_types: Optional[Set[SegmentDataType]] = None,
    source_ignore: Optional[re.Pattern] = None,
    target_filter: Optional[re.Pattern] = None,
    verbosity: int = 0,
) -> List[Tuple[MapSymbol, List[CorrelatedSymbol]]]:
    """Find symbols in a target binary that match symbols in the source binary.

    Args:
        source (MappedBinary): xMAP-annotated source binary
        target (MappedBinary): xMAP-annotated target binary
        no_veneers (bool, optional): Skip veneer function symbols. Defaults to False.
        match_data_pools (bool, optional): Require data pools to match for function
            symbols. Defaults to True.
        match_length (bool, optional): Require symbols to match in length. Defaults to True.
        ignore_ldr_str_offset (bool, optional): Allow mismatching offsets on ldr/str instructions. Defaults to False.
        ignore_b_offset (bool, optional): Allow mismatching offsets on b instructions. Defaults to False.
        min_length (int, optional): Length threshold under which to skip matches for source symbols. Defaults to 0.
        source_symbol_types (Optional[Set[SegmentDataType]], optional): Set restricting the
            types of source symbols to match. Defaults to None.
        source_ignore (Optional[re.Pattern], optional): Full-match regex for source
            symbols to skip. Defaults to None.
        target_filter (Optional[re.Pattern], optional): Full-match regex restricting
            which pre-existing target symbols can be overridden by matching source
            symbols. Defaults to None.
        verbosity (int, optional): verbosity (0-4). Defaults to 0.

    Returns:
        List[Tuple[MapSymbol, List[CorrelatedSymbol]]]: list of entries of the form
            (source symbol, [target matches]).
    """

    class SearchCandidate(NamedTuple):
        start: int
        end: int
        data_pool_mismatch: bool

    def report(message: str, level: int = 0):
        if verbosity >= level:
            print(message)

    def debug(message: str, level: int = 4):
        report(f"DEBUG: {message}", level)

    correlations: List[Tuple[MapSymbol, List[CorrelatedSymbol]]] = []
    for src_symbol in source.symbols:
        if not src_symbol.segments:
            debug(f"{src_symbol.format()} has no segments?")
            continue
        if (
            src_symbol.length() < min_length
            or (source_ignore is not None and source_ignore.fullmatch(src_symbol.name))
            or (
                source_symbol_types is not None
                and src_symbol.data_type() not in source_symbol_types
            )
        ):
            report(
                f"ignored source symbol: {src_symbol.format(include_length=True)}", 3
            )
            continue
        if no_veneers and source.is_veneer(src_symbol):
            report(
                f"ignored source veneer: {src_symbol.format(include_length=True)}", 3
            )
            continue

        debug(f"searching for source symbol: {src_symbol.format(include_length=True)}")
        target_candidates: Optional[List[SearchCandidate]] = None
        symbol_type = src_symbol.data_type()
        for src_seg, src_regex in source.symbol_regexes(
            src_symbol,
            ignore_ldr_str_offset=ignore_ldr_str_offset,
            ignore_b_offset=ignore_b_offset,
        ):
            if target_candidates is None:
                # first segment
                target_candidates = [
                    SearchCandidate(
                        match.start(), match.start() + src_seg.length, False
                    )
                    for match in src_regex.finditer(target.contents)
                ]
            else:
                # subsequent segments
                for i, candidate in enumerate(target_candidates):
                    match = src_regex.match(target.contents, candidate.end)
                    if match:
                        # move the cursor forward
                        target_candidates[i] = candidate._replace(
                            end=candidate.end + src_seg.length
                        )
                    elif (
                        symbol_type.is_asm()
                        and src_seg.data_type == SegmentDataType.DATA
                    ):
                        # skip mismatching data pool
                        target_candidates[i] = SearchCandidate(
                            candidate.start, candidate.end + src_seg.length, True
                        )
                    else:
                        # no match, drop the candidate
                        del target_candidates[i]
            if not target_candidates:
                break
        if not target_candidates:
            continue

        debug(f"{src_symbol.name}: found {len(target_candidates)} match(es)")
        target_matches: List[CorrelatedSymbol] = []
        for candidate in target_candidates:
            abs_addr = target.get_absolute(candidate.start)
            target_match = CorrelatedSymbol(
                abs_addr,
                candidate.end - candidate.start,
                target.symbol_occupying(abs_addr),
                candidate.data_pool_mismatch,
            )
            if (match_data_pools and target_match.data_pool_mismatch) or (
                match_length and target_match.length_mismatch()
            ):
                # drop candidates with data pool or length mismatches
                report(
                    "dropped match: "
                    + f"{src_symbol.format()} -> {target_match.format()}",
                    2,
                )
                continue
            if (
                target_filter is not None
                and target_match.symbol is not None
                and target_match.address == target_match.symbol.address()
                and src_symbol.name != target_match.symbol.name
                and not target_filter.fullmatch(target_match.symbol.name)
            ):
                # symbol name doesn't pass the filter, omit it from the output
                report(
                    "filtered match overriding target symbol "
                    + f"{target_match.symbol.name}: "
                    + f"{src_symbol.format()} -> {target_match.format()}",
                    1,
                )
                continue
            target_matches.append(target_match)
        if not target_matches:
            continue

        correlations.append((src_symbol, target_matches))
    return correlations


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Correlate segments of ARMv5 assembly from one binary to segments in another binary via xMAP symbol map information"
    )
    parser.add_argument(
        "-v", "--verbose", action="count", default=0, help="verbosity level"
    )
    parser.add_argument(
        "-V",
        "--no-veneers",
        action="store_true",
        help="don't match veneer functions",
    )
    parser.add_argument(
        "-D",
        "--ignore-data-pools",
        action="count",
        default=0,
        help="ignore data pools when matching function symbols"
        + "(specify twice to show all matches, even if matches with matching data exist)",
    )
    parser.add_argument(
        "-L",
        "--ignore-length",
        action="store_true",
        help="ignore length when matching symbols",
    )
    parser.add_argument(
        "--ignore-ldr-str-offset",
        action="store_true",
        help="ignore the offset of ldr/str assembly instructions",
    )
    parser.add_argument(
        "--ignore-b-offset",
        action="store_true",
        help="ignore the offset of b assembly instructions",
    )
    parser.add_argument(
        "-l",
        "--min-length",
        help="minimum number of bytes under which to ignore symbols from the source binary",
        default=8,  # 2 ARM instructions
    )
    parser.add_argument(
        "-t",
        "--type",
        help="character set specifying the symbol types to match "
        + "(a = ARM, t = Thumb, d = data)",
        default="at",
    )
    parser.add_argument(
        "-i",
        "--ignore",
        help="regular expression for symbol names (full matches) in the source binary to ignore",
    )
    parser.add_argument(
        "-o",
        "--override-filter",
        help="regular expression to restrict which existing symbol names (full matches) in the target binary can be overridden",
    )
    parser.add_argument(
        "-f",
        "--filter",
        help="shorthand for setting both --ignore and --override-filter",
    )
    parser.add_argument(
        "-s",
        "--include-self",
        action="store_true",
        help="include matches where the source and target symbol names are identical",
    )
    parser.add_argument("source_bin", help="source binary file to take symbols from")
    parser.add_argument(
        "source_map", help="source xMAP file with known symbols in the source binary"
    )
    parser.add_argument(
        "source_section", help="section within the source xMAP file to parse"
    )
    parser.add_argument(
        "target_bin", help="target binary file to search for symbols in"
    )
    parser.add_argument(
        "target_map",
        help="target xMAP file with known symbols in the target binary",
        nargs="?",
    )
    parser.add_argument(
        "target_section",
        help="section within the target xMAP file to parse (defaults to source-section if unspecified)",
        nargs="?",
    )
    args = parser.parse_args()

    if args.filter is not None:
        if args.ignore is None:
            args.ignore = args.filter
        if args.override_filter is None:
            args.override_filter = args.filter
    if args.target_map is not None:
        if args.target_section is None:
            args.target_section = args.source_section

    symbol_types: Set[SegmentDataType] = set()
    for c in args.type:
        try:
            symbol_types.add(SegmentDataType(f"${c.lower()}"))
        except ValueError:
            print(
                f"error: unrecognized symbol type '{c}' (valid types: 'a', 't', 'd')",
                file=sys.stderr,
            )
            raise SystemExit(1)

    ignore_re = None
    override_filter_re = None
    if args.ignore is not None:
        ignore_re = re.compile(args.ignore)
    if args.override_filter is not None:
        override_filter_re = re.compile(args.override_filter)

    source = MappedBinary(
        args.source_bin, args.source_map, args.source_section, debug=args.verbose >= 4
    )
    if args.verbose > 0:
        print(f"Source: {source}")

    target = MappedBinary(
        args.target_bin, args.target_map, args.target_section, debug=args.verbose >= 4
    )
    if args.verbose > 0:
        print(f"Target: {target}")

    correlations = xmap_correlate(
        source,
        target,
        no_veneers=args.no_veneers,
        match_data_pools=args.ignore_data_pools <= 0,
        match_length=not args.ignore_length,
        ignore_ldr_str_offset=args.ignore_ldr_str_offset,
        ignore_b_offset=args.ignore_b_offset,
        min_length=args.min_length,
        source_symbol_types=symbol_types,
        source_ignore=ignore_re,
        target_filter=override_filter_re,
        verbosity=args.verbose,
    )

    for src_symbol, target_matches in correlations:
        if not args.include_self:
            # filter out any self-matches
            target_matches = [
                t
                for t in target_matches
                if t.symbol is None or t.symbol.name != src_symbol.name
            ]
            if not target_matches:
                continue
        if args.ignore_data_pools == 1:
            # if there's at least one non-self target match with exact data pool
            # match, filter out other target matches with data pool mismatches
            has_exact_match = any(
                not t.data_pool_mismatch
                and (t.symbol is None or t.symbol.name != src_symbol.name)
                for t in target_matches
            )
            if has_exact_match:
                target_matches = [t for t in target_matches if not t.data_pool_mismatch]
        target_str = ", ".join([t.format() for t in target_matches])
        if len(target_matches) > 1:
            target_str = f"({len(target_matches)}) {target_str}"
        print(f"{src_symbol.format(include_length=True)} -> {target_str}")
