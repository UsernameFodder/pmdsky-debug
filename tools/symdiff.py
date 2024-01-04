#!/usr/bin/env python3

"""
`symdiff.py` is a command line utility for comparing Git revisions of the
pmdsky-debug symbol tables. It will output a list of changes to the symbols in
a target revision relative to a base revision, possibly for a subset of symbol
files if specified.

Note that the focus is on symbols. Only material changes to the contents or
file locations of symbols will be reported; changes to block-level metadata
will not be reported unless they affect one or more symbols.

The diff algorithm tries to intelligently correlate symbols across revisions,
even if the names or addresses change. For the most part, this is a simple
one-to-one matching by address and/or name, but in the event of conflicts, the
following conflict-resolution procedure is followed:
    1. Give priority to better matches, defined by the number of matching
       addresses, and whether or not the symbol names match.
    2. If there are conflicting matches of the same quality, compute a maximum
       cardinality matching for the collection of symbols in contention.

By default, the `symdiff.py` will ignore certain "unimportant" changes to
symbols. This includes changes to symbol descriptions, and when symbols are
moved between subregions but kept within the same top-level block. These
changes can be included in the diff with the `--descriptions` and
`--subregion-resolution` flags, respectively.

Example usage:

python3 symdiff.py
python3 symdiff.py HEAD~5 HEAD~2 -- </path/to/arm9.yml> </path/to/overlay29.yml>
python3 symdiff.py -sdv <tag name, commit hash, or branch name>
"""

import argparse
import collections
import difflib
from io import StringIO
from pathlib import Path
import subprocess
import sys
from typing import (
    cast,
    Deque,
    Dict,
    Iterable,
    List,
    Optional,
    Set,
    TextIO,
    Tuple,
    Union,
)
import yaml

import ansi

REPO_ROOT: Path = Path(__file__).resolve().parent.parent
SYMBOL_DIR: Path = REPO_ROOT / "symbols"


def git_cmd(args: List[str]) -> str:
    """
    Run a git command with non-interactive output from the repository root, and
    return the output as a string.

    Args:
        args (List[str]): subprocess-style argument list for the git command

    Returns:
        str: output of the git command
    """
    return subprocess.run(
        ["git", "-C", str(REPO_ROOT), "--no-pager"] + args,
        capture_output=True,
        check=True,
    ).stdout.decode()


def revision_str(revision: Optional[str]) -> str:
    """
    Returns the given revision, or a special value for None, which corresponds
    to the current working tree.

    Args:
        revision (Optional[str]): git revision

    Returns:
        str: string representing the git revision
    """
    return "[current]" if revision is None else revision


def ensure_revision_exists(revision: str):
    """Make sure the given revision exists.
    See https://stackoverflow.com/questions/18515488/how-to-check-if-the-commit-exists-in-a-git-repository-by-its-sha-1

    Args:
        revision (str): git revision

    Raises:
        ValueError: revision does not exist
    """
    try:
        git_cmd(["cat-file", "-e", f"{revision}^{{commit}}"])
    except subprocess.CalledProcessError as e:
        # Invalid revision
        raise ValueError(e.stderr.decode())


def open_file_at_revision(path: Path, revision: Optional[str]) -> TextIO:
    """Read a file from the repository as it was at the given revision.

    Args:
        path (Path): path to file
        revision (Optional[str]): git revision, or None for the working tree

    Raises:
        ValueError: file path is outside of the git repository
        FileNotFoundError: file path does not exist for the given revision

    Returns:
        TextIO: text stream for the given file
    """
    # Git requires forward slashes, even on Windows.
    # Resolve paths to be relative to the repo root to make things easier.
    try:
        path_from_root: str = path.resolve().relative_to(REPO_ROOT).as_posix()
    except ValueError:
        raise ValueError(f"'{path}' is outside of git repository")

    if revision is None:
        return path.open("r")

    ensure_revision_exists(revision)
    try:
        return StringIO(git_cmd(["show", f"{revision}:{path_from_root}"]))
    except subprocess.CalledProcessError as e:
        # Since we already made sure the revision exists, assume a
        # CalledProcessError means the file doesn't exist in the revision
        raise FileNotFoundError(e.stderr.decode())


class SymbolPath:
    """
    A fully qualified symbol identifier with a file path, block, and name.
    """

    def __init__(self, name: str, filepath: Path, blockname: str):
        self.name = name
        self.file = filepath
        self.blockname = blockname

    def location(self) -> str:
        return f"{self.file}::{self.blockname}"

    def __repr__(self) -> str:
        return f"{self.name} ({self.location()})"


