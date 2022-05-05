//! Supporting types used by the `resymgen` YAML format.

use std::cmp::{self, Ordering};
use std::collections::{btree_map::Entry, BTreeMap, HashMap};
use std::fmt::{self, Display, Formatter};
use std::iter::{self, FromIterator};
use std::ops::{Deref, DerefMut};
use std::slice;

use serde::{Deserialize, Serialize};

/// Unsigned integer type for addresses and lengths. This should be at least as large as the
/// system register size of the binary being reverse engineered.
pub type Uint = u64;
/// Map from [`String`]s to sort indexes.
pub type OrderMap = Option<HashMap<String, u64>>;

/// Types that can be sorted.
pub trait Sort {
    fn sort(&mut self);
}

impl<T, V> Sort for V
where
    V: DerefMut<Target = [T]>,
    T: Ord,
{
    fn sort(&mut self) {
        <[T]>::sort(self);
    }
}

/// For returning either a [`Once`] iterator or some other iterator, while still allowing
/// static dispatch.
pub enum OrOnce<T, I>
where
    I: Iterator<Item = T>,
{
    Iter(I),
    Once(iter::Once<T>),
}

impl<T, I> Iterator for OrOnce<T, I>
where
    I: Iterator<Item = T>,
{
    type Item = T;

    fn next(&mut self) -> Option<Self::Item> {
        match self {
            Self::Iter(i) => i.next(),
            Self::Once(o) => o.next(),
        }
    }
}

/// For returning either an `Empty` iterator or some other iterator, while still allowing
/// static dispatch.
pub enum OrEmpty<T, I>
where
    I: Iterator<Item = T>,
{
    Iter(I),
    Empty(iter::Empty<T>),
}

impl<T, I> Iterator for OrEmpty<T, I>
where
    I: Iterator<Item = T>,
{
    type Item = T;

    fn next(&mut self) -> Option<Self::Item> {
        match self {
            Self::Iter(i) => i.next(),
            Self::Empty(e) => e.next(),
        }
    }
}

/// An iterator over the data within a [`Linkable`].
pub type LinkableIter<'a> = OrOnce<&'a Uint, slice::Iter<'a, Uint>>;

/// A serializable string whose sorting order can be controlled dynamically by assigning it an
/// ordinal at runtime.
#[derive(Debug, PartialEq, Eq, PartialOrd, Ord, Hash, Clone, Serialize, Deserialize)]
#[serde(transparent)]
pub struct OrdString {
    /// An ordinal that dynamically controls the sort order or the [`OrdString`].
    #[serde(skip)]
    ord: u64,

    /// The actual [`String`] value of the [`OrdString`].
    pub val: String,
}

impl OrdString {
    /// Returns an [`OrderMap`] that preserves the order of a given array of `vals`.
    pub fn get_order_map<S>(vals: Option<&[S]>) -> OrderMap
    where
        S: ToString,
    {
        vals.map(|v| {
            v.iter()
                .enumerate()
                .map(|(idx, val)| (val.to_string(), idx as u64))
                .collect()
        })
    }
    /// Initializes the [`OrdString`] with the ordinal specified by `order_map`, or `u64::MAX`
    /// for values not contained within `order_map`.
    pub fn init(&mut self, order_map: &OrderMap) {
        // Default to a high value so unknown values get sorted last
        self.ord = u64::MAX;
        if let Some(orders) = order_map {
            if let Some(&i) = orders.get(&self.val) {
                self.ord = i;
            }
        }
    }
}

impl From<(&str, u64)> for OrdString {
    fn from(args: (&str, u64)) -> Self {
        OrdString {
            ord: args.1,
            val: args.0.to_string(),
        }
    }
}

impl From<&str> for OrdString {
    fn from(val: &str) -> Self {
        // uninitialized ord
        Self::from((val, 0))
    }
}

impl Display for OrdString {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.val)
    }
}

/// Compare against just the value of an [`OrdString`], irrespective of the ordinal.
impl<T> PartialEq<T> for OrdString
where
    T: Deref<Target = str>,
{
    fn eq(&self, other: &T) -> bool {
        self.val == other.deref()
    }
}

