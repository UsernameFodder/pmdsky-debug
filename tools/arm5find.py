#!/usr/bin/env python3

"""
`arm5find.py` is a command line utility for searching for matching
instructions or data across different ARMv5 binaries. It can be used to fill
in symbol addresses that are known in some EoS versions but not others. The
tool will search in one or more target binaries for the specified byte
segments in a source file. With assembly instructions, matches don't need to
be exact, just equivalent (e.g., function call offsets in `bl` instructions
can differ).

Example usage (note that offsets are file-relative):
python3 arm5find.py -a 0x1390 0x28c </path/to/arm9_NA.bin> </path/to/arm9_EU.bin>
python3 arm5find.py -d 0x77330 0x14 </path/to/overlay29_NA.bin> </path/to/overlay29_EU.bin>

You can include more than one `-a`/`-d` inputs to search for multiple segments
at once. You can also include more than one target file to search multiple
files at once.
"""

import argparse
import re
from typing import BinaryIO, Iterator, List, Union


class Segment:
    """Represents a contiguous segment of bytes within a file"""

    def __init__(self, offset: Union[int, str], length: Union[int, str]):
        self.offset = offset if type(offset) == int else int(offset, 0)
        self.length = length if type(length) == int else int(length, 0)
        if self.offset < 0:
            raise ValueError("segment offset must be nonnegative")
        if self.length <= 0:
            raise ValueError("segment length must be positive")

    def __repr__(self) -> str:
        return f"{self.offset:#x}..{self.end():#x}"

    def __str__(self) -> str:
        return f"{self.offset:#x}..{self.end():#x}"

    def __eq__(self, other: "Segment") -> bool:
        return self.offset == other.offset and self.length == other.length

    def __ne__(self, other: "Segment") -> bool:
        return not (self == other)

    def __lt__(self, other: "Segment") -> bool:
        return self.offset < other.offset or (
            self.offset == other.offset and self.length < other.length
        )

    def __le__(self, other: "Segment") -> bool:
        return self < other or self == other

    def __gt__(self, other: "Segment") -> bool:
        return not (self <= other)

    def __ge__(self, other: "Segment") -> bool:
        return not (self < other)

    def __hash__(self) -> int:
        return hash((self.offset, self.length))

    def end(self) -> int:
        return self.offset + self.length

    def read_from_file(self, file: BinaryIO) -> bytes:
        """Read a file at the specified segment"""
        file.seek(self.offset)
        return file.read(self.length)

    def read_from_buf(self, buf: bytes) -> bytes:
        """Read a raw buffer at the specified segment"""
        return buf[self.offset : self.offset + self.length]

    def read(self, stream: Union[BinaryIO, bytes]) -> bytes:
        """Read a file or raw buffer at the specified segment"""
        if isinstance(stream, bytes):
            return self.read_from_buf(stream)
        return self.read_from_file(stream)

    def regex(self, stream: Union[BinaryIO, bytes]) -> re.Pattern:
        """Get regex for the contents of the specified segment within a file or raw buffer"""
        return re.compile(re.escape(self.read(stream)))


