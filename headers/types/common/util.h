#ifndef HEADERS_TYPES_COMMON_UTIL_H_
#define HEADERS_TYPES_COMMON_UTIL_H_

// Common data structures not specific to EoS

// RGB8 color
struct rgb {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};
ASSERT_SIZE(struct rgb, 3);

// RGBA8 structure. Sometimes alpha is ignored and only used for padding
struct rgba {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a; // Sometimes used only for padding
};
ASSERT_SIZE(struct rgba, 4);

// BGR5 color
struct bgr5 {
    uint8_t r : 5;
    // Note that the g value is split in two to reflect its actual memory layout
    uint8_t g_low : 3;
    uint8_t g_high : 2;
    uint8_t b : 5;
    uint8_t padding : 1; // maybe transparency sometimes
};
ASSERT_SIZE(struct bgr5, 2);

// a 2d uint (32bit) vector
struct uvec2 {
    uint32_t x;
    uint32_t y;
};
ASSERT_SIZE(struct uvec2, 8);

// a 2d int (32bit) vector
struct vec2 {
    int32_t x;
    int32_t y;
};
ASSERT_SIZE(struct vec2, 8);

// a 2d ushort (16bit) vector
struct uvec2_16 {
    uint16_t x;
    uint16_t y;
};
ASSERT_SIZE(struct uvec2_16, 4);

// a 2d short (16bit) vector
struct vec2_16 {
    int16_t x;
    int16_t y;
};
ASSERT_SIZE(struct vec2_16, 4);

// A slice in the usual programming sense: a pointer, length, and capacity.
// Used for the implementation of vsprintf(3), but maybe it's used elsewhere as well.
struct slice {
    void* data;      // Pointer to the data buffer
    size_t capacity; // How much space is available in total
    size_t length;   // How much space is currently filled
};
ASSERT_SIZE(struct slice, 12);

// Function to append data to a struct slice, and return a success flag.
typedef bool (*slice_append_fn_t)(struct slice* slice, const void* data, size_t data_len);

// 64-bit signed fixed-point number with 16 fraction bits.
// Represents the number ((upper << 16) + (lower >> 16) + (lower & 0xFFFF) * 2^-16)
struct fx64 {
    int32_t upper;  // sign bit, plus the 31 most significant integer bits
    uint32_t lower; // the 32 least significant bits (16 integer + 16 fraction)
};
ASSERT_SIZE(struct fx64, 8);
#endif
