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
    verbose: bool = False,
) -> List[List[List[Segment]]]:
    """Search through target ARMv5 binary files for contents from a source file

    Args:
        src_filename (str): source file name
        target_filenames (List[str]): target file names
        segments (List[Segment]): segments within the source file to match
        self_matches (bool, optional): include self-matches if searching the source file. Defaults to False.
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
        search_regexes = [seg.regex(src_file) for seg in segments]
        if verbose:
            # Print the regexes in verbose mode
            for seg, regex in zip(segments, search_regexes):
                print(f"{seg} regex: {regex.pattern}")

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
        verbose=args.verbose,
    )

    # Report search results
    print(f"*** SOURCE FILE: {args.source} ***")
    for seg, seg_matches in zip(segments, search_results):
        print(f"== Matches for {seg} ==")
        for tname, file_matches in zip(args.target, seg_matches):
            for match in file_matches:
                print(f"  - [{tname}]: {match}")