class SymbolDiff:
    """Difference between two symbols"""

    def __init__(
        self,
        path: SymbolPath,
        *,
        new_path: Optional[SymbolPath] = None,
        added_aliases: Optional[List[str]] = None,
        deleted_aliases: Optional[List[str]] = None,
        added_addrs: Optional[List[Tuple[str, int]]] = None,
        deleted_addrs: Optional[List[Tuple[str, int]]] = None,
        modified_addrs: Optional[List[Tuple[str, int, int]]] = None,
        added_lens: Optional[List[Tuple[str, int]]] = None,
        deleted_lens: Optional[List[Tuple[str, int]]] = None,
        modified_lens: Optional[List[Tuple[str, int, int]]] = None,
        description_diff: Optional[Tuple[str, Iterable[str]]] = None,
    ):
        self.path = path
        self.new_path = new_path
        self.added_aliases = added_aliases if added_aliases is not None else []
        self.deleted_aliases = deleted_aliases if deleted_aliases is not None else []
        self.added_addrs = added_addrs if added_addrs is not None else []
        self.deleted_addrs = deleted_addrs if deleted_addrs is not None else []
        self.modified_addrs = modified_addrs if modified_addrs is not None else []
        self.added_lens = added_lens if added_lens is not None else []
        self.deleted_lens = deleted_lens if deleted_lens is not None else []
        self.modified_lens = modified_lens if modified_lens is not None else []
        # This is in the format of (type, lines), where type is "+" for an
        # addition, "-" for a deletion, or something else for a modification.
        # Lines is a displayable list of lines in the diff
        self.description_diff = description_diff

    def __str__(self) -> str:
        if self.new_path is not None:
            name_matches = self.new_path.name == self.path.name
            loc_matches = self.new_path.location() == self.path.location()
            if not name_matches and loc_matches:
                return (
                    f"{self.path.name} => {self.new_path.name}"
                    + f" ({self.path.location()})"
                )
            elif name_matches and not loc_matches:
                return (
                    f"{self.path.name} ({self.path.location()}"
                    + f" => {self.new_path.location()})"
                )
            elif not name_matches and not loc_matches:
                return f"{self.path} => {self.new_path}"
        return str(self.path)

    def is_content_modification(self) -> bool:
        return bool(
            self.added_aliases
            or self.deleted_aliases
            or self.added_addrs
            or self.deleted_addrs
            or self.modified_addrs
            or self.added_lens
            or self.deleted_lens
            or self.modified_lens
            or self.description_diff
        )

    def is_nonempty(self, relocation: bool = False) -> bool:
        if self.new_path is not None:
            if self.new_path.name != self.path.name:
                return True
            # Only consider a difference in location to be nonempty if the
            # relocation flag is specified
            elif relocation and self.new_path.location() != self.path.location():
                return True
        return self.is_content_modification()

    def details(self, indent_level: int = 0, indent_width: int = 4):
        def detail_print(msg: str, indent: int = 0, code: ansi.Code = ansi.BLUE):
            ansi.print(code, f"{' ' * indent_width * (indent_level + indent)}{msg}")

        def format_hex(i: int) -> str:
            return f"{'-' if i < 0 else ''}0x{abs(i):X}"

        def versioned_int(v: Tuple[str, int]) -> str:
            return f"[{v[0]}] {format_hex(v[1])}"

        def versioned_int_diff(v: Tuple[str, int, int]) -> str:
            return f"[{v[0]}] {format_hex(v[1])} => {format_hex(v[2])}"

        if self.is_content_modification():
            desc_added = 0
            desc_deleted = 0
            desc_modified = 0
            if self.description_diff:
                if self.description_diff[0] == "+":
                    desc_added = 1
                elif self.description_diff[0] == "-":
                    desc_deleted = 1
                else:
                    desc_modified = 1
            addition_count = (
                len(self.added_aliases)
                + len(self.added_addrs)
                + len(self.added_lens)
                + desc_added
            )
            deletion_count = (
                len(self.deleted_aliases)
                + len(self.deleted_addrs)
                + len(self.deleted_lens)
                + desc_deleted
            )
            modification_count = (
                len(self.modified_addrs) + len(self.modified_lens) + desc_modified
            )
            detail_print(
                f"{addition_count} addition(s),"
                + f" {deletion_count} deletion(s),"
                + f" {modification_count} modification(s)",
                code=ansi.BOLD + ansi.BLUE,
            )
            for alias in self.added_aliases:
                detail_print(f"+ alias: {alias}")
            for alias in self.deleted_aliases:
                detail_print(f"- alias: {alias}")
            for addr in self.added_addrs:
                detail_print(f"+ address: {versioned_int(addr)}")
            for addr in self.deleted_addrs:
                detail_print(f"- address: {versioned_int(addr)}")
            for addr_diff in self.modified_addrs:
                detail_print(f"* address: {versioned_int_diff(addr_diff)}")
            for length in self.added_lens:
                detail_print(f"+ length: {versioned_int(length)}")
            for length in self.deleted_lens:
                detail_print(f"- length: {versioned_int(length)}")
            for len_diff in self.modified_lens:
                detail_print(f"* length: {versioned_int_diff(len_diff)}")
            if self.description_diff:
                # Use the "diff type" as the prefix
                detail_print(f"{self.description_diff[0] or '*'} description:")
                for line in self.description_diff[1]:
                    detail_print(line, indent=1, code=ansi.MAGENTA)


