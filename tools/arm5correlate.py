#!/usr/bin/env python3

"""
`arm5correlate.py` is a command line utility for correlating segments of
similar code across different ARMv5 binaries.

Correlation is done in 3 phases. In phase 1, each binary is broken into
discrete chunks of fixed size (the "chunk size"). Each chunk in the source file
is searched for (accounting for differences in offsets for bl, b, ldr, and str
instructions) in each target file, and all matches are recorded. In phase 2,
any adjacent or almost-adjacent matches (within a tolerance) within a given
target file are merged into a single larger chunk. In phase 3, chunks in the
source file are each mapped to the target file with the best (longest) match.

A correlation diagram can optionally be generated based on the final
(post-phase-3) computed correlations.

Example usage:

python3 arm5correlate.py \
    </path/to/combined.bin> \
    </path/to/bin1.bin> </path/to/bin2.bin>

python3 arm5correlate.py -vv -c 32 -t 512 -p correlations.png \
    </path/to/combined.bin> \
    </path/to/bin1.bin> </path/to/bin2.bin>
"""

import argparse
import bisect
import os
import re
from typing import Dict, List, Optional, Set, Tuple

import arm5find

Segments = List[arm5find.Segment]
# [segment][match index]
SegmentsMatches = List[List[arm5find.Segment]]
# [(source segment, target segment matches)]
SegmentCorrelations = List[Tuple[arm5find.Segment, Segments]]
# (tag, segments)
TaggedSegments = Tuple[int, Segments]
# [(source segment, tagged target segment matches)]
TaggedSegmentCorrelations = List[Tuple[arm5find.Segment, List[TaggedSegments]]]
# [(source segment, tag, target segment)]
TaggedSegmentCorrelation = Tuple[arm5find.Segment, int, arm5find.Segment]


def armv5_correlate(
    src_filename: str,
    target_filenames: List[str],
    *,
    src_offset: int,
    src_length: Optional[int],
    instruction_chunk_size: int = 4,
    strict_ldr_str_offset: bool = False,
    strict_b_offset: bool = False,
    verbosity: int = 0,
) -> Tuple[Segments, List[SegmentsMatches]]:
    """Matches instruction segments in the source file with segments in the
    target files, in the specified chunk size.

    Args:
        src_filename (str): source filename
        target_filenames (List[str]): target filenames
        src_offset (int): offset into source file to search
        src_length (Optional[int]): length of source file to search. Defaults
            to the whole file starting from src_offset.
        instruction_chunk_size (int, optional): number of instructions in a
            chunk. Defaults to 4.
        strict_ldr_str_offset (bool, optional): force all ldr/str offsets to
            match exactly. Defaults to False.
        strict_b_offset (bool, optional): force all b offsets to match exactly.
            Defaults to False.
        verbosity (int, optional): verbosity (0-4). Defaults to 0.

    Returns:
        Tuple[Segments, List[SegmentsMatches]]: Source segments, and matching
        target segments corresponding to each target file.
    """

    def report(message: str, level: int = 0, flush: bool = False):
        if verbosity >= level:
            print(message, flush=flush)

    def debug(message: str, level: int = 4):
        report(f"DEBUG: {message}", level, flush=True)

    chunk_len = instruction_chunk_size * arm5find.AsmSegment.INSTRUCTION_SIZE
    with open(src_filename, "rb") as f:
        src_contents = f.read()
    scan_end = len(src_contents)
    if src_offset > len(src_contents):
        raise ValueError(
            f"source offset {src_offset} exceeds size {len(src_contents)} of file '{src_filename}'"
        )
    if src_length is not None:
        if src_offset + src_length > len(src_contents):
            raise ValueError(
                f"source length {src_length} from offset {src_offset} exceeds size {len(src_contents)} of file '{src_filename}'"
            )
        scan_end = src_offset + src_length
    debug(f"Scanning source file from 0x{src_offset:x}..0x{scan_end:x}")

    src_segments = [
        arm5find.AsmSegment(i, min(i + chunk_len, scan_end) - i)
        for i in range(src_offset, scan_end, chunk_len)
    ]
    # List of matches by target file, by source segment
    search_results: List[List[List[arm5find.Segment]]] = [
        [[] for s in range(len(src_segments))] for t in range(len(target_filenames))
    ]
    search_regexes = [
        seg.regex(
            src_contents,
            ignore_ldr_str_offset=not strict_ldr_str_offset,
            ignore_b_offset=not strict_b_offset,
        )
        for seg in src_segments
    ]
    # Lots of segments might be duplicates, especially with small chunk sizes
    # Each target file gets its own independent result cache
    result_cache: Dict[str, Dict[re.Pattern, List[arm5find.Segment]]] = {}

    # The outer loop is over target files to search. Only load one at a time.
    for target_fname, file_matches in zip(target_filenames, search_results):
        with open(target_fname, "rb") as target_file:
            contents = target_file.read()
            file_result_cache = result_cache.setdefault(target_fname, {})

            # The inner loop is over search segments
            for seg, regex, seg_matches in zip(
                src_segments, search_regexes, file_matches
            ):
                if regex not in file_result_cache:
                    file_result_cache[regex] = [
                        arm5find.Segment(match.start(), match.end() - match.start())
                        for match in regex.finditer(contents)
                    ]
                seg_matches += file_result_cache[regex]
    return src_segments, search_results


