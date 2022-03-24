# Tools

This directory contains miscellaneous tools for reverse engineering _Explorers of Sky_.

## `arm5find.py`
`arm5find.py` is a command line utility for searching for matching instructions or data across different ARMv5 binaries. It can be used to fill in symbol addresses that are known in some EoS versions but not others. The tool will search in one or more target binaries for the specified byte segments in a source file. With assembly instructions, matches don't need to be exact, just equivalent (e.g., function call offsets can differ). The script is invokable with the `python3` command. See the help text (`python3 arm5find.py --help`) for usage instructions, and see the description in [`arm5find.py`](arm5find.py) itself for more details.

## `offsets.py`
`offsets.py` is a command line utility for converting EoS offsets between absolute memory addresses and relative file offsets. One possible use is for converting addresses in the symbol tables into file-relative offsets for `arm5find.py`, and vice versa, but the tool is useful whenever such conversions are needed. The script is invokable with the `python3` command. See the help text (`python3 offsets.py --help`) for usage instructions, and see the description in [`offsets.py`](offsets.py) itself for more details.
