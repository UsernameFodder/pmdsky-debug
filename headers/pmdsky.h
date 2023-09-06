#ifndef HEADERS_PMDSKY_H_
#define HEADERS_PMDSKY_H_

#include "versions.h"

// Default to NA if there's no predefined version
#ifndef PMDSKY_VERSION
#define PMDSKY_VERSION PMDSKY_NA
#endif

#ifndef PMDSKY_UNSIZED_HEADERS
// ======== BEGIN NORMAL PREAMBLE ========
// This is the normal include preamble.
// It defines all the constructs needed for the headers to be self-contained.

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

// Define size_t for convenience
typedef unsigned long size_t;

// Define the wchar_t type to be 2 bytes. Use the built-in unsigned short type here because
// Ghidra's default parser configuration complains if we use uint16_t
typedef unsigned short wchar_t;

// Based on the code for vsprintf(3), it seems like va_list is implemented in the ARM9 binary
// by just passing a pointer into the stack, so define va_list to be void*, rather than using
// whatever stdarg.h defines
typedef void* va_list;

// Ghidra-style "undefined*" types should be used to signify that a type is unknown
typedef uint8_t undefined1;
typedef uint16_t undefined2;
typedef uint32_t undefined4;
typedef undefined1 undefined;

#ifdef IMPLICIT_STRUCT_PACKING
// In implicit packing mode, all structs are packed by default.
// If padding is desired, it must be defined explicitly.
#pragma pack(1)
#endif

// A wrapper struct for an enum stored as an 8-bit integer.
// For some `enum foo_id`, ENUM_8_BIT(foo_id) will define `struct foo_id_8`.
// This should only be used within `#pragma pack(push, 1)`.
#define ENUM_8_BIT(tag)                                                                            \
    struct tag##_8 {                                                                               \
        enum tag val : 8;                                                                          \
    };                                                                                             \
    ASSERT_SIZE(struct tag##_8, 1)

// A wrapper struct for an enum stored as a 16-bit integer.
// For some `enum foo_id`, ENUM_16_BIT(foo_id) will define `struct foo_id_16`.
// This should only be used within `#pragma pack(push, 2)`.
#define ENUM_16_BIT(tag)                                                                           \
    struct tag##_16 {                                                                              \
        enum tag val : 16;                                                                         \
    };                                                                                             \
    ASSERT_SIZE(struct tag##_16, 2)

// ======== END NORMAL PREAMBLE ========
#else // #ifndef PMDSKY_UNSIZED_HEADERS
// ======== BEGIN UNSIZED PREAMBLE ========
// This is the unsized preamble.
// It's only meant for use by external applications that want to use the types defined by the
// pmdsky-debug headers, without caring about the specifics of struct sizing and memory layouts.
// The intent is to allow applications to do something like:
// ```
// #define PMDSKY_UNSIZED_HEADERS
// #include "pmdsky-debug/headers/pmdsky.h"
// #undef PMDSKY_UNSIZED_HEADERS
// ```
//
// If you don't know exactly what you're doing, you can probably safely ignore this part.
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

// PMDSKY_UNSIZED_HEADERS implies PMDSKY_NO_BUILTIN, because trying to declare certain builtins
// will cause the compiler to complain in many typical contexts.
#ifndef PMDSKY_NO_BUILTIN
#define PMDSKY_NO_BUILTIN
#endif

// Stub this out since we don't care about sizes
#define ASSERT_SIZE(type, size)

// Ghidra-style "undefined*" types still need to be defined
typedef uint8_t undefined1;
typedef uint16_t undefined2;
typedef uint32_t undefined4;
typedef undefined1 undefined;

// The ENUM_*_BIT macros still need to be defined, but they don't need to be bitfields
#define ENUM_8_BIT(tag)                                                                            \
    struct tag##_8 {                                                                               \
        enum tag val;                                                                              \
    };
#define ENUM_16_BIT(tag)                                                                           \
    struct tag##_16 {                                                                              \
        enum tag val;                                                                              \
    };
// ======== END UNSIZED PREAMBLE ========
#endif // #ifndef PMDSKY_UNSIZED_HEADERS

// Now include the actual type definitions, function signatures, and global declarations
#include "types/types.h"
#include "functions/functions.h"
#include "data/data.h"

#endif