class AsmSegment(Segment):
    """Represents a contiguous segment of ARMv5 assembly instructions within a file"""

    INSTRUCTION_SIZE: int = 4

    def __repr__(self) -> str:
        return f"asm: {super().__repr__()}"

    def instructions(self, stream: Union[BinaryIO, bytes]) -> Iterator[bytes]:
        """Iterator over instructions as raw little endian 4-byte arrays"""
        raw = self.read(stream)
        return (
            raw[i : i + AsmSegment.INSTRUCTION_SIZE]
            for i in range(0, len(raw), AsmSegment.INSTRUCTION_SIZE)
        )

    @staticmethod
    def instruction_is_bl(instruction: bytes) -> bool:
        """Checks if an instruction (as a little endian 4-byte array) is `bl`"""
        return (
            len(instruction) == AsmSegment.INSTRUCTION_SIZE
            and (instruction[-1] & 0b1111) == 0b1011
        )

    @staticmethod
    def instruction_is_b(instruction: bytes) -> bool:
        """Checks if an instruction (as a little endian 4-byte array) is `b{l}`"""
        return (
            len(instruction) == AsmSegment.INSTRUCTION_SIZE
            and (instruction[-1] & 0b1110) == 0b1010
        )

    @staticmethod
    def instruction_is_addr_mode2(instruction: bytes) -> bool:
        # Reference manual A5-18: LDR{B}{T}, STR{B}{T}
        return len(instruction) == AsmSegment.INSTRUCTION_SIZE and (
            # LDR, LDRB, STR, STRB
            ((instruction[3] & 0b1100) == 0b0100)
            or
            # LDRT, LDRBT, STRT, STRBT
            (
                (instruction[3] & 0b1101) == 0b0100
                and (instruction[2] & 0b00100000) == 0b00100000
            )
        )

    @staticmethod
    def instruction_is_addr_mode3(instruction: bytes) -> bool:
        # Reference manual A5-33: LDRD, LDREX, LDRH, LDRSB, LDRSH, STRD, STREX, STRH
        return len(instruction) == AsmSegment.INSTRUCTION_SIZE and (
            # LDRD and LDRSB
            (
                (instruction[3] & 0b1110) == 0
                and (instruction[0] & 0b11110000) == 0b11010000
            )
            or
            # LDREX and STREX
            (
                (instruction[3] & 0b1111) == 0b0001
                and (instruction[2] & 0b11100000) == 0b10000000
                and (instruction[0] & 0b11110000) == 0b10010000
            )
            or
            # LDRH and LDRSH
            (
                (instruction[3] & 0b1110) == 0
                and (instruction[2] & 0b00010000) == 0b00010000
                and (instruction[0] & 0b10110000) == 0b10110000
            )
            or
            # STRD
            (
                (instruction[3] & 0b1110) == 0
                and (instruction[2] & 0b00010000 == 0)
                and (instruction[0] & 0b11110000) == 0b11110000
            )
            or
            # STRH
            (
                (instruction[3] & 0b1110) == 0
                and (instruction[2] & 0b00010000) == 0
                and (instruction[0] & 0b11110000) == 0b10110000
            )
        )

    @staticmethod
    def bit_prefix_to_re(byte: int, bits: int) -> bytes:
        mask = ((1 << bits) - 1) << (8 - bits)
        inv_mask = mask ^ 0b11111111
        prefix = byte & mask
        return (
            "[".encode()
            + re.escape(bytes([prefix]))
            + "-".encode()
            + re.escape(bytes([prefix + inv_mask]))
            + "]".encode()
        )

    def regex(
        self,
        stream: Union[BinaryIO, bytes],
        *,
        ignore_ldr_str_offset: bool = False,
        ignore_b_offset: bool = False,
    ) -> re.Pattern:
        pattern = b""
        for instr in self.instructions(stream):
            if (
                ignore_b_offset and AsmSegment.instruction_is_b(instr)
            ) or AsmSegment.instruction_is_bl(instr):
                # Allow any offset (least significant 3 bytes) for bl instructions
                pattern += (
                    f".{{{AsmSegment.INSTRUCTION_SIZE - 1}}}".encode()
                    + re.escape(instr[-1:])
                )
            elif ignore_ldr_str_offset and AsmSegment.instruction_is_addr_mode2(instr):
                # Allow any addr_mode (last 12 bits) for addr mode 2 instructions
                rd = instr[1] & 0b11110000
                pattern += (
                    ".".encode()
                    + AsmSegment.bit_prefix_to_re(rd, 4)
                    + re.escape(instr[2:])
                )
            elif ignore_ldr_str_offset and AsmSegment.instruction_is_addr_mode3(instr):
                # Allow any addr_mode (bits 0-3, 8-11) for addr mode 3 instructions
                rd = instr[1] & 0b11110000
                typ = instr[0] & 0b11110000
                pattern += (
                    AsmSegment.bit_prefix_to_re(typ, 4)
                    + AsmSegment.bit_prefix_to_re(rd, 4)
                    + re.escape(instr[2:])
                )
                pass
            else:
                pattern += re.escape(instr)
        return re.compile(pattern, flags=re.DOTALL)


class DataSegment(Segment):
    """Represents a contiguous segment of raw data within a file"""

    def __repr__(self) -> str:
        return f"data: {super().__repr__()}"