def fill_source_holes(
    segments: List[Tuple[arm5find.Segment, List]], start: int, end: int
) -> List[arm5find.Segment]:
    """Fill in any uncovered source segments with empty target lists. Assumes
    segments is sorted by source offset. Mutates segments and returns a list of
    source holes filled."""
    holes: List[arm5find.Segment] = []
    hole_start = start
    for seg, _ in segments + [(arm5find.Segment(end, 1), [])]:
        hole_end = seg.offset
        hole_len = hole_end - hole_start
        if hole_len > 0:
            holes.append(arm5find.Segment(hole_start, hole_len))
        hole_start = seg.end()
    segments += [(hole, []) for hole in holes]
    segments.sort(key=lambda x: x[0])
    return holes


def armv5_merge_segment_correlations(
    src_segments: Segments,
    target_matches: SegmentsMatches,
    *,
    instruction_tolerance: int = 8,
    verbosity: int = 0,
) -> SegmentCorrelations:
    """Merge adjacent source segments, if they're correlated to adjacent
    (within the specified tolerance) target segments. In the event that some
    sequence of target matches works for a given source interval, but other
    matches don't, the target matches that don't work will be discarded.

    Args:
        src_segments (Segments): source segments
        target_matches (SegmentsMatches): matching segments in one target
        instruction_tolerance (int, optional): maximum number of instructions
            between "adjacent" target segments. Defaults to 8.
        verbosity (int, optional): verbosity (0-4). Defaults to 0.

    Returns:
        SegmentCorrelations: merged [(src_segment, target_matches)]. The output
        source segments will jointly cover the same offset range as all the
        input source segments.
    """

    def report(message: str, level: int = 0, flush: bool = False):
        if verbosity >= level:
            print(message, flush=flush)

    def debug(message: str, level: int = 4):
        report(f"DEBUG: {message}", level, flush=True)

    # Convert tolerance to bytes
    tol = instruction_tolerance * arm5find.AsmSegment.INSTRUCTION_SIZE

    # Build a DAG segment by segment linking adjacent target segments
    # Also keep track of the
    Node = Tuple[int, int]
    edges: Dict[Node, List[Node]] = {}
    downstream_nodes: Set[Node] = set()  # Nodes with parents
    start_nodes: List[Node] = []  # Nodes with no parents
    # For each segment
    for i, (src_segment, target_segment) in enumerate(
        zip(src_segments, target_matches)
    ):
        src_end = src_segment.end()
        # For each target match within a segment
        for j, match in enumerate(target_segment):
            node = (i, j)
            if node not in downstream_nodes:
                # If we haven't recorded this node as being downstream of a
                # previous node by this point, it must be a start node
                start_nodes.append(node)

            match_end = match.end()
            # Check downstream segments for the first adjacency
            for di, (later_src, later_target) in enumerate(
                zip(src_segments[i + 1 :], target_matches[i + 1 :])
            ):
                if later_src.offset - src_end > tol:
                    break
                # Check each target match within a downstream segment for adjacency
                adjacencies = [
                    (i + 1 + di, other_j)
                    # NOTE: assume later_target is sorted by offset
                    for other_j, later_match in enumerate(later_target)
                    if 0 <= later_match.offset - match_end <= tol
                ]
                if adjacencies:
                    # Found neighbors within this segment, so don't check
                    # further segments
                    edges[node] = adjacencies
                    downstream_nodes.update(adjacencies)
                    break
    debug(f"DAG: {edges}")
    debug(f"Start nodes: {start_nodes}")

    # Identify the longest path starting from each start node.
    # These are the merge candidates.
    # {(start, end, path len)}
    candidate_set: Set[Tuple[Node, Node, int]] = set()
    for start in start_nodes:
        start_match = target_matches[start[0]][start[1]]
        best_ends: List[Node] = [start]
        max_path_len = start_match.length
        end = start
        # NOTE: This assumes edges[i] is sorted by chunk offset
        # We only have to traverse the graph to the chunk with the highest
        # offset (and therefore the highest end, since all chunks are of equal
        # size), since we're only interested in a path getting to the highest
        # end possible, and a path can never backtrack to a lower offset
        while end in edges and edges[end]:
            end = edges[end][-1]
            i, j = end
            new_path_len = target_matches[i][j].end() - start_match.offset
            if new_path_len > max_path_len:
                max_path_len = new_path_len
                best_ends = [end]
            elif new_path_len == max_path_len:
                best_ends.append(end)
        candidate_set.update((start, end, max_path_len) for end in best_ends)

    # First pass: fill in source segments greedily, longest target paths first,
    # skipping any paths that conflict (overlapping source or target segments)

    class RangeSet:
        SegmentPair = Tuple[arm5find.Segment, arm5find.Segment]

        def __init__(self):
            self.src_covering: List[arm5find.Segment] = []
            self.target_covering: List[arm5find.Segment] = []

        # Returns (valid, is subset left, is subset right, insert point)
        @staticmethod
        def _check_covering(
            covering: List[arm5find.Segment], segment: arm5find.Segment
        ) -> Tuple[bool, bool, int]:
            insert_point = bisect.bisect_left(covering, segment)
            overlaps_right = (
                insert_point < len(covering)
                and segment.end() > covering[insert_point].offset
            )
            overlaps_left = (0 <= insert_point - 1 < len(covering)) and covering[
                insert_point - 1
            ].end() > segment.offset
            if overlaps_right or overlaps_left:
                subset_right = (
                    overlaps_right and segment.offset >= covering[insert_point].offset
                )
                subset_left = (
                    overlaps_left and covering[insert_point - 1].end() >= segment.end()
                )
                return False, subset_left, subset_right, insert_point
            return True, False, False, insert_point

        # Returns (added, merged left source neighbor, merged right source neighbor, right truncation)
        # If interp_keyframes is given, partial segments can be added, and
        # right truncation will contain the partial segment on the right NOT added
        def add(
            self,
            seg_pair: SegmentPair,
            interp_keyframes: Optional[List[Node]] = None,
        ) -> Tuple[
            Optional[SegmentPair],
            Optional[arm5find.Segment],
            Optional[arm5find.Segment],
            Optional[Tuple[SegmentPair, List[Node]]],
        ]:
            src_seg, target_seg = seg_pair
            (
                src_valid,
                src_subset_left,
                src_subset_right,
                src_insert_point,
            ) = RangeSet._check_covering(self.src_covering, src_seg)
            src_duplicate = (
                src_insert_point < len(self.src_covering)
                and src_seg == self.src_covering[src_insert_point]
            )
            (
                target_valid,
                target_subset_left,
                target_subset_right,
                target_insert_point,
            ) = RangeSet._check_covering(self.target_covering, target_seg)
            if (src_valid or src_duplicate) and target_valid:
                if not src_duplicate:
                    self.src_covering.insert(src_insert_point, src_seg)
                self.target_covering.insert(target_insert_point, target_seg)
                return (src_seg, target_seg), None, None, None

            is_subset_left = src_subset_left or target_subset_left
            is_subset_right = src_subset_right or target_subset_right
            is_subset = is_subset_left or is_subset_right
            if not is_subset and interp_keyframes is not None:
                # Try to add partial segments
                RangeSet._validate_keyframes(seg_pair, interp_keyframes)
                partial_add, merge_left, merge_right, rtrunc = self._add_partial(
                    interp_keyframes, src_insert_point, target_insert_point
                )
                if partial_add is not None:
                    return partial_add, merge_left, merge_right, rtrunc
            return (
                None,
                seg_pair[0] if is_subset_left else None,
                seg_pair[0] if is_subset_right else None,
                None,
            )

        @staticmethod
        def _validate_keyframes(seg_pair: SegmentPair, interp_keyframes: List[Node]):
            istart, jstart = interp_keyframes[0]
            src_start = src_segments[istart]
            target_start = target_matches[istart][jstart]
            assert seg_pair[0].offset == src_start.offset
            assert seg_pair[1].offset == target_start.offset
            iend, jend = interp_keyframes[-1]
            src_end = src_segments[iend]
            target_end = target_matches[iend][jend]
            assert seg_pair[0].end() == src_end.end()
            assert seg_pair[1].end() == target_end.end()

        # Returns (added, mergeable left source neighbor, mergeable right source neighbor, right truncation)
        def _add_partial(
            self,
            interp_keyframes: List[Node],
            src_insert_point: int,
            target_insert_point: int,
        ) -> Tuple[
            Optional[SegmentPair],
            Optional[arm5find.Segment],
            Optional[arm5find.Segment],
            Optional[Tuple[SegmentPair, List[Node]]],
        ]:
            # Truncate keyframes from the left
            left_src = (
                self.src_covering[src_insert_point - 1]
                if 0 <= src_insert_point - 1 < len(self.src_covering)
                else None
            )
            left_target = (
                self.target_covering[target_insert_point - 1]
                if 0 <= target_insert_point - 1 < len(self.target_covering)
                else None
            )
            for kstart, (istart, jstart) in enumerate(interp_keyframes):
                src_start = src_segments[istart]
                target_start = target_matches[istart][jstart]
                if (left_src is None or left_src.end() <= src_start.offset) and (
                    left_target is None or left_target.end() <= target_start.offset
                ):
                    break
            else:
                return None, None, None, None

            # Truncate keyframes from the right
            right_src = (
                self.src_covering[src_insert_point]
                if src_insert_point < len(self.src_covering)
                else None
            )
            right_target = (
                self.target_covering[target_insert_point]
                if target_insert_point < len(self.target_covering)
                else None
            )
            for kend, (iend, jend) in reversed(list(enumerate(interp_keyframes))):
                src_end = src_segments[iend]
                target_end = target_matches[iend][jend]
                if (right_src is None or src_end.end() <= right_src.offset) and (
                    right_target is None or target_end.end() <= right_target.offset
                ):
                    break
            else:
                return None, None, None, None

            partial_src_start = src_start.offset
            partial_target_start = target_start.offset
            partial_src_end = src_end.end()
            partial_target_end = target_end.end()

            # Add back chunks on the left between keyframes
            if kstart > 0:
                for istart in range(interp_keyframes[kstart - 1][0] + 1, istart + 1):
                    src_start = src_segments[istart]
                    if left_src is None or left_src.end() <= src_start.offset:
                        d_offset = partial_src_start - src_segments[istart].offset
                        partial_src_start = src_segments[istart].offset
                        partial_target_start -= d_offset
                        break
            # Add back chunks on the right between keyframes
            if kend + 1 < len(interp_keyframes):
                for iend in range(max(iend, istart), interp_keyframes[kend + 1][0]):
                    src_end = src_segments[iend]
                    if right_src is None or src_end.end() <= right_src.offset:
                        d_end = src_segments[iend].end() - partial_src_end
                        partial_src_end = src_segments[iend].end()
                        partial_target_end += d_end
                        break

            # Sanity check
            if not (
                (0 <= partial_src_start < partial_src_end)
                and (0 <= partial_target_start < partial_target_end)
            ):
                return None, None, None, None

            partial_src = arm5find.Segment(
                partial_src_start, partial_src_end - partial_src_start
            )
            partial_target = arm5find.Segment(
                partial_target_start, partial_target_end - partial_target_start
            )
            rtrunc_keyframes = interp_keyframes[kend + 1 :]
            if rtrunc_keyframes:
                rtrunc_istart, rtrunc_jstart = rtrunc_keyframes[0]
                rtrunc_src_start = src_segments[rtrunc_istart].offset
                rtrunc_target_start = target_matches[rtrunc_istart][
                    rtrunc_jstart
                ].offset
                rtrunc_iend, rtrunc_jend = rtrunc_keyframes[-1]
                rtrunc_src_end = src_segments[rtrunc_iend].end()
                rtrunc_target_end = target_matches[rtrunc_iend][rtrunc_jend].end()
                rtrunc = (
                    (
                        arm5find.Segment(
                            rtrunc_src_start, rtrunc_src_end - rtrunc_src_start
                        ),
                        arm5find.Segment(
                            rtrunc_target_start, rtrunc_target_end - rtrunc_target_start
                        ),
                    ),
                    rtrunc_keyframes,
                )
            else:
                rtrunc = None

            # Check if the new partial segments are mergeable with
            # neighboring entries in the RangeSet
            src_merge_left = None
            target_merge_left = None
            src_merge_right = None
            target_merge_right = None
            if (
                left_src is not None and 0 <= partial_src.offset - left_src.end() <= tol
            ) and (
                left_target is None
                or 0 <= partial_target.offset - left_target.end() <= tol
            ):
                src_merge_left = left_src
                partial_src = arm5find.Segment(
                    left_src.offset, partial_src.end() - left_src.offset
                )
                if left_target is not None:
                    target_merge_left = left_target
                    partial_target = arm5find.Segment(
                        left_target.offset,
                        partial_target.end() - left_target.offset,
                    )
            if (
                right_src is not None
                and 0 <= right_src.offset - partial_src.end() <= tol
            ) and (
                right_target is None
                or 0 <= right_target.offset - partial_target.end() <= tol
            ):
                src_merge_right = right_src
                partial_src = arm5find.Segment(
                    partial_src.offset, right_src.end() - partial_src.offset
                )
                if right_target is not None:
                    target_merge_right = right_target
                    partial_target = arm5find.Segment(
                        partial_target.offset,
                        right_target.end() - partial_target.offset,
                    )

            # Merge src/target within the RangeSet
            # src_covering must be kept in sync with what's returned to the
            # caller since it'll be relied on in future calls when deciding
            # future merges
            if src_merge_left is not None:
                self.src_covering[src_insert_point - 1] = partial_src
                if src_merge_right is not None:
                    del self.src_covering[src_insert_point]
                if target_merge_left is not None:
                    self.target_covering[target_insert_point - 1] = partial_target
                    if target_merge_right is not None:
                        del self.target_covering[target_insert_point]
                else:
                    self.target_covering.insert(target_insert_point, partial_target)
            elif src_merge_right is not None:
                self.src_covering[src_insert_point] = partial_src
                if target_merge_right is not None:
                    self.target_covering[target_insert_point] = partial_target
                else:
                    self.target_covering.insert(target_insert_point, partial_target)
            else:
                self.src_covering.insert(src_insert_point, partial_src)
                self.target_covering.insert(target_insert_point, partial_target)

            return (
                (partial_src, partial_target),
                src_merge_left,
                src_merge_right,
                rtrunc,
            )

    merge_candidates = sorted(candidate_set, key=lambda x: (-x[2], x[:2]))
    conflicting_merge_candidates: List[Tuple[Node, Node, int]] = []
    merged_correlations: Dict[arm5find.Segment, Segments] = {}
    target_covering = RangeSet()

    def candidate_to_segments(
        candidate: Tuple[Node, Node, int]
    ) -> Tuple[arm5find.Segment, arm5find.Segment]:
        (i_start, j_start), (i_end, j_end) = candidate[0], candidate[1]
        src_start, src_end = src_segments[i_start], src_segments[i_end]
        src_segment = arm5find.Segment(
            src_start.offset, src_end.end() - src_start.offset
        )
        target_start = target_matches[i_start][j_start]
        target_end = target_matches[i_end][j_end]
        target_segment = arm5find.Segment(
            target_start.offset, target_end.end() - target_start.offset
        )
        return (src_segment, target_segment)

    debug(f"Merge candidates: {merge_candidates}")
    for candidate in merge_candidates:
        src_segment, target_segment = candidate_to_segments(candidate)
        added, subset_left, subset_right, _ = target_covering.add(
            (src_segment, target_segment)
        )
        if added is not None:
            merged_correlations.setdefault(src_segment, []).append(target_segment)
        elif not subset_left and not subset_right:
            conflicting_merge_candidates.append(candidate)

    # Second pass: allow partial paths to fill in gaps left by the
    # non-conflicting full paths
    def find_node_path(candidate: Tuple[Node, Node, int]) -> List[Node]:
        if candidate[0] == candidate[1]:
            return [candidate[0]]

        ancestors: Dict[Node, Node] = {}
        end_nodes: List[Node] = [candidate[0]]
        while end_nodes:
            end = end_nodes.pop()
            # This will find the "leftmost" path that works, which may be
            # different from the "rightmost" path we took when identifying
            # candidates initially. The leftmost path is likely to be more
            # granular (more nodes) than the rightmost path, which is
            # preferable if we're going to split up the path
            for i, j in reversed(edges.get(end, [])):
                new_end = (i, j)
                if new_end == candidate[1]:
                    path = [new_end, end]
                    parent = end
                    while parent in ancestors:
                        path.append(ancestors[parent])
                        parent = ancestors[parent]
                    return list(reversed(path))
                if new_end in ancestors:
                    continue
                ancestors[new_end] = end
                end_nodes.append(new_end)
        raise RuntimeError(f"failed to find path for candidate {candidate}")

    debug(f"Conflicting merge candidates: {conflicting_merge_candidates}")
    resolved_adds: List[List[Tuple[arm5find.Segment, arm5find.Segment]]] = []
    for candidate in conflicting_merge_candidates:
        partial_add_done = False
        resolved_adds_candidate: List[Tuple[arm5find.Segment, arm5find.Segment]] = []
        seg_pair = candidate_to_segments(candidate)
        interp_keyframes = find_node_path(candidate)
        while not partial_add_done:
            added, src_merged_left, src_merged_right, rtrunc = target_covering.add(
                seg_pair, interp_keyframes
            )
            partial_add_done = True
            if added is not None:
                resolved_src, resolved_target = added
                # Even if src_merged_left/src_merged_right had multiple target
                # segments, the new merged segment will take precedence over all of
                # them since it'll be longer
                if src_merged_left is not None:
                    del merged_correlations[src_merged_left]
                if src_merged_right is not None:
                    del merged_correlations[src_merged_right]
                merged_correlations.setdefault(resolved_src, []).append(resolved_target)
                resolved_adds_candidate.append((resolved_src, resolved_target))

                # There might be multiple gaps that can be filled with the
                # conflicting merge candidate. Each partial add fills the
                # leftmost possible gap, so run another partial add with the
                # right-truncated part of the merge candidate, if it exists
                if rtrunc is not None:
                    partial_add_done = False
                    seg_pair, interp_keyframes = rtrunc
        resolved_adds.append(resolved_adds_candidate)

    debug(f"Conflict-resolved segment pairs: {resolved_adds}")

    # Third pass: fill in any uncovered source segments with empty target lists
    merged_list = sorted(merged_correlations.items(), key=lambda x: x[0])
    if src_segments:
        holes = fill_source_holes(
            merged_list, src_segments[0].offset, src_segments[-1].end()
        )
        debug(f"Merge holes: {holes}")

    return merged_list


