#ifndef HEADERS_PMDSKY_H_
#define HEADERS_PMDSKY_H_

// Try to define the ASSERT_SIZE macro
#ifdef __has_feature
#if __has_feature(c_static_assert)
// clang
#define ASSERT_SIZE(type, size)                                                                    \
    _Static_assert(sizeof(type) == size, "type '" #type "' must have size " #size)
#endif // __has_feature(c_static_assert)
#elif __GNUC__ > 4 || (__GNUC__ == 4 && (__GNUC_MINOR__ > 5))
// gcc (_Static_assert support since gcc 4.6: https://gcc.gnu.org/wiki/C11Status)
#define ASSERT_SIZE(type, size)                                                                    \
    _Static_assert(sizeof(type) == size, "type '" #type "' must have size " #size)
#else
// _Static_assert is unsupported, so just skip assertions
#define ASSERT_SIZE(type, size)
#endif // ifdef __has_feature

// Make sure primitive types are sized as we expect
ASSERT_SIZE(char, 1);
ASSERT_SIZE(short, 2);
ASSERT_SIZE(int, 4);
// The following ensures we're compiling for a 32-bit architecture
ASSERT_SIZE(long, 4);
ASSERT_SIZE(void*, 4);
ASSERT_SIZE(long long, 8);

// Define fixed size types without including <stdint.h> (or similar) for compatibility
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;

// Define the bool type to be 1 byte
typedef uint8_t bool;

// Define the wchar_t type to be 2 bytes. Use the built-in short type here because Ghidra's
// default parser configuration complains if we use int16_t
typedef short wchar_t;

// Ghidra-style "undefined*" types should be used to signify that a type is unknown
typedef uint8_t undefined1;
typedef uint16_t undefined2;
typedef uint32_t undefined4;
typedef undefined1 undefined;

// All structs are assumed to be packed. If padding is desired, it must be defined explicitly
#pragma pack(1)

// Now include the actual type definitions and function signatures
#include "types/types.h"
#include "functions/functions.h"

#endif
