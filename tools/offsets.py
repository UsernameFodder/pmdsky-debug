#!/usr/bin/env python3

"""
`offsets.py` is a command line utility for converting EoS offsets between
absolute memory addresses and relative file offsets.

Given a game version (the default is NA), an optional list of binary files to
consider, and one or more offsets, the tool will infer whether each input
offset is an absolute address or a file offset and convert it accordingly
(this is possible to do unambiguously because memory load addresses are always
bigger than file offsets). For each input offset, if the offset could
correspond to multiple binary files (from the provided list), all possible
conversions will be listed. If no binary files are provided and only absolute
offsets are given, all possible binary files will be considered. If any of the
input offsets are relative, an explicit list of binary files must be provided.

Example usage:
python3 offsets.py 0x2010000 0x22DC260
python3 offsets.py -b arm9 -b overlay29 0x2010000 0x22DC260
python3 offsets.py -v EU -b overlay29 0x22DCBA0
python3 offsets.py -b arm9 0x100 0x200 0x2010000
"""

import argparse
from typing import List, Optional, Union


class Binary:
    """Represents a binary file"""

    def __init__(self, address: int, length: int, file_offset: int = 0):
        self.address = address
        self.length = length
        self.file_offset = file_offset
        if self.address < 0:
            raise ValueError("binary load address must be nonnegative")
        if self.length <= 0:
            raise ValueError("binary length must be positive")
        if self.file_offset < 0:
            raise ValueError("binary file offset must be nonnegative")

    def __str__(self) -> str:
        s = f"0x{self.address:X}..0x{self.address + self.length:X}"
        if self.file_offset != 0:
            s += f" [file offset = 0x{self.file_offset:X}]"
        return s

    def __contains__(self, address: int) -> bool:
        return address >= self.address and address < self.address + self.length

    def absolute(self, relative: int) -> int:
        if relative >= self.file_offset and relative < self.file_offset + self.length:
            return self.address + relative - self.file_offset
        raise ValueError(f"relative offset {hex(relative)} is out of binary range")

    def relative(self, absolute: int) -> int:
        if absolute in self:
            return absolute - self.address + self.file_offset
        raise ValueError(f"absolute offset {hex(absolute)} is out of binary range")


