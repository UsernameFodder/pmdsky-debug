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

# Compile headers
# Structs constrained by ASSERT_SIZE can only compile under both natural
# alignment and packing if there is no implicit padding between members.
.PHONY: headers
headers: headers-aligned headers-packed

# Compile headers with natural struct member alignment by default
.PHONY: headers-aligned
headers-aligned:
# -mno-ms-bitfields is needed for some Windows builds, e.g., MinGW's gcc/clang
	$(CC) -m32 -fsyntax-only -mno-ms-bitfields pmdsky.h

# Compile headers with implicit packing by default
.PHONY: headers-packed
headers-packed:
# -mno-ms-bitfields is needed for some Windows builds, e.g., MinGW's gcc/clang
	$(CC) -m32 -fsyntax-only -mno-ms-bitfields -DIMPLICIT_STRUCT_PACKING pmdsky.h

.PHONY: format
format:
	find . -iname '*.h' | xargs clang-format -i

.PHONY: format-check
format-check:
	find . -iname '*.h' | xargs clang-format --dry-run -Werror

.PHONY: symbol-check
symbol-check:
	./symbol_check.py