class SymbolTableDiff:
    """Difference between two collections of symbols"""

    def __init__(
        self,
        added: Optional[List[SymbolPath]] = None,
        deleted: Optional[List[SymbolPath]] = None,
        modified: Optional[List[SymbolDiff]] = None,
    ):
        self.added = added if added is not None else []
        self.deleted = deleted if deleted is not None else []
        self.modified = modified if modified is not None else []

    def __bool__(self) -> bool:
        return bool(self.added or self.deleted or self.modified)

    def __iadd__(self, other: "SymbolTableDiff"):
        self.added += other.added
        self.deleted += other.deleted
        self.modified += other.modified
        return self

    def __add__(self, other: "SymbolTableDiff") -> "SymbolTableDiff":
        new = SymbolTableDiff(self.added, self.deleted, self.modified)
        new += other
        return new

    def summary(self, verbose: bool = False):
        ansi.print(
            ansi.BOLD,
            f"{len(self.added)} addition(s), {len(self.deleted)} deletion(s),"
            + f" {len(self.modified)} modification(s)",
        )
        for added in self.added:
            ansi.print(ansi.GREEN, f"+ {added}")
        for deleted in self.deleted:
            ansi.print(ansi.RED, f"- {deleted}")
        for modified in self.modified:
            ansi.print(ansi.CYAN, f"* {modified}")
            if verbose:
                modified.details(indent_level=1)


class Symbol:
    """A symbol from the pmdsky-debug symbol tables"""

    def __init__(
        self,
        filepath: Path,
        blockname: str,
        symbol: Dict,
        *,
        include_description: bool = False,
    ):
        self.file = filepath
        self.blockname = blockname
        self.name: str = symbol["name"]
        self.aliases: List[str] = symbol.get("aliases", [])
        self.address: Dict[str, List[int]] = {
            version: Symbol.listify(addrs)
            for version, addrs in symbol["address"].items()
        }
        self.length: Dict[str, int] = symbol.get("length", {})
        self.description: Optional[str] = (
            symbol.get("description") if include_description else None
        )

    @staticmethod
    def listify(maybe_list: Union[List[int], int]) -> List[int]:
        return maybe_list if isinstance(maybe_list, list) else [maybe_list]

    def path(self) -> SymbolPath:
        return SymbolPath(self.name, self.file, self.blockname)

    def diff(self, base: "Symbol") -> SymbolDiff:
        """Get the difference between this symbol and a base symbol.

        Args:
            base (Symbol): symbol on which to base the difference

        Returns:
            SymbolDiff: difference between base and self
        """
        path = base.path()
        new_path = None
        if (
            self.file != base.file
            or self.blockname != base.blockname
            or self.name != base.name
        ):
            new_path = self.path()

        def ordered_diff(a: List, b: List) -> Tuple[List, List]:
            """
            Find elements that are only in list a and only in list b. The
            ordering in the output lists matches that of the input lists.
            """
            a_set = set(a)
            b_set = set(b)
            a_idx_map = {(v, i) for i, v in enumerate(a)}
            b_idx_map = {(v, i) for i, v in enumerate(b)}
            return (
                sorted(a_set - b_set, key=lambda v: a_idx_map),
                sorted(b_set - a_set, key=lambda v: b_idx_map),
            )

        def diff_by_vers(
            a: Dict, b: Dict
        ) -> Tuple[
            List[Tuple[str, int]], List[Tuple[str, int]], List[Tuple[str, int, int]]
        ]:
            """
            Find elements that are only in versioned collection a and versioned
            collection b. Return a list of (version, value) for additions in a,
            deletions from b, and a list of (version, value, value) for
            modifications from b to a.
            """
            added: List[Tuple[str, int]] = []
            deleted: List[Tuple[str, int]] = []
            modified: List[Tuple[str, int, int]] = []
            # Loop explicitly rather than operating on dict key sets to
            # preserve order (at least, in Python 3.7+ where dicts are ordered)
            for vers in a:
                if vers not in b:
                    added += [(vers, val) for val in Symbol.listify(a[vers])]
                    continue

                diff_a_b, diff_b_a = ordered_diff(
                    Symbol.listify(a[vers]), Symbol.listify(b[vers])
                )
                if len(diff_a_b) == 1 and len(diff_b_a) == 1:
                    # If there's exactly one addition and one deletion, we can
                    # unambiguously treat it as a modification.
                    modified.append((vers, diff_b_a[0], diff_a_b[0]))
                else:
                    added += [(vers, val) for val in diff_a_b]
                    deleted += [(vers, val) for val in diff_b_a]
            for vers in b:
                if vers not in a:
                    deleted += [(vers, val) for val in Symbol.listify(b[vers])]

            return added, deleted, modified

        added_aliases = []
        deleted_aliases = []
        if self.aliases != base.aliases:
            # We only expect a small number of aliases, so linear list search
            # is probably faster than using a set
            for alias in self.aliases:
                if alias not in base.aliases:
                    # addition
                    added_aliases.append(alias)
            for alias in base.aliases:
                if alias not in self.aliases:
                    # deletion
                    deleted_aliases.append(alias)

        added_addrs, deleted_addrs, modified_addrs = diff_by_vers(
            self.address, base.address
        )
        added_lens, deleted_lens, modified_lens = diff_by_vers(self.length, base.length)

        description_diff = None
        if self.description != base.description:
            if base.description is None:
                # addition
                description_diff = ("+", self.description.split("\n"))
            elif self.description is None:
                # deletion
                description_diff = ("-", base.description.split("\n"))
            else:
                # modification
                description_diff = (
                    "",
                    difflib.unified_diff(
                        base.description.split("\n"),
                        self.description.split("\n"),
                        fromfile=f"[old] {base.name}",
                        tofile=f"[new] {self.name}",
                        lineterm="",
                    ),
                )

        return SymbolDiff(
            path,
            new_path=new_path,
            added_aliases=added_aliases,
            deleted_aliases=deleted_aliases,
            added_addrs=added_addrs,
            deleted_addrs=deleted_addrs,
            modified_addrs=modified_addrs,
            added_lens=added_lens,
            deleted_lens=deleted_lens,
            modified_lens=modified_lens,
            description_diff=description_diff,
        )


