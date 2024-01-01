//! Defines the `resymgen` YAML format and its programmatic representation, the [`SymGen`] struct.

pub mod cursor;
pub use cursor::{BlockCursor, SymGenCursor};

use std::any;
use std::borrow::Cow;
use std::cmp::Ordering;
use std::collections::{BTreeMap, BTreeSet, HashMap};
use std::fmt::{self, Display, Formatter};
use std::io::{self, Read, Write};
use std::iter;
use std::ops::Deref;
use std::path::{Path, PathBuf};
use std::slice::SliceIndex;

use regex::{Captures, Regex};
use serde::{Deserialize, Serialize};
use serde_yaml;
use syn::{self, LitStr};

use super::error::{Error, Result, SubregionError};
use super::types::*;

/// Specifies how integers should be formatted during serialization.
#[derive(Clone, Copy)]
pub enum IntFormat {
    Decimal,
    Hexadecimal,
}

/// Information about a [`Block`] to be propagated down to the block's contents.
struct BlockContext {
    version_order: Option<HashMap<String, u64>>,
}

fn option_vec_is_empty<T>(opt: &Option<Vec<T>>) -> bool {
    match opt {
        None => true,
        Some(v) => v.is_empty(),
    }
}

/// A symbol in a `resymgen` symbol table, with some metadata.
#[derive(Debug, PartialEq, Eq, Clone, Serialize, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct Symbol {
    /// The symbol name.
    pub name: String,
    /// Aliases for the symbol.
    #[serde(skip_serializing_if = "option_vec_is_empty")]
    pub aliases: Option<Vec<String>>,
    /// The starting address of the symbol in memory.
    pub address: MaybeVersionDep<Linkable>,
    /// The length of the symbol in memory (in bytes).
    #[serde(skip_serializing_if = "Option::is_none")]
    pub length: Option<MaybeVersionDep<Uint>>,
    /// A description of the symbol.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub description: Option<String>,
}

/// Combines possibly version-dependent `addrs` and `opt_len` into a single `MaybeVersionDep`
/// with the data (addr, opt_len). Assumes `addrs` and `opt_len` have the same `Version` key space.
fn zip_addr_len<T>(
    addrs: &MaybeVersionDep<T>,
    opt_len: Option<&MaybeVersionDep<Uint>>,
) -> MaybeVersionDep<(T, Option<Uint>)>
where
    T: Clone,
{
    match addrs {
        MaybeVersionDep::ByVersion(addr) => {
            let version_dep = match opt_len {
                Some(len) => addr
                    .iter()
                    .map(|(v, a)| (v.clone(), (a.clone(), len.get_native(Some(v)).copied())))
                    .collect(),
                None => addr
                    .iter()
                    .map(|(v, a)| (v.clone(), (a.clone(), None)))
                    .collect(),
            };
            MaybeVersionDep::ByVersion(version_dep)
        }
        MaybeVersionDep::Common(addr) => match opt_len {
            Some(lens) => match lens {
                MaybeVersionDep::Common(len) => MaybeVersionDep::Common((addr.clone(), Some(*len))),
                // If we don't have explicit versions for addr but do for length,
                // use the versions from length as a best-effort output.
                MaybeVersionDep::ByVersion(len) => MaybeVersionDep::ByVersion(
                    len.iter()
                        .map(|(v, &l)| (v.clone(), (addr.clone(), Some(l))))
                        .collect(),
                ),
            },
            None => MaybeVersionDep::Common((addr.clone(), None)),
        },
    }
}

impl Symbol {
    /// Initializes the [`Symbol`] with a given `ctx`.
    fn init(&mut self, ctx: &BlockContext) {
        self.address.init(&ctx.version_order);
        if let Some(l) = &mut self.length {
            l.init(&ctx.version_order);
        }
    }
    /// Coerces the [`Symbol`]'s address and length fields to be [`ByVersion`].
    ///
    /// If either field is [`Common`], it will be expanded with the versions in `all_versions`.
    ///
    /// [`ByVersion`]: MaybeVersionDep::ByVersion
    /// [`Common`]: MaybeVersionDep::Common
    pub fn expand_versions(&mut self, all_versions: &[Version]) {
        self.address.expand_versions(all_versions);
        if let Some(len) = &mut self.length {
            len.expand_versions(all_versions);
        }
    }
    /// Gets the extents occupied by the [`Symbol`], possibly by version, represented as
    /// address-length pairs.
    ///
    /// If the optional `all_versions` is provided, the returned extents are guaranteed to be
    /// [`ByVersion`].
    ///
    /// [`ByVersion`]: MaybeVersionDep::ByVersion
    pub fn extents(
        &self,
        all_versions: Option<&[Version]>,
    ) -> MaybeVersionDep<(Linkable, Option<Uint>)> {
        match all_versions {
            // Always realize the address with all versions if possible
            Some(versions) => zip_addr_len(
                &MaybeVersionDep::ByVersion(self.address.by_version(versions)),
                self.length.as_ref(),
            ),
            None => zip_addr_len(&self.address, self.length.as_ref()),
        }
    }
    /// Returns an [`Iterator`] over references to all the [`Symbol`]'s names.
    ///
    /// The [`Symbol`]'s primary name is yielded first, then any aliases.
    pub fn iter_names(&self) -> impl Iterator<Item = &str> {
        iter::once(self.name.deref()).chain(self.iter_aliases())
    }
    /// Returns an [`Iterator`] over references to all the [`Symbol`]'s aliases.
    pub fn iter_aliases(&self) -> impl Iterator<Item = &str> {
        self.aliases
            .as_deref()
            .unwrap_or_default()
            .iter()
            .map(|s| s.deref())
    }
}

impl Sort for Symbol {
    // Note: only applies to the address values. Version keys will always be sorted.
    fn sort(&mut self) {
        if let Some(aliases) = &mut self.aliases {
            aliases.sort();
        }
        self.address.sort();
    }
}

impl PartialOrd for Symbol {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Ord for Symbol {
    fn cmp(&self, other: &Self) -> Ordering {
        self.address.cmp(&other.address)
    }
}

/// A concrete realization of a [`Symbol`] for some [`Version`] (which can be [`None`]).
#[derive(Debug, PartialEq, Eq, Clone, Copy, Serialize)]
pub struct RealizedSymbol<'a> {
    pub name: &'a str,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub aliases: Option<&'a [String]>,
    pub address: Uint,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub length: Option<Uint>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub description: Option<&'a str>,
}

/// Wraps an [`Iterator`] over [`Symbol`]s to yield a stream of [`RealizedSymbol`]s.
pub struct RealizedSymbolIter<'v, 's, I>
where
    I: Iterator<Item = &'s Symbol>,
{
    version: Option<&'v Version>,
    symbols: I,
    cur: Option<(&'s Symbol, LinkableIter<'s>, Option<&'s Uint>)>,
}

impl<'v, 's, I> Iterator for RealizedSymbolIter<'v, 's, I>
where
    I: Iterator<Item = &'s Symbol>,
{
    type Item = RealizedSymbol<'s>;

    fn next(&mut self) -> Option<Self::Item> {
        loop {
            while self.cur.is_none() {
                // Try to fill cur
                match self.symbols.next() {
                    Some(symbol) => {
                        if let Some(address) = symbol.address.get(self.version) {
                            // This symbol can be realized; store it as cur
                            self.cur = Some((
                                symbol,
                                address.iter(),
                                symbol.length.as_ref().and_then(|l| l.get(self.version)),
                            ));
                        }
                    }
                    None => return None, // Out of symbols; we are done
                }
            }
            // cur is filled; yield realized symbols
            let (symbol, mut addrs, len) = self.cur.take().expect("self.cur is None?");
            if let Some(&a) = addrs.next() {
                // Still addresses to yield; put cur back for the next yield
                self.cur = Some((symbol, addrs, len));
                return Some(RealizedSymbol {
                    name: &symbol.name,
                    aliases: symbol.aliases.as_deref(),
                    address: a,
                    length: len.copied(),
                    description: symbol.description.as_deref(),
                });
            }
            // cur is depleted; don't put it back and get a new one next loop
        }
    }
}

/// Implementers of [`Realize`] can produce iterators over [`RealizedSymbol`]s
/// for arbitrary [`Version`]s.
pub trait Realize<'s> {
    type Iter: Iterator<Item = &'s Symbol>;

    /// Returns a [`RealizedSymbolIter`] for the given `version`.
    fn realize<'v>(self, version: Option<&'v Version>) -> RealizedSymbolIter<'v, 's, Self::Iter>;
}

impl<'s, I> Realize<'s> for I
where
    I: Iterator<Item = &'s Symbol>,
{
    type Iter = I;

    fn realize<'v>(self, version: Option<&'v Version>) -> RealizedSymbolIter<'v, 's, Self::Iter> {
        RealizedSymbolIter {
            version,
            symbols: self,
            cur: None,
        }
    }
}

/// A list of [`Symbol`]s.
///
/// Implements a similar accessor interface to [`Vec<Symbol>`].
#[derive(Debug, PartialEq, Eq, Clone, Serialize, Deserialize)]
pub struct SymbolList(Vec<Symbol>);

impl SymbolList {
    /// Initializes the [`SymbolList`] with a given `ctx`.
    fn init(&mut self, ctx: &BlockContext) {
        for symbol in self.0.iter_mut() {
            symbol.init(ctx);
        }
    }
    /// Expands the versions of all the [`Symbol`]s contained within the [`SymbolList`].
    ///
    /// See [`Symbol::expand_versions()`].
    pub fn expand_versions(&mut self, all_versions: &[Version]) {
        for symbol in self.0.iter_mut() {
            symbol.expand_versions(all_versions);
        }
    }

    pub fn get<I>(&self, index: I) -> Option<&<I as SliceIndex<[Symbol]>>::Output>
    where
        I: SliceIndex<[Symbol]>,
    {
        self.0.get(index)
    }
    pub fn get_mut<I>(&mut self, index: I) -> Option<&mut <I as SliceIndex<[Symbol]>>::Output>
    where
        I: SliceIndex<[Symbol]>,
    {
        self.0.get_mut(index)
    }
    /// Returns a mutable reference to a symbol, without doing bounds checking.
    ///
    /// # Safety
    /// Calling this method with an out-of-bounds index is undefined behavior.
    pub unsafe fn get_unchecked_mut<I>(
        &mut self,
        index: I,
    ) -> &mut <I as SliceIndex<[Symbol]>>::Output
    where
        I: SliceIndex<[Symbol]>,
    {
        self.0.get_unchecked_mut(index)
    }
    pub fn iter(&self) -> impl Iterator<Item = &Symbol> {
        self.0.iter()
    }
    pub fn len(&self) -> usize {
        self.0.len()
    }
    pub fn is_empty(&self) -> bool {
        self.0.is_empty()
    }
    pub fn push(&mut self, value: Symbol) {
        self.0.push(value)
    }
    pub fn append(&mut self, other: &mut SymbolList) {
        self.0.append(&mut other.0)
    }
}

impl Deref for SymbolList {
    type Target = [Symbol];

    fn deref(&self) -> &Self::Target {
        self.0.deref()
    }
}

impl<const N: usize> From<[Symbol; N]> for SymbolList {
    fn from(arr: [Symbol; N]) -> Self {
        SymbolList(Vec::from(arr))
    }
}

/// A symbol in a [`SymbolList`], potentially augmented by additional addresses for sorting
#[derive(Debug, PartialEq, Eq, Clone)]
struct SortSymbol {
    /// The symbol.
    symbol: Symbol,
    /// Addresses temporarily assigned while the parent SymbolList is being sorted.
    sort_address: Option<VersionDep<Linkable>>,
}

impl SortSymbol {
    fn get_native(&self, native_key: &Version) -> Option<&Linkable> {
        if let Some(addr) = &self.sort_address {
            return addr.get_native(native_key);
        } else if let MaybeVersionDep::ByVersion(addr) = &self.symbol.address {
            return addr.get_native(native_key);
        }
        None
    }
    fn contains_key_native(&self, native_key: &Version) -> bool {
        self.get_native(native_key).is_some()
    }
    fn insert_native(&mut self, native_key: Version, value: Linkable) -> Option<Linkable> {
        if let Some(addr) = &mut self.sort_address {
            addr.insert_native(native_key, value)
        } else if let MaybeVersionDep::ByVersion(addr) = &self.symbol.address {
            let mut addr = addr.clone();
            let old = addr.insert_native(native_key, value);
            self.sort_address = Some(addr);
            old
        } else {
            self.sort_address = Some([(native_key, value)].into());
            None
        }
    }
}

