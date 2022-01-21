// Shared types used throughout the game

#ifndef HEADERS_TYPES_COMMON_H_
#define HEADERS_TYPES_COMMON_H_

#include "enums.h"

// This is essentially the standard (32-bit) Unix I/O vector struct.
// It's used for file I/O and represents a buffer of data with a pointer and a length.
struct iovec {
    void* iov_base;
    uint32_t iov_len;
};
ASSERT_SIZE(struct iovec, 8);

#endif
