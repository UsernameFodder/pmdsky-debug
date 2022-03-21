#!/usr/bin/env python3

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
        return f"{self.offset:#x}..{self.offset + self.length:#x}"

    def __eq__(self, other: "Segment") -> bool:
        return self.offset == other.offset and self.length == other.length

    def __ne__(self, other: "Segment") -> bool:
        return not (self == other)

    def __hash__(self) -> int:
        return hash((self.offset, self.length))

    def read(self, file: BinaryIO) -> bytes:
        """Read a file at the specified segment"""
        file.seek(self.offset)
        return file.read(self.length)

    def regex(self, file: BinaryIO) -> re.Pattern:
        """Get regex for the contents of the specified segment within a file"""
        return re.compile(re.escape(self.read(file)))


class AsmSegment(Segment):
    """Represents a contiguous segment of ARMv5 assembly instructions within a file"""

    INSTRUCTION_SIZE: int = 4

    def __repr__(self) -> str:
        return f"asm: {super().__repr__()}"

    def instructions(self, file: BinaryIO) -> Iterator[bytes]:
        """Iterator over instructions as raw little endian 4-byte arrays"""
        raw = self.read(file)
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

    def regex(self, file: BinaryIO) -> re.Pattern:
        pattern = b""
        for instr in self.instructions(file):
            if AsmSegment.instruction_is_bl(instr):
                # Allow any offset (least significant 3 bytes) for bl instructions
                pattern += (
                    f".{{{AsmSegment.INSTRUCTION_SIZE - 1}}}".encode()
                    + re.escape(instr[-1:])
                )
                pass
            else:
                pattern += re.escape(instr)
        return re.compile(pattern)


class DataSegment(Segment):
    """Represents a contiguous segment of raw data within a file"""

    def __repr__(self) -> str:
        return f"data: {super().__repr__()}"


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
parser.add_argument("-v", "--verbose", action="store_true", help="verbose output")
parser.add_argument("source", help="source binary file to take search segments from")
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

# List of matches by source segment, by target file
search_results: List[List[List[Segment]]] = [
    [[] for s in range(len(args.target))] for t in range(len(segments))
]

# Perform the search
with open(args.source, "rb") as src_file:
    # Open all the files up front to avoid opening/closing them with each inner
    # loop iteration.
    target_files = [open(target, "rb") for target in args.target]

    # The outer loop is over search segments, since each one might be fairly
    # large, and there could be a lot of them, so we only want to read one into
    # memory at a time, rather than all at once
    for seg, seg_matches in zip(segments, search_results):
        search_regex = seg.regex(src_file)
        if args.verbose:
            print(f"{seg} regex: {search_regex}")

        # The inner loop is over target files to search; there shouldn't be many
        for target_file, file_matches, tname in zip(
            target_files, seg_matches, args.target
        ):
            # Scan contents of the target file for the segment from the source file
            contents = target_file.read()
            for match in search_regex.finditer(contents):
                match_segment = Segment(match.start(), match.end() - match.start())
                if (
                    not args.include_self_matches
                    and tname == args.source
                    and match_segment == seg
                ):
                    # Omit the original segment within the source file,
                    # which is a guaranteed match
                    continue
                file_matches.append(match_segment)

    # Cleanup
    for target_file in target_files:
        target_file.close()

# Report search results
print(f"*** SOURCE FILE: {args.source} ***")
for seg, seg_matches in zip(segments, search_results):
    print(f"== Matches for {seg} ==")
    for tname, file_matches in zip(args.target, seg_matches):
        for match in file_matches:
            print(f"  - [{tname}]: {match}")