/// A single scalar value or a collection of multiple values.
///
/// This is kind of a bad name, but I couldn't think of anything better.
/// This enum represents a value or set of values generated by a linker. Namely, symbol addresses.
/// A symbol with external linkage will be assigned one address by the linker,
/// but a function or constant can be assigned to multiple separate addresses if defined with
/// internal linkage and included in multiple sources, which is a common pattern in C/C++.
#[derive(Debug, PartialEq, Eq, Clone, Serialize, Deserialize)]
#[serde(untagged)]
pub enum Linkable {
    Single(Uint),
    Multiple(Vec<Uint>),
}

impl Linkable {
    /// The value to use for comparisons.
    fn cmp_key(&self) -> Uint {
        match self {
            Self::Single(x) => *x,
            Self::Multiple(v) => *v.iter().min().unwrap_or(&0),
        }
    }
    /// Returns an iterator over the values within a [`Linkable`].
    ///
    /// This is defined to return a concrete type so it can be stored in struct fields.
    pub fn iter(&self) -> LinkableIter {
        match self {
            Self::Single(x) => OrOnce::Once(iter::once(x)),
            Self::Multiple(v) => OrOnce::Iter(v.iter()),
        }
    }
}

impl From<Uint> for Linkable {
    fn from(val: Uint) -> Self {
        Self::Single(val)
    }
}

impl<const N: usize> From<[Uint; N]> for Linkable {
    fn from(arr: [Uint; N]) -> Self {
        Self::Multiple(Vec::from(arr))
    }
}

impl Sort for Linkable {
    fn sort(&mut self) {
        if let Self::Multiple(m) = self {
            m.sort();
        }
    }
}

impl PartialOrd for Linkable {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl Ord for Linkable {
    fn cmp(&self, other: &Self) -> Ordering {
        self.cmp_key().cmp(&other.cmp_key())
    }
}

/// A version of a binary.
#[derive(Debug, PartialEq, Eq, PartialOrd, Ord, Hash, Clone, Serialize, Deserialize)]
pub struct Version(OrdString);

impl Version {
    /// Initialize the [`Version`] with the ordinal specified by `version_order`.
    pub fn init(&mut self, version_order: &OrderMap) {
        self.0.init(version_order)
    }
    /// Returns the name of the [`Version`].
    pub fn name(&self) -> &str {
        &self.0.val
    }
}

impl From<(&str, u64)> for Version {
    fn from(args: (&str, u64)) -> Self {
        Version(OrdString::from(args))
    }
}

impl From<&str> for Version {
    fn from(name: &str) -> Self {
        Version(OrdString::from(name))
    }
}

impl Display for Version {
    fn fmt(&self, f: &mut Formatter<'_>) -> fmt::Result {
        write!(f, "{}", self.0)
    }
}

/// Compare against just the name of a [`Version`], irrespective of the ordinal.
impl<T> PartialEq<T> for Version
where
    T: Deref<Target = str>,
{
    fn eq(&self, other: &T) -> bool {
        self.name() == other.deref()
    }
}

/// A version-dependent piece of data, which can have potentially different values for
/// different [`Version`]s.
///
/// [`VersionDep<T>`] implements a similar API to a [`HashMap<Version, T>`], including the entry API.
#[derive(Debug, PartialEq, Eq, Clone, Serialize, Deserialize)]
pub struct VersionDep<T>(BTreeMap<Version, T>);

impl<T> VersionDep<T> {
    /// Initializes the [`VersionDep<T>`] with the ordinals specified by `order_map`.
    ///
    /// Note: This assumes the value type T does not need initialization.
    pub fn init(&mut self, order_map: &OrderMap) {
        // Remove each element, init the key, and reinsert
        let keys: Vec<_> = self.0.keys().cloned().collect();
        for mut vers in keys {
            let val = self.0.remove(&vers).unwrap();
            vers.init(order_map);
            self.0.insert(vers, val);
        }
    }