BINARIES = {
    "NA": {
        "arm9": Binary(0x2000000, 0xB73F8),
        "arm9.itcm": Binary(0x1FF8000, 0x4000, file_offset=0xB3380),
        "arm7": Binary(0x2380000, 0x27080),
        "arm7.wram": Binary(0x37F8000, 0xF608, file_offset=0x1E8),
        "arm7.ram": Binary(0x27E0000, 0x17878, file_offset=0xF7F0),
        "overlay0": Binary(0x22BCA80, 0x609A0),
        "overlay1": Binary(0x2329520, 0x12D20),
        "overlay2": Binary(0x2329520, 0x2AFA0),
        "overlay3": Binary(0x233CA80, 0xA160),
        "overlay4": Binary(0x233CA80, 0x2BE0),
        "overlay5": Binary(0x233CA80, 0x3240),
        "overlay6": Binary(0x233CA80, 0x2460),
        "overlay7": Binary(0x233CA80, 0x5100),
        "overlay8": Binary(0x233CA80, 0x2200),
        "overlay9": Binary(0x233CA80, 0x2D80),
        "overlay10": Binary(0x22BCA80, 0x1F7A0),
        "overlay11": Binary(0x22DC240, 0x48C40),
        "overlay12": Binary(0x238A140, 0x20),
        "overlay13": Binary(0x238A140, 0x2E80),
        "overlay14": Binary(0x238A140, 0x3AE0),
        "overlay15": Binary(0x238A140, 0x1060),
        "overlay16": Binary(0x238A140, 0x2D20),
        "overlay17": Binary(0x238A140, 0x1CE0),
        "overlay18": Binary(0x238A140, 0x3500),
        "overlay19": Binary(0x238A140, 0x4240),
        "overlay20": Binary(0x238A140, 0x3000),
        "overlay21": Binary(0x238A140, 0x2E20),
        "overlay22": Binary(0x238A140, 0x4B40),
        "overlay23": Binary(0x238A140, 0x3780),
        "overlay24": Binary(0x238A140, 0x24E0),
        "overlay25": Binary(0x238A140, 0x14C0),
        "overlay26": Binary(0x238A140, 0xE40),
        "overlay27": Binary(0x238A140, 0x2D60),
        "overlay28": Binary(0x238A140, 0xC60),
        "overlay29": Binary(0x22DC240, 0x77620),
        "overlay30": Binary(0x2382820, 0x38A0),
        "overlay31": Binary(0x2382820, 0x7A80),
        "overlay32": Binary(0x2382820, 0x20),
        "overlay33": Binary(0x2382820, 0x20),
        "overlay34": Binary(0x22DC240, 0xE60),
        "overlay35": Binary(0x22BCA80, 0x20),
    },
    "EU": {
        "arm9": Binary(0x2000000, 0xB7D38),
        "arm9.itcm": Binary(0x1FF8000, 0x4000, file_offset=0xB3CC0),
        "arm7": Binary(0x2380000, 0x27080),
        "arm7.wram": Binary(0x37F8000, 0xF608, file_offset=0x1E8),
        "arm7.ram": Binary(0x27E0000, 0x17878, file_offset=0xF7F0),
        "overlay0": Binary(0x22BD3C0, 0x60880),
        "overlay1": Binary(0x2329D40, 0x12C80),
        "overlay2": Binary(0x2329D40, 0x2AFC0),
        "overlay3": Binary(0x233D200, 0xA160),
        "overlay4": Binary(0x233D200, 0x2BE0),
        "overlay5": Binary(0x233D200, 0x3240),
        "overlay6": Binary(0x233D200, 0x2460),
        "overlay7": Binary(0x233D200, 0x3300),
        "overlay8": Binary(0x233D200, 0x2620),
        "overlay9": Binary(0x233D200, 0x2D80),
        "overlay10": Binary(0x22BD3C0, 0x1F7A0),
        "overlay11": Binary(0x22DCB80, 0x48E40),
        "overlay12": Binary(0x238AC80, 0x20),
        "overlay13": Binary(0x238AC80, 0x2E80),
        "overlay14": Binary(0x238AC80, 0x3B40),
        "overlay15": Binary(0x238AC80, 0x1080),
        "overlay16": Binary(0x238AC80, 0x2D20),
        "overlay17": Binary(0x238AC80, 0x1CE0),
        "overlay18": Binary(0x238AC80, 0x3500),
        "overlay19": Binary(0x238AC80, 0x4220),
        "overlay20": Binary(0x238AC80, 0x3000),
        "overlay21": Binary(0x238AC80, 0x2E20),
        "overlay22": Binary(0x238AC80, 0x4B40),
        "overlay23": Binary(0x238AC80, 0x3780),
        "overlay24": Binary(0x238AC80, 0x24E0),
        "overlay25": Binary(0x238AC80, 0x14C0),
        "overlay26": Binary(0x238AC80, 0xE40),
        "overlay27": Binary(0x238AC80, 0x2D60),
        "overlay28": Binary(0x238AC80, 0xC60),
        "overlay29": Binary(0x22DCB80, 0x77900),
        "overlay30": Binary(0x2383420, 0x38A0),
        "overlay31": Binary(0x2383420, 0x7AA0),
        "overlay32": Binary(0x2383420, 0x20),
        "overlay33": Binary(0x2383420, 0x20),
        "overlay34": Binary(0x22DCB80, 0xDC0),
        "overlay35": Binary(0x22BD3C0, 0x20),
    },
    "JP": {
        "arm9": Binary(0x2000000, 0xB8CB8),
        "arm9.itcm": Binary(0x1FF8000, 0x4060, file_offset=0xB4BE0),
        "arm7": Binary(0x2380000, 0x27080),
        "arm7.wram": Binary(0x37F8000, 0xF608, file_offset=0x1E8),
        "arm7.ram": Binary(0x27E0000, 0x17878, file_offset=0xF7F0),
        "overlay0": Binary(0x22BE220, 0x609A0),
        "overlay1": Binary(0x232ACC0, 0x12E00),
        "overlay2": Binary(0x232ACC0, 0x2AFA0),
        "overlay3": Binary(0x233E300, 0xA160),
        "overlay4": Binary(0x233E300, 0x2BE0),
        "overlay5": Binary(0x233E300, 0x3260),
        "overlay6": Binary(0x233E300, 0x2460),
        "overlay7": Binary(0x233E300, 0x53E0),
        "overlay8": Binary(0x233E300, 0x2200),
        "overlay9": Binary(0x233E300, 0x2D20),
        "overlay10": Binary(0x22BE220, 0x1F6A0),
        "overlay11": Binary(0x22DD8E0, 0x48B00),
        "overlay12": Binary(0x238B6A0, 0x20),
        "overlay13": Binary(0x238B6A0, 0x2E80),
        "overlay14": Binary(0x238B6A0, 0x3AE0),
        "overlay15": Binary(0x238B6A0, 0x1060),
        "overlay16": Binary(0x238B6A0, 0x2D40),
        "overlay17": Binary(0x238B6A0, 0x1CE0),
        "overlay18": Binary(0x238B6A0, 0x3520),
        "overlay19": Binary(0x238B6A0, 0x4220),
        "overlay20": Binary(0x238B6A0, 0x3000),
        "overlay21": Binary(0x238B6A0, 0x2E40),
        "overlay22": Binary(0x238B6A0, 0x4B40),
        "overlay23": Binary(0x238B6A0, 0x37E0),
        "overlay24": Binary(0x238B6A0, 0x24E0),
        "overlay25": Binary(0x238B6A0, 0x14C0),
        "overlay26": Binary(0x238B6A0, 0xE40),
        "overlay27": Binary(0x238B6A0, 0x2DA0),
        "overlay28": Binary(0x238B6A0, 0xC60),
        "overlay29": Binary(0x22DD8E0, 0x77200),
        "overlay30": Binary(0x2383AA0, 0x3880),
        "overlay31": Binary(0x2383AA0, 0x7AC0),
        "overlay32": Binary(0x2383AA0, 0x20),
        "overlay33": Binary(0x2383AA0, 0x20),
        "overlay34": Binary(0x22DD8E0, 0xDC0),
        "overlay35": Binary(0x22BE220, 0x20),
    },
}
BINARY_NAMES = sorted(
    {b for v in BINARIES.values() for b in v},
    key=lambda x: f"overlay{int(x.lstrip('overlay')):04}"
    if x.startswith("overlay")
    else x,
)


