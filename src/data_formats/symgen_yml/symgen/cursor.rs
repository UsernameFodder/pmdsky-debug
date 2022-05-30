//! Cursors for convenient immutable access to [`SymGen`]s and [`Block`]s with recursive subregions.

use std::borrow::Cow;
use std::collections::{HashSet, VecDeque};
use std::path::Path;
use std::rc::Rc;

use super::{Block, OrdString, Subregion, SymGen};

/// A cursor into a [`SymGen`] that allows traversal into nested blocks and subregions while
/// keeping track of associated nested file paths.
#[derive(Debug, PartialEq, Eq, Clone)]
pub struct SymGenCursor<'s, 'p> {
    symgen: &'s SymGen,
    path: Rc<Cow<'p, Path>>,
}

impl<'s, 'p> SymGenCursor<'s, 'p> {
    /// Creates a new [`SymGenCursor`] for the given [`SymGen`] and path.
    pub fn new(symgen: &'s SymGen, path: Cow<'p, Path>) -> Self {
        SymGenCursor {
            symgen,
            path: Rc::new(path),
        }
    }
    /// Gets the underlying [`SymGen`] associated with this cursor.
    pub fn symgen(&self) -> &'s SymGen {
        self.symgen
    }
    /// Gets the underlying path associated with this cursor.
    pub fn path(&self) -> &Path {
        &self.path
    }

    fn block_cursor(&self, name: &'s str, block: &'s Block) -> BlockCursor<'s, 'p> {
        BlockCursor {
            block,
            name,
            path: Rc::clone(&self.path),
        }
    }
    /// Gets a [`BlockCursor`] for the [`Block`] associated with `key`, if present.
    ///
    /// This function is similar to `SymGen::get`, except it returns a [`BlockCursor`] rather
    /// than a [`Block`] reference.
    pub fn get(&self, key: &'s OrdString) -> Option<BlockCursor<'s, 'p>> {
        self.symgen
            .get(key)
            .map(|block| self.block_cursor(&key.val, block))
    }
    /// Returns an [`Iterator`] over [`BlockCursor`]s for each [`Block`] in the [`SymGen`].
    ///
    /// This function is similar to `SymGen::blocks`, except it returns an iterator over
    /// [`BlockCursor`]s rather than [`Block`] references.
    pub fn blocks(&self) -> impl Iterator<Item = BlockCursor<'s, 'p>> + '_ {
        self.symgen
            .iter()
            .map(move |(bname, block)| self.block_cursor(&bname.val, block))
    }

    /// Returns an [`Iterator`] over [`SymGenCursor`]s for each resolved [`Subregion`] within any
    /// of the [`Block`]s in the [`SymGen`].
    ///
    /// If multiple [`Block`]s contain a [`Subregion`] with the same path, the iterator will only
    /// yield a cursor for the first instance of the [`Subregion`].
    pub fn subregions(&self) -> impl Iterator<Item = SymGenCursor<'s, 'p>> + '_ {
        let mut paths_seen: HashSet<Rc<Cow<'p, Path>>> = [Rc::clone(&self.path)].into();
        self.symgen
            .blocks()
            .flat_map(|block| block.subregions.as_deref().unwrap_or_default().iter())
            .filter_map(move |subregion| {
                if let Some(symgen) = &subregion.contents {
                    let path = Rc::new(Cow::Owned(
                        Subregion::subregion_dir(self.path()).join(&subregion.name),
                    ));
                    if paths_seen.insert(Rc::clone(&path)) {
                        return Some(SymGenCursor { symgen, path });
                    }
                }
                None
            })
    }
    /// Whether or not the [`SymGen`] has any [`Block`]s containing at least one resolved
    /// [`Subregion`].
    pub fn has_subregions(&self) -> bool {
        self.symgen
            .blocks()
            .flat_map(|block| block.subregions.as_deref().unwrap_or_default().iter())
            .any(|subregion| subregion.contents.is_some())
    }
    /// Returns an [`Iterator`] over [`SymGenCursor`]s for all [`SymGen`]s nested within this
    /// cursor's [`SymGen`] (i.e., in subregions), including this cursor itself.
    ///
    /// Traversal is performed in breadth-first order.
    pub fn btraverse(self) -> SymGenBTraverser<'s, 'p> {
        SymGenBTraverser {
            queue: VecDeque::from([self]),
        }
    }
    /// Returns an [`Iterator`] over [`SymGenCursor`]s for all [`SymGen`]s nested within this
    /// cursor's [`SymGen`] (i.e., in subregions), including this cursor itself.
    ///
    /// Traversal is performed in depth-first order. Note that if order is not important, `btraverse` should be preferred over `dtraverse`, as it is slightly more efficient.
    pub fn dtraverse(self) -> SymGenDTraverser<'s, 'p> {
        SymGenDTraverser { stack: vec![self] }
    }
}