    pub fn entry(&mut self, key: Version) -> Entry<Version, T> {
        self.0.entry(key)
    }
    pub fn get(&self, key: &Version) -> Option<&T> {
        self.0.get(key)
    }
    pub fn get_mut(&mut self, key: &Version) -> Option<&mut T> {
        self.0.get_mut(key)
    }
    pub fn insert(&mut self, key: Version, value: T) -> Option<T> {
        self.0.insert(key, value)
    }
    pub fn iter(&self) -> impl Iterator<Item = (&Version, &T)> {
        self.0.iter()
    }
    pub fn iter_mut(&mut self) -> impl Iterator<Item = (&Version, &mut T)> {
        self.0.iter_mut()
    }
    pub fn len(&self) -> usize {
        self.0.len()
    }
    pub fn is_empty(&self) -> bool {
        self.0.is_empty()
    }
    pub fn values(&self) -> impl Iterator<Item = &T> {
        self.0.values()
    }
    pub fn values_mut(&mut self) -> impl Iterator<Item = &mut T> {
        self.0.values_mut()
    }
    /// Returns an [`Iterator`] over all the [`Version`] keys in the [`VersionDep`].
    pub fn versions(&self) -> impl Iterator<Item = &Version> {
        self.0.keys()
    }
}

impl<T, const N: usize> From<[(Version, T); N]> for VersionDep<T> {
    fn from(arr: [(Version, T); N]) -> Self {
        VersionDep(BTreeMap::from(arr))
    }
}

impl<V> FromIterator<(Version, V)> for VersionDep<V> {
    fn from_iter<T>(iter: T) -> Self
    where
        T: IntoIterator<Item = (Version, V)>,
    {
        VersionDep(BTreeMap::from_iter(iter))
    }
}

impl<T: Sort> Sort for VersionDep<T> {
    // Note: only applies to the values. As a BTreeMap, version keys will always be sorted.
    fn sort(&mut self) {
        for val in self.0.values_mut() {
            val.sort();
        }
    }
}

impl<T: Ord> PartialOrd for VersionDep<T> {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl<T: Ord> Ord for VersionDep<T> {
    fn cmp(&self, other: &Self) -> Ordering {
        // BTreeMap comparison lexicographically compares the two as iterators of key-value tuples.
        // Since keys are unique and strictly ordered, this is equivalent to successively comparing
        // values of the same key, filling in a map's missing key with a value of infinity if it
        // still has entries left, or -infinity if it doesn't.
        self.0.cmp(&other.0)
    }
}

/// A possibly version-dependent piece of data, which can either be a single scalar value common to
/// all relevant versions, or an explicit [`VersionDep<T>`].
///
/// [`MaybeVersionDep<T>`] implements a similar but more restricted access API to
/// [`VersionDep<T>`].
#[derive(Debug, PartialEq, Eq, Clone, Serialize, Deserialize)]
#[serde(untagged)]
pub enum MaybeVersionDep<T> {
    /// A single scalar value common to all versions.
    Common(T),
    /// An explicitly version-dependent set of values.
    ByVersion(VersionDep<T>),
}

impl<T> MaybeVersionDep<T> {
    /// Initializes the [`MaybeVersionDep<T>`] with the ordinals specified by `order_map`.
    ///
    /// Note: This assumes the value type T does not need initialization.
    pub fn init(&mut self, order_map: &OrderMap) {
        if let Self::ByVersion(v) = self {
            v.init(order_map);
        }
    }
    /// Returns whether or not this [`MaybeVersionDep`] is [`Common`].
    ///
    /// [`Common`]: MaybeVersionDep::Common
    pub fn is_common(&self) -> bool {
        matches!(self, Self::Common(_))
    }

