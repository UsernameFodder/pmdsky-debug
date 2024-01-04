# Tools

This directory contains miscellaneous tools for reverse engineering _Explorers of Sky_.

## `arm5find.py`
`arm5find.py` is a command line utility for searching for matching instructions or data across different ARMv5 binaries. It can be used to fill in symbol addresses that are known in some EoS versions but not others. The tool will search in one or more target binaries for the specified byte segments in a source file. With assembly instructions, matches don't need to be exact, just equivalent (e.g., function call offsets can differ). The script is invokable with the `python3` command. See the help text (`python3 arm5find.py --help`) for usage instructions, and see the description in [`arm5find.py`](arm5find.py) itself for more details.

## `offsets.py`
`offsets.py` is a command line utility for converting EoS offsets between absolute memory addresses and relative file offsets. One possible use is for converting addresses in the symbol tables into file-relative offsets for `arm5find.py`, and vice versa, but the tool is useful whenever such conversions are needed. The script is invokable with the `python3` command. See the help text (`python3 offsets.py --help`) for usage instructions, and see the description in [`offsets.py`](offsets.py) itself for more details.

## `resymgen.py`
`resymgen.py` is a Python interface for calling `resymgen` programmatically from Python via `subprocess`. It requires `cargo` to be available in the runtime environment. See the description of [`resymgen.py`](resymgen.py) for usage instructions.

## `symbols_vfill.py`
`symbols_vfill.py` is a command line utility for filling in missing function addresses in the `pmdsky-debug` [symbol tables](../symbols), for addresses that are known in some game versions (e.g., NA, EU) but not in others. It relies on [`resymgen.py`](#resymgenpy) and thus has the same prerequisites. See the help text (`python3 symbols_vfill.py --help`) for usage instructions, and see the description in [`symbols_vfill.py`](symbols_vfill.py) itself for more details.

## `symcompat.py`
`symcompat.py` is a command line utility for checking symbol name compatibility across different revisions of the `pmdsky-debug` [symbol tables](../symbols). It is largely based on [`symdiff.py`](symdiff.py). See the help text (`python3 symcompat.py --help`) for usage instructions, and see the description in [`symcompat.py`](symcompat.py) itself for more details.

## `symdiff.py`
`symdiff.py` is a command line diff utility for comparing the `pmdsky-debug` [symbol tables](../symbols) across different revisions. It has a similar interface to `git diff`, but runs a specialized diffing algorithm. See the help text (`python3 symdiff.py --help`) for usage instructions, and see the description in [`symdiff.py`](symdiff.py) itself for more details.