class MatchRank:
    """
    Represents the quality of the match between two symbols. Functions as a
    tuple (# matching addresses, whether or not the names match).
    """

    def __init__(self, n_matches: int = 0, matching_name: bool = False):
        self.n_matches = n_matches
        self.matching_name = matching_name

    def __repr__(self) -> str:
        return repr(self.key())

    def key(self) -> Tuple[int, bool]:
        return (self.n_matches, self.matching_name)


class SymbolList(list):
    """List of symbols from the pmdsky-debug symbol tables"""

    def __init__(self, symbols: List[Symbol] = []):
        super().__init__(symbols)

    def _build_match_graph(self, base: "SymbolList") -> List[Dict[int, List[int]]]:
        """
        Find all candidate matches between symbols in self and symbols in base.
        This is a graph where the symbols are nodes and the matches are edges.

        First, the symbols in base are indexed by (version, address) and by
        name. Then, the indexes are queried for each (version, address) and
        name of each symbol in self. Candidate matches are drawn when the query
        returns a valid result, and the number and kind of valid query results
        are tallied to assign each candidate match with a "match rank", which
        measures the quality of the match. The match rank for a symbol pair is
        defined as the tuple (# matching addresses, whether the names match).

        The collection of candidate matches are then grouped by match rank,
        and the groups are returned in descending order of rank.

        Args:
            base (SymbolList): base symbol list with which to find matches

        Returns:
            List[Dict[int, List[int]]]: list of candidate match groups, in
                descending order of match rank. Each dictionary element
                corresponds to edges within a match rank group, and maps
                {self index -> list of matching base indexes} for one or more
                self indexes.
        """
        # Build lookup tables by address/name. The values need to be lists
        # since addresses and names can technically be duplicated (although
        # they shouldn't be)
        base_addr_to_idx: Dict[Tuple[str, int], List[int]] = {}
        base_name_to_idx: Dict[str, List[int]] = {}
        for i, s in enumerate(cast(List[Symbol], base)):
            for version, addrs in s.address.items():
                for addr in addrs:
                    base_addr_to_idx.setdefault((version, addr), []).append(i)
            base_name_to_idx.setdefault(s.name, []).append(i)

        # Matches in base for each element in self, alongside a "match rank"
        matches: List[Dict[int, MatchRank]] = [{} for _ in range(len(self))]
        for s, match_list in zip(cast(List[Symbol], self), matches):
            for version, addrs in s.address.items():
                for addr in addrs:
                    for i_base in base_addr_to_idx.get((version, addr), []):
                        match_list.setdefault(i_base, MatchRank()).n_matches += 1
            for i_base in base_name_to_idx.get(s.name, []):
                match_list.setdefault(i_base, MatchRank()).matching_name = True

        # Aggregate the match list entries by match rank
        matches_by_rank: Dict[Tuple[int, bool], Dict[int, List[int]]] = {}
        for i, mlist in enumerate(matches):
            for m, rank in mlist.items():
                matches_by_rank.setdefault(
                    rank.key(), collections.OrderedDict()
                ).setdefault(i, []).append(m)
        # Sort descending by rank, then throw out the ranks since we no longer
        # need them.
        return [
            match
            for _, match in sorted(
                matches_by_rank.items(), key=lambda x: x[0], reverse=True
            )
        ]

    @staticmethod
    def _maximum_bipartite_matching(
        edges: Dict[int, List[int]]
    ) -> List[Tuple[int, int]]:
        """
        Computes a maximum cardinality matching for a bipartite graph using
        the Ford-Fulkerson algorithm with breadth-first search.

        See: https://en.wikipedia.org/wiki/Maximum_cardinality_matching

        Args:
            edges (Dict[int, List[int]]): Graph edges, as described by the
                one-sided mapping of {left node -> right node neighbors}. In
                other words, this is a one-sided (left -> right), sparsely
                encoded adjacency matrix. Neighbor lists are assumed to be
                nonempty.

        Returns:
            List[Tuple[int, int]]: node pairs in a maximum cardinality matching
                in the form (left node, right node)
        """
        # The edges that are currently assigned to the matching, keyed by the
        # "right" nodes in the bipartite graph for quick lookup. Note that
        # while the algorithm deals with directed edges, the actual matching
        # has undirected edges
        matching_reverse_edges: Dict[int, int] = {}
        for i in edges:
            # Hard-coded fast path; edges[i] will always have at least one
            # element, and  in the vast majority of cases there should be no
            # conflicts
            if edges[i][0] not in matching_reverse_edges:
                matching_reverse_edges[edges[i][0]] = i
                continue

            def update_matching(reverse_edges: Dict[int, int]):
                """
                Search for paths using breadth-first search, and update the
                matching in reverse_edges if a valid path is found.
                """

                # The "left-to-right" edges in BFS's current search paths, but
                # keyed in reverse (by the right nodes in the bipartite graph).
                # This dict serves a double purpose, allowing us to keep track
                # of which nodes have been visited and also keep track of the
                # path itself, so that we can later update reverse_edges.
                path_right_parents: Dict[int, int] = {}
                # The "right-to-left" edges in BFS's current search paths, but
                # keyed in reverse (by the left nodes in the bipartite graph).
                # This dict is only needed to recover the full path once BFS
                # succeeds.
                path_left_parents: Dict[int, int] = {}
                start_node_queue: Deque[int] = collections.deque([i])
                while start_node_queue:
                    start = start_node_queue.popleft()
                    for end in edges[start]:
                        if end in path_right_parents:
                            # Already visited this node; skip it
                            continue
                        if end not in reverse_edges:
                            # Node is free; Trace back along this path, update
                            # the matching, and return
                            reverse_edges[end] = start
                            while start in path_left_parents:
                                end = path_left_parents[start]
                                # This should always exist
                                start = path_right_parents[end]
                                reverse_edges[end] = start
                            return
                        # This node already has an edge in the matching.
                        # Traverse the edge from left to right and record the
                        # reverse edge for later, then backtrack along the old
                        # edge from right to left.
                        path_right_parents[end] = start
                        path_left_parents[reverse_edges[end]] = end
                        start_node_queue.append(reverse_edges[end])
                # There's nothing to pair node i with in the matching

            # Try to make room for node i in the matching by shuffling edges
            # around
            update_matching(matching_reverse_edges)
        # Since we've been storing the matching edges in reverse, we need to
        # flip the order in the output
        return [(i, j) for j, i in matching_reverse_edges.items()]

    def locate_pairs(
        self, base: "SymbolList"
    ) -> Tuple[List[Tuple[int, int]], List[int], List[int]]:
        """Infer pairs of corresponding symbols between self and base.

        First, all candidate matches in base are found for every symbol in
        self. This forms a bipartite graph, with the symbols of self/base as
        the nodes and potential matches between them as the edges.

        Once the graph has been constructed, pairing is done over multiple
        passes. Edges are grouped by "match rank", which is a measure of how
        well the incident nodes match each other. Symbol pairs (edges) with the
        highest match rank are paired up first, and edges of lower and lower
        match rank are sequentially selected until no more symbols can be
        paired up. Among candidate edges with the same rank (which may or may
        not be in conflict), a non-conflicting subset with the maximum possible
        number of edges (a maximum cardinality matching) is selected.

        Args:
            base (SymbolList): base symbol list being compared against

        Returns:
            List[Tuple[int, int]]: list of successfully paired symbols, as
                indexes into (self, base)
            List[int]: list of unpaired symbols in self (as indexes)
            List[int]: list of unpaired symbols in base (as indexes)
        """

        # All the candidate matches, grouped in descending order of match rank
        matches = self._build_match_graph(base)
        # The selected matches in the form {self index -> base index}
        self_to_base_idx: Dict[int, int] = {}
        # This set is effectively set(self_to_base_idx.values()), but we need
        # it for quick lookups on the base index
        paired_base_idxs: Set[int] = set()
        # Do a pass for each match rank group
        for edges in matches:
            # Filter out already indexes that we've already paired; higher
            # match ranks always win conflicts
            edges = collections.OrderedDict(
                (
                    i_self,
                    [i_base for i_base in base_idxs if i_base not in paired_base_idxs],
                )
                for i_self, base_idxs in edges.items()
                if i_self not in self_to_base_idx
                # Make sure no keys correspond to an empty list
                and not (set(base_idxs) <= paired_base_idxs)
            )
            # Assign pairs from matches among the same rank by computing a
            # maximum cardinality matching
            pairs = SymbolList._maximum_bipartite_matching(edges)
            self_to_base_idx.update(pairs)
            paired_base_idxs.update(p[1] for p in pairs)

        # Pairing is complete; determine which indexes are left unpaired
        unpaired_self_idxs = [i for i in range(len(self)) if i not in self_to_base_idx]
        unpaired_base_idxs = [i for i in range(len(base)) if i not in paired_base_idxs]
        return sorted(self_to_base_idx.items()), unpaired_self_idxs, unpaired_base_idxs

    def diff(self, base: "SymbolList", relocation: bool = False) -> SymbolTableDiff:
        """Get the difference between this symbol list and a base symbol list.

        The diff algorithm works as follows:
        1. Infer which symbols in self correspond to symbols in base.
        2. For each pair of corresponding symbols, perform a symbol diff.
        3. Unpaired symbols in self are counted as additions.
        4. Unpaired symbols in base are counted as deletions.

        Args:
            base (SymbolList): symbol list on which to base the difference
            relocation (bool, optional): whether or not to count symbol
                relocations as "modifications" in the diff. Defaults to False.

        Returns:
            SymbolTableDiff: difference between base and self
        """
        pairs, unpaired_self_idxs, unpaired_base_idxs = self.locate_pairs(base)
        modified: List[SymbolDiff] = [self[i].diff(base[j]) for i, j in pairs]
        modified = [d for d in modified if d.is_nonempty(relocation)]
        return SymbolTableDiff(
            added=[self[i].path() for i in unpaired_self_idxs],
            deleted=[base[i].path() for i in unpaired_base_idxs],
            modified=modified,
        )


