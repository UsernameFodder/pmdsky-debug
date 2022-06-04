//! Utilities for symbol and block bounds checking.

use super::symgen::*;
use super::types::*;

/// A violation of a bounds check.
pub struct BoundViolation {
    pub version: Option<Version>,
    /// The bound that was violated, as an offset and an optional length.
    pub bound: (Uint, Option<Uint>),
    /// The extent that does not lie within the expected bound, as an offset and an optional length.
    pub extent: (Uint, Option<Uint>),
}

/// Checks that a given extent lies within a given bound.
fn bounds_check(
    (addr, opt_len): (Uint, Option<Uint>),
    (bound_start, opt_bound_len): (Uint, Option<Uint>),
) -> bool {
    if addr < bound_start {
        return false;
    }
    if let Some(bound_len) = opt_bound_len {
        if addr >= bound_start + bound_len {
            return false;
        }
        if let Some(len) = opt_len {
            if addr + len > bound_start + bound_len {
                return false;
            }
        }
    }
    true
}

/// An type that can be checked against a range bound (addr, Option<len>) and potentially returns
/// a [`BoundViolation`].
trait Bounded {
    fn check_against_bound(
        &self,
        bound: (Uint, Option<Uint>),
        version: Option<&Version>,
    ) -> Option<BoundViolation>;
}

impl Bounded for (Linkable, Option<Uint>) {
    fn check_against_bound(
        &self,
        bound: (Uint, Option<Uint>),
        version: Option<&Version>,
    ) -> Option<BoundViolation> {
        for &addr in self.0.iter() {
            let extent = (addr, self.1);
            if !bounds_check(extent, bound) {
                return Some(BoundViolation {
                    version: version.cloned(),
                    bound,
                    extent,
                });
            }
        }
        None
    }
}

impl Bounded for (Uint, Option<Uint>) {
    fn check_against_bound(
        &self,
        bound: (Uint, Option<Uint>),
        version: Option<&Version>,
    ) -> Option<BoundViolation> {
        if !bounds_check(*self, bound) {
            Some(BoundViolation {
                version: version.cloned(),
                bound,
                extent: *self,
            })
        } else {
            None
        }
    }
}

/// Checks that the possibly version-dependent extents provided are contained within their
/// respective, possibly version-dependent bounds.
fn extents_in_bounds<B: Bounded>(
    bounds: &MaybeVersionDep<(Uint, Option<Uint>)>,
    extents: &MaybeVersionDep<B>,
) -> Option<BoundViolation> {
    match extents {
        MaybeVersionDep::ByVersion(ext) => {
            // By-version extents are the easy case: just check each version against the
            // corresponding bound if present
            for (vers, extent) in ext.iter() {
                if let Some(&bound) = bounds.get(Some(vers)) {
                    if let Some(violation) = extent.check_against_bound(bound, Some(vers)) {
                        return Some(violation);
                    }
                }
            }
        }
        MaybeVersionDep::Common(extent) => {
            // Common extents means it wasn't version-expanded, or the version list was None.
            // Check this extent against the bounds for every version (this is possible if a
            // block omits a version list but still has inferred versions based on the
            // address/length fields).
            match bounds {
                MaybeVersionDep::ByVersion(bound_by_vers) => {
                    for (vers, &bound) in bound_by_vers.iter() {
                        if let Some(violation) = extent.check_against_bound(bound, Some(vers)) {
                            return Some(violation);
                        }
                    }
                }
                MaybeVersionDep::Common(bound) => {
                    if let Some(violation) = extent.check_against_bound(*bound, None) {
                        return Some(violation);
                    }
                }
            }
        }
    }
    None
}

/// Checks that a `symbol` falls within the given `bounds` (as an offset and an optional length)
/// for the matching versions.
///
/// If `symbol` is not explicitly version-dependent, and `all_versions` is provided, the check
/// will be done for every given version.
///
/// Returns [`None`] on success, or a [`BoundViolation`] if the bound is found to be violated.
pub fn symbol_in_bounds(
    bounds: &MaybeVersionDep<(Uint, Option<Uint>)>,
    symbol: &Symbol,
    all_versions: &Option<Vec<Version>>,
) -> Option<BoundViolation> {
    extents_in_bounds(bounds, &symbol.extents(all_versions.as_deref()))
}

/// Checks that a `block` falls within the given `bounds` (as an offset and an optional length)
/// for the matching versions.
///
/// Returns [`None`] on success, or a [`BoundViolation`] if the bound is found to be violated.
pub fn block_in_bounds(
    bounds: &MaybeVersionDep<(Uint, Option<Uint>)>,
    block: &Block,
) -> Option<BoundViolation> {
    extents_in_bounds(bounds, &block.extent())
}

/// Checks that `symbol` falls within the bounds of `block`.
pub fn block_contains_symbol(block: &Block, symbol: &Symbol) -> bool {
    symbol_in_bounds(&block.extent(), symbol, &block.versions).is_none()
}