/// Performs breadth-first traversal over a nested hierarchy of [`SymGen`]s.
pub struct SymGenBTraverser<'s, 'p> {
    queue: VecDeque<SymGenCursor<'s, 'p>>,
}

impl<'s, 'p> Iterator for SymGenBTraverser<'s, 'p> {
    type Item = SymGenCursor<'s, 'p>;

    fn next(&mut self) -> Option<Self::Item> {
        self.queue.pop_front().map(|cursor| {
            self.queue.extend(cursor.subregions());
            cursor
        })
    }
}

/// Performs depth-first traversal over a nested hierarchy of [`SymGen`]s.
pub struct SymGenDTraverser<'s, 'p> {
    stack: Vec<SymGenCursor<'s, 'p>>,
}

impl<'s, 'p> Iterator for SymGenDTraverser<'s, 'p> {
    type Item = SymGenCursor<'s, 'p>;

    fn next(&mut self) -> Option<Self::Item> {
        self.stack.pop().map(|cursor| {
            let subregions: Vec<_> = cursor.subregions().collect();
            self.stack.reserve(subregions.len());
            self.stack.extend(subregions.into_iter().rev());
            cursor
        })
    }
}

/// A cursor into a [`Block`] that allows traversal into nested subregions and blocks while
/// keeping track of associated block names and nested file paths.
#[derive(Debug, PartialEq, Eq, Clone)]
pub struct BlockCursor<'s, 'p> {
    block: &'s Block,
    name: &'s str,
    path: Rc<Cow<'p, Path>>,
}