class SymbolBlock:
    """
    Block of symbols (with function and data lists) from the pmdsky-debug
    symbol tables
    """

    def __init__(self, functions: SymbolList, data: SymbolList):
        self.functions = functions
        self.data = data

    def symbol_paths(self) -> List[SymbolPath]:
        """Get the symbol paths for each symbol in the block.

        Returns:
            List[SymbolPath]: list of all symbol paths
        """
        return [s.path() for s in self.functions + self.data]

    def diff(self, base: "SymbolBlock", relocation: bool = False) -> SymbolTableDiff:
        """Get the difference between this block and a base block.

        Args:
            base (SymbolBlock): block on which to base the difference
            relocation (bool, optional): whether or not to count symbol
                relocations as "modifications" in the diff. Defaults to False.

        Returns:
            SymbolTableDiff: difference between base and self
        """
        return self.functions.diff(base.functions, relocation) + self.data.diff(
            base.data, relocation
        )


class SymbolTable:
    """
    pmdsky-debug symbol table, including the collapsed contents of any
    nested subregions
    """

    def __init__(
        self, path: Path, *, revision: Optional[str] = None, descriptions: bool = False
    ):
        """
        Load a symbol table at the given path and revision, and its associated
        subregions.

        Args:
            path (Path): Path to symbol table file. Must be within the
                pmdsky-debug repository.
            revision (Optional[str], optional): Revision, or None for the
                working tree. Defaults to None.
            descriptions (bool, optional): Whether or not to load symbol
                descriptions. Defaults to False.
        """
        self.blocks: Dict[str, SymbolBlock] = {}
        try:
            with open_file_at_revision(path, revision) as f:
                contents = yaml.safe_load(f)
            self.valid = True
        except FileNotFoundError:
            # This file doesn't exist in the given revision; mark it as invalid
            # and leave the block list empty
            self.valid = False
            return

        for bname, block in contents.items():
            functions: List[Symbol] = []
            data: List[Symbol] = []
            subregions: List[Path] = []

            def process_subregion(sub_path: Path, sub_contents: Dict):
                """
                Process the given contents of a subregion at the given path,
                and accumulate into the overall symbol containers
                """
                nonlocal functions, data, subregions
                for sub_bname, sub_block in sub_contents.items():
                    # Store symbols with the path relative to the symbol
                    # directory to make output look nicer
                    sub_path_rel: Path = sub_path.resolve().relative_to(SYMBOL_DIR)
                    functions += [
                        Symbol(
                            sub_path_rel, sub_bname, s, include_description=descriptions
                        )
                        for s in sub_block["functions"]
                    ]
                    data += [
                        Symbol(
                            sub_path_rel, sub_bname, s, include_description=descriptions
                        )
                        for s in sub_block["data"]
                    ]
                    # Reversed to preserve the proper ordering in the
                    # stack-based depth-first subregion traversal
                    subregions += reversed(
                        [
                            sub_path.parents[0] / Path(sub_path.stem) / Path(sub)
                            for sub in sub_block.get("subregions", [])
                        ]
                    )

            # Collapse subregions for each block with a depth-first traversal
            process_subregion(path, {bname: block})
            while subregions:
                sub_path = subregions.pop()
                try:
                    with open_file_at_revision(sub_path, revision) as f:
                        sub_contents = yaml.safe_load(f)
                except FileNotFoundError:
                    continue
                process_subregion(sub_path, sub_contents)

            self.blocks[bname] = SymbolBlock(
                functions=SymbolList(functions), data=SymbolList(data)
            )

    def diff(
        self, base: "SymbolTable", subregion_resolution: bool = False
    ) -> SymbolTableDiff:
        """Get the difference between this table and a base table.

        Args:
            base (SymbolTable): table on which to base the difference
            subregion_resolution (bool, optional): whether or not to count
                a symbols moving between subregions within the same top-level
                block as a "modification" in the diff. Defaults to False.

        Returns:
            SymbolTableDiff: difference between base and self
        """
        symdiff = SymbolTableDiff()
        # Loop explicitly rather than operating on dict key sets to preserve
        # order (at least, in Python 3.7+ where dicts are ordered)
        for k in self.blocks:
            if k not in base.blocks:
                symdiff.added += self.blocks[k].symbol_paths()
                continue
            symdiff += self.blocks[k].diff(base.blocks[k], subregion_resolution)
        for k in base.blocks:
            if k not in self.blocks:
                symdiff.deleted += base.blocks[k].symbol_paths()

        return symdiff