/// Checks that `other` falls within the bounds of `block`.
pub fn block_contains_block(block: &Block, other: &Block) -> bool {
    block_in_bounds(&block.extent(), other).is_none()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_bounds_check_with_lengths() {
        let cases = [
            ((2, Some(10)), (0, Some(100)), true),
            ((2, Some(98)), (0, Some(100)), true),
            ((2, Some(200)), (0, Some(100)), false),
            ((0, Some(10)), (2, Some(100)), false),
            ((100, Some(20)), (0, Some(110)), false),
        ];
        for case in cases {
            assert_eq!(bounds_check(case.0, case.1), case.2);
        }
    }

    #[test]
    fn test_bounds_check_no_lengths() {
        let cases = [((2, None), (0, None), true), ((0, None), (2, None), false)];
        for case in cases {
            assert_eq!(bounds_check(case.0, case.1), case.2);
        }
    }

    #[test]
    fn test_bounds_check_no_extent_length() {
        let cases = [
            ((2, None), (0, Some(100)), true),
            ((200, None), (0, Some(100)), false),
        ];
        for case in cases {
            assert_eq!(bounds_check(case.0, case.1), case.2);
        }
    }

    #[test]
    fn test_by_version_bounds_and_extents() {
        let cases: [(VersionDep<_>, VersionDep<_>, bool); 3] = [
            (
                [("v1".into(), (0, Some(100))), ("v2".into(), (0, Some(100)))].into(),
                [("v1".into(), (0, Some(100))), ("v2".into(), (0, Some(100)))].into(),
                true,
            ),
            (
                [("v1".into(), (0, Some(100))), ("v2".into(), (0, Some(100)))].into(),
                [("v1".into(), (0, Some(150))), ("v2".into(), (0, Some(100)))].into(),
                false,
            ),
            (
                [("v2".into(), (0, Some(100)))].into(),
                [("v1".into(), (0, Some(150))), ("v2".into(), (0, Some(100)))].into(),
                true,
            ),
        ];
        for case in cases {
            assert_eq!(
                extents_in_bounds(
                    &MaybeVersionDep::ByVersion(case.0.into()),
                    &MaybeVersionDep::ByVersion(case.1.into())
                )
                .is_none(),
                case.2
            );
        }
    }

    #[test]
    fn test_by_version_bounds_and_extents_different_ordinals() {
        let cases: [(VersionDep<_>, VersionDep<_>, bool); 3] = [
            (
                [
                    (("v1", 0).into(), (0, Some(100))),
                    (("v2", 0).into(), (0, Some(100))),
                ]
                .into(),
                [
                    (("v1", 1).into(), (0, Some(100))),
                    (("v2", 2).into(), (0, Some(100))),
                ]
                .into(),
                true,
            ),
            (
                [
                    (("v1", 0).into(), (0, Some(100))),
                    (("v2", 0).into(), (0, Some(100))),
                ]
                .into(),
                [
                    (("v1", 1).into(), (0, Some(150))),
                    (("v2", 2).into(), (0, Some(100))),
                ]
                .into(),
                false,
            ),
            (
                [(("v2", 0).into(), (0, Some(100)))].into(),
                [
                    (("v1", 1).into(), (0, Some(150))),
                    (("v2", 2).into(), (0, Some(100))),
                ]
                .into(),
                true,
            ),
        ];
        for case in cases {
            assert_eq!(
                extents_in_bounds(
                    &MaybeVersionDep::ByVersion(case.0.into()),
                    &MaybeVersionDep::ByVersion(case.1.into())
                )
                .is_none(),
                case.2
            );
        }
    }

    #[test]
    fn test_by_version_bounds_with_common_extents() {
        let cases: [(VersionDep<_>, _, bool); 3] = [
            (
                [("v1".into(), (0, Some(100))), ("v2".into(), (0, Some(100)))].into(),
                (0, Some(100)),
                true,
            ),
            (
                [("v1".into(), (0, Some(50))), ("v2".into(), (0, Some(100)))].into(),
                (0, Some(100)),
                false,
            ),
            (
                [("v1".into(), (0, Some(100))), ("v2".into(), (0, Some(50)))].into(),
                (0, Some(100)),
                false,
            ),
        ];
        for case in cases {
            assert_eq!(
                extents_in_bounds(
                    &MaybeVersionDep::ByVersion(case.0.into()),
                    &MaybeVersionDep::Common(case.1)
                )
                .is_none(),
                case.2
            );
        }
    }

    #[test]
    fn test_common_bounds_with_by_version_extents() {
        let cases: [(_, VersionDep<_>, bool); 3] = [
            (
                (0, Some(100)),
                [("v1".into(), (0, Some(100))), ("v2".into(), (0, Some(100)))].into(),
                true,
            ),
            (
                (0, Some(100)),
                [("v1".into(), (0, Some(150))), ("v2".into(), (0, Some(100)))].into(),
                false,
            ),
            (
                (0, Some(100)),
                [("v1".into(), (0, Some(100))), ("v2".into(), (0, Some(150)))].into(),
                false,
            ),
        ];
        for case in cases {
            assert_eq!(
                extents_in_bounds(
                    &MaybeVersionDep::Common(case.0),
                    &MaybeVersionDep::ByVersion(case.1.into())
                )
                .is_none(),
                case.2
            );
        }
    }

    #[test]
    fn test_common_bounds_and_extents() {
        let cases = [
            ((0, Some(100)), (0, Some(100)), true),
            ((0, Some(100)), (0, Some(150)), false),
        ];
        for case in cases {
            assert_eq!(
                extents_in_bounds(
                    &MaybeVersionDep::Common(case.0),
                    &MaybeVersionDep::Common(case.1)
                )
                .is_none(),
                case.2
            );
        }
    }

    #[test]
    fn test_extents_with_linkable() {
        let cases = [
            ((0, Some(100)), (Linkable::from([0]), Some(100)), true),
            ((0, Some(100)), (Linkable::from([0]), Some(150)), false),
            (
                (0, Some(100)),
                (Linkable::from([0, 10, 50]), Some(50)),
                true,
            ),
            ((0, Some(100)), (Linkable::from([0, 60]), Some(50)), false),
            ((0, Some(100)), (Linkable::from([100, 0]), Some(50)), false),
        ];
        for case in cases {
            assert_eq!(
                extents_in_bounds(
                    &MaybeVersionDep::Common(case.0),
                    &MaybeVersionDep::Common(case.1)
                )
                .is_none(),
                case.2
            );
        }
    }
}
