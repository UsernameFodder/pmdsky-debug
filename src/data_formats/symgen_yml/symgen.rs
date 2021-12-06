/// The resymgen YAML format
use std::any;
use std::borrow::Cow;
use std::cmp::Ordering;
use std::collections::{BTreeMap, HashMap};
use std::fmt::{self, Display, Formatter};
use std::io::{Read, Write};
use std::ops::Deref;
use std::slice::SliceIndex;

use regex::{Captures, Regex};
use serde::{Deserialize, Serialize};
use serde_yaml;
use syn::{self, LitStr};

use super::error::{Error, Result};
use super::types::*;

/// Specifies how integers should be serialized
#[derive(Clone, Copy)]
pub enum IntFormat {
    Decimal,
    Hexadecimal,
}

struct BlockContext {
    version_order: Option<HashMap<String, u64>>,
}

#[derive(Debug, PartialEq, Eq, Clone, Serialize, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct Symbol {
    pub name: String,
    pub address: MaybeVersionDep<Linkable>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub length: Option<MaybeVersionDep<Uint>>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub description: Option<String>,
}

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
                    .map(|(v, a)| (v.clone(), (a.clone(), len.get(Some(v)).copied())))
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
    fn init(&mut self, ctx: &BlockContext) {
        self.address.init(&ctx.version_order);
        if let Some(l) = &mut self.length {
            l.init(&ctx.version_order);
        }
    }
    pub fn expand_versions(&mut self, all_versions: &[Version]) {
        self.address.expand_versions(all_versions);
        if let Some(len) = &mut self.length {
            len.expand_versions(all_versions);
        }
    }
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
}

impl Sort for Symbol {
    // Note: only applies to the address values. Version keys will always be sorted.
    fn sort(&mut self) {
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

/// A concrete realization of a symbol for some version.
#[derive(Debug, PartialEq, Eq, Clone, Copy, Serialize, Deserialize)]
pub struct RealizedSymbol<'a> {
    pub name: &'a str,
    pub address: Uint,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub length: Option<Uint>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub description: Option<&'a str>,
}

/// Wraps a symbol iterator and yields RealizedSymbols.
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
                    address: a,
                    length: len.copied(),
                    description: symbol.description.as_deref(),
                });
            }
            // cur is depleted; don't put it back and get a new one next loop
        }
    }
}

pub trait Realize<'s> {
    type Iter: Iterator<Item = &'s Symbol>;

    /// Returns an iterator over realized symbols.
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

#[derive(Debug, PartialEq, Eq, Clone, Serialize, Deserialize)]
pub struct SymbolList(Vec<Symbol>);

impl SymbolList {
    fn init(&mut self, ctx: &BlockContext) {
        for symbol in self.0.iter_mut() {
            symbol.init(ctx);
        }
    }
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
    pub fn push(&mut self, value: Symbol) {
        self.0.push(value)
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

impl Sort for SymbolList {
    fn sort(&mut self) {
        for symbol in self.0.iter_mut() {
            symbol.sort();
        }
        self.0.sort();
    }
}

#[derive(Debug, PartialEq, Eq, Clone, Serialize, Deserialize)]
#[serde(deny_unknown_fields)]
pub struct Block {
    // Metadata
    #[serde(skip_serializing_if = "Option::is_none")]
    pub versions: Option<Vec<Version>>,
    pub address: MaybeVersionDep<Uint>,
    pub length: MaybeVersionDep<Uint>,
    #[serde(skip_serializing_if = "Option::is_none")]
    pub description: Option<String>,

    // Symbols
    pub functions: SymbolList,
    pub data: SymbolList,
}

impl Block {
    fn get_context(&self) -> BlockContext {
        BlockContext {
            version_order: OrdString::get_order_map(self.versions.as_deref()),
        }
    }
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
    pub fn expand_versions(&mut self) {
        // Note: this function does not try to infer versions from address/length because, unlike
        // with extent(), this function actually CHANGES Common to ByVersion, which sort of loses
        // information (because Common is purely generic).
        if let Some(vers) = &self.versions {
            self.address.expand_versions(&vers);
            self.length.expand_versions(&vers);
            self.functions.expand_versions(&vers);
            self.data.expand_versions(&vers);
        }
    }
    /// Look up a version by name
    pub fn version(&self, name: &str) -> Option<&Version> {
        self.versions
            .as_ref()
            .and_then(|vs| vs.iter().find(|v| v.name() == name))
    }
    /// Returns an iterator over both functions and data.
    pub fn iter(&self) -> impl Iterator<Item = &Symbol> {
        self.functions.iter().chain(self.data.iter())
    }
    pub fn iter_realized(&self, version_name: &str) -> impl Iterator<Item = RealizedSymbol> + '_ {
        let version = self.version(version_name);
        self.iter().realize(version)
    }
    pub fn functions_realized(
        &self,
        version_name: &str,
    ) -> impl Iterator<Item = RealizedSymbol> + '_ {
        let version = self.version(version_name);
        self.functions.iter().realize(version)
    }
    pub fn data_realized(&self, version_name: &str) -> impl Iterator<Item = RealizedSymbol> + '_ {
        let version = self.version(version_name);
        self.data.iter().realize(version)
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
        self.functions.sort();
        self.data.sort();
    }
}