class OffsetMapping:
    """A mapping from some relative/absolute offset to a list of complementary offsets"""

    def __init__(self, offset: int, is_relative: bool, is_absolute: bool):
        self.offset = offset
        self.is_relative = is_relative
        self.is_absolute = is_absolute
        self.mapped: List[Union[int, (str, int)]] = []

    def add(self, mapped_val: int, label: Optional[str] = None):
        self.mapped.append((label, mapped_val) if label is not None else mapped_val)

    def get_mapped(self) -> List[int]:
        return [m if type(m) == int else m[1] for m in self.mapped]

    def __str__(self) -> str:
        s = f"0x{self.offset:X}"
        if self.is_relative and not self.is_absolute:
            s += " (relative)"
        elif self.is_absolute and not self.is_relative:
            s += " (absolute)"
        s += ": "
        if not self.mapped:
            s += "???"
        else:
            s += ", ".join(
                f"0x{m:X}" if type(m) == int else f"0x{m[1]:X} ({m[0]})"
                for m in self.mapped
            )
        return s


def convert_offsets(
    version: str, bin_names: Optional[List[str]], offsets: List[int]
) -> List[OffsetMapping]:
    """Convert a list of offsets from absolute to relative or vice versa.

    Args:
        version (str): game version
        bin_names (Optional[List[str]]): list of binary file names to consider
        offsets (List[int]): list of offsets to convert

    Raises:
        ValueError: invalid offsets

    Returns:
        List[OffsetMapping]: list of conversions for each input offset
    """

    local_bin_map = BINARIES[version]

    # All binaries have lengths far smaller than their load addresses,
    # which makes inference simple
    min_bin_addr = min([b.address for b in local_bin_map.values()])
    max_bin_len = max([b.length for b in local_bin_map.values()])
    assert min_bin_addr > max_bin_len

    selected_binaries = {
        bname: b
        for bname, b in local_bin_map.items()
        if bin_names is None or bname in set(bin_names)
    }

    offset_mappings: List[OffsetMapping] = []
    for offset in offsets:
        if offset < 0:
            raise ValueError(f"negative offset -0x{abs(offset):X} is invalid")

        # By the above assert, is_relative and is_absolute are mutually exclusive
        is_relative = offset < max_bin_len
        is_absolute = offset >= min_bin_addr
        if is_relative and bin_names is None:
            raise ValueError(
                f"no binary specified, cannot interpret relative offset 0x{offset:X}"
            )

        # Do the offset conversion with any matching binaries
        mapping = OffsetMapping(offset, is_relative, is_absolute)
        for bname, b in selected_binaries.items():

            def add_to_mapping(val):
                if len(selected_binaries) == 1:
                    mapping.add(val)
                else:
                    mapping.add(val, bname)

            try:
                if is_relative:
                    add_to_mapping(b.absolute(offset))
                elif is_absolute:
                    add_to_mapping(b.relative(offset))
            except ValueError:
                continue
        offset_mappings.append(mapping)

    return offset_mappings


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Convert between absolute and relative offsets in the EoS binaries"
    )
    parser.add_argument(
        "-v",
        "--version",
        choices=BINARIES.keys(),
        type=str.upper,
        default="NA",
        help="EoS version",
    )
    parser.add_argument(
        "-b",
        "--binary",
        choices=BINARY_NAMES,
        action="append",
        help="EoS binary",
    )
    parser.add_argument(
        "offset",
        nargs="*",
        type=lambda x: int(x, 0),
        help="offset to convert (supports prefixed code literals, e.g., 0xff)",
    )
    args = parser.parse_args()

    offset_mappings = convert_offsets(args.version, args.binary, args.offset)

    print(f"Version: {args.version}")
    if args.binary:
        print(
            f"Binary(s): "
            + ", ".join([f"{b} ({BINARIES[args.version][b]})" for b in args.binary])
        )

    if offset_mappings:
        print()
    for mapping in offset_mappings:
        print(mapping)