impl PartialOrd for SortSymbol {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Ord for SortSymbol {
    fn cmp(&self, other: &Self) -> Ordering {
        // Use the sort_address field if present, otherwise fall back to the normal address field
        match &self.sort_address {
            Some(self_sort_addr) => match &other.sort_address {
                Some(other_sort_addr) => self_sort_addr.cmp(other_sort_addr),
                None => {
                    MaybeVersionDep::ByVersion(self_sort_addr.clone()).cmp(&other.symbol.address)
                }
            },
            None => match &other.sort_address {
                Some(other_sort_addr) => self
                    .symbol
                    .address
                    .cmp(&MaybeVersionDep::ByVersion(other_sort_addr.clone())),
                None => self.symbol.address.cmp(&other.symbol.address),
            },
        }
    }
}

/// A disjoint set of closed numeric ranges
#[derive(Debug, PartialEq, Eq)]
struct RangeSet(Vec<(Uint, Uint)>);

impl From<Vec<(Uint, Uint)>> for RangeSet {
    fn from(mut ranges: Vec<(Uint, Uint)>) -> Self {
        ranges.retain(|r| r.0 <= r.1);
        if ranges.is_empty() {
            return Self(ranges);
        }
        ranges.sort_unstable();

        let mut rangeset = Vec::with_capacity(ranges.len());
        let mut range_iter = ranges.into_iter();
        // Buffer the current range so that we can coalesce if needed before pushing
        let mut current_range = range_iter.next().unwrap();
        for r in range_iter {
            // Note: can't just check (r.0 - current_range.1) as i64 <= 1 because of integer
            // underflow.
            if r.0 <= current_range.1 || (r.0 - current_range.1 == 1) {
                // The new range's left endpoint overlaps with or borders current_range, so we can
                // combine it with current_range.
                current_range.1 = current_range.1.max(r.1);
            } else {
                rangeset.push(current_range);
                current_range = r;
            }
        }
        rangeset.push(current_range);
        Self(rangeset)
    }
}

impl RangeSet {
    fn contains(&self, val: Uint) -> bool {
        self.0
            .binary_search_by(|r| {
                if let Ordering::Greater = r.0.cmp(&val) {
                    Ordering::Greater
                } else if let Ordering::Less = r.1.cmp(&val) {
                    Ordering::Less
                } else {
                    Ordering::Equal
                }
            })
            .is_ok()
    }
}

impl Sort for SymbolList {
    fn sort(&mut self) {
        // Sort each individual symbol's contents, and gather a sorted list of all versions
        // inferred from the symbol contents
        let mut all_versions = BTreeSet::new();
        for symbol in self.0.iter_mut() {
            symbol.sort();

            for v in symbol.address.versions() {
                all_versions.insert(v);
            }
        }
        let all_versions: Vec<_> = all_versions.into_iter().cloned().collect();

        // Move symbols over to an auxiliary array, since we need to be able to augment the symbol
        // data for sorting purposes
        let mut sort_list: Vec<SortSymbol> = Vec::with_capacity(self.len());
        for symbol in self.0.drain(..) {
            sort_list.push(SortSymbol {
                symbol,
                sort_address: None,
            });
        }

        // Realize Common addresses for sorting purposes, if possible. Comparison between
        // Common/ByVersion variants is not consistent/transitive if the ByVersion variant
        // is missing some versions, and realization prevents such intransitivity.
        for ss in sort_list.iter_mut() {
            if ss.symbol.address.is_common() && !all_versions.is_empty() {
                ss.sort_address = Some(ss.symbol.address.by_version(&all_versions));
            }
        }

        // First pass: naive lexicographic sort.
        sort_list.sort();

        // The following block performs a more sophisticated sorting algorithm for symbols with
        // versioned addresses.
        //
        // # Motivation
        // A naive lexicographic sort has limitations when some symbols are missing addresses
        // for certain versions. For example, say you have a list like this:
        // ```yml
        // - name: a
        //   address:
        //     v1: 0
        //     v2: 1
        // - name: b
        //   address:
        //     v1: 10
        //     v2: 11
        // - name: c
        //   address:
        //     v2: 2
        // ```
        // The naive sort will leave "c" at the end, even though it should really be in between
        // "a" and "b", based on the v2 value.
        //
        // However, it's not always possible to unambiguously order symbols with missing addresses.
        // For example, say you have a list like this:
        // ```yml
        // - name: a
        //   address:
        //     v1: 0
        //     v2: 10
        // - name: b
        //   address:
        //     v1: 1
        //     v2: 2
        // - name: c
        //   address:
        //     v2: 5
        // ```
        // Here, it's not clear whether "c" should come before "a", or after "b". A good sorting
        // algorithm should be able to detect such situations, and leave these symbols at the end
        // in these cases.
        //
        // # Algorithm Overview
        // Sorting happens over multiple passes, one for each version, in order (with the first
        // pass being the naive sort in the previous line). The passes accumulate, and by the last
        // pass, the list will be fully sorted. In each pass (for a version "v"):
        //
        // 1. Of the symbols with addresses for version "v" but not for prior versions, determine
        // which can be unambigously resorted.
        // 2. Of the good symbols in step 1, figure out where to relocate them, and then do so.
        //
        // See subsequent comments for more detail.
        let mut first_unsorted_idx = 0;
        for (pass_idx, vpair) in all_versions.windows(2).enumerate() {
            let vsorted = &vpair[0]; // the previous version, which a pass was already done for
            let v = &vpair[1]; // the currrent version, which this pass is focused on
                               // all previous versions for which a pass was already done for
            let all_vsorted = &all_versions[..=pass_idx];

            // Find the first symbol (that isn't already sorted) whose address set doesn't have
            // vsorted. This is the first unsorted symbol.
            for ss in sort_list.iter().skip(first_unsorted_idx) {
                if ss.contains_key_native(vsorted) {
                    first_unsorted_idx += 1;
                } else {
                    break;
                }
            }
            // Everything is already sorted; nothing to do
            if first_unsorted_idx == sort_list.len() {
                break;
            }

            // Search for sort violations among the vsorted-sorted symbols with v addresses,
            // and fill in v addresses if missing. A sort violation for version v is when the
            // sequence of v addresses (the ordering of which is controlled by addresses from prior
            // versions) is not in sorted order. For example:
            //
            // v2: 1, 2, 3, 4, 5, 3, 6, 4, 8, 12, 11
            //                    ^     ^         ^
            //                     sort violations
            //
            // What we care about is the range of values that are "passed through" when sorting
            // order is violated. In the above example, we pass from 5 -> 3, 6 -> 4, and 12 -> 11.
            // This means that we can't sort any currently unsorted symbol with a v address in the
            // ranges [3, 5], [4, 6], or [11, 12].
            let mut prev_val = Uint::MIN;
            let mut contested_ranges = Vec::new();
            for ss in sort_list.iter_mut().take(first_unsorted_idx) {
                if let Some(cur_val) = ss.get_native(v).map(|val| val.cmp_key()) {
                    if cur_val < prev_val {
                        contested_ranges.push((cur_val, prev_val));
                    }
                    prev_val = cur_val;
                } else {
                    // We need to fill in an artificial v address so the binary search in the
                    // next step works properly. We can just use prev_val to maintain the existing
                    // order.
                    ss.insert_native(v.clone(), prev_val.into());
                }
            }
            let contested_ranges = RangeSet::from(contested_ranges);

            // Go through each of the unsorted symbols (with v addresses but not vsorted addresses)
            // and try to assign fake addresses for all the addresses in all_vsorted, such that the
            // symbols will end up appropriately sorted.
            let (sorted_slice, unsorted_slice) = sort_list.split_at_mut(first_unsorted_idx);
            for ss in unsorted_slice.iter_mut() {
                if let Some(cur_val) = ss.get_native(v).map(|val| val.cmp_key()) {
                    first_unsorted_idx += 1; // this just saves us some work in the next pass

                    if contested_ranges.contains(cur_val) {
                        // This symbol is in a contested range...we can't sort it, so just skip
                        continue;
                    }

                    // Search for the first fully sorted symbol (had a vsorted address) with a
                    // version v address that exceeds that of the current unsorted symbol. This
                    // is the sorted symbol we want to insert the unsorted symbol in front of.
                    let idx =
                        sorted_slice.partition_point(|ss| {
                            ss.get_native(v).expect(
                            "SymbolList::Sort reference symbol does not have reference value?",
                        ).cmp_key() <= cur_val
                        });
                    // If idx == sorted_slice.len(), there's nothing to do; the current unsorted
                    // symbol comes after all the currently sorted symbols and should stay at the
                    // end of the list.
                    if idx < sorted_slice.len() {
                        // # Safety
                        // We just checked that idx < sorted_slice.len()
                        let ref_ss = unsafe { sorted_slice.get_unchecked(idx) };
                        // Copy the values for the all_vsorted version from the matched sorted
                        // symbol to the current unsorted symbol. Since the version v value for
                        // the current unsorted symbol is less than that of the matched sorted
                        // symbol by construction, this ensures that the current unsorted symbol
                        // will end up directly in front of the sorted symbol when we resort the
                        // list.
                        for vother in all_vsorted.iter() {
                            // ref_ss must have a value for v, but not necessarily for the vother's
                            // before it, since it could've been skipped on previous iterations due
                            // to contested ranges.
                            if let Some(vother_val) = ref_ss.get_native(vother) {
                                ss.insert_native(vother.clone(), vother_val.cmp_key().into());
                            }
                        }
                    }
                } else {
                    // This symbol doesn't have a v address. Since sort_list was already
                    // pre-sorted, none of the later symbols will either. This pass is finished.
                    break;
                }
            }

            // Next pass: now that we've added new sort_addresses, redo the lexicographic sort
            // to put the symbols with version v addresses but not vsorted addresses in order
            sort_list.sort();
        }

        // Transfer the fully sorted symbols back from the auxiliary array
        for ss in sort_list.into_iter() {
            self.0.push(ss.symbol);
        }
    }
}

/// A contiguous block of [`Symbol`]s in a `resymgen` symbol table, with some metadata.
///
/// Like its consituent [`Symbol`]s, a [`Block`] contains an address, a length, and a description.
/// Unlike [`Symbol`]s, the `length` field is required. [`Block`]s can also contain a list of all
/// relevant [`Version`]s, which may or may not be used by the [`Symbol`]s it contains.
///
/// Every [`Block`] contains two separate [`SymbolList`]s: one for function symbols, and one for
/// data symbols.
#[derive(Debug, PartialEq, Eq, Clone, Serialize, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct Block {
    // Metadata
    /// List of [`Version`]s relevant to the block.
    #[serde(skip_serializing_if = "option_vec_is_empty")]
    pub versions: Option<Vec<Version>>,
    /// The starting address of the block in memory.
    pub address: MaybeVersionDep<Uint>,
    /// The length of the block in memory (in bytes).
    pub length: MaybeVersionDep<Uint>,
    /// A description of the block.
    #[serde(skip_serializing_if = "Option::is_none")]
    pub description: Option<String>,

    // Symbols
    /// List of subregions.
    #[serde(skip_serializing_if = "option_vec_is_empty")]
    pub subregions: Option<Vec<Subregion>>,
    /// List of function symbols.
    pub functions: SymbolList,
    /// List of data symbols.
    pub data: SymbolList,
}

