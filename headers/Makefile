# Look for clang or gcc
CLANG_VERSION := $(shell clang --version 2>/dev/null)
GCC_VERSION := $(shell gcc --version 2>/dev/null)
ifdef CLANG_VERSION
CC := clang
else ifdef GCC_VERSION
CC := gcc
else
$(error C compiler not found)
endif

# All the possible files that can function as a top-level header
TOP_LEVEL_HEADERS := pmdsky.h pmdsky_na.h pmdsky_eu.h pmdsky_jp.h

# Compile headers
# Structs constrained by ASSERT_SIZE can only compile under both natural
# alignment and packing if there is no implicit padding between members.
.PHONY: headers
headers: headers-aligned headers-packed headers-no-builtin

# Compile headers with natural struct member alignment by default
.PHONY: headers-aligned
headers-aligned:
# -mno-ms-bitfields is needed for some Windows builds, e.g., MinGW's gcc/clang
	$(CC) -m32 -fshort-wchar -fsyntax-only -mno-ms-bitfields $(TOP_LEVEL_HEADERS)

# Compile headers with implicit packing by default
.PHONY: headers-packed
headers-packed:
# -mno-ms-bitfields is needed for some Windows builds, e.g., MinGW's gcc/clang
	$(CC) -m32 -fshort-wchar -fsyntax-only -mno-ms-bitfields -Wno-pragma-pack -DIMPLICIT_STRUCT_PACKING $(TOP_LEVEL_HEADERS)

# Variant of the headers that excludes declarations for common builtin functions. This mode is
# intended for dev use, if the builtin declarations cause unexpected compilation issues.
.PHONY: headers-no-builtin
headers-no-builtin:
	$(CC) -m32 -fshort-wchar -fsyntax-only -mno-ms-bitfields -DPMDSKY_NO_BUILTIN $(TOP_LEVEL_HEADERS)

# Unsized variant of the headers. This isn't included in the default target because it assumes
# the presence of system headers. Don't use any funky options here, since this mode is intended
# for normal dev use.
.PHONY: headers-unsized
headers-unsized:
	$(CC) -fsyntax-only -DPMDSKY_UNSIZED_HEADERS $(TOP_LEVEL_HEADERS)

.PHONY: format
format:
	find . -iname '*.h' | xargs clang-format -i

.PHONY: format-check
format-check:
	find . -iname '*.h' | xargs clang-format --dry-run -Werror

.PHONY: symbol-check
symbol-check:
	./symbol_check.py --sort-order

.PHONY: symbol-check-extra
symbol-check-extra:
	./symbol_check.py --extra-symbols
