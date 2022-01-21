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

.PHONY: headers
headers:
# -mno-ms-bitfields is needed for some Windows builds, e.g., MinGW's gcc/clang
	$(CC) -m32 -fsyntax-only -mno-ms-bitfields pmdsky.h

.PHONY: headers-format
format:
	find . -iname '*.h' | xargs clang-format -i

.PHONY: format-check
format-check:
	find . -iname '*.h' | xargs clang-format --dry-run -i