impl Block {
    /// Gets a [`BlockContext`] associated with the [`Block`].
    fn get_context(&self) -> BlockContext {
        BlockContext {
            version_order: OrdString::get_order_map(self.versions.as_deref()),
        }
    }
    /// Initializes the [`Block`]'s contents using its version list.
    fn init(&mut self) {
        let ctx = self.get_context();

        // Init metadata
        if let Some(vers) = &mut self.versions {
            for v in vers {
                v.init(&ctx.version_order);
            }
        }
        self.address.init(&ctx.version_order);
        self.length.init(&ctx.version_order);

        // Init symbols
        self.functions.init(&ctx);
        self.data.init(&ctx);
    }
    /// Recursively resolves the contents of all [`Subregion`]s in the [`Block`].
    ///
    /// [`Subregion`]s are read from files using `file_opener`, with file paths based on the root
    /// directory specified by `dir_path`.
    pub fn resolve_subregions<P, R, F>(&mut self, dir_path: P, file_opener: F) -> Result<()>
    where
        P: AsRef<Path>,
        R: Read,
        F: Fn(&Path) -> io::Result<R> + Copy,
    {
        if let Some(subregions) = &mut self.subregions {
            for s in subregions.iter_mut() {
                s.resolve(&dir_path, file_opener)?;
                // Recursively resolve
                let subdir_path = dir_path.as_ref().join(Subregion::subregion_dir(&s.name));
                // Explicitly block symlinks, which could lead to infinite recursion.
                // If the path itself is invalid, just carry on and let file_opener deal with it.
                // Note that the documentation on is_symlink() is a bit ambiguous, but this method
                // (at least on Unix) will still follow symlinks on the path to get to the file,
                // it just won't follow the file's link if the file itself is a symlink.
                if subdir_path.is_symlink() {
                    return Err(Error::Subregion(SubregionError::Symlink(subdir_path)));
                }
                s.contents
                    .as_mut()
                    .expect("subregion not resolved after Subregion::resolve()")
                    .resolve_subregions(&subdir_path, file_opener)?;
            }
        }
        Ok(())
    }
    /// Moves all symbols within [`Subregion`]s into the [`Block`]'s main symbol lists, destroying
    /// the [`Subregion`]s in the process.
    pub fn collapse_subregions(&mut self) {
        if let Some(subregions) = self.subregions.take() {
            for s in subregions {
                if let Some(mut symgen) = s.contents {
                    // Recursively collapse
                    symgen.collapse_subregions();
                    for blocks in symgen.blocks_mut() {
                        self.functions.append(&mut blocks.functions);
                        self.data.append(&mut blocks.data);
                    }
                }
            }
        }
    }
    /// Gets the extent occupied by the [`Block`], possibly by version, represented as
    /// address-length pairs.
    pub fn extent(&self) -> MaybeVersionDep<(Uint, Option<Uint>)> {
        let mut versions: Vec<Version> = Vec::new();
        if let Some(v) = &self.versions {
            versions.extend(v.iter().cloned());
        }
        for v in self.address.versions().chain(self.length.versions()) {
            if !versions.contains(v) {
                versions.push(v.clone());
            }
        }
        if !versions.is_empty() {
            // Always realize the address with all versions (including inferred ones) if possible
            zip_addr_len(
                &MaybeVersionDep::ByVersion(self.address.by_version(&versions)),
                Some(&self.length),
            )
        } else {
            zip_addr_len(&self.address, Some(&self.length))
        }
    }
    /// Expands the versions of all the addresses and lengths contained within the [`Block`]
    /// (both in its metadata and the [`Symbol`]s it contains).
    ///
    /// See [`Symbol::expand_versions()`].
    pub fn expand_versions(&mut self) {
        // Note: this function does not try to infer versions from address/length because, unlike
        // with extent(), this function actually CHANGES Common to ByVersion, which sort of loses
        // information (because Common is purely generic).
        if let Some(vers) = &self.versions {
            self.address.expand_versions(vers);
            self.length.expand_versions(vers);
            self.functions.expand_versions(vers);
            self.data.expand_versions(vers);
        }
    }
    /// Looks up a [`Version`] in the [`Block`] by name.
    pub fn version(&self, name: &str) -> Option<&Version> {
        self.versions
            .as_ref()
            .and_then(|vs| vs.iter().find(|v| v.name() == name))
    }
    /// Returns a combined iterator over both function and data symbols in the [`Block`].
    pub fn iter(&self) -> impl Iterator<Item = &Symbol> {
        self.functions.iter().chain(self.data.iter())
    }
    /// Returns a combined iterator over both function and data symbols in the [`Block`], realized
    /// for the [`Version`] corresponding to `version_name`.
    pub fn iter_realized(&self, version_name: &str) -> impl Iterator<Item = RealizedSymbol> + '_ {
        let version = self.version(version_name);
        self.iter().realize(version)
    }
    /// Returns an iterator over function symbols in the [`Block`], realized for the [`Version`]
    /// corresponding to `version_name`.
    pub fn functions_realized(
        &self,
        version_name: &str,
    ) -> impl Iterator<Item = RealizedSymbol> + '_ {
        let version = self.version(version_name);
        self.functions.iter().realize(version)
    }
    /// Returns an iterator over data symbols in the [`Block`], realized for the [`Version`]
    /// corresponding to `version_name`.
    pub fn data_realized(&self, version_name: &str) -> impl Iterator<Item = RealizedSymbol> + '_ {
        let version = self.version(version_name);
        self.data.iter().realize(version)
    }

    /// Returns a [`BlockCursor`] for this [`Block`] with the given block name and file path.
    pub fn cursor<'s, 'p>(&'s self, name: &'s str, path: &'p Path) -> BlockCursor<'s, 'p> {
        BlockCursor::new(self, name, Cow::Borrowed(path))
    }
}

impl PartialOrd for Block {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Ord for Block {
    fn cmp(&self, other: &Self) -> Ordering {
        self.address.cmp(&other.address)
    }
}

impl Sort for Block {
    fn sort(&mut self) {
        if let Some(subregions) = &mut self.subregions {
            subregions.sort();
            for s in subregions {
                if let Some(contents) = &mut s.contents {
                    contents.sort();
                }
            }
        }
        self.functions.sort();
        self.data.sort();
    }
}

/// A programmatic representation of the `resymgen` YAML format.
///
/// At its core, a [`SymGen`] is just a mapping between block names and [`Block`]s, along with
/// convenient methods for manipulating the data within those [`Block`]s.
#[derive(Debug, PartialEq, Eq, Clone, Serialize, Deserialize)]
pub struct SymGen(BTreeMap<OrdString, Block>);

impl SymGen {
    /// Initializes all the block names and [`Block`]s within the [`SymGen`].
    pub fn init(&mut self) {
        // Get entries sorted by (block, name)
        let mut sorted_blocks: Vec<_> = self.0.iter().map(|(name, block)| (block, name)).collect();
        sorted_blocks.sort();
        // Might as well clone the names here because we'll need to do it later anyway
        let names: Vec<_> = sorted_blocks
            .into_iter()
            .map(|(_, name)| name.clone())
            .collect();
        let name_order = OrdString::get_order_map(Some(&names));

        // Remove each element, init keys and values, and reinsert
        for mut name in names {
            let mut block = self.0.remove(&name).unwrap();
            name.init(&name_order);
            block.init();
            self.0.insert(name, block);
        }
    }
    /// Reads an uninitialized [`SymGen`] from `rdr`.
    pub fn read_no_init<R: Read>(rdr: R) -> Result<SymGen> {
        serde_yaml::from_reader(rdr).map_err(Error::Yaml)
    }
    /// Reads a [`SymGen`] from `rdr`. The returned [`SymGen`] will be initialized.
    pub fn read<R: Read>(rdr: R) -> Result<SymGen> {
        let mut symgen: SymGen = SymGen::read_no_init(rdr)?;
        symgen.init();
        Ok(symgen)
    }
    /// Reads a [`SymGen`] from `rdr`. The returned [`SymGen`] will be initialized and sorted.
    ///
    /// [`Block`]s and their contained [`Symbol`]s are sorted by address. For version-dependent
    /// addresses, comparison is lexicographic in [`Version`] order.
    pub fn read_sorted<R: Read>(rdr: R) -> Result<SymGen> {
        let mut symgen = SymGen::read(rdr)?;
        symgen.sort();
        Ok(symgen)
    }
    /// Converts target fields in a `resymgen` YAML string with some inline operation,
    /// via line-by-line text processing.
    ///
    /// `F` injects modified lines into the final YAML string accumulator, based on the given line
    /// to be modified and the current indentation level, and returns a success flag.
    ///
    /// This is kind of a hack. Might be worth investigating whether it's easy to mod `yaml-rust`
    /// and `serde-yaml` to serialize in the desired format directly, rather than doing it via
    /// post-processing. But this is serviceable for now.
    fn convert_fields_inline<F, const N: usize>(
        yaml: &str,
        field_prefixes: [&str; N],
        convert: F,
    ) -> String
    where
        F: Fn(&mut String, &str, usize) -> bool,
    {
        // If under a target field, the level of whitespace for the field so we can tell when
        // the field has ended.
        let mut field_whitespace_level: Option<usize> = None;
        let mut converted_yaml = String::with_capacity(yaml.len());
        for line in yaml.lines() {
            // Strip out hyphens so that the first field in a SymbolList entry we'll still match
            // properly against it. Currently this is always be "name" (see Symbol), but this
            // is pretty easy to do and makes things less fragile if we change the field order.
            // Note that the strings in the Block version list cannot look like a YAML map
            // key (identifier followed by a colon) without being quoted, so we're safe from
            // user-defined versions in this case.
            let trimmed = line.trim_start().trim_start_matches('-').trim_start();
            // Safe to subtract usize here since trimmed.len() <= line.len()
            let whitespace_level = line.len() - trimmed.len();

            if let Some(l) = field_whitespace_level {
                if whitespace_level <= l {
                    // Field has ended; stop converting for now.
                    field_whitespace_level = None;
                }
            }

            if field_whitespace_level.is_none() {
                // Not currently matching a target field; look for a new one.
                //
                // Checking whitespace_level > 0 prevents matching against user-specified block
                // names that happen to start with the target field prefixes. This works because
                // yaml-rust doesn't indent top-level keys. In the unlikely event that this changes
                // we'd need to keep track of the whitespace level of the current top-level key
                // instead of assuming it's 0.
                //
                // YAML supports multiline strings in various forms
                // (https://stackoverflow.com/questions/3790454/how-do-i-break-a-string-in-yaml-over-multiple-lines),
                // but yaml-rust always emits string map values on a single line right after the
                // key (the symgen format doesn't use complex keys)
                // - map values: https://github.com/chyh1990/yaml-rust/blob/4fffe95cddbcf444f8a3f080364caf16a6c11ca6/src/emitter.rs#L230
                // - strings: https://github.com/chyh1990/yaml-rust/blob/4fffe95cddbcf444f8a3f080364caf16a6c11ca6/src/emitter.rs#L156
                // so we don't need to worry about the case where a user-specified string matching
                // target field prefixes.
                //
                // The only other place with arbitrary keys is the MaybeVersionDep's under the
                // target field keys, but since we should match the parent key first and enter
                // conversion mode before seeing the version keys, this shouldn't be a problem.
                if whitespace_level > 0
                    && field_prefixes
                        .iter()
                        .any(|prefix| trimmed.starts_with(prefix))
                {
                    field_whitespace_level = Some(whitespace_level);
                }
            };

            let mut success = false;
            if let Some(indent) = field_whitespace_level {
                // In a target field; convert subsequent lines.
                success = convert(&mut converted_yaml, line, indent);
            }
            if !success {
                // No conversion happened. Add the line unmodified
                converted_yaml.push_str(line);
            }
            // Add the newline back in. Note: Rust has standardized on '\n' for newlines on all platforms.
            // - https://doc.rust-lang.org/std/macro.println.html
            // - https://stackoverflow.com/questions/66450942/in-rust-is-there-a-way-to-make-literal-newlines-in-r-using-windows-c
            // Anyway, newer versions of Notepad support Unix line endings :D.
            // - https://devblogs.microsoft.com/commandline/extended-eol-in-notepad/
            converted_yaml.push('\n');
        }
        converted_yaml
    }
    /// Converts all integer values in a `resymgen` YAML string from decimal to hexadecimal.
    fn convert_dec_to_hex(yaml: &str) -> String {
        let re_int = Regex::new(r"\b\d+\b").unwrap();
        SymGen::convert_fields_inline(
            yaml,
            ["address:", "length:"],
            |converted_yaml, line, indent| {
                // Skip past any colons. This prevents us from replacing "numbers" that appear
                // within quoted version string keys, and we never expect to see any colons
                // after the key-value separator. Even if there's no colon, we can still skip
                // past the whitespace for free, since we have that stored already anyway.
                let start_idx = line.rfind(':').unwrap_or(indent);
                let converted = re_int.replace_all(&line[start_idx..], |caps: &Captures| {
                    let int = caps[0].parse::<Uint>().unwrap_or_else(|_| {
                        panic!(
                            "Could not parse {} as {}",
                            &caps[0],
                            any::type_name::<Uint>()
                        )
                    });
                    format!("{:#X}", int)
                });
                converted_yaml.push_str(&line[..start_idx]);
                converted_yaml.push_str(&converted);
                true
            },
        )
    }
    /// Converts all multiline description strings in a `resymgen` YAML string to block scalar
    /// format, for readability.
    fn convert_multiline_desc_to_block_scalar(yaml: &str) -> String {
        SymGen::convert_fields_inline(yaml, ["description:"], |converted_yaml, line, indent| {
            const SUB_INDENT: usize = 2;
            let start_idx;
            let contents;
            if let Some(idx) = line.find('"') {
                start_idx = idx;
                contents = Cow::Borrowed(&line[start_idx..]);
            } else if let Some(colon) = line.find(':') {
                if let Some(i) = line[colon + 1..].find(|c: char| !c.is_ascii_whitespace()) {
                    start_idx = colon + 1 + i;
                    // Manually add quotes so it can be parsed as a Rust string literal
                    contents = Cow::Owned(format!("\"{}\"", &line[start_idx..]));
                } else {
                    return false;
                }
            } else {
                return false;
            }
            if let Ok(l) = syn::parse_str::<LitStr>(&contents) {
                // Only convert multiline strings
                if l.value().trim_end().lines().count() > 1 {
                    converted_yaml.push_str(&line[..start_idx]);
                    converted_yaml.push_str("|-"); // There's no reason to have trailing newlines
                    for desc_ln in l.value().trim_end().lines() {
                        converted_yaml.push('\n');
                        for _ in 0..indent + SUB_INDENT {
                            converted_yaml.push(' ');
                        }
                        converted_yaml.push_str(desc_ln);
                    }
                    return true;
                }
            }
            false
        })
    }
    /// Writes the [`SymGen`] data to `writer` in `resymgen` YAML format.
    ///
    /// Integers will be written with the given `int_format`.
    pub fn write<W: Write>(&self, mut writer: W, int_format: IntFormat) -> Result<()> {
        // I don't expect these YAML files to be too big to fit in memory, so it's easier and
        // faster to keep the serialized data in memory for processing. And anyway,
        // serde_yaml::from_reader already uses read_to_end()
        // (https://github.com/dtolnay/serde-yaml/blob/644be1654d382627c4419f613e300c5e4df3650f/src/de.rs#L141)
        // so this shouldn't be much worse. If it ever becomes an issue (like with merging huge
        // files or something) this can be refactored to use another intermediate tempfile and a
        // BufReader/BufWriter or something.
        let mut yaml = serde_yaml::to_string(self).map_err(Error::Yaml)?;
        // yaml-rust's built-in behavior is to dump integers in decimal
        // (https://github.com/chyh1990/yaml-rust/blob/4fffe95cddbcf444f8a3f080364caf16a6c11ca6/src/emitter.rs#L173)
        // so writing in hex format requires further processing.
        if let IntFormat::Hexadecimal = int_format {
            yaml = SymGen::convert_dec_to_hex(&yaml);
        }
        yaml = SymGen::convert_multiline_desc_to_block_scalar(&yaml);

        // Skip past the unsightly "---" document-start that serde_yaml inserts (or rather
        // yaml-rust: https://github.com/chyh1990/yaml-rust/blob/4fffe95cddbcf444f8a3f080364caf16a6c11ca6/src/emitter.rs#L135).
        // We aren't using any YAML directives, we only ever serialize one object/document, and
        // serde_yaml doesn't support deserializing multiple documents anyway, so it's totally
        // optional.
        let yaml_bytes = yaml
            .strip_prefix("---")
            .unwrap_or(&yaml)
            .trim_start()
            .as_bytes();
        writer.write_all(yaml_bytes).map_err(Error::Io)
    }
    /// Writes the [`SymGen`] data to a [`String`] in `resymgen` YAML format.
    ///
    /// Integers will be written with the given `int_format`.
    pub fn write_to_str(&self, int_format: IntFormat) -> Result<String> {
        let mut bytes = Vec::<u8>::new();
        self.write(&mut bytes, int_format)?;
        String::from_utf8(bytes).map_err(Error::FromUtf8)
    }