def armv5_select_segment_correlations(
    correlations: List[SegmentCorrelations],
    *,
    verbosity: int = 0,
) -> Tuple[TaggedSegmentCorrelations, Set[TaggedSegmentCorrelation]]:
    """Select the best source-target segment pairs with non-overlapping source
    segments, given a list of independent source-target segment pairs. "Best"
    is determined by the length of the source segment. If there are multiple
    conflicting matches with the same source segment length but a different
    starting point, the source segment that starts earlier is chosen.

    Args:
        correlations (List[SegmentCorrelations]): list of independent
            source-target segment pairs
        verbosity (int, optional): verbosity (0-4). Defaults to 0.

    Returns:
        Tuple[TaggedSegmentCorrelations, Set[TaggedSegmentCorrelation]]:
        - combined list of source-target segment
          pairs, with non-overlapping source segments. Each target match list
          is tagged with the index from the input list it corresponds to. The
          output source segments will jointly cover the same offset range as
          all the input source segments.
        - set of correlations in the returned list that are
          approximate/interpolated
    """

    def report(message: str, level: int = 0, flush: bool = False):
        if verbosity >= level:
            print(message, flush=flush)

    def debug(message: str, level: int = 4):
        report(f"DEBUG: {message}", level, flush=True)

    # Returns (interpolated segment, is exact)
    def interpolate_pair(
        segment: arm5find.Segment,
        reference: arm5find.Segment,
        ref_start: int,
        ref_end: int,
    ) -> Tuple[arm5find.Segment, bool]:
        if ref_end < 0:
            ref_end += len(reference)
        scale = segment.length / reference.length
        d_start = ref_start - reference.offset
        d_end = ref_end - reference.end()
        new_start = segment.offset + round(scale * d_start)
        new_end = segment.end() + round(scale * d_end)
        return (
            arm5find.Segment(new_start, max(new_end - new_start, 1)),
            segment.length == reference.length,
        )

    selected: TaggedSegmentCorrelations = []
    interpolated: Set[TaggedSegmentCorrelation] = set()

    idxs = [0] * len(correlations)
    while True:
        # Select the target and corresponding segment at the next lowest offset
        tnext = 0
        cnext = None
        for t, (c, i) in enumerate(zip(correlations, idxs)):
            if i < len(c) and (cnext is None or c[i][0].offset < cnext[0].offset):
                tnext, cnext = t, c[i]
        if cnext is None:
            # All targets are out of segments
            break
        idxs[tnext] += 1
        # This source segment has no corresponding target segments; skip it
        if not cnext[1]:
            continue

        if not selected or selected[-1][0].end() <= cnext[0].offset:
            # No conflicts, select
            selected.append((cnext[0], [(tnext, cnext[1])]))
            continue
        cprev = selected[-1]

        # This segment conflicts with a previous selection. There are five
        # possibilities

        # 1. This segment is equal to the previous one. Append it to the
        # previous one
        if cnext[0] == cprev[0]:
            cprev[1].append((tnext, cnext[1]))
            continue

        # 2. This segment is a subset of a previous one. Skip it
        if cnext[0].end() <= cprev[0].end():
            continue

        # 3. This segment is a superset of the previous one. Overwrite the
        # previous one
        if cnext[0].offset <= cprev[0].offset and cnext[0].end() >= cprev[0].end():
            # Remove any interpolated entries from tracking
            for rm_tag, rm_targets in cprev[1]:
                for rm_target in rm_targets:
                    rm_key = (cprev[0], rm_tag, rm_target)
                    if rm_key in interpolated:
                        interpolated.remove(rm_key)
            selected[-1] = (cnext[0], [(tnext, cnext[1])])
            continue

        # 4. This segment isn't larger than the previous one and isn't a full
        # subset. Split this segment and append just the non-overlapping part.
        if cnext[0].length <= cprev[0].length:
            split_start = cprev[0].end()
            split_end = cnext[0].end()
            split_src = arm5find.Segment(split_start, split_end - split_start)
            split_targets: Segments = []
            for tseg in cnext[1]:
                interp, is_exact = interpolate_pair(
                    tseg, cnext[0], split_start, split_end
                )
                split_targets.append(interp)
                if not is_exact:
                    interpolated.add((split_src, tnext, interp))
            selected.append((split_src, [(tnext, split_targets)]))
            continue

        # 5. This segment is larger than the previous one but isn't a full
        # superset. Truncate the previous one before appending the new one.
        if cnext[0].length > cprev[0].length:
            # Truncate previous
            split_start = cprev[0].offset
            split_end = cnext[0].offset
            split_src = arm5find.Segment(split_start, split_end - split_start)
            split_targets: List[TaggedSegments] = []
            for tag, segs in cprev[1]:
                split_segs: Segments = []
                for tseg in segs:
                    interp, is_exact = interpolate_pair(
                        tseg, cprev[0], split_start, split_end
                    )
                    # Remove original entries from tracking before adding back
                    # the truncated ones
                    rm_key = (cprev[0], tag, tseg)
                    if rm_key in interpolated:
                        interpolated.remove(rm_key)
                    split_segs.append(interp)
                    if not is_exact:
                        interpolated.add((split_src, tag, interp))
                split_targets.append((tag, split_segs))
            selected[-1] = (split_src, split_targets)

            # Append new
            selected.append((cnext[0], [(tnext, cnext[1])]))
            continue

        # We should never get here
        raise RuntimeError(f"Invalid source segment {cnext[0]}?")

    debug(f"Interpolated segments: {interpolated}")

    # fill in any uncovered source segments with empty target lists
    try:
        start = min(src_seg.offset for c in correlations for src_seg, _ in c)
        end = max(end_seg.end() for c in correlations for end_seg, _ in c)
        holes = fill_source_holes(selected, start, end)
        debug(f"Filled selection holes: {holes}")
    except ValueError:
        pass

    return selected, interpolated


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Correlate segments of ARMv5 assembly from one binary to segments in other binaries"
    )
    parser.add_argument(
        "-c",
        "--chunksize",
        type=int,
        default=8,
        help="instruction chunk size controlling search granularity",
    )
    parser.add_argument(
        "-t",
        "--tolerance",
        type=int,
        default=16,
        help="tolerance of extra instructions within the source when merging correlated segments",
    )
    parser.add_argument(
        "-o",
        "--offset",
        type=lambda x: int(x, 0),
        default=0,
        help="offset into the source binary file to start correlating from",
    )
    parser.add_argument(
        "-l",
        "--length",
        type=lambda x: int(x, 0),
        help="length of the region in the source binary file to correlate",
    )
    parser.add_argument(
        "--strict-ldr-str-offset",
        action="store_true",
        help="require the offset of ldr/str assembly instructions to match exactly",
    )
    parser.add_argument(
        "--strict-b-offset",
        action="store_true",
        help="require the offset of b assembly instructions to match exactly",
    )
    parser.add_argument(
        "-v", "--verbose", action="count", default=0, help="verbosity level"
    )
    parser.add_argument(
        "-p",
        "--plot",
        help="graphically plot correlation coverage of the source binary file and save to file",
    )
    parser.add_argument(
        "source", help="source binary file to take search segments from"
    )
    parser.add_argument("target", nargs="+", help="target binary file to search")
    args = parser.parse_args()

    if args.chunksize <= 0:
        raise ValueError(f"invalid chunk size {args.chunksize}, must be positive")
    if args.tolerance < 0:
        raise ValueError(f"invalid tolerance {args.tolerance}, must be nonnegative")
    if args.offset < 0:
        raise ValueError(f"invalid offset {args.offset}, must be nonnegative")
    if args.length and args.length < 0:
        raise ValueError(f"invalid length {args.length}, must be nonnegative")

    # Check for file existence up-front to report errors quickly
    for fname in [args.source] + args.target:
        if not os.path.exists(fname):
            raise FileNotFoundError(f"No such file or directory: '{fname}'")

    commonpath = os.path.commonpath([args.source] + args.target)

    def reduced_path(p):
        return os.path.relpath(p, commonpath)

    # Try to import MPL ahead of time to report errors quickly
    if args.plot is not None:
        import matplotlib as mpl
        import matplotlib.pyplot as plt
        import numpy as np

    GAP_SEPARATOR = 32 * "*"
    print(f"*** SOURCE FILE: {reduced_path(args.source)} ***")

    # Correlate source chunks to chunks in each target
    src_segments, targets_matches = armv5_correlate(
        args.source,
        args.target,
        src_offset=args.offset,
        src_length=args.length,
        instruction_chunk_size=args.chunksize,
        strict_ldr_str_offset=args.strict_ldr_str_offset,
        strict_b_offset=args.strict_b_offset,
        verbosity=args.verbose,
    )

    # Report raw correlation map in verbose mode
    if args.verbose > 0:
        print(f"== Raw chunk correlations ==")
        printed_separator = False
        for i, s in enumerate(src_segments):
            if args.verbose >= 3 or any(
                target_matches[i] for target_matches in targets_matches
            ):
                print(s)
                printed_separator = False
                for target, target_matches in zip(args.target, targets_matches):
                    if target_matches[i]:
                        print(f"  - {reduced_path(target)}: {target_matches[i]}")
            elif args.verbose < 3 and not printed_separator:
                print(GAP_SEPARATOR)
                printed_separator = True
        print(flush=True)

    # Merge correlations within a target
    target_correlations: List[SegmentCorrelations] = []
    for target, target_matches in zip(args.target, targets_matches):
        merged_correlations = armv5_merge_segment_correlations(
            src_segments,
            target_matches,
            instruction_tolerance=args.tolerance,
            verbosity=args.verbose,
        )
        target_correlations.append(merged_correlations)

        # Report individual merge correlation maps in very verbose mode
        if args.verbose > 0:
            print(f"== Merged chunk correlations ({reduced_path(target)}) ==")
            printed_separator = False
            for s, t in merged_correlations:
                if args.verbose >= 3 or t:
                    print(f"{s} -> {t}")
                    printed_separator = False
                elif args.verbose < 3 and not printed_separator:
                    print(GAP_SEPARATOR)
                    printed_separator = True
            print(flush=True)

    # Select correlations across targets
    correlations, interpolated = armv5_select_segment_correlations(
        target_correlations, verbosity=args.verbose
    )

    # Report the final inferred correlation map
    print("== Inferred correlations ==")
    INTERP_TAG = " (interpolated)"
    min_offset = args.offset
    max_offset = min_offset
    covered_length = 0
    printed_separator = False
    for s, tagged_matches in correlations:
        min_offset = min(min_offset, s.offset)
        max_offset = max(max_offset, s.end())
        has_matches = any(matches for _, matches in tagged_matches)
        if args.verbose >= 3 or has_matches:
            print(s)
            printed_separator = False
            if has_matches:
                covered_length += s.length
            for i, target_matches in tagged_matches:
                if target_matches:
                    target = args.target[i]
                    target_matches_strs = [
                        f"{t}{INTERP_TAG if (s, i, t) in interpolated else ''}"
                        for t in target_matches
                    ]
                    print(
                        f"  - {reduced_path(target)}: [{', '.join(target_matches_strs)}]"
                    )
        elif args.verbose < 3 and not printed_separator:
            print(GAP_SEPARATOR)
            printed_separator = True
    print(flush=True)
    coverage_pct = 100 * covered_length / (max_offset - min_offset)
    print(f"*** Source coverage: {coverage_pct:.2f}% ***")

    if args.plot is not None:
        NO_TARGET = -1
        MULTI_TARGET = len(args.target)
        has_multitarget = False
        target_map = {}
        min_offset = args.offset
        max_offset = min_offset
        for s, tagged_matches in correlations:
            min_offset = min(min_offset, s.offset)
            max_offset = max(max_offset, s.end())

            target = NO_TARGET
            for tag, segs in tagged_matches:
                if not segs:
                    continue
                if target != NO_TARGET and tag != target:
                    # More than one target
                    target = MULTI_TARGET
                    has_multitarget = True
                    break
                target = tag
            if target == NO_TARGET:
                continue
            for c in range(s.offset, s.end(), args.chunksize):
                target_map[c] = target
        imdata = np.array(
            [
                target_map.get(c, NO_TARGET)
                for c in range(min_offset, max_offset, args.chunksize)
            ]
        )

        # Create colormap
        base_cmap = mpl.colormaps["tab10"]
        if len(args.target) > base_cmap.N:
            base_cmap = mpl.colormaps["hsv"].resampled(len(args.target) + 1)
        cmaplist = [base_cmap(i) for i in range(len(args.target))]
        cmaplabels = [os.path.basename(t) for t in args.target]
        if has_multitarget:
            cmaplist.append((0, 0, 0, 1))
            cmaplabels.append("(multiple)")
        ncolors = len(cmaplist)
        if len(cmaplist) == 1:
            # XXX: LinearSegmentedColormap doesn't work with a color list of
            # length 1 since np.linspace(0, 1, 1) returns just [0.], so just
            # use the same color twice
            cmaplist *= 2
        cmap = mpl.colors.LinearSegmentedColormap.from_list(
            "categorical", cmaplist, ncolors
        ).with_extremes(under="white")

        fig, ax = plt.subplots(figsize=(10, 10))
        ax.set_title(f"{os.path.basename(args.source)} ({coverage_pct:.2f}% coverage)")
        im = ax.imshow(
            imdata[:, np.newaxis],
            cmap=cmap,
            aspect=1 / (2 * len(imdata)),
            interpolation="none",
            interpolation_stage="rgba",
            extent=(0, 1, max_offset, min_offset),
            vmin=0,
            vmax=cmap.N,
        )
        cbar = fig.colorbar(im, ticks=[])
        ax.set_xticks([])
        ax.set_yticks([int(x) for x in np.linspace(min_offset, max_offset, 9)])
        ax.get_yaxis().set_major_formatter(lambda x, loc: hex(int(x)))
        for i, label in enumerate(cmaplabels):
            cbar.ax.text(1.2, i + 0.5, label, va="center")
        cbar.ax.invert_yaxis()
        fig.tight_layout()

        if args.plot:
            fig.savefig(args.plot)
        plt.show()