#[derive(Debug, PartialEq, Clone, Serialize, Deserialize)]
pub struct SymGen(BTreeMap<OrdString, Block>);

impl SymGen {
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
    pub fn read_no_init<R: Read>(rdr: R) -> Result<SymGen> {
        serde_yaml::from_reader(rdr).map_err(|e| Error::Yaml(e))
    }
    pub fn read<R: Read>(rdr: R) -> Result<SymGen> {
        let mut symgen: SymGen = SymGen::read_no_init(rdr)?;
        symgen.init();
        Ok(symgen)
    }
    pub fn read_sorted<R: Read>(rdr: R) -> Result<SymGen> {
        let mut symgen = SymGen::read(rdr)?;
        symgen.sort();
        Ok(symgen)
    }
    /// Convert target fields in a symgen YAML string with some inline operation,
    /// via line-by-line text processing. F injects modified lines into the final YAML string
    /// accumulator, based on the given line to be modified and the current indentation level,
    /// and returns a success flag.
    ///
    /// This is a gross hack. Should investigate whether it's easy to mod yaml-rust and serde-yaml
    /// to serialize in the desired format directly, rather than doing it via post-processing.
    /// But this is serviceable for now.
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
            let trimmed = line.trim_start().trim_start_matches("-").trim_start();
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
    /// Convert all integer values in a symgen YAML string from decimal to hexadecimal.
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
    /// Convert all multiline description strings in a symgen YAML string to block scalar format for readability.
    fn convert_multiline_desc_to_block_scalar(yaml: &str) -> String {
        SymGen::convert_fields_inline(yaml, ["description:"], |converted_yaml, line, indent| {
            const SUB_INDENT: usize = 2;
            let start_idx;
            let contents;
            if let Some(idx) = line.find('"') {
                start_idx = idx;
                contents = Cow::Borrowed(&line[start_idx..]);
            } else {
                if let Some(colon) = line.find(':') {
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
    pub fn write<W: Write>(&self, mut writer: W, int_format: IntFormat) -> Result<()> {
        // I don't expect these YAML files to be too big to fit in memory, so it's easier and
        // faster to keep the serialized data in memory for processing. And anyway,
        // serde_yaml::from_reader already uses read_to_end()
        // (https://github.com/dtolnay/serde-yaml/blob/644be1654d382627c4419f613e300c5e4df3650f/src/de.rs#L141)
        // so this shouldn't be much worse. If it ever becomes an issue (like with merging huge
        // files or something) this can be refactored to use another intermediate tempfile and a
        // BufReader/BufWriter or something.
        let mut yaml = serde_yaml::to_string(self).map_err(|e| Error::Yaml(e))?;
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
        writer.write_all(yaml_bytes).map_err(|e| Error::Io(e))
    }
    pub fn write_to_str(&self, int_format: IntFormat) -> Result<String> {
        let mut bytes = Vec::<u8>::new();
        self.write(&mut bytes, int_format)?;
        Ok(String::from_utf8(bytes).map_err(|e| Error::FromUtf8(e))?)
    }

    pub fn expand_versions(&mut self) {
        for block in self.0.values_mut() {
            block.expand_versions();
        }
    }
    pub fn block_key(&self, block_name: &str) -> Option<&OrdString> {
        self.0.keys().find(|k| k.val == block_name)
    }

    pub fn get(&self, key: &OrdString) -> Option<&Block> {
        self.0.get(key)
    }
    pub fn get_mut(&mut self, key: &OrdString) -> Option<&mut Block> {
        self.0.get_mut(key)
    }
    pub fn insert(&mut self, key: OrdString, value: Block) -> Option<Block> {
        self.0.insert(key, value)
    }

    pub fn iter(&self) -> impl Iterator<Item = (&OrdString, &Block)> {
        self.0.iter()
    }
    pub fn iter_mut(&mut self) -> impl Iterator<Item = (&OrdString, &mut Block)> {
        self.0.iter_mut()
    }
    pub fn blocks(&self) -> impl Iterator<Item = &Block> {
        self.0.values()
    }
    pub fn blocks_mut(&mut self) -> impl Iterator<Item = &mut Block> {
        self.0.values_mut()
    }
    pub fn symbols(&self) -> impl Iterator<Item = &Symbol> {
        self.blocks().flat_map(|b| b.iter())
    }
    pub fn symbols_realized(
        &self,
        version_name: &str,
    ) -> impl Iterator<Item = RealizedSymbol> + '_ {
        let v = String::from(version_name);
        self.blocks().flat_map(move |b| b.iter_realized(&v))
    }
    pub fn functions_realized(
        &self,
        version_name: &str,
    ) -> impl Iterator<Item = RealizedSymbol> + '_ {
        let v = String::from(version_name);
        self.blocks().flat_map(move |b| b.functions_realized(&v))
    }
    pub fn data_realized(&self, version_name: &str) -> impl Iterator<Item = RealizedSymbol> + '_ {
        let v = String::from(version_name);
        self.blocks().flat_map(move |b| b.data_realized(&v))
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
                    address: MaybeVersionDep::Common(Linkable::from([0x2101000, 0x2101100])),
                    length: None,
                    description: None,
                },
                Symbol {
                    name: "function1".to_string(),
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
        fn test_iter_realize() {
            let (_, versions, _, _, _, list) = get_block_data();

            let mut iter0 = list.iter().realize(Some(&versions[0]));
            let exp0 = [
                RealizedSymbol {
                    name: &"function1",
                    address: 0x2100000,
                    length: Some(0x100),
                    description: None,
                },
                RealizedSymbol {
                    name: &"function1",
                    address: 0x2100100,
                    length: Some(0x100),
                    description: None,
                },
                RealizedSymbol {
                    name: &"function2",
                    address: 0x2101000,
                    length: None,
                    description: None,
                },
                RealizedSymbol {
                    name: &"function2",
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
            let exp1 = [
                RealizedSymbol {
                    name: &"function1",
                    address: 0x2100c00,
                    length: Some(0x100),
                    description: None,
                },
                RealizedSymbol {
                    name: &"function2",
                    address: 0x2101000,
                    length: None,
                    description: None,
                },
                RealizedSymbol {
                    name: &"function2",
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
                    address: 0x2101000,
                    length: None,
                    description: None,
                },
                RealizedSymbol {
                    name: &"function2",
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

    mod block_tests {
        use super::*;

        fn get_sorted_block() -> Block {
            let (versions, _, addresses, _, symbols, _) = get_block_data();

            let mut block = Block {
                versions: Some(versions),
                address: addresses.clone(),
                length: addresses.clone(),
                description: None,
                functions: symbols.clone(),
                data: symbols.clone(),
            };
            block.init();
            block.sort();
            block
        }

        #[test]
        fn test_init_sort() {
            let block = get_sorted_block();
            let (_, final_versions, _, final_addresses, _, final_symbols) = get_block_data();
            assert_eq!(
                &block,
                &Block {
                    versions: Some(final_versions),
                    address: final_addresses.clone(),
                    length: final_addresses.clone(),
                    description: None,
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
            let exp = [
                RealizedSymbol {
                    name: &"function1",
                    address: 0x2100000,
                    length: Some(0x100),
                    description: None,
                },
                RealizedSymbol {
                    name: &"function1",
                    address: 0x2100100,
                    length: Some(0x100),
                    description: None,
                },
                RealizedSymbol {
                    name: &"function2",
                    address: 0x2101000,
                    length: None,
                    description: None,
                },
                RealizedSymbol {
                    name: &"function2",
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
                    address: 0x2101000,
                    length: None,
                    description: None,
                },
                RealizedSymbol {
                    name: &"function2",
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
                            functions: [
                                Symbol {
                                    name: "fn1".to_string(),
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
                            functions: [Symbol {
                                name: "fn3".to_string(),
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

        #[test]
        fn test_read() {
            let (input, expected) = get_symgen_data();
            let obj = SymGen::read(input.as_bytes()).expect("Read failed");
            assert_eq!(&obj, &expected);
        }

        #[test]
        fn test_write() {
            let (expected, input) = get_symgen_data();
            let yaml = input
                .write_to_str(IntFormat::Hexadecimal)
                .expect("Write failed");
            assert_eq!(&yaml, &expected);
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
            let functions_main_exp = [
                RealizedSymbol {
                    name: &"fn1",
                    address: 0x2001000,
                    length: Some(0x1000),
                    description: Some(&"multi\nline\ndescription"),
                },
                RealizedSymbol {
                    name: &"fn2",
                    address: 0x2002000,
                    length: None,
                    description: Some(&"baz"),
                },
                RealizedSymbol {
                    name: &"fn2",
                    address: 0x2003000,
                    length: None,
                    description: Some(&"baz"),
                },
            ];
            let data_main_exp = [RealizedSymbol {
                name: &"SOME_DATA",
                address: 0x2000000,
                length: Some(0x1000),
                description: Some(&"foo bar baz"),
            }];
            let functions_other_exp = [RealizedSymbol {
                name: &"fn3",
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
}