    /// Recursively resolves the contents of all [`Subregion`]s in all [`Block`]s within the
    /// [`SymGen`].
    ///
    /// [`Subregion`]s are read from files using `file_opener`, with file paths based on the root
    /// directory specified by `dir_path`.
    pub fn resolve_subregions<P, R, F>(&mut self, dir_path: P, file_opener: F) -> Result<()>
    where
        P: AsRef<Path>,
        R: Read,
        F: Fn(&Path) -> io::Result<R> + Copy,
    {
        for block in self.0.values_mut() {
            block.resolve_subregions(&dir_path, file_opener)?;
        }
        Ok(())
    }
    /// Moves all symbols within [`Subregion`]s into their parent [`Block`]s' main symbol lists,
    /// destroying the [`Subregion`]s in the process.
    pub fn collapse_subregions(&mut self) {
        for block in self.0.values_mut() {
            block.collapse_subregions();
        }
    }

    /// Expands the versions of all the addresses and lengths contained within the [`SymGen`]
    /// (in all the contained [`Block`]s).
    ///
    /// See [`Block::expand_versions()`].
    pub fn expand_versions(&mut self) {
        for block in self.0.values_mut() {
            block.expand_versions();
        }
    }
    /// Gets a reference to the [`OrdString`] key in the [`SymGen`] corresponding to `block_name`,
    /// if present.
    pub fn block_key(&self, block_name: &str) -> Option<&OrdString> {
        self.0.keys().find(|k| k.val == block_name)
    }

    /// Gets a reference to the [`Block`] associated with `key`, if present.
    pub fn get(&self, key: &OrdString) -> Option<&Block> {
        self.0.get(key)
    }
    /// Gets a mutable reference to the [`Block`] associated with `key`, if present.
    pub fn get_mut(&mut self, key: &OrdString) -> Option<&mut Block> {
        self.0.get_mut(key)
    }
    /// Inserts the [`Block`] contained by `value` into the [`SymGen`], keyed by `key`.
    ///
    /// If the [`SymGen`] already had a [`Block`] keyed by `key`, the old [`Block`] is returned.
    pub fn insert(&mut self, key: OrdString, value: Block) -> Option<Block> {
        self.0.insert(key, value)
    }

    /// Returns an [`Iterator`] over references to (block name, [`Block`]) pairs in the [`SymGen`].
    pub fn iter(&self) -> impl Iterator<Item = (&OrdString, &Block)> {
        self.0.iter()
    }
    /// Returns an [`Iterator`] over mutable references to (block name, [`Block`]) pairs in the
    /// [`SymGen`].
    pub fn iter_mut(&mut self) -> impl Iterator<Item = (&OrdString, &mut Block)> {
        self.0.iter_mut()
    }
    /// Returns an [`Iterator`] over references to [`Block`]s in the [`SymGen`].
    pub fn blocks(&self) -> impl Iterator<Item = &Block> {
        self.0.values()
    }
    /// Returns an [`Iterator`] over mutable references to [`Block`]s in the [`SymGen`].
    pub fn blocks_mut(&mut self) -> impl Iterator<Item = &mut Block> {
        self.0.values_mut()
    }
    /// Returns a flat [`Iterator`] over references to the [`Symbol`]s contained within every
    /// [`Block`] in the [`SymGen`].
    pub fn symbols(&self) -> impl Iterator<Item = &Symbol> {
        self.blocks().flat_map(|b| b.iter())
    }
    /// Returns a flat [`Iterator`] over all symbols contained within every [`Block`] in
    /// the [`SymGen`], realized for the [`Version`] corresponding to `version_name`.
    pub fn symbols_realized(
        &self,
        version_name: &str,
    ) -> impl Iterator<Item = RealizedSymbol> + '_ {
        let v = String::from(version_name);
        self.blocks().flat_map(move |b| b.iter_realized(&v))
    }
    /// Returns a flat [`Iterator`] over all function symbols contained within every [`Block`] in
    /// the [`SymGen`], realized for the [`Version`] corresponding to `version_name`.
    pub fn functions_realized(
        &self,
        version_name: &str,
    ) -> impl Iterator<Item = RealizedSymbol> + '_ {
        let v = String::from(version_name);
        self.blocks().flat_map(move |b| b.functions_realized(&v))
    }
    /// Returns a flat [`Iterator`] over all data symbols contained within every [`Block`] in
    /// the [`SymGen`], realized for the [`Version`] corresponding to `version_name`.
    pub fn data_realized(&self, version_name: &str) -> impl Iterator<Item = RealizedSymbol> + '_ {
        let v = String::from(version_name);
        self.blocks().flat_map(move |b| b.data_realized(&v))
    }

    /// Returns a [`SymGenCursor`] for this [`SymGen`] with the given file path.
    pub fn cursor<'s, 'p>(&'s self, path: &'p Path) -> SymGenCursor<'s, 'p> {
        SymGenCursor::new(self, Cow::Borrowed(path))
    }
}

impl<const N: usize> From<[(OrdString, Block); N]> for SymGen {
    fn from(arr: [(OrdString, Block); N]) -> Self {
        SymGen(BTreeMap::from(arr))
    }
}

impl Display for SymGen {
    fn fmt(&self, f: &mut Formatter) -> fmt::Result {
        let string = self.write_to_str(IntFormat::Hexadecimal);
        match string {
            Ok(s) => write!(f, "{}", s),
            Err(e) => write!(f, "{}", e),
        }
    }
}

impl Sort for SymGen {
    // Note: only applies to the blocks. As a BTreeMap, block keys will always be sorted.
    fn sort(&mut self) {
        // Sort each block
        for block in self.0.values_mut() {
            block.sort();
        }
    }
}

/// A subsidiary [`SymGen`] (a collection of named [`Block`]s) nested within a parent [`Block`].
///
/// A minimal [`Subregion`] consists of just a file name, which may or may not correspond to a
/// valid file. A [`Subregion`] can be "resolved" by associating a concrete [`SymGen`] with it,
/// typically by reading the contents of a file corresponding to the [`Subregion`]'s name.
/// The contents of a resolved [`Subregion`] are logically grouped together, but are ultimately
/// owned by the parent [`Block`].
#[derive(Debug, PartialEq, Eq, Clone, Serialize, Deserialize)]
#[serde(transparent)]
pub struct Subregion {
    pub name: PathBuf,
    // This doesn't actually need to be a Box to compile, but it's more space-efficient for the
    // common case of an unresolved subregion. It also allows for the null pointer optimization.
    #[serde(skip)]
    pub contents: Option<Box<SymGen>>,
}

impl Subregion {
    /// Get the canonical directory containing the subregion files for a given parent file path.
    pub fn subregion_dir<P: AsRef<Path>>(filepath: P) -> PathBuf {
        filepath.as_ref().with_extension("")
    }

    /// Whether this [`Subregion`] is associated with a concrete [`SymGen`].
    pub fn is_resolved(&self) -> bool {
        self.contents.is_some()
    }

    /// Resolve this [`Subregion`] with the contents of a file.
    ///
    /// The file is read using `file_opener`, with the file path derived from the directory
    /// specified by `dir_path` and the [`Subregion`]'s name.
    pub fn resolve<P, R, F>(&mut self, dir_path: P, file_opener: F) -> Result<()>
    where
        P: AsRef<Path>,
        R: Read,
        F: Fn(&Path) -> io::Result<R> + Copy,
    {
        if self.name.components().count() != 1 {
            return Err(Error::Subregion(SubregionError::InvalidPath(
                self.name.clone(),
            )));
        }
        let filepath = dir_path.as_ref().join(&self.name);
        let rdr = file_opener(&filepath).map_err(|e| {
            Error::Subregion(SubregionError::SymGen((
                filepath.clone(),
                Box::new(Error::Io(e)),
            )))
        })?;
        self.contents = Some(Box::new(SymGen::read(rdr).map_err(|e| {
            Error::Subregion(SubregionError::SymGen((filepath.clone(), Box::new(e))))
        })?));
        Ok(())
    }
    /// Unresolves this [`Subregion`] by discarding its contents, if any.
    pub fn unresolve(&mut self) {
        self.contents = None;
    }
}