def armv5_search(
    src_filename: str,
    target_filenames: List[str],
    segments: List[Segment],
    *,
    self_matches: bool = False,
    ignore_ldr_str_offset: bool = False,
    ignore_b_offset: bool = False,
    verbose: bool = False,
) -> List[List[List[Segment]]]:
    """Search through target ARMv5 binary files for contents from a source file

    Args:
        src_filename (str): source file name
        target_filenames (List[str]): target file names
        segments (List[Segment]): segments within the source file to match
        self_matches (bool, optional): include self-matches if searching the source file. Defaults to False.
        ignore_ldr_str_offset (bool, optional): ignore the offset of ldr and str assembly instructions. Defaults to False.
        ignore_b_offset (bool, optional): ignore the offset of b assembly instructions. Defaults to False.
        verbose (bool, optional): verbose printing. Defaults to False.

    Returns:
        List[List[List[Segment]]]: Search results, as a list of matches by source segment, by target file
    """

    # List of matches by source segment, by target file
    search_results: List[List[List[Segment]]] = [
        [[] for s in range(len(target_filenames))] for t in range(len(segments))
    ]

    # Perform the search
    with open(src_filename, "rb") as src_file:
        # Compile all the search regexes up front to avoid repeating the work with
        # each target file. The combined size of all search segments is bounded by
        # the source file size, so it shouldn't be an issue to load everything
        # into memory at once
        search_regexes = [
            seg.regex(
                src_file,
                ignore_ldr_str_offset=ignore_ldr_str_offset,
                ignore_b_offset=ignore_b_offset,
            )
            if isinstance(seg, AsmSegment)
            else seg.regex(src_file)
            for seg in segments
        ]
        if verbose:
            # Print the regexes in verbose mode
            for seg, regex in zip(segments, search_regexes):
                print(f"{seg!r} regex: {regex.pattern}")

        # The outer loop is over target files to search. Only load one at a time.
        for t, target_fname in enumerate(target_filenames):
            with open(target_fname, "rb") as target_file:
                contents = target_file.read()

                # The inner loop is over search segments
                for seg, regex, seg_matches in zip(
                    segments, search_regexes, search_results
                ):
                    for match in regex.finditer(contents):
                        match_segment = Segment(
                            match.start(), match.end() - match.start()
                        )
                        if (
                            not self_matches
                            and target_fname == src_filename
                            and match_segment == seg
                        ):
                            # Omit the original segment within the source file,
                            # which is a guaranteed match
                            continue
                        seg_matches[t].append(match_segment)
    return search_results


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Find ARMv5 assembly or raw data from one binary within another binary"
    )
    parser.add_argument(
        "-a",
        "--asm",
        action="append",
        nargs=2,
        metavar=("offset", "length"),
        default=[],
        help="assembly instructions from source to search for (supports prefixed code literals)",
    )
    parser.add_argument(
        "-d",
        "--data",
        action="append",
        nargs=2,
        metavar=("offset", "length"),
        default=[],
        help="data from source to search for (supports prefixed code literals)",
    )
    parser.add_argument(
        "-s",
        "--include-self-matches",
        action="store_true",
        help="include self-matches from the source file in search results",
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
    parser.add_argument("-v", "--verbose", action="store_true", help="verbose output")
    parser.add_argument(
        "source", help="source binary file to take search segments from"
    )
    parser.add_argument("target", nargs="*", help="target binary file to search")
    args = parser.parse_args()

    # If no target files were provided, search the source file
    if not args.target:
        args.target = [args.source]

    segments: List[Segment] = []
    for a in args.asm:
        segments.append(AsmSegment(*a))
    for d in args.data:
        segments.append(DataSegment(*d))

    search_results = armv5_search(
        args.source,
        args.target,
        segments,
        self_matches=args.include_self_matches,
        ignore_ldr_str_offset=args.ignore_ldr_str_offset,
        ignore_b_offset=args.ignore_b_offset,
        verbose=args.verbose,
    )

    # Report search results
    print(f"*** SOURCE FILE: {args.source} ***")
    for seg, seg_matches in zip(segments, search_results):
        print(f"== Matches for {seg!r} ==")
        for tname, file_matches in zip(args.target, seg_matches):
            for match in file_matches:
                print(f"  - [{tname}]: {match}")