def print_symbol_diff(
    path: Path,
    base: str,
    target: Optional[str],
    *,
    verbose: bool = False,
    subregion_resolution: bool = False,
    descriptions: bool = False,
    preceding_newline: bool = False,
) -> bool:
    """Print a diff for the given symbol table file between two revisions.

    Args:
        path (Path): path to symbol table file. Must be within the pmdsky-debug
            repository
        base (str): base revision
        target (Optional[str]): target revision
        verbose (bool, optional): whether or not to print symbol diff details
            as part of the diff. Defaults to False.
        subregion_resolution (bool, optional): whether or not to count a symbol
            moving between subregions within the same top-level block as a
            "modification" in the diff. Defaults to False.
        descriptions (bool, optional): whether or not to count symbol
            description changes in the diff. Defaults to False.
        preceding_newline (bool, optional): whether to print a newline before
            a nonempty symbol diff, for fenceposting. Defaults to False.

    Returns:
            bool: True if the symbol diff was nonempty
    """
    old_table = SymbolTable(path, revision=base, descriptions=descriptions)
    new_table = SymbolTable(path, revision=target, descriptions=descriptions)
    diff = new_table.diff(old_table, subregion_resolution)
    if diff and (old_table.valid or new_table.valid):
        if preceding_newline:
            print()

        prefix = ""
        if not old_table.valid:
            prefix = "[added] "
        elif not new_table.valid:
            prefix = "[deleted] "
        # Print the path relative to the repo root for neatness
        ansi.print(
            ansi.BOLD,
            f"=== {prefix}{path.resolve().relative_to(REPO_ROOT)}:"
            + f" {revision_str(base)}..{revision_str(target)} ===",
        )
        diff.summary(verbose)
        return True
    return False


