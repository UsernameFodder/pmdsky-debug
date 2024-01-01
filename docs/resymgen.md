# `resymgen`, a Reverse Engineering Symbol Table Generator
`resymgen` is a command line utility that generates symbol tables of various formats, given YAML configuration files. The output symbol tables are meant to be directly imported into different reverse engineering tools, whereas the YAML configuration files are meant to be generic, flexible, and supportive of documentation.

## Motivation
`resymgen` is a tool meant to ease collaboration in specialized reverse engineering efforts that are subject to one or more of the following constraints:

- The binary being reverse engineered has multiple versions, all of which are of interest, and symbol offsets vary by version.
- The binary being reverse engineered involves [overlays](https://en.wikipedia.org/wiki/Overlay_(programming)), meaning a single address can correspond to different symbols depending on context.
- Different collaborators use different sets of reverse engineering tools.
- The collaboration has not established an easy, standardized way to share debugging information in a comprehensive manner.
- There are potential legal issues with sharing an annotated version of the binary being reverse engineered.

The above constraints are particularly likely to appear in domains like video game reverse engineering. Given these constraints, sharing information can be very difficult to do well. With `resymgen`, a group can share address-to-symbol associations on a per-version and per-overlay basis, and in a human-readable YAML format that can double as documentation. The generic YAML format can then be transpiled into different symbol table formats for direct use with different reverse engineering tools, all without sharing the binary itself.

### Caveats
What `resymgen` does may not seem generally useful to most reverse engineering projects, and that's because it's not meant to be. If none of the aforementioned constraints apply to your reverse engineering problem, there are likely better ways to share information with collaborators than with `resymgen`. For example, if all collaborators standardize on one reverse engineering tool (e.g., Ghidra or IDA), it would be easier and more effective to use the tool's built-in import/export and collaboration features, especially if the binary can be legally shared in a full-project export. If the binary of interest only has one version (or only one version is cared about) and does not have overlays, it would likely be simpler to share debugging information through industry-standard formats such as PDB, DWARF, linker maps, etc.

## Usage
The `resymgen` binary is provided with this package. Run `resymgen --help` for detailed usage information. Each of the subcommands also have their own `--help` flag to print detailed usage information. The following list provides an overview of `resymgen`'s different subcommands.

- `gen`: Generate symbol tables for specified versions and output formats, given a `resymgen` YAML file.
- `fmt`: Formatter for `resymgen` YAML files.
- `check`: Validator for `resymgen` YAML files. Provides a collection of different checks that can be run on the contents of a file to ensure correctness.
- `merge`: Merge symbols from various structured input formats into another `resymgen` YAML file. This is in some sense the opposite of the `gen` subcommand.

### The `resymgen` YAML specification
A `resymgen` YAML file consists of one or more named _blocks_.

Each block is tagged with some metadata, including a starting memory address, a length, an optional version list, and an optional description. The address and length are allowed to be version-dependent. Each block also contains two lists of _symbols_, one for functions and one for data, and optionally a list of _subregions_.

A _symbol_ represents one or more memory regions containing an identifiable chunk of instructions or data. Each symbol has the following fields:
- A name (required)
- Zero or more _aliases_ (optional), which are alternate names for the symbol
- An address (required) and a length (optional). Similar to blocks, the address and length fields are allowed to be version-dependent.
    - Additionally, the address (or each address, if version-dependent) can be either a single value or a list of values. This is useful when a data symbol was defined as a constant or inlined, and was placed in multiple different locations (usually in data pools) by the compiler. This is also useful for functions that were defined with a static linkage in a C header and macro-included in multiple different source files, since this can result in the same function existing in multiple places in the compiled binary.
- A description (optional)

A _subregion_ represents a nested `resymgen` YAML file, which has one or more of its own named blocks, that is contained within the parent block. In a `resymgen` YAML file, a subregion is represented as a file name (note that it should _not_ be a file path with multiple components). If the parent file has the file path `/path/to/parent.yml`, and one of its blocks has a subregion with the name `sub.yml`, then this subregion name references a corresponding subregion file with the file path `/path/to/parent/sub.yml`.

Subregions are useful for splitting up large `resymgen` YAML files. If a parent file has one or more subregion files, blocks in the parent file can still contain metadata describing the region as a whole, and the parent file can be treated as an aggregate entity by `resymgen` subcommands.

#### Quick reference
```
<block1_name>:
  versions (optional):
    - <string>
    ...
  address: MaybeVersionDep[number]
  length: MaybeVersionDep[number]
  description (optional): <string>
  subregions (optional):
    - <file name>
    ...
  functions:
    - name: <string>
      aliases (optional):
        - <string>
        ...
      address: MaybeVersionDep[ScalarOrList[number]]
      length (optional): MaybeVersionDep[number]
      description (optional): <string>
    ...
  data:
    - name: <string>
      aliases (optional):
        - <string>
        ...
      address: MaybeVersionDep[ScalarOrList[number]]
      length (optional): MaybeVersionDep[number]
      description (optional): <string>
    ...
...
```

Assuming the following type definitions:
```
MaybeVersionDep[T] = <T> OR {<string>: <T>, ...}
ScalarOrList[T] = <T> OR [<T>, ...]
```

#### Example
```yml
main:
  versions:
    - v1
    - v2
  address:
    v1: 0x2000000
    v2: 0x2010000
  length:
    v1: 0x100000
    v2: 0x100000
  description: The main memory region
  subregions:
    - sub1.yml
    - sub2.yml
  functions:
    - name: function1
      aliases:
        - function1_alias1
        - function1_alias2
      address:
        v1: 0x2001000
        v2: 0x2012000
      description: |-
        multi
        line
        description
    - name: function2
      address:
        v1:
          - 0x2002000
          - 0x2003000
        v2: 0x2013000
      description: simple description
  data:
    - name: SOME_DATA
      address:
        v1: 0x2000000
        v2: 0x2010000
      length:
        v1: 0x1000
        v2: 0x1600
other:
  address: 0x2400000
  length: 0x100000
  functions: []
  data:
    - name: OTHER_DATA
      address: 0x2400000
```

### Currently supported output formats (`gen`)
- Ghidra-compatible symbol table (imported via the `ImportSymbolsScript.py` script)
- JSON
- No$GBA SYM format

### Currently supported input formats (`merge`)
- `resymgen` YAML
- Ghidra-exported CSV format with "Name", "Location", and "Type" columns and newline-delimited records