    pub fn get(&self, key: Option<&Version>) -> Option<&T> {
        match self {
            Self::Common(x) => Some(x),
            Self::ByVersion(v) => key.and_then(move |k| v.get(k)),
        }
    }
    pub fn get_mut(&mut self, key: Option<&Version>) -> Option<&mut T> {
        match self {
            Self::Common(x) => Some(x),
            Self::ByVersion(v) => key.and_then(move |k| v.get_mut(k)),
        }
    }
    pub fn len(&self) -> usize {
        match self {
            Self::Common(_) => 1,
            Self::ByVersion(v) => v.len(),
        }
    }
    pub fn is_empty(&self) -> bool {
        match self {
            Self::Common(_) => false,
            Self::ByVersion(v) => v.is_empty(),
        }
    }
    pub fn values(&self) -> impl Iterator<Item = &T> {
        match self {
            Self::Common(x) => OrOnce::Once(iter::once(x)),
            Self::ByVersion(v) => OrOnce::Iter(v.values()),
        }
    }
    pub fn values_mut(&mut self) -> impl Iterator<Item = &mut T> {
        match self {
            Self::Common(x) => OrOnce::Once(iter::once(x)),
            Self::ByVersion(v) => OrOnce::Iter(v.values_mut()),
        }
    }
    /// Returns an [`Iterator`] over all the [`Version`] keys in the [`MaybeVersionDep`].
    pub fn versions(&self) -> impl Iterator<Item = &Version> {
        match self {
            Self::Common(_) => OrEmpty::Empty(iter::empty()),
            Self::ByVersion(v) => OrEmpty::Iter(v.versions()),
        }
    }
}

impl<T: Clone> MaybeVersionDep<T> {
    /// Coerces the [`MaybeVersionDep<T>`] into a [`VersionDep<T>`].
    ///
    /// If the [`MaybeVersionDep<T>`] was [`Common`], it will be expanded to a [`VersionDep<T>`]
    /// with the version keys within `all_versions`.
    ///
    /// [`Common`]: MaybeVersionDep::Common
    pub fn by_version(&self, all_versions: &[Version]) -> VersionDep<T> {
        match self {
            Self::Common(x) => all_versions
                .iter()
                .cloned()
                .map(|v| (v, x.clone()))
                .collect(),
            Self::ByVersion(v) => v.clone(),
        }
    }
    /// The same as [`by_version()`], but mutates the [`MaybeVersionDep<T>`] into a [`ByVersion`]
    /// if it wasn't already, rather than returning a [`VersionDep<T>`].
    ///
    /// [`by_version()`]: MaybeVersionDep::by_version
    /// [`ByVersion`]: MaybeVersionDep::ByVersion
    pub fn expand_versions(&mut self, all_versions: &[Version]) {
        if self.is_common() {
            *self = Self::ByVersion(self.by_version(all_versions));
        }
    }
}

impl<T: Sort> Sort for MaybeVersionDep<T> {
    // Note: only applies to the values. As a BTreeMap, the ByVersion keys will always be sorted.
    fn sort(&mut self) {
        match self {
            Self::Common(val) => val.sort(),
            Self::ByVersion(val) => val.sort(),
        };
    }
}

impl<T: Ord> PartialOrd for MaybeVersionDep<T> {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(self.cmp(other))
    }
}

impl<T: Ord> Ord for MaybeVersionDep<T> {
    fn cmp(&self, other: &Self) -> Ordering {
        match self {
            Self::Common(val1) => {
                match other {
                    Self::Common(val2) => {
                        // Both version-independent; just compare values.
                        val1.cmp(val2)
                    }
                    Self::ByVersion(vals2) => {
                        // Compare self against all values in other.
                        // Repeat self at least so it is greater than an empty VersionDep.
                        iter::repeat(val1)
                            .take(cmp::max(vals2.0.len(), 1))
                            .cmp(vals2.0.values())
                    }
                }
            }
            Self::ByVersion(vals1) => {
                match other {
                    Self::Common(val2) => {
                        // Compare all values in self against other.
                        // Repeat other at least so it is greater than an empty VersionDep.
                        vals1
                            .0
                            .values()
                            .cmp(iter::repeat(val2).take(cmp::max(vals1.0.len(), 1)))
                    }
                    Self::ByVersion(vals2) => {
                        // Compare the version maps directly
                        vals1.cmp(vals2)
                    }
                }
            }
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn test_ord_string() {
        let versions = ["v1", "v2", "v3"];
        let map = OrdString::get_order_map(Some(&versions));
        assert_eq!(
            map.as_ref().unwrap(),
            &HashMap::from([
                ("v1".to_string(), 0),
                ("v2".to_string(), 1),
                ("v3".to_string(), 2)
            ])
        );

        let mut s2 = OrdString::from("v2");
        let mut s0 = OrdString::from("v0");
        s2.init(&map);
        s0.init(&map);
        assert_eq!(s2.ord, 1);
        assert_eq!(s0.ord, u64::MAX);
        assert_eq!(s2, "v2");
        assert_ne!(s2, "v0");
        assert_eq!(s0, "v0");
    }

    #[test]
    fn test_version_eq() {
        assert_eq!(Version::from("v1"), "v1");
    }

    #[cfg(test)]
    mod linkable_tests {
        use super::*;

        fn get_sorted_linkables() -> (Linkable, Linkable) {
            let mut single = Linkable::from(0xeeee);
            single.sort();
            let mut multiple = Linkable::from([0xffff, 0xdddd]);
            multiple.sort();
            (single, multiple)
        }

        #[test]
        fn test_sort() {
            let (single, multiple) = get_sorted_linkables();
            assert_eq!(&single, &Linkable::from(0xeeee));
            assert_eq!(&multiple, &Linkable::from([0xdddd, 0xffff]));
        }

        #[test]
        fn test_cmp() {
            let (single, multiple) = get_sorted_linkables();
            assert_eq!(single.cmp(&multiple), Ordering::Greater);
            assert_eq!(
                multiple.cmp(&Linkable::from([0xffff, 0xffff])),
                Ordering::Less
            );
        }

        #[test]
        fn test_iter() {
            let (single, multiple) = get_sorted_linkables();
            let mut single_iter = single.iter();
            assert_eq!(single_iter.next(), Some(&0xeeee));
            assert_eq!(single_iter.next(), None);
            let mut multiple_iter = multiple.iter();
            assert_eq!(multiple_iter.next(), Some(&0xdddd));
            assert_eq!(multiple_iter.next(), Some(&0xffff));
            assert_eq!(multiple_iter.next(), None);
        }
    }

    #[cfg(test)]
    mod version_dep_tests {
        use super::*;

        #[test]
        fn test_init() {
            let versions = ["v3", "v1", "v2"];
            let version_order = OrdString::get_order_map(Some(&versions));
            let mut vals =
                VersionDep::from([("v1".into(), 100), ("v2".into(), 200), ("v3".into(), 300)]);
            vals.init(&version_order);
            assert_eq!(
                &vals,
                &VersionDep::from([
                    (("v3", 0).into(), 300),
                    (("v1", 1).into(), 100),
                    (("v2", 2).into(), 200),
                ])
            );
        }

        #[test]
        fn test_sort() {
            let mut vals =
                VersionDep::from([("v1".into(), vec![3, 2, 1]), ("v2".into(), vec![2, 1, 3])]);
            vals.sort();
            assert_eq!(
                &vals,
                &VersionDep::from([("v1".into(), vec![1, 2, 3]), ("v2".into(), vec![1, 2, 3]),])
            );
        }

        #[test]
        fn test_cmp() {
            let cases = [
                (
                    VersionDep::from([("v1".into(), 1), ("v2".into(), 2)]),
                    VersionDep::from([("v1".into(), 1), ("v2".into(), 2)]),
                    Ordering::Equal,
                ),
                (
                    VersionDep::from([("v1".into(), 1), ("v2".into(), 2)]),
                    VersionDep::from([("v1".into(), 1), ("v2".into(), 3)]),
                    Ordering::Less,
                ),
                (
                    VersionDep::from([("v1".into(), 1), ("v2".into(), 2)]),
                    VersionDep::from([("v1".into(), 0), ("v2".into(), 3)]),
                    Ordering::Greater,
                ),
                (
                    VersionDep::from([("v1".into(), 1), ("v2".into(), 2)]),
                    VersionDep::from([("v1".into(), 1)]),
                    Ordering::Greater,
                ),
                (
                    VersionDep::from([("v1".into(), 1), ("v2".into(), 2)]),
                    VersionDep::from([("v2".into(), 2)]),
                    Ordering::Less,
                ),
                (
                    VersionDep::from([("v1".into(), 1), ("v3".into(), 3)]),
                    VersionDep::from([("v1".into(), 1), ("v2".into(), 2)]),
                    Ordering::Greater,
                ),
                (
                    VersionDep::from([]),
                    VersionDep::from([("v1".into(), 1), ("v2".into(), 2)]),
                    Ordering::Less,
                ),
            ];
            for (x, y, order) in cases.iter() {
                assert_eq!(x.cmp(y), *order)
            }
        }
    }

    mod maybe_version_dep_tests {
        use super::*;

        fn get_maybe_version_deps() -> (MaybeVersionDep<Uint>, MaybeVersionDep<Uint>) {
            let versions = ["v3", "v1", "v2"];
            let version_order = OrdString::get_order_map(Some(&versions));

            let mut common = MaybeVersionDep::Common(500);
            common.init(&version_order);
            let mut by_version = MaybeVersionDep::ByVersion(
                [("v1".into(), 100), ("v2".into(), 200), ("v3".into(), 300)].into(),
            );
            by_version.init(&version_order);

            (common, by_version)
        }

        #[test]
        fn test_init() {
            let (common, by_version) = get_maybe_version_deps();
            assert_eq!(&common, &MaybeVersionDep::Common(500));
            assert_eq!(
                &by_version,
                &MaybeVersionDep::ByVersion(
                    [
                        (("v3", 0).into(), 300),
                        (("v1", 1).into(), 100),
                        (("v2", 2).into(), 200),
                    ]
                    .into()
                )
            );
        }

        #[test]
        fn test_is_common() {
            let (common, by_version) = get_maybe_version_deps();
            assert!(common.is_common());
            assert!(!by_version.is_common());
        }

        #[test]
        fn test_len() {
            let (common, by_version) = get_maybe_version_deps();
            assert_eq!(common.len(), 1);
            assert_eq!(by_version.len(), 3);
        }

        #[test]
        fn test_get() {
            let (common, by_version) = get_maybe_version_deps();
            assert_eq!(common.get(None), Some(&500));
            assert_eq!(common.get(Some(&("v3", 0).into())), Some(&500));

            assert_eq!(by_version.get(None), None);
            assert_eq!(by_version.get(Some(&("v3", 0).into())), Some(&300));
        }

        #[test]
        fn test_versions() {
            let (common, by_version) = get_maybe_version_deps();
            assert_eq!(common.versions().next(), None);

            let mut by_version_vers = by_version.versions();
            assert_eq!(by_version_vers.next(), Some(&("v3", 0).into()));
            assert_eq!(by_version_vers.next(), Some(&("v1", 1).into()));
            assert_eq!(by_version_vers.next(), Some(&("v2", 2).into()));
            assert_eq!(by_version_vers.next(), None);
        }

        #[test]
        fn test_values() {
            let (common, by_version) = get_maybe_version_deps();
            let mut common_vals = common.values();
            assert_eq!(common_vals.next(), Some(&500));
            assert_eq!(common_vals.next(), None);

            let mut by_version_vals = by_version.values();
            assert_eq!(by_version_vals.next(), Some(&300));
            assert_eq!(by_version_vals.next(), Some(&100));
            assert_eq!(by_version_vals.next(), Some(&200));
            assert_eq!(by_version_vals.next(), None);
        }

        #[test]
        fn test_expand_versions() {
            let versions = [
                Version::from(("v1", 0)),
                Version::from(("v2", 1)),
                Version::from(("v3", 2)),
            ];
            let version_order = OrdString::get_order_map(Some(&versions));
            let mut common = MaybeVersionDep::Common(500);
            common.init(&version_order);
            let common_expanded = VersionDep::from([
                (versions[0].clone(), 500),
                (versions[1].clone(), 500),
                (versions[2].clone(), 500),
            ]);
            assert_eq!(&common.by_version(&versions), &common_expanded);
            common.expand_versions(&versions);
            assert_eq!(&common, &MaybeVersionDep::ByVersion(common_expanded));

            let by_version_inner =
                VersionDep::from([(versions[0].clone(), 100), (versions[1].clone(), 200)]);
            let mut by_version = MaybeVersionDep::ByVersion(by_version_inner.clone());
            by_version.init(&version_order);
            assert_eq!(&by_version.by_version(&versions), &by_version_inner);
            by_version.expand_versions(&versions);
            assert_eq!(&by_version, &MaybeVersionDep::ByVersion(by_version_inner));
        }

        #[test]
        fn test_sort() {
            let mut common = MaybeVersionDep::Common(vec![3, 2, 1]);
            common.sort();
            assert_eq!(&common, &MaybeVersionDep::Common(vec![1, 2, 3]));

            let mut by_version = MaybeVersionDep::ByVersion(
                [("v1".into(), vec![3, 2, 1]), ("v2".into(), vec![2, 1, 3])].into(),
            );
            by_version.sort();
            assert_eq!(
                &by_version,
                &MaybeVersionDep::ByVersion(
                    [("v1".into(), vec![1, 2, 3]), ("v2".into(), vec![1, 2, 3]),].into()
                )
            );
        }

        #[test]
        fn test_cmp() {
            let cases = [
                (
                    MaybeVersionDep::Common(1),
                    MaybeVersionDep::Common(1),
                    Ordering::Equal,
                ),
                (
                    MaybeVersionDep::ByVersion([("v1".into(), 1), ("v2".into(), 1)].into()),
                    MaybeVersionDep::Common(1),
                    Ordering::Equal,
                ),
                (
                    MaybeVersionDep::ByVersion([("v1".into(), 1), ("v2".into(), 2)].into()),
                    MaybeVersionDep::Common(1),
                    Ordering::Greater,
                ),
                (
                    MaybeVersionDep::ByVersion([].into()),
                    MaybeVersionDep::Common(1),
                    Ordering::Less,
                ),
            ];
            for (x, y, order) in cases.iter() {
                assert_eq!(x.cmp(y), *order)
            }
        }
    }
}