def get_modified_paths(base: str, target: Optional[str]) -> List[Path]:
    """
    Get the paths of all modified symbol table files in pmdsky-debug between a
    base and target revision.

    Args:
        base (str): base revision
        target (Optional[str]): target revision, or None for the working tree

    Returns:
        List[Path]: list of modified file paths between base and target
    """

    def get_top_level_tables(path_strs: List[str]) -> Set[Path]:
        """
        Given a list of file paths within the symbols directory, return a set
        of paths for the corresponding top-level symbol tables.

        Assumes paths are relative to the repository root (as is the case for
        git command output). Returned paths are relative to the symbols
        directory.
        """
        table_paths: Set[Path] = set()
        for p in path_strs:
            path = Path(p)
            if path.suffix != ".yml":
                continue
            try:
                # Convert subregion file paths into the file paths for the
                # top-level tables they belong to
                table_paths.add(
                    Path(path.relative_to("symbols").parts[0]).with_suffix(".yml")
                )
            except ValueError:
                # Something weird happened; skip this path
                continue
        return table_paths

    tables: Set[Path] = set()

    # Run `git diff` to check which files have been modified
    git_args = ["diff", "--name-only", base]
    if target is not None:
        git_args.append(target)
    else:
        # None as the target means the working tree. This includes untracked
        # files that won't be detected by `git diff`, so we need to do run a
        # separate `git ls-files` command to check for untracked files.
        untracked: List[str] = (
            git_cmd(["ls-files", "symbols", "--exclude-standard", "--others"])
            .strip()
            .split("\n")
        )
        tables |= get_top_level_tables(untracked)

    try:
        diff_list: List[str] = git_cmd(git_args).strip().split("\n")
        tables |= get_top_level_tables(diff_list)
    except subprocess.CalledProcessError as e:
        print(e.stderr.decode(), file=sys.stderr)
        raise

    # Convert symbol table names into absolute file paths before returning
    return sorted(SYMBOL_DIR / t for t in tables)