impl<'s, 'p> BlockCursor<'s, 'p> {
    /// Creates a new [`BlockCursor`] for the given [`Block`], block name, and path.
    pub fn new(block: &'s Block, name: &'s str, path: Cow<'p, Path>) -> Self {
        BlockCursor {
            block,
            name,
            path: Rc::new(path),
        }
    }
    /// Gets the underlying [`Block`] associated with this cursor.
    pub fn block(&self) -> &'s Block {
        self.block
    }
    /// Gets the underlying block name associated with this cursor.
    pub fn name(&self) -> &'s str {
        self.name
    }
    /// Gets the underlying path associated with this cursor.
    pub fn path(&self) -> &Path {
        &self.path
    }

    /// Returns an [`Iterator`] over [`SymGenCursor`]s for each resolved [`Subregion`] in the
    /// [`Block`].
    pub fn subregions(&self) -> impl Iterator<Item = SymGenCursor<'s, 'p>> + '_ {
        self.block
            .subregions
            .as_deref()
            .unwrap_or_default()
            .iter()
            .filter_map(move |subregion| {
                subregion.contents.as_ref().map(|symgen| {
                    SymGenCursor::new(
                        symgen,
                        Cow::Owned(Subregion::subregion_dir(self.path()).join(&subregion.name)),
                    )
                })
            })
    }
    /// Whether or not the [`Block`] contains at least one resolved [`Subregion`].
    pub fn has_subregions(&self) -> bool {
        self.block
            .subregions
            .as_deref()
            .unwrap_or_default()
            .iter()
            .any(|subregion| subregion.contents.is_some())
    }

    /// Returns an [`Iterator`] over [`BlockCursor`]s for [`Block`]s within all resolved
    /// [`Subregion`]s in the [`Block`].
    pub fn subblocks(&self) -> impl Iterator<Item = BlockCursor<'s, 'p>> + '_ {
        self.block
            .subregions
            .as_deref()
            .unwrap_or_default()
            .iter()
            .filter_map(move |subregion| {
                subregion.contents.as_ref().map(|symgen| {
                    let path = Rc::new(Cow::Owned(
                        Subregion::subregion_dir(self.path()).join(&subregion.name),
                    ));
                    symgen.iter().map(move |(bname, block)| BlockCursor {
                        block,
                        name: &bname.val,
                        path: Rc::clone(&path),
                    })
                })
            })
            .flatten()
    }
    /// Returns an [`Iterator`] over [`BlockCursor`]s for all [`Block`]s nested within this
    /// cursor's [`Block`] (i.e., in subregions), including this cursor itself.
    ///
    /// Traversal is performed in breadth-first order.
    pub fn btraverse(self) -> BlockBTraverser<'s, 'p> {
        BlockBTraverser {
            queue: VecDeque::from([self]),
        }
    }
    /// Returns an [`Iterator`] over [`BlockCursor`]s for all [`Block`]s nested within this
    /// cursor's [`Block`] (i.e., in subregions), including this cursor itself.
    ///
    /// Traversal is performed in depth-first order. Note that if order is not important, `btraverse` should be preferred over `dtraverse`, as it is slightly more efficient.
    pub fn dtraverse(self) -> BlockDTraverser<'s, 'p> {
        BlockDTraverser { stack: vec![self] }
    }
}

/// Performs breadth-first traversal over a nested hierarchy of [`Block`]s.
pub struct BlockBTraverser<'s, 'p> {
    queue: VecDeque<BlockCursor<'s, 'p>>,
}

impl<'s, 'p> Iterator for BlockBTraverser<'s, 'p> {
    type Item = BlockCursor<'s, 'p>;

    fn next(&mut self) -> Option<Self::Item> {
        self.queue.pop_front().map(|cursor| {
            self.queue.extend(cursor.subblocks());
            cursor
        })
    }
}

/// Performs depth-first traversal over a nested hierarchy of [`Block`]s.
pub struct BlockDTraverser<'s, 'p> {
    stack: Vec<BlockCursor<'s, 'p>>,
}

impl<'s, 'p> Iterator for BlockDTraverser<'s, 'p> {
    type Item = BlockCursor<'s, 'p>;

    fn next(&mut self) -> Option<Self::Item> {
        self.stack.pop().map(|cursor| {
            let subblocks: Vec<_> = cursor.subblocks().collect();
            self.stack.reserve(subblocks.len());
            self.stack.extend(subblocks.into_iter().rev());
            cursor
        })
    }
}

#[cfg(test)]
mod tests {
    use super::super::test_utils;
    use super::*;