impl<P> From<P> for Subregion
where
    P: AsRef<Path>,
{
    fn from(val: P) -> Self {
        // unresolved subregion
        Subregion {
            name: val.as_ref().to_owned(),
            contents: None,
        }
    }
}

impl<P> PartialEq<P> for Subregion
where
    P: AsRef<Path>,
{
    fn eq(&self, other: &P) -> bool {
        self.name == other.as_ref()
    }
}

impl PartialOrd for Subregion {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Ord for Subregion {
    fn cmp(&self, other: &Self) -> Ordering {
        self.name.cmp(&other.name)
    }
}

#[cfg(test)]
pub mod test_utils {
    use super::*;
    use std::collections::HashMap;
    use std::io;
    use std::path::{Path, PathBuf};

    pub fn get_symgen_with_subregions<P: AsRef<Path>>(
        root: &str,
        subregions: &[(P, &str)],
    ) -> SymGen {
        let mut symgen = SymGen::read(root.as_bytes()).expect("Failed to read SymGen");
        let root_dir = Path::new(file!());
        let file_map: HashMap<PathBuf, String> = subregions
            .iter()
            .map(|(p, s)| (root_dir.join(p.as_ref()), s.to_string()))
            .collect();
        symgen
            .resolve_subregions(root_dir, |p| {
                file_map
                    .get(p)
                    .map(|s| s.as_bytes())
                    .ok_or_else(|| io::Error::new(io::ErrorKind::NotFound, p.to_string_lossy()))
            })
            .expect("Failed to resolve subregions");
        symgen
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[cfg(test)]
    mod symbol_tests {
        use super::*;

        #[test]
        fn test_init_sort() {
            let versions = ["SI", "imperial", "natural"];
            let version_order = OrdString::get_order_map(Some(&versions));
            let ctx = BlockContext { version_order };

            let mut symbol = Symbol {
                name: "c".to_string(),
                aliases: Some(vec!["speed_of_light".to_string(), "lightspeed".to_string()]),
                address: MaybeVersionDep::ByVersion(
                    [
                        ("SI".into(), Linkable::from([1080000000, 299792458])),
                        ("natural".into(), Linkable::from(1)),
                        ("imperial".into(), Linkable::from([671000000, 186000])),
                    ]
                    .into(),
                ),
                length: Some(MaybeVersionDep::ByVersion(
                    [
                        ("natural".into(), 200),
                        ("imperial".into(), 300),
                        ("SI".into(), 100),
                    ]
                    .into(),
                )),
                description: Some("the speed of light".to_string()),
            };
            symbol.init(&ctx);
            symbol.sort();

            assert_eq!(
                &symbol,
                &Symbol {
                    name: "c".to_string(),
                    aliases: Some(vec!["lightspeed".to_string(), "speed_of_light".to_string()]),
                    address: MaybeVersionDep::ByVersion(
                        [
                            (("SI", 0).into(), Linkable::from([299792458, 1080000000])),
                            (("imperial", 1).into(), Linkable::from([186000, 671000000])),
                            (("natural", 2).into(), Linkable::from(1)),
                        ]
                        .into()
                    ),
                    length: Some(MaybeVersionDep::ByVersion(
                        [
                            (("SI", 0).into(), 100),
                            (("imperial", 1).into(), 300),
                            (("natural", 2).into(), 200),
                        ]
                        .into()
                    )),
                    description: Some("the speed of light".to_string()),
                }
            );
        }

        #[test]
        fn test_expand_versions() {
            let versions = [
                Version::from(("NA", 0)),
                Version::from(("EU", 1)),
                Version::from(("JP", 2)),
            ];
            let version_order = OrdString::get_order_map(Some(&versions));
            let ctx = BlockContext { version_order };
            let address = MaybeVersionDep::ByVersion(
                [
                    (versions[0].clone(), Linkable::from(0x2100000)),
                    (versions[1].clone(), Linkable::from(0x2100c00)),
                ]
                .into(),
            );
            let mut function = Symbol {
                name: "function".to_string(),
                aliases: None,
                address: address.clone(),
                length: Some(MaybeVersionDep::Common(0x100)),
                description: None,
            };
            function.init(&ctx);
            function.sort();
            function.expand_versions(&versions);

            assert_eq!(
                &function,
                &Symbol {
                    name: "function".to_string(),
                    aliases: None,
                    address: address.clone(),
                    length: Some(MaybeVersionDep::ByVersion(
                        [
                            (versions[0].clone(), 0x100),
                            (versions[1].clone(), 0x100),
                            (versions[2].clone(), 0x100),
                        ]
                        .into()
                    )),
                    description: None,
                }
            )
        }

        #[test]
        fn test_extents() {
            let versions = [
                Version::from(("NA", 0)),
                Version::from(("EU", 1)),
                Version::from(("JP", 1)),
            ];
            let function1 = Symbol {
                name: "function1".to_string(),
                aliases: None,
                address: MaybeVersionDep::ByVersion(
                    [
                        (versions[0].clone(), Linkable::from([0x2100000, 0x2100100])),
                        (versions[1].clone(), Linkable::from(0x2100c00)),
                    ]
                    .into(),
                ),
                length: Some(MaybeVersionDep::ByVersion(
                    [(versions[0].clone(), 0x100), (versions[2].clone(), 0x200)].into(),
                )),
                description: None,
            };
            let expected_extents = MaybeVersionDep::ByVersion(
                [
                    (
                        versions[0].clone(),
                        (Linkable::from([0x2100000, 0x2100100]), Some(0x100)),
                    ),
                    (versions[1].clone(), (Linkable::from(0x2100c00), None)),
                ]
                .into(),
            );
            assert_eq!(&function1.extents(Some(&versions)), &expected_extents);
            assert_eq!(&function1.extents(None), &expected_extents);

            // Weird edge case where we have versions for length but not address
            let function2 = Symbol {
                name: "function2".to_string(),
                aliases: None,
                address: MaybeVersionDep::Common(Linkable::from(0x2100000)),
                length: Some(MaybeVersionDep::ByVersion(
                    [(versions[0].clone(), 0x100), (versions[1].clone(), 0x200)].into(),
                )),
                description: None,
            };
            assert_eq!(
                &function2.extents(Some(&versions)),
                &MaybeVersionDep::ByVersion(
                    [
                        (
                            versions[0].clone(),
                            (Linkable::from(0x2100000), Some(0x100)),
                        ),
                        (
                            versions[1].clone(),
                            (Linkable::from(0x2100000), Some(0x200))
                        ),
                        (versions[2].clone(), (Linkable::from(0x2100000), None)),
                    ]
                    .into(),
                )
            );
            assert_eq!(
                &function2.extents(None),
                &MaybeVersionDep::ByVersion(
                    [
                        (
                            versions[0].clone(),
                            (Linkable::from(0x2100000), Some(0x100)),
                        ),
                        (
                            versions[1].clone(),
                            (Linkable::from(0x2100000), Some(0x200))
                        ),
                        // version[2] is missing!
                    ]
                    .into(),
                )
            );
        }

        #[test]
        fn test_cmp() {
            let versions = ["NA", "EU", "JP"];
            let version_order = OrdString::get_order_map(Some(&versions));
            let ctx = BlockContext { version_order };

            let mut function1 = Symbol {
                name: "function1".to_string(),
                aliases: None,
                address: MaybeVersionDep::ByVersion(
                    [
                        ("NA".into(), Linkable::from(0x2100000)),
                        ("EU".into(), Linkable::from(0x2100c00)),
                    ]
                    .into(),
                ),
                length: None,
                description: None,
            };
            function1.init(&ctx);
            function1.sort();

            let mut function2 = Symbol {
                name: "function2".to_string(),
                aliases: None,
                address: MaybeVersionDep::ByVersion(
                    [
                        ("NA".into(), Linkable::from(0x2101000)),
                        ("EU".into(), Linkable::from(0x2101c00)),
                        ("JP".into(), Linkable::from(0x2100e00)),
                    ]
                    .into(),
                ),
                length: None,
                description: None,
            };
            function2.init(&ctx);
            function2.sort();

            assert_eq!(function1.cmp(&function2), Ordering::Less)
        }
    }

    /// Returns a tuple of (uninited version list, final version list, uninited block addresses,
    /// final block addresses, uninited SymbolList, final SymbolList)
    fn get_block_data() -> (
        Vec<Version>,
        Vec<Version>,
        MaybeVersionDep<Uint>,
        MaybeVersionDep<Uint>,
        SymbolList,
        SymbolList,
    ) {
        (
            vec!["NA".into(), "EU".into()],
            vec![("NA", 0).into(), ("EU", 1).into()],
            MaybeVersionDep::ByVersion([("NA".into(), 0x2000000), ("EU".into(), 0x2000004)].into()),
            MaybeVersionDep::ByVersion(
                [(("NA", 0).into(), 0x2000000), (("EU", 1).into(), 0x2000004)].into(),
            ),
            SymbolList::from([
                Symbol {
                    name: "function2".to_string(),
                    aliases: None,
                    address: MaybeVersionDep::Common(Linkable::from([0x2101000, 0x2101100])),
                    length: None,
                    description: None,
                },
                Symbol {
                    name: "function1".to_string(),
                    aliases: Some(vec!["function1_alias".to_string()]),
                    address: MaybeVersionDep::ByVersion(
                        [
                            ("EU".into(), Linkable::from(0x2100c00)),
                            ("NA".into(), Linkable::from([0x2100100, 0x2100000])),
                        ]
                        .into(),
                    ),
                    length: Some(MaybeVersionDep::Common(0x100)),
                    description: None,
                },
            ]),
            SymbolList::from([
                Symbol {
                    name: "function1".to_string(),
                    aliases: Some(vec!["function1_alias".to_string()]),
                    address: MaybeVersionDep::ByVersion(
                        [
                            (("NA", 0).into(), Linkable::from([0x2100000, 0x2100100])),
                            (("EU", 1).into(), Linkable::from(0x2100c00)),
                        ]
                        .into(),
                    ),
                    length: Some(MaybeVersionDep::Common(0x100)),
                    description: None,
                },
                Symbol {
                    name: "function2".to_string(),
                    aliases: None,
                    address: MaybeVersionDep::Common(Linkable::from([0x2101000, 0x2101100])),
                    length: None,
                    description: None,
                },
            ]),
        )
    }

    #[cfg(test)]
    mod symbol_list_tests {
        use super::*;

        #[test]
        fn test_init_sort() {
            let (versions, _, _, _, mut list, final_list) = get_block_data();
            let version_order = OrdString::get_order_map(Some(&versions));
            let ctx = BlockContext { version_order };

            list.init(&ctx);
            list.sort();

            assert_eq!(&list, &final_list);
        }

        #[test]
        fn test_rangeset_from() {
            let rangeset = RangeSet::from(vec![
                (11, 19),
                (10, 20),   // fully subsume (11, 19)
                (15, 18),   // fully within (10, 20)
                (5, 12),    // extend (10, 20) leftwards
                (15, 30),   // extend (5, 20) rightwards
                (40, 50),   // disjoint
                (100, 200), // disjoint
                (1, 55),    // fully subsume (5, 30) + (40, 50)
                (56, 60),   // extend (1, 55) rightwards
            ]);
            assert_eq!(&rangeset, &RangeSet(vec![(1, 60), (100, 200),]));
        }

        #[test]
        fn test_rangeset_contains() {
            let rangeset = RangeSet(vec![(1, 60), (100, 200)]);
            assert!(!rangeset.contains(0));
            assert!(rangeset.contains(1));
            assert!(rangeset.contains(30));
            assert!(rangeset.contains(60));
            assert!(!rangeset.contains(75));
            assert!(rangeset.contains(100));
            assert!(rangeset.contains(150));
            assert!(rangeset.contains(200));
            assert!(!rangeset.contains(300));
        }

        fn make_symbol_list<const N: usize>(
            list: [(&str, MaybeVersionDep<Linkable>); N],
        ) -> SymbolList {
            let mut versions = BTreeSet::new();
            for i in list.iter() {
                for v in i.1.versions() {
                    versions.insert(v);
                }
            }
            let versions: Vec<_> = versions.into_iter().collect();
            let version_order = OrdString::get_order_map(Some(&versions));
            let ctx = BlockContext { version_order };

            let mut slist = SymbolList(
                list.iter()
                    .map(|i| Symbol {
                        name: i.0.to_string(),
                        aliases: None,
                        address: i.1.clone(),
                        length: None,
                        description: None,
                    })
                    .collect(),
            );
            slist.init(&ctx);
            slist
        }

        fn assert_sort_order<const N: usize>(
            list: [(&str, MaybeVersionDep<Linkable>); N],
            list_sorted: [(&str, MaybeVersionDep<Linkable>); N],
        ) {
            let mut list = make_symbol_list(list);
            let list_sorted = make_symbol_list(list_sorted);
            list.sort();
            assert_eq!(&list, &list_sorted);
        }

        #[test]
        fn test_sort_common() {
            assert_sort_order(
                [
                    ("symbol3", MaybeVersionDep::Common(3.into())),
                    ("symbol1", MaybeVersionDep::Common(1.into())),
                    ("symbol2", MaybeVersionDep::Common(2.into())),
                ],
                [
                    ("symbol1", MaybeVersionDep::Common(1.into())),
                    ("symbol2", MaybeVersionDep::Common(2.into())),
                    ("symbol3", MaybeVersionDep::Common(3.into())),
                ],
            );
        }

        #[test]
        fn test_sort_multipass() {
            assert_sort_order(
                [
                    (
                        "symbol1",
                        MaybeVersionDep::ByVersion(
                            [
                                ("v1".into(), 0.into()),
                                ("v2".into(), 1.into()),
                                ("v3".into(), 2.into()),
                            ]
                            .into(),
                        ),
                    ),
                    (
                        "symbol3",
                        MaybeVersionDep::ByVersion(
                            [("v1".into(), 10.into()), ("v2".into(), 11.into())].into(),
                        ),
                    ),
                    (
                        "symbol5",
                        MaybeVersionDep::ByVersion(
                            [
                                ("v1".into(), 20.into()),
                                ("v2".into(), 21.into()),
                                ("v3".into(), 22.into()),
                            ]
                            .into(),
                        ),
                    ),
                    (
                        "symbol0",
                        MaybeVersionDep::ByVersion([("v2".into(), 0.into())].into()),
                    ),
                    (
                        "symbol6",
                        MaybeVersionDep::ByVersion([("v2".into(), 100.into())].into()),
                    ),
                    (
                        "symbol2",
                        MaybeVersionDep::ByVersion([("v2".into(), 1.into())].into()),
                    ),
                    (
                        "symbol4",
                        MaybeVersionDep::ByVersion([("v3".into(), 15.into())].into()),
                    ),
                ],
                [
                    (
                        "symbol0",
                        MaybeVersionDep::ByVersion([("v2".into(), 0.into())].into()),
                    ),
                    (
                        "symbol1",
                        MaybeVersionDep::ByVersion(
                            [
                                ("v1".into(), 0.into()),
                                ("v2".into(), 1.into()),
                                ("v3".into(), 2.into()),
                            ]
                            .into(),
                        ),
                    ),
                    (
                        "symbol2",
                        MaybeVersionDep::ByVersion([("v2".into(), 1.into())].into()),
                    ),
                    (
                        "symbol3",
                        MaybeVersionDep::ByVersion(
                            [("v1".into(), 10.into()), ("v2".into(), 11.into())].into(),
                        ),
                    ),
                    (
                        "symbol4",
                        MaybeVersionDep::ByVersion([("v3".into(), 15.into())].into()),
                    ),
                    (
                        "symbol5",
                        MaybeVersionDep::ByVersion(
                            [
                                ("v1".into(), 20.into()),
                                ("v2".into(), 21.into()),
                                ("v3".into(), 22.into()),
                            ]
                            .into(),
                        ),
                    ),
                    (
                        "symbol6",
                        MaybeVersionDep::ByVersion([("v2".into(), 100.into())].into()),
                    ),
                ],
            );
        }

        #[test]
        fn test_sort_multipass_conflict() {
            assert_sort_order(
                [
                    (
                        "symbol1",
                        MaybeVersionDep::ByVersion(
                            [
                                ("v1".into(), 0.into()),
                                ("v2".into(), 1.into()),
                                ("v3".into(), 2.into()),
                            ]
                            .into(),
                        ),
                    ),
                    (
                        "symbol2",
                        MaybeVersionDep::ByVersion(
                            [("v1".into(), 10.into()), ("v2".into(), 0.into())].into(),
                        ),
                    ),
                    (
                        "symbol4",
                        MaybeVersionDep::ByVersion(
                            [
                                ("v1".into(), 20.into()),
                                ("v2".into(), 21.into()),
                                ("v3".into(), 22.into()),
                            ]
                            .into(),
                        ),
                    ),
                    (
                        "symbol5",
                        MaybeVersionDep::ByVersion(
                            [
                                ("v1".into(), 30.into()),
                                ("v2".into(), 20.into()),
                                ("v3".into(), 32.into()),
                            ]
                            .into(),
                        ),
                    ),
                    (
                        "symbol6",
                        MaybeVersionDep::ByVersion([("v2".into(), 1.into())].into()),
                    ),
                    (
                        "symbol3a",
                        MaybeVersionDep::ByVersion([("v2".into(), 10.into())].into()),
                    ),
                    (
                        "symbol3b",
                        MaybeVersionDep::ByVersion([("v3".into(), 15.into())].into()),
                    ),
                    (
                        "symbol7",
                        MaybeVersionDep::ByVersion([("v2".into(), 20.into())].into()),
                    ),
                ],
                [
                    (
                        "symbol1",
                        MaybeVersionDep::ByVersion(
                            [
                                ("v1".into(), 0.into()),
                                ("v2".into(), 1.into()),
                                ("v3".into(), 2.into()),
                            ]
                            .into(),
                        ),
                    ),
                    (
                        "symbol2",
                        MaybeVersionDep::ByVersion(
                            [("v1".into(), 10.into()), ("v2".into(), 0.into())].into(),
                        ),
                    ),
                    (
                        "symbol3a",
                        MaybeVersionDep::ByVersion([("v2".into(), 10.into())].into()),
                    ),
                    (
                        "symbol3b",
                        MaybeVersionDep::ByVersion([("v3".into(), 15.into())].into()),
                    ),
                    (
                        "symbol4",
                        MaybeVersionDep::ByVersion(
                            [
                                ("v1".into(), 20.into()),
                                ("v2".into(), 21.into()),
                                ("v3".into(), 22.into()),
                            ]
                            .into(),
                        ),
                    ),
                    (
                        "symbol5",
                        MaybeVersionDep::ByVersion(
                            [
                                ("v1".into(), 30.into()),
                                ("v2".into(), 20.into()),
                                ("v3".into(), 32.into()),
                            ]
                            .into(),
                        ),
                    ),
                    (
                        "symbol6",
                        // Conflicts with symbol1/symbol2
                        MaybeVersionDep::ByVersion([("v2".into(), 1.into())].into()),
                    ),
                    (
                        "symbol7",
                        // Conflicts with symbol4/symbol5
                        MaybeVersionDep::ByVersion([("v2".into(), 20.into())].into()),
                    ),
                ],
            );
        }

        #[test]
        fn test_sort_multipass_conflict_cascade() {
            assert_sort_order(
                [
                    (
                        "symbol1",
                        MaybeVersionDep::ByVersion(
                            [
                                ("v1".into(), 0.into()),
                                ("v2".into(), 1.into()),
                                ("v3".into(), 2.into()),
                            ]
                            .into(),
                        ),
                    ),
                    (
                        "symbol3",
                        MaybeVersionDep::ByVersion(
                            [
                                ("v1".into(), 10.into()),
                                ("v2".into(), 0.into()),
                                ("v3".into(), 12.into()),
                            ]
                            .into(),
                        ),
                    ),
                    (
                        "symbol5",
                        MaybeVersionDep::ByVersion(
                            [
                                ("v1".into(), 20.into()),
                                ("v2".into(), 21.into()),
                                ("v3".into(), 22.into()),
                            ]
                            .into(),
                        ),
                    ),
                    (
                        "symbol6",
                        MaybeVersionDep::ByVersion(
                            [("v2".into(), 1.into()), ("v3".into(), 13.into())].into(),
                        ),
                    ),
                    (
                        "symbol4",
                        MaybeVersionDep::ByVersion([("v2".into(), 10.into())].into()),
                    ),
                    (
                        "symbol7",
                        MaybeVersionDep::ByVersion([("v3".into(), 15.into())].into()),
                    ),
                    (
                        "symbol2",
                        MaybeVersionDep::ByVersion([("v3".into(), 10.into())].into()),
                    ),
                ],
                [
                    (
                        "symbol1",
                        MaybeVersionDep::ByVersion(
                            [
                                ("v1".into(), 0.into()),
                                ("v2".into(), 1.into()),
                                ("v3".into(), 2.into()),
                            ]
                            .into(),
                        ),
                    ),
                    (
                        "symbol2",
                        MaybeVersionDep::ByVersion([("v3".into(), 10.into())].into()),
                    ),
                    (
                        "symbol3",
                        MaybeVersionDep::ByVersion(
                            [
                                ("v1".into(), 10.into()),
                                ("v2".into(), 0.into()),
                                ("v3".into(), 12.into()),
                            ]
                            .into(),
                        ),
                    ),
                    (
                        "symbol4",
                        MaybeVersionDep::ByVersion([("v2".into(), 10.into())].into()),
                    ),
                    (
                        "symbol5",
                        MaybeVersionDep::ByVersion(
                            [
                                ("v1".into(), 20.into()),
                                ("v2".into(), 21.into()),
                                ("v3".into(), 22.into()),
                            ]
                            .into(),
                        ),
                    ),
                    (
                        "symbol6",
                        // Conflicts with symbol1/symbol3
                        MaybeVersionDep::ByVersion(
                            [("v2".into(), 1.into()), ("v3".into(), 13.into())].into(),
                        ),
                    ),
                    (
                        "symbol7",
                        // Conflicts with symbol5/symbol6
                        MaybeVersionDep::ByVersion([("v3".into(), 15.into())].into()),
                    ),
                ],
            );
        }

        #[test]
        fn test_iter_realize() {
            let (_, versions, _, _, _, list) = get_block_data();

            let mut iter0 = list.iter().realize(Some(&versions[0]));
            let function1_aliases = ["function1_alias".to_string()];
            let exp0 = [
                RealizedSymbol {
                    name: &"function1",
                    aliases: Some(&function1_aliases),
                    address: 0x2100000,
                    length: Some(0x100),
                    description: None,
                },
                RealizedSymbol {
                    name: &"function1",
                    aliases: Some(&function1_aliases),
                    address: 0x2100100,
                    length: Some(0x100),
                    description: None,
                },
                RealizedSymbol {
                    name: &"function2",
                    aliases: None,
                    address: 0x2101000,
                    length: None,
                    description: None,
                },
                RealizedSymbol {
                    name: &"function2",
                    aliases: None,
                    address: 0x2101100,
                    length: None,
                    description: None,
                },
            ];
            for e in exp0.iter() {
                assert_eq!(iter0.next().as_ref(), Some(e));
            }
            assert_eq!(iter0.next(), None);

            let mut iter1 = list.iter().realize(Some(&versions[1]));
            let function1_aliases = ["function1_alias".to_string()];
            let exp1 = [
                RealizedSymbol {
                    name: &"function1",
                    aliases: Some(&function1_aliases),
                    address: 0x2100c00,
                    length: Some(0x100),
                    description: None,
                },
                RealizedSymbol {
                    name: &"function2",
                    aliases: None,
                    address: 0x2101000,
                    length: None,
                    description: None,
                },
                RealizedSymbol {
                    name: &"function2",
                    aliases: None,
                    address: 0x2101100,
                    length: None,
                    description: None,
                },
            ];
            for e in exp1.iter() {
                assert_eq!(iter1.next().as_ref(), Some(e));
            }
            assert_eq!(iter1.next(), None);
        }

        #[test]
        fn test_iter_realize_with_none() {
            let (_, _, _, _, _, list) = get_block_data();
            let mut iter = list.iter().realize(None);
            let exp = [
                RealizedSymbol {
                    name: &"function2",
                    aliases: None,
                    address: 0x2101000,
                    length: None,
                    description: None,
                },
                RealizedSymbol {
                    name: &"function2",
                    aliases: None,
                    address: 0x2101100,
                    length: None,
                    description: None,
                },
            ];
            for e in exp.iter() {
                assert_eq!(iter.next().as_ref(), Some(e));
            }
            assert_eq!(iter.next(), None);
        }
    }

    #[cfg(test)]
    mod block_tests {
        use super::*;

        fn get_sorted_block() -> Block {
            let (versions, _, addresses, _, symbols, _) = get_block_data();

            let mut block = Block {
                versions: Some(versions),
                address: addresses.clone(),
                length: addresses.clone(),
                description: None,
                subregions: None,
                functions: symbols.clone(),
                data: symbols.clone(),
            };
            block.init();
            block.sort();
            block
        }

        #[test]
        fn test_init_sort() {
            let mut block = get_sorted_block();
            // Add some subregions manually
            block.subregions = Some(vec!["subregion2".into(), "subregion1".into()]);
            block.sort();

            let (_, final_versions, _, final_addresses, _, final_symbols) = get_block_data();
            let mut final_subregions = block.subregions.clone().unwrap();
            final_subregions.sort();
            assert_eq!(
                &block,
                &Block {
                    versions: Some(final_versions),
                    address: final_addresses.clone(),
                    length: final_addresses.clone(),
                    description: None,
                    subregions: Some(final_subregions.clone()),
                    functions: final_symbols.clone(),
                    data: final_symbols.clone(),
                }
            )
        }

        #[test]
        fn test_expand_versions() {
            let mut block = get_sorted_block();
            let versions = block.versions.clone();
            let description = block.description.clone();
            let address = block.address.clone();
            let length = block.length.clone();
            let expanded_symbols = SymbolList::from([
                Symbol {
                    name: "function1".to_string(),
                    aliases: Some(vec!["function1_alias".to_string()]),
                    address: MaybeVersionDep::ByVersion(
                        [
                            (("NA", 0).into(), Linkable::from([0x2100000, 0x2100100])),
                            (("EU", 1).into(), Linkable::from(0x2100c00)),
                        ]
                        .into(),
                    ),
                    length: Some(MaybeVersionDep::ByVersion(
                        [(("NA", 0).into(), 0x100), (("EU", 1).into(), 0x100)].into(),
                    )),
                    description: None,
                },
                Symbol {
                    name: "function2".to_string(),
                    aliases: None,
                    address: MaybeVersionDep::ByVersion(
                        [
                            (("NA", 0).into(), Linkable::from([0x2101000, 0x2101100])),
                            (("EU", 1).into(), Linkable::from([0x2101000, 0x2101100])),
                        ]
                        .into(),
                    ),
                    length: None,
                    description: None,
                },
            ]);
            block.expand_versions();
            assert_eq!(
                &block,
                &Block {
                    versions,
                    address,
                    length,
                    description,
                    subregions: None,
                    functions: expanded_symbols.clone(),
                    data: expanded_symbols.clone(),
                }
            )
        }

        #[test]
        fn test_extent() {
            let block = get_sorted_block();
            assert_eq!(
                &block.extent(),
                &MaybeVersionDep::ByVersion(
                    [
                        (("NA", 0).into(), (0x2000000, Some(0x2000000))),
                        (("EU", 1).into(), (0x2000004, Some(0x2000004))),
                    ]
                    .into()
                )
            );
        }

        #[test]
        fn test_iter() {
            let block = get_sorted_block();
            let (_, _, _, _, _, final_symbols) = get_block_data();
            let mut block_iter = block.iter();
            for e in final_symbols.iter().chain(final_symbols.iter()) {
                assert_eq!(block_iter.next(), Some(e));
            }
            assert_eq!(block_iter.next(), None);
        }

        #[test]
        fn test_iter_realized() {
            let block = get_sorted_block();
            let mut iter = block.iter_realized("NA");
            let mut function_iter = block.functions_realized("NA");
            let mut data_iter = block.data_realized("NA");
            let function1_aliases = ["function1_alias".to_string()];
            let exp = [
                RealizedSymbol {
                    name: &"function1",
                    aliases: Some(&function1_aliases),
                    address: 0x2100000,
                    length: Some(0x100),
                    description: None,
                },
                RealizedSymbol {
                    name: &"function1",
                    aliases: Some(&function1_aliases),
                    address: 0x2100100,
                    length: Some(0x100),
                    description: None,
                },
                RealizedSymbol {
                    name: &"function2",
                    aliases: None,
                    address: 0x2101000,
                    length: None,
                    description: None,
                },
                RealizedSymbol {
                    name: &"function2",
                    aliases: None,
                    address: 0x2101100,
                    length: None,
                    description: None,
                },
            ];
            for e in exp.iter().chain(exp.iter()) {
                assert_eq!(iter.next().as_ref(), Some(e));
            }
            assert_eq!(iter.next(), None);

            // realizing function and data individually should work pretty much the same
            for e in exp.iter() {
                assert_eq!(function_iter.next().as_ref(), Some(e));
            }
            assert_eq!(function_iter.next(), None);

            for e in exp.iter() {
                assert_eq!(data_iter.next().as_ref(), Some(e));
            }
            assert_eq!(data_iter.next(), None);
        }

        #[test]
        fn test_iter_realized_missing_key() {
            let block = get_sorted_block();
            let mut iter = block.iter_realized("JP");
            // Should still yield the Common info.
            let exp = [
                RealizedSymbol {
                    name: &"function2",
                    aliases: None,
                    address: 0x2101000,
                    length: None,
                    description: None,
                },
                RealizedSymbol {
                    name: &"function2",
                    aliases: None,
                    address: 0x2101100,
                    length: None,
                    description: None,
                },
            ];
            for e in exp.iter().chain(exp.iter()) {
                assert_eq!(iter.next().as_ref(), Some(e));
            }
            assert_eq!(iter.next(), None);
        }
    }

    #[cfg(test)]
    mod symgen_tests {
        use super::*;

        /// Returns a tuple of (symgen string, inited+sorted SymGen)
        fn get_symgen_data() -> (String, SymGen) {
            (
                String::from(
                    r#"main:
  versions:
    - v1
    - v2
  address:
    v1: 0x2000000
    v2: 0x2000000
  length:
    v1: 0x100000
    v2: 0x100004
  description: foo
  functions:
    - name: fn1
      aliases:
        - fn1_alias
      address:
        v1: 0x2001000
        v2: 0x2002000
      length: 0x1000
      description: |-
        multi
        line
        description
    - name: fn2
      address:
        v1:
          - 0x2002000
          - 0x2003000
        v2: 0x2003000
      description: baz
  data:
    - name: SOME_DATA
      address:
        v1: 0x2000000
        v2: 0x2000000
      length:
        v1: 0x1000
        v2: 0x2000
      description: foo bar baz
other:
  address: 0x2100000
  length: 0x100000
  functions:
    - name: fn3
      address: 0x2100000
  data: []
"#,
                ),
                SymGen::from([
                    (
                        ("main", 0).into(),
                        Block {
                            versions: Some(vec![("v1", 0).into(), ("v2", 1).into()]),
                            address: MaybeVersionDep::ByVersion(
                                [(("v1", 0).into(), 0x2000000), (("v2", 1).into(), 0x2000000)]
                                    .into(),
                            ),
                            length: MaybeVersionDep::ByVersion(
                                [(("v1", 0).into(), 0x100000), (("v2", 1).into(), 0x100004)].into(),
                            ),
                            description: Some("foo".to_string()),
                            subregions: None,
                            functions: [
                                Symbol {
                                    name: "fn1".to_string(),
                                    aliases: Some(vec!["fn1_alias".to_string()]),
                                    address: MaybeVersionDep::ByVersion(
                                        [
                                            (("v1", 0).into(), 0x2001000.into()),
                                            (("v2", 1).into(), 0x2002000.into()),
                                        ]
                                        .into(),
                                    ),
                                    length: Some(MaybeVersionDep::Common(0x1000)),
                                    description: Some("multi\nline\ndescription".to_string()),
                                },
                                Symbol {
                                    name: "fn2".to_string(),
                                    aliases: None,
                                    address: MaybeVersionDep::ByVersion(
                                        [
                                            (("v1", 0).into(), [0x2002000, 0x2003000].into()),
                                            (("v2", 1).into(), 0x2003000.into()),
                                        ]
                                        .into(),
                                    ),
                                    length: None,
                                    description: Some("baz".to_string()),
                                },
                            ]
                            .into(),
                            data: [Symbol {
                                name: "SOME_DATA".to_string(),
                                aliases: None,
                                address: MaybeVersionDep::ByVersion(
                                    [
                                        (("v1", 0).into(), 0x2000000.into()),
                                        (("v2", 1).into(), 0x2000000.into()),
                                    ]
                                    .into(),
                                ),
                                length: Some(MaybeVersionDep::ByVersion(
                                    [(("v1", 0).into(), 0x1000), (("v2", 1).into(), 0x2000)].into(),
                                )),
                                description: Some("foo bar baz".to_string()),
                            }]
                            .into(),
                        },
                    ),
                    (
                        ("other", 1).into(),
                        Block {
                            versions: None,
                            address: MaybeVersionDep::Common(0x2100000),
                            length: MaybeVersionDep::Common(0x100000),
                            description: None,
                            subregions: None,
                            functions: [Symbol {
                                name: "fn3".to_string(),
                                aliases: None,
                                address: MaybeVersionDep::Common(0x2100000.into()),
                                length: None,
                                description: None,
                            }]
                            .into(),
                            data: [].into(),
                        },
                    ),
                ]),
            )
        }

        /// Same as get_symgen_data(), but with 64-bit data
        fn get_symgen_data_64bit() -> (String, SymGen) {
            (
                String::from(
                    r#"main:
  versions:
    - v1
    - v2
  address:
    v1: 0x2000000FF
    v2: 0x2000000FF
  length:
    v1: 0x100000FF
    v2: 0x100004FF
  description: foo
  functions:
    - name: fn1
      address:
        v1: 0x2001000FF
        v2: 0x2002000FF
      length: 0x1000
      description: |-
        multi
        line
        description
    - name: fn2
      address:
        v1:
          - 0x2002000FF
          - 0x2003000FF
        v2: 0x2003000FF
      description: baz
  data:
    - name: SOME_DATA
      address:
        v1: 0x2000000FF
        v2: 0x2000000FF
      length:
        v1: 0x1000
        v2: 0x2000
      description: foo bar baz
other:
  address: 0x2100000FFFF
  length: 0x100000FFFF
  functions:
    - name: fn3
      address: 0x2100000FFFF
  data: []
"#,
                ),
                SymGen::from([
                    (
                        ("main", 0).into(),
                        Block {
                            versions: Some(vec![("v1", 0).into(), ("v2", 1).into()]),
                            address: MaybeVersionDep::ByVersion(
                                [
                                    (("v1", 0).into(), 0x2000000FF),
                                    (("v2", 1).into(), 0x2000000FF),
                                ]
                                .into(),
                            ),
                            length: MaybeVersionDep::ByVersion(
                                [
                                    (("v1", 0).into(), 0x100000FF),
                                    (("v2", 1).into(), 0x100004FF),
                                ]
                                .into(),
                            ),
                            description: Some("foo".to_string()),
                            subregions: None,
                            functions: [
                                Symbol {
                                    name: "fn1".to_string(),
                                    aliases: None,
                                    address: MaybeVersionDep::ByVersion(
                                        [
                                            (("v1", 0).into(), 0x2001000FF.into()),
                                            (("v2", 1).into(), 0x2002000FF.into()),
                                        ]
                                        .into(),
                                    ),
                                    length: Some(MaybeVersionDep::Common(0x1000)),
                                    description: Some("multi\nline\ndescription".to_string()),
                                },
                                Symbol {
                                    name: "fn2".to_string(),
                                    aliases: None,
                                    address: MaybeVersionDep::ByVersion(
                                        [
                                            (("v1", 0).into(), [0x2002000FF, 0x2003000FF].into()),
                                            (("v2", 1).into(), 0x2003000FF.into()),
                                        ]
                                        .into(),
                                    ),
                                    length: None,
                                    description: Some("baz".to_string()),
                                },
                            ]
                            .into(),
                            data: [Symbol {
                                name: "SOME_DATA".to_string(),
                                aliases: None,
                                address: MaybeVersionDep::ByVersion(
                                    [
                                        (("v1", 0).into(), 0x2000000FF.into()),
                                        (("v2", 1).into(), 0x2000000FF.into()),
                                    ]
                                    .into(),
                                ),
                                length: Some(MaybeVersionDep::ByVersion(
                                    [(("v1", 0).into(), 0x1000), (("v2", 1).into(), 0x2000)].into(),
                                )),
                                description: Some("foo bar baz".to_string()),
                            }]
                            .into(),
                        },
                    ),
                    (
                        ("other", 1).into(),
                        Block {
                            versions: None,
                            address: MaybeVersionDep::Common(0x2100000FFFF),
                            length: MaybeVersionDep::Common(0x100000FFFF),
                            description: None,
                            subregions: None,
                            functions: [Symbol {
                                name: "fn3".to_string(),
                                aliases: None,
                                address: MaybeVersionDep::Common(0x2100000FFFF.into()),
                                length: None,
                                description: None,
                            }]
                            .into(),
                            data: [].into(),
                        },
                    ),
                ]),
            )
        }

        fn read_test_template<F: FnOnce() -> (String, SymGen)>(get_data: F) {
            let (input, expected) = get_data();
            let obj = SymGen::read(input.as_bytes()).expect("Read failed");
            assert_eq!(&obj, &expected);
        }

        #[test]
        fn test_read() {
            read_test_template(get_symgen_data);
        }

        #[test]
        fn test_read_64bit() {
            read_test_template(get_symgen_data_64bit);
        }

        fn write_test_template<F: FnOnce() -> (String, SymGen)>(get_data: F) {
            let (expected, input) = get_data();
            let yaml = input
                .write_to_str(IntFormat::Hexadecimal)
                .expect("Write failed");
            assert_eq!(&yaml, &expected);
        }

        #[test]
        fn test_write() {
            write_test_template(get_symgen_data);
        }

        #[test]
        fn test_write_64bit() {
            write_test_template(get_symgen_data_64bit);
        }

        #[test]
        fn test_expand_versions() {
            let (_, mut symgen) = get_symgen_data();
            let main_block_key = symgen.block_key("main").unwrap().clone();
            let other_block_key = symgen.block_key("other").unwrap().clone();
            let mut main_block = symgen.get(&main_block_key).unwrap().clone();
            let other_block = symgen.get(&other_block_key).unwrap().clone();
            main_block.expand_versions();
            // other_block doesn't have a version list so expanding shouldn't do anything
            symgen.expand_versions();
            assert_eq!(
                &symgen,
                &SymGen::from([(main_block_key, main_block), (other_block_key, other_block)])
            );
        }

        #[test]
        fn test_block_key() {
            let (_, symgen) = get_symgen_data();
            assert_eq!(symgen.block_key("main"), Some(&("main", 0).into()));
            assert_eq!(symgen.block_key("other"), Some(&("other", 1).into()));
            assert_eq!(symgen.block_key("not_a_block_name"), None);
        }

        #[test]
        fn test_iter() {
            let (_, symgen) = get_symgen_data();
            let main_block_key = symgen.block_key("main").unwrap();
            let other_block_key = symgen.block_key("other").unwrap();
            let main_block = symgen.get(main_block_key).unwrap();
            let other_block = symgen.get(other_block_key).unwrap();

            let mut iter = symgen.iter();
            assert_eq!(iter.next(), Some((main_block_key, main_block)));
            assert_eq!(iter.next(), Some((other_block_key, other_block)));
            assert_eq!(iter.next(), None);
        }

        #[test]
        fn test_symbols_realized() {
            let (_, symgen) = get_symgen_data();
            let version_str = "v1";
            let fn1_aliases = ["fn1_alias".to_string()];
            let functions_main_exp = [
                RealizedSymbol {
                    name: &"fn1",
                    aliases: Some(&fn1_aliases),
                    address: 0x2001000,
                    length: Some(0x1000),
                    description: Some(&"multi\nline\ndescription"),
                },
                RealizedSymbol {
                    name: &"fn2",
                    aliases: None,
                    address: 0x2002000,
                    length: None,
                    description: Some(&"baz"),
                },
                RealizedSymbol {
                    name: &"fn2",
                    aliases: None,
                    address: 0x2003000,
                    length: None,
                    description: Some(&"baz"),
                },
            ];
            let data_main_exp = [RealizedSymbol {
                name: &"SOME_DATA",
                aliases: None,
                address: 0x2000000,
                length: Some(0x1000),
                description: Some(&"foo bar baz"),
            }];
            let functions_other_exp = [RealizedSymbol {
                name: &"fn3",
                aliases: None,
                address: 0x2100000,
                length: None,
                description: None,
            }];

            let mut iter = symgen.symbols_realized(version_str);
            for e in functions_main_exp
                .iter()
                .chain(data_main_exp.iter())
                .chain(functions_other_exp.iter())
            {
                assert_eq!(iter.next().as_ref(), Some(e));
            }
            assert_eq!(iter.next(), None);

            // These should work basically the same
            let mut functions_iter = symgen.functions_realized(version_str);
            for e in functions_main_exp.iter().chain(functions_other_exp.iter()) {
                assert_eq!(functions_iter.next().as_ref(), Some(e));
            }
            assert_eq!(functions_iter.next(), None);

            let mut data_iter = symgen.data_realized(version_str);
            for e in data_main_exp.iter() {
                assert_eq!(data_iter.next().as_ref(), Some(e));
            }
            assert_eq!(data_iter.next(), None);
        }
    }

    #[cfg(test)]
    mod subregion_tests {
        use super::*;

        #[test]
        fn test_subregion_dir() {
            assert_eq!(Subregion::subregion_dir("test.yml"), Path::new("test"));
            assert_eq!(
                Subregion::subregion_dir("path/to/test.yml"),
                Path::new("path/to/test")
            );
            assert_eq!(
                Subregion::subregion_dir("/abs/path/to/test.yml"),
                Path::new("/abs/path/to/test")
            );
        }

        fn get_basic_subregion<P: AsRef<Path>>(name: P) -> (Subregion, String) {
            let text = format!(
                r#"{}:
                address: 0x0
                length: 0x100
                functions: []
                data: []
                "#,
                Subregion::subregion_dir(&name).display()
            );
            let sub = Subregion {
                name: name.as_ref().to_owned(),
                contents: Some(Box::new(
                    SymGen::read(text.as_bytes()).expect("Failed to read SymGen"),
                )),
            };
            (sub, text)
        }

        fn get_parent_subregion<P: AsRef<Path>>(
            name: P,
            subregions: &[(P, Subregion)],
        ) -> (Subregion, String) {
            let text = format!(
                r#"{}:
                address: 0x0
                length: 0x100
                subregions: {:?}
                functions: []
                data: []
                "#,
                Subregion::subregion_dir(&name).display(),
                subregions
                    .iter()
                    .map(|(p, _)| p.as_ref().display())
                    .collect::<Vec<_>>()
            );
            let mut sub = Subregion {
                name: name.as_ref().to_owned(),
                contents: Some(Box::new(
                    SymGen::read(text.as_bytes()).expect("Failed to read SymGen"),
                )),
            };
            sub.contents
                .as_mut()
                .unwrap()
                .blocks_mut()
                .next()
                .unwrap()
                .subregions = Some(subregions.iter().map(|(_, s)| s.clone()).collect());
            (sub, text)
        }

        #[test]
        fn test_resolve() {
            let name = "sub.yml";
            let (resolved, text) = get_basic_subregion(name);
            let mut subregion = Subregion::from(name);
            assert!(!subregion.is_resolved());
            subregion
                .resolve("", |_| Ok(text.as_bytes()))
                .expect("Failed to resolve subregion");
            assert!(subregion.is_resolved());
            assert_eq!(&subregion, &resolved);
        }

        #[test]
        fn test_invalid_path() {
            let mut subregion = Subregion::from("dir/sub.yml");
            let res = subregion.resolve("", |_| Ok("".as_bytes()));
            assert!(matches!(
                res,
                Err(Error::Subregion(SubregionError::InvalidPath(_)))
            ));
        }

        #[test]
        fn test_recursive_resolve_subregions() {
            let (name1, name2, name3) = ("sub1.yml", "sub2.yml", "sub3.yml");
            let mut symgen = SymGen::read(
                format!(
                    r#"main:
                    address: 0x0
                    length: 0x100
                    subregions:
                      - {}
                      - {}
                    functions: []
                    data: []
                    "#,
                    name1, name2
                )
                .as_bytes(),
            )
            .expect("Failed to read SymGen");
            let (sub1, text1) = get_basic_subregion(name1);
            let (sub3, text3) = get_basic_subregion(name3);
            let (sub2, text2) = get_parent_subregion(name2, &[(name3, sub3)]);
            // Use this source file path as the root_dir in order to ensure that none of the test
            // subregion paths are actually real, and thus that the recursive symlink check will
            // never be set off. Technically this depends on the working directory when the test
            // binary is run, but this should be a good enough safeguard...
            let root_dir = Path::new(file!());
            let file_map: HashMap<PathBuf, String> = [
                (root_dir.join(name1), text1),
                (root_dir.join(name2), text2),
                (
                    root_dir.join(Subregion::subregion_dir(name2)).join(name3),
                    text3,
                ),
            ]
            .into();

            symgen
                .resolve_subregions(root_dir, |p| {
                    file_map
                        .get(p)
                        .map(|s| s.as_bytes())
                        .ok_or_else(|| io::Error::new(io::ErrorKind::NotFound, p.to_string_lossy()))
                })
                .expect("Failed to resolve subregions");

            let block = symgen.blocks().next().unwrap();
            let block_subregions: Vec<&Subregion> = block
                .subregions
                .as_ref()
                .expect("Block has no subregions?")
                .iter()
                .collect();
            assert_eq!(block_subregions[0], &sub1);
            assert_eq!(block_subregions[1], &sub2);
        }

        #[test]
        fn test_recursive_collapse_subregions() {
            let (name1, name2, name3) = ("sub1.yml", "sub2.yml", "sub3.yml");
            let mut symgen = SymGen::read(
                format!(
                    r#"main:
                    address: 0x0
                    length: 0x100
                    subregions:
                      - {}
                      - {}
                    functions:
                      - name: fn0
                        address: 0x0
                    data: []
                    "#,
                    name1, name2
                )
                .as_bytes(),
            )
            .expect("Failed to read SymGen");
            let text1 = r#"sub1:
                address: 0x0
                length: 0x100
                functions: []
                data:
                  - name: data1
                    address: 0x10
                    length: 0x4
                "#;
            let text2 = r#"sub2:
                address: 0x0
                length: 0x100
                subregions:
                  - sub3.yml
                functions:
                  - name: fn2
                    address: 0x8
                data:
                  - name: data2
                    address: 0x20
                    length: 0x4
                "#;
            let text3 = r#"sub3:
                address: 0x0
                length: 0x100
                functions:
                  - name: fn3
                    address: 0xC
                data:
                  - name: data3
                    address: 0x30
                    length: 0x4
                "#;

            let collapsed_symgen = SymGen::read(
                r#"main:
                address: 0x0
                length: 0x100
                functions:
                  - name: fn0
                    address: 0x0
                  - name: fn2
                    address: 0x8
                  - name: fn3
                    address: 0xC
                data:
                  - name: data1
                    address: 0x10
                    length: 0x4
                  - name: data2
                    address: 0x20
                    length: 0x4
                  - name: data3
                    address: 0x30
                    length: 0x4
                "#
                .as_bytes(),
            )
            .expect("Failed to read SymGen");

            let root_dir = Path::new(file!());
            let file_map: HashMap<PathBuf, String> = [
                (root_dir.join(name1), text1.to_owned()),
                (root_dir.join(name2), text2.to_owned()),
                (
                    root_dir.join(Subregion::subregion_dir(name2)).join(name3),
                    text3.to_owned(),
                ),
            ]
            .into();

            symgen
                .resolve_subregions(root_dir, |p| {
                    file_map
                        .get(p)
                        .map(|s| s.as_bytes())
                        .ok_or_else(|| io::Error::new(io::ErrorKind::NotFound, p.to_string_lossy()))
                })
                .expect("Failed to resolve subregions");

            symgen.collapse_subregions();
            assert_eq!(&symgen, &collapsed_symgen);
        }
    }
}