def symdiff_parse_args(parser: argparse.ArgumentParser) -> argparse.Namespace:
    """Like parser.parse_args(), but with added processing similar to git.

    This appends the following optional arguments to the end of the parser:
      [base] [target] [--] [path ...]
    With appropriate help text, processing, and validation for symbol table
    diffing.
    """
    parser.add_argument(
        "base", nargs="?", default="HEAD", help="base revision against which to compare"
    )
    parser.add_argument(
        "target",
        nargs="?",
        help="target revision against which to compare (defaults to working directory)",
    )
    parser.add_argument(
        "path",
        nargs="*",
        type=Path,
        help="path to symbol table file (top-level, excluding subregions)",
    )
    # Insert an optional "--" before the "path" arguments
    parser.usage = (
        parser.format_usage()
        .replace("usage: ", "", 1)
        .replace("[path", "[--] [path", 1)
    )
    try:
        # Manually handle anything after a "--"; argparse doesn't have a way to
        # handle the git-style argument pattern
        n = sys.argv.index("--")
        args = parser.parse_args(sys.argv[1:n])
        if args.path:
            # This should only be filled if the positional arguments before the
            # "--" spilled over into the path list
            parser.print_usage()
            raise SystemExit("fatal: too many revision arguments")
        args.path = [Path(s) for s in sys.argv[n + 1 :]]
    except ValueError:
        args = parser.parse_args()

    # git diff supports commit range syntax (a..b and a...b), so we should too.
    # But having args.base being a range would break other things, so
    # explicitly convert the commit range to a base and target revision.
    if args.target is None and ".." in args.base:
        sep = "..." if "..." in args.base else ".."
        args.base, args.target = args.base.split(sep, 1)

    # Ensure that any specified revisions exist up-front, so we can print nicer
    # error messages
    try:
        ensure_revision_exists(args.base)
    except ValueError:
        raise SystemExit(f"fatal: invalid revision {args.base}")
    if args.target is not None:
        try:
            ensure_revision_exists(args.target)
        except ValueError:
            raise SystemExit(f"fatal: invalid revision {args.target}")

    if not args.path:
        # Default to all modified symbol tables
        args.path = get_modified_paths(args.base, args.target)

    # Make sure all the paths are within the repository before diffing
    nonrepo_paths: List[Path] = []
    for path in args.path:
        try:
            path.resolve().relative_to(REPO_ROOT)
        except ValueError:
            nonrepo_paths.append(path)
    if nonrepo_paths:
        path_list_str = ", ".join(f"'{p}'" for p in nonrepo_paths)
        raise SystemExit(f"error: paths outside of git repository: {path_list_str}")

    return args


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Compare Git revisions of the pmdsky-debug symbol tables.",
    )
    parser.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        help="verbose output (with symbol modification details)",
    )
    parser.add_argument(
        "-s",
        "--subregion-resolution",
        action="store_true",
        help=(
            "count symbols moving around subregions within the same"
            + " top-level block as a modification"
        ),
    )
    parser.add_argument(
        "-d",
        "--descriptions",
        action="store_true",
        help="count symbol description changes as a modification",
    )
    args = symdiff_parse_args(parser)

    preceding_newline = False
    for path in args.path:
        preceding_newline |= print_symbol_diff(
            path,
            args.base,
            args.target,
            verbose=args.verbose,
            subregion_resolution=args.subregion_resolution,
            descriptions=args.descriptions,
            preceding_newline=preceding_newline,
        )