    fn get_subregion<'s>(symgen: &'s SymGen, block_name: &str, subregion_idx: usize) -> &'s SymGen {
        symgen
            .get(symgen.block_key(block_name).unwrap())
            .unwrap()
            .subregions
            .as_ref()
            .unwrap()[subregion_idx]
            .contents
            .as_ref()
            .unwrap()
            .as_ref()
    }

    fn get_block<'s>(symgen: &'s SymGen, block_name: &str) -> &'s Block {
        symgen.get(symgen.block_key(block_name).unwrap()).unwrap()
    }

    fn get_test_symgen() -> SymGen {
        test_utils::get_symgen_with_subregions(
            r#"main:
            address: 0x0
            length: 0x100
            subregions:
              - sub1.yml
              - sub2.yml
            functions:
              - name: main_fn
                address: 0x80
            data: []
            "#,
            &[
                (
                    "sub1.yml",
                    r#"sub1:
                    address: 0x40
                    length: 0x40
                    subregions:
                      - sub3.yml
                      - sub4.yml
                    functions:
                      - name: sub1_fn
                        address: 0x40
                    data: []
                    "#,
                ),
                (
                    "sub1/sub3.yml",
                    r#"sub3:
                    address: 0x60
                    length: 0x20
                    functions:
                      - name: sub3_fn
                        address: 0x64
                    data:
                      - name: sub3_data
                        address: 0x60
                        length: 0x4
                    "#,
                ),
                (
                    "sub1/sub4.yml",
                    r#"sub4:
                    address: 0x50
                    length: 0x10
                    functions:
                      - name: sub4_fn
                        address: 0x50
                    data: []
                    "#,
                ),
                (
                    "sub2.yml",
                    r#"
                    sub2a:
                      address: 0x0
                      length: 0x40
                      functions: []
                      data:
                        - name: sub2_data
                          address: 0x0
                          length: 0x4

                    sub2b:
                      address: 0xFC
                      length: 0x4
                      functions:
                        - name: sub2_fn
                          address: 0xFC
                      data: []
                    "#,
                ),
            ],
        )
    }

    fn get_test_subregions(symgen: &SymGen) -> (&SymGen, &SymGen, &SymGen, &SymGen) {
        let sub1 = get_subregion(&symgen, "main", 0);
        let sub2 = get_subregion(&symgen, "main", 1);
        let sub3 = get_subregion(&sub1, "sub1", 0);
        let sub4 = get_subregion(&sub1, "sub1", 1);
        (sub1, sub2, sub3, sub4)
    }

    fn get_test_blocks(symgen: &SymGen) -> (&Block, &Block, &Block, &Block, &Block, &Block) {
        let (sub1_yml, sub2_yml, sub3_yml, sub4_yml) = get_test_subregions(symgen);
        let main = get_block(symgen, "main");
        let sub1 = get_block(sub1_yml, "sub1");
        let sub2a = get_block(sub2_yml, "sub2a");
        let sub2b = get_block(sub2_yml, "sub2b");
        let sub3 = get_block(sub3_yml, "sub3");
        let sub4 = get_block(sub4_yml, "sub4");
        (main, sub1, sub2a, sub2b, sub3, sub4)
    }

    #[test]
    fn test_symgen_and_block_cursors() {
        let main = get_test_symgen();
        let (sub1, sub2, sub3, sub4) = get_test_subregions(&main);
        let scursor_main = main.cursor(Path::new("main.yml"));

        // main.yml contents
        assert_eq!(scursor_main.path(), Path::new("main.yml"));
        assert_eq!(scursor_main.symgen(), &main);
        let mut blocks_main = scursor_main.blocks();
        let bcursor_main = blocks_main.next().unwrap();
        assert!(blocks_main.next().is_none());

        // main.yml::main contents
        assert_eq!(bcursor_main.name(), "main");
        assert_eq!(bcursor_main.path(), Path::new("main.yml"));
        let mut subregions_main = bcursor_main.subregions();
        let scursor_sub1 = subregions_main.next().unwrap();
        let scursor_sub2 = subregions_main.next().unwrap();
        assert!(subregions_main.next().is_none());

        // main/sub1.yml contents
        assert_eq!(scursor_sub1.path(), Path::new("main/sub1.yml"));
        assert_eq!(scursor_sub1.symgen(), sub1);
        let mut blocks_sub1 = scursor_sub1.blocks();
        let bcursor_sub1 = blocks_sub1.next().unwrap();
        assert!(blocks_sub1.next().is_none());

        // main/sub1.yml::sub1 contents
        assert_eq!(bcursor_sub1.name(), "sub1");
        let mut subregions_sub1 = bcursor_sub1.subregions();
        let scursor_sub3 = subregions_sub1.next().unwrap();
        let scursor_sub4 = subregions_sub1.next().unwrap();
        assert!(subregions_sub1.next().is_none());

        // main/sub1/sub3.yml contents
        assert_eq!(scursor_sub3.path(), Path::new("main/sub1/sub3.yml"));
        assert_eq!(scursor_sub3.symgen(), sub3);
        let mut blocks_sub3 = scursor_sub3.blocks();
        let bcursor_sub3 = blocks_sub3.next().unwrap();
        assert!(blocks_sub3.next().is_none());

        // main/sub1/sub3.yml::sub3 contents
        assert_eq!(bcursor_sub3.name(), "sub3");
        assert!(bcursor_sub3.subregions().next().is_none());

        // main/sub1/sub4.yml contents
        assert_eq!(scursor_sub4.path(), Path::new("main/sub1/sub4.yml"));
        assert_eq!(scursor_sub4.symgen(), sub4);
        let mut blocks_sub4 = scursor_sub4.blocks();
        let bcursor_sub4 = blocks_sub4.next().unwrap();
        assert!(blocks_sub4.next().is_none());

        // main/sub1/sub3.yml::sub4 contents
        assert_eq!(bcursor_sub4.name(), "sub4");
        assert!(bcursor_sub4.subregions().next().is_none());

        // main/sub2.yml contents
        assert_eq!(scursor_sub2.path(), Path::new("main/sub2.yml"));
        assert_eq!(scursor_sub2.symgen(), sub2);
        let mut blocks_sub2 = scursor_sub2.blocks();
        let bcursor_sub2a = blocks_sub2.next().unwrap();
        let bcursor_sub2b = blocks_sub2.next().unwrap();
        assert!(blocks_sub2.next().is_none());

        // main/sub2.yml::sub2a contents
        assert_eq!(bcursor_sub2a.name(), "sub2a");
        assert!(bcursor_sub2a.subregions().next().is_none());

        // main/sub2.yml::sub2b contents
        assert_eq!(bcursor_sub2b.name(), "sub2b");
        assert!(bcursor_sub2b.subregions().next().is_none());
    }

    #[test]
    fn test_symgen_cursor_subregions() {
        let main = get_test_symgen();
        let (sub1, sub2, sub3, sub4) = get_test_subregions(&main);
        let cursor_main = main.cursor(Path::new("main.yml"));

        // main.yml contents
        assert_eq!(cursor_main.path(), Path::new("main.yml"));
        assert_eq!(cursor_main.symgen(), &main);
        let mut subregions_main = cursor_main.subregions();
        let cursor_sub1 = subregions_main.next().unwrap();
        let cursor_sub2 = subregions_main.next().unwrap();
        assert!(subregions_main.next().is_none());

        // main/sub1.yml contents
        assert_eq!(cursor_sub1.path(), Path::new("main/sub1.yml"));
        assert_eq!(cursor_sub1.symgen(), sub1);
        let mut subregions_sub1 = cursor_sub1.subregions();
        let cursor_sub3 = subregions_sub1.next().unwrap();
        let cursor_sub4 = subregions_sub1.next().unwrap();
        assert!(subregions_sub1.next().is_none());

        // main/sub1/sub3.yml contents
        assert_eq!(cursor_sub3.path(), Path::new("main/sub1/sub3.yml"));
        assert_eq!(cursor_sub3.symgen(), sub3);
        assert!(cursor_sub3.subregions().next().is_none());

        // main/sub1/sub4.yml contents
        assert_eq!(cursor_sub4.path(), Path::new("main/sub1/sub4.yml"));
        assert_eq!(cursor_sub4.symgen(), sub4);
        assert!(cursor_sub4.subregions().next().is_none());

        // main/sub2.yml contents
        assert_eq!(cursor_sub2.path(), Path::new("main/sub2.yml"));
        assert_eq!(cursor_sub2.symgen(), sub2);
        assert!(cursor_sub2.subregions().next().is_none());
    }

    #[test]
    fn test_symgen_cursor_subregions_dedup() {
        let main = test_utils::get_symgen_with_subregions(
            r#"
            main1:
              address: 0x0
              length: 0x100
              subregions:
                - sub1.yml
              functions:
                - name: main_fn
                  address: 0x80
              data: []
            main2:
              address: 0x0
              length: 0x100
              subregions:
                - sub1.yml
              functions: []
              data:
                - name: main_data
                  address: 0x80
                  length: 0x10
            "#,
            &[(
                "sub1.yml",
                r#"sub1:
                    address: 0x0
                    length: 0x40
                    functions: []
                    data:
                      - name: sub1_data
                        address: 0x0
                        length: 0x4
                    "#,
            )],
        );
        let sub1 = get_subregion(&main, "main1", 0);
        let cursor_main = main.cursor(Path::new("main.yml"));

        // main.yml contents
        assert_eq!(cursor_main.path(), Path::new("main.yml"));
        assert_eq!(cursor_main.symgen(), &main);
        let mut subregions_main = cursor_main.subregions();
        let cursor_sub1 = subregions_main.next().unwrap();
        assert!(subregions_main.next().is_none());

        // sub1.yml contents
        assert_eq!(cursor_sub1.path(), Path::new("main/sub1.yml"));
        assert_eq!(cursor_sub1.symgen(), sub1);
        assert!(cursor_sub1.subregions().next().is_none());
    }

    #[test]
    fn test_symgen_cursor_has_subregions() {
        let main = get_test_symgen();
        let (_, _, sub3, _) = get_test_subregions(&main);

        assert!(main.cursor(Path::new("main.yml")).has_subregions());
        assert!(!sub3.cursor(Path::new("")).has_subregions());
    }

    #[test]
    fn test_block_cursor_has_subregions() {
        let symgen = get_test_symgen();
        let (main, _, sub2a, _, _, _) = get_test_blocks(&symgen);

        assert!(main.cursor("main", Path::new("main.yml")).has_subregions());
        assert!(!sub2a.cursor("", Path::new("")).has_subregions());
    }

    #[test]
    fn test_block_cursor_subblocks() {
        let symgen = get_test_symgen();
        let (main, sub1, sub2a, sub2b, sub3, sub4) = get_test_blocks(&symgen);
        let cursor_symgen = symgen.cursor(Path::new("main.yml"));
        let cursor_main = cursor_symgen
            .get(cursor_symgen.symgen().block_key("main").unwrap())
            .unwrap();

        // main contents
        assert_eq!(cursor_main.path(), Path::new("main.yml"));
        assert_eq!(cursor_main.block(), main);
        let mut subblocks_main = cursor_main.subblocks();
        let cursor_sub1 = subblocks_main.next().unwrap();
        let cursor_sub2a = subblocks_main.next().unwrap();
        let cursor_sub2b = subblocks_main.next().unwrap();
        assert!(subblocks_main.next().is_none());

        // sub1 contents
        assert_eq!(cursor_sub1.path(), Path::new("main/sub1.yml"));
        assert_eq!(cursor_sub1.block(), sub1);
        let mut subblocks_sub1 = cursor_sub1.subblocks();
        let cursor_sub3 = subblocks_sub1.next().unwrap();
        let cursor_sub4 = subblocks_sub1.next().unwrap();
        assert!(subblocks_sub1.next().is_none());

        // sub3 contents
        assert_eq!(cursor_sub3.path(), Path::new("main/sub1/sub3.yml"));
        assert_eq!(cursor_sub3.block(), sub3);
        assert!(cursor_sub3.subblocks().next().is_none());

        // sub4 contents
        assert_eq!(cursor_sub4.path(), Path::new("main/sub1/sub4.yml"));
        assert_eq!(cursor_sub4.block(), sub4);
        assert!(cursor_sub4.subblocks().next().is_none());

        // sub2a contents
        assert_eq!(cursor_sub2a.path(), Path::new("main/sub2.yml"));
        assert_eq!(cursor_sub2a.block(), sub2a);
        assert!(cursor_sub2a.subblocks().next().is_none());

        // sub2b contents
        assert_eq!(cursor_sub2b.path(), Path::new("main/sub2.yml"));
        assert_eq!(cursor_sub2b.block(), sub2b);
        assert!(cursor_sub2b.subblocks().next().is_none());
    }

    #[test]
    fn test_symgen_cursor_btraverse() {
        let main = get_test_symgen();
        let (sub1, sub2, sub3, sub4) = get_test_subregions(&main);
        let cursor_main = main.cursor(Path::new("main.yml"));
        let expected = [
            ("main.yml", &main),
            ("main/sub1.yml", sub1),
            ("main/sub2.yml", sub2),
            ("main/sub1/sub3.yml", sub3),
            ("main/sub1/sub4.yml", sub4),
        ];

        for (cursor, exp) in cursor_main.btraverse().zip(expected.iter()) {
            assert_eq!(cursor.path(), Path::new(exp.0));
            assert_eq!(cursor.symgen(), exp.1);
        }
    }

    #[test]
    fn test_symgen_cursor_dtraverse() {
        let main = get_test_symgen();
        let (sub1, sub2, sub3, sub4) = get_test_subregions(&main);
        let cursor_main = main.cursor(Path::new("main.yml"));
        let expected = [
            ("main.yml", &main),
            ("main/sub1.yml", sub1),
            ("main/sub1/sub3.yml", sub3),
            ("main/sub1/sub4.yml", sub4),
            ("main/sub2.yml", sub2),
        ];

        for (cursor, exp) in cursor_main.dtraverse().zip(expected.iter()) {
            assert_eq!(cursor.path(), Path::new(exp.0));
            assert_eq!(cursor.symgen(), exp.1);
        }
    }

    #[test]
    fn test_block_cursor_btraverse() {
        let symgen = get_test_symgen();
        let (main, sub1, sub2a, sub2b, sub3, sub4) = get_test_blocks(&symgen);
        let cursor_symgen = symgen.cursor(Path::new("main.yml"));
        let cursor_main = cursor_symgen
            .get(cursor_symgen.symgen().block_key("main").unwrap())
            .unwrap();
        let expected = [
            ("main.yml", "main", main),
            ("main/sub1.yml", "sub1", sub1),
            ("main/sub2.yml", "sub2a", sub2a),
            ("main/sub2.yml", "sub2b", sub2b),
            ("main/sub1/sub3.yml", "sub3", sub3),
            ("main/sub1/sub4.yml", "sub4", sub4),
        ];

        for (cursor, exp) in cursor_main.btraverse().zip(expected.iter()) {
            assert_eq!(cursor.path(), Path::new(exp.0));
            assert_eq!(cursor.name(), exp.1);
            assert_eq!(cursor.block(), exp.2);
        }
    }

    #[test]
    fn test_block_cursor_dtraverse() {
        let symgen = get_test_symgen();
        let (main, sub1, sub2a, sub2b, sub3, sub4) = get_test_blocks(&symgen);
        let cursor_symgen = symgen.cursor(Path::new("main.yml"));
        let cursor_main = cursor_symgen
            .get(cursor_symgen.symgen().block_key("main").unwrap())
            .unwrap();
        let expected = [
            ("main.yml", "main", main),
            ("main/sub1.yml", "sub1", sub1),
            ("main/sub1/sub3.yml", "sub3", sub3),
            ("main/sub1/sub4.yml", "sub4", sub4),
            ("main/sub2.yml", "sub2a", sub2a),
            ("main/sub2.yml", "sub2b", sub2b),
        ];

        for (cursor, exp) in cursor_main.dtraverse().zip(expected.iter()) {
            assert_eq!(cursor.path(), Path::new(exp.0));
            assert_eq!(cursor.name(), exp.1);
            assert_eq!(cursor.block(), exp.2);
        }
    }
}
