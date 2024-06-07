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

// RGB5 color
struct rgb5 {
    uint16_t r : 5;
    uint16_t g : 5;
    uint16_t b : 5;
    uint16_t padding : 1; // maybe transparency sometimes
};
ASSERT_SIZE(struct rgb5, 2);

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
struct fx64_16 {
    int32_t upper;  // sign bit, plus the 31 most significant integer bits
    uint32_t lower; // the 32 least significant bits (16 integer + 16 fraction)
};
ASSERT_SIZE(struct fx64_16, 8);

// Define custom types for fixed-point numbers
typedef int fx32_16;       // 32-bit signed fixed-point number with 16 fraction bits
typedef int fx32_12;       // 32-bit signed fixed-point number with 12 fraction bits
typedef int fx32_8;        // 32-bit signed fixed-point number with 8 fraction bits
typedef int16_t fx16_14;   // 16-bit signed fixed-point number with 14 fraction bits
typedef int16_t fx16_12;   // 16-bit signed fixed-point number with 12 fraction bits
typedef uint32_t ufx32_16; // 32-bit unsigned fixed-point number with 16 fraction bits
typedef uint32_t ufx32_8;  // 32-bit unsigned fixed-point number with 8 fraction bits

// A raw ARMv5 data-processing instruction, such as MOV, ADD, AND, CMP, etc.
// See the ARMv5 Architecture Reference Manual, Section A3.4.1
// https://developer.arm.com/documentation/ddi0100/latest/
struct data_processing_instruction {
    // second source operand, either a shifted immediate value or a register, see Section A5.1
    uint32_t shifter_operand : 12;
    uint32_t rd : 4;     // destination register
    uint32_t rn : 4;     // first source operand register
    uint32_t s : 1;      // status flag, set if the instruction updates the status registers
    uint32_t opcode : 4; // see Section A3.4, Table A3-2
    uint32_t i : 1;      // immediate flag, set if shifter_operand represents an immediate
    uint32_t _zero : 2;  // always 0
    uint32_t cond : 4;   // condition code, see Section A3.2
};
ASSERT_SIZE(struct data_processing_instruction, 4);

// 4x3 matrix for the 3D geometry engine, stored in row-major format.
// Each element is a signed fixed-point number with 12 fraction bits.
// See https://problemkaputt.de/gbatek.htm#ds3dmatrixtypes
struct matrix_4x3 {
    fx32_12 entry[4][3];
};
ASSERT_SIZE(struct matrix_4x3, 48);

// TRIG_TABLE entry containing the value of sine/cosine at some angle
// Each value is a signed fixed-point number with 12 fraction bits.
struct trig_values {
    fx16_12 sin;
    fx16_12 cos;
};
ASSERT_SIZE(struct trig_values, 4);

// Specifies an entry function for a thread's execution
typedef void (*thread_entry_fn_t)(void);
// Specifies a function called when a thread exits
typedef void (*thread_exit_fn_t)(void);

// Contains information about a running thread
struct thread {
    int flags;                // Probably a flags field. Usually 0x1F.
    struct thread* field_0x4; // r2 parameter in ThreadStart
    undefined4 field_0x8;     // Initialized to 0
    undefined4 field_0xC;     // Initialized to 0
    undefined4 field_0x10;    // Initialized to 0
    undefined4 field_0x14;    // Initialized to 0
    undefined4 field_0x18;    // Initialized to 0
    undefined4 field_0x1C;    // Initialized to 0
    undefined4 field_0x20;    // Initialized to 0
    undefined4 field_0x24;    // Initialized to 0
    undefined4 field_0x28;    // Initialized to 0
    undefined4 field_0x2C;    // Initialized to 0
    undefined4 field_0x30;    // Initialized to 0
    undefined4 field_0x34;    // Initialized to 0
    // 0x38: Points to the area of the allocated stack area that can actually be used to store data.
    // This is equal to stack_pointer - 0x44, with an additional - 4 if the resulting addess is not
    // aligned to 8 bytes.
    void* usable_stack_pointer;
    // 0x3C: Address of the function to return to once the thread exits. Usually ThreadExit.
    thread_exit_fn_t exit_function;
    void* function_address_plus_4; // 0x40: Pointer to the function to run + 4
    void* stack_pointer_minus_4;   // 0x44: Pointer to the start of the stack area - 4
    undefined field_0x48;
    undefined field_0x49;
    undefined field_0x4A;
    undefined field_0x4B;
    undefined field_0x4C;
    undefined field_0x4D;
    undefined field_0x4E;
    undefined field_0x4F;
    undefined field_0x50;
    undefined field_0x51;
    undefined field_0x52;
    undefined field_0x53;
    undefined field_0x54;
    undefined field_0x55;
    undefined field_0x56;
    undefined field_0x57;
    undefined field_0x58;
    undefined field_0x59;
    undefined field_0x5A;
    undefined field_0x5B;
    undefined field_0x5C;
    undefined field_0x5D;
    undefined field_0x5E;
    undefined field_0x5F;
    undefined field_0x60;
    undefined field_0x61;
    undefined field_0x62;
    undefined field_0x63;
    undefined4 field_0x64; // Initialized to 0
    // 0x68: Pointer to the next thread. This forms a linked list sorted in ascending order
    // according to sorting_order
    struct thread* next_thread;
    // 0x6C: Seems to be a thread ID that gets incremented for each new thread created.
    int thread_id;
    int sorting_order;       // 0x70: Unknown. Used to sort threads.
    undefined4 field_0x74;   // Initialized to 0
    undefined4 field_0x78;   // Initialized to 0
    undefined4 field_0x7C;   // Initialized to 0
    undefined4 field_0x80;   // Initialized to 0
    undefined4 field_0x84;   // Initialized to 0
    undefined4 field_0x88;   // Initialized to 0
    undefined4 field_0x8C;   // Initialized to 0
    void* stack_end_pointer; // 0x90: Pointer to the end of the stack area (exclusive)
    void* stack_pointer;     // 0x94: Pointer to the start of the stack area (inclusive)
    undefined4 field_0x98;   // Initialized to 0
    undefined4 field_0x9C;   // Initialized to 0
    undefined4 field_0xA0;   // Initialized to 0
    // Initialized to 0. Actually part of a separate struct alongside field_0xA8 and field_0xAC.
    undefined4 field_0xA4;
    undefined4 field_0xA8; // Initialized to 0
    undefined4 field_0xAC; // Initialized to 0
    undefined4 field_0xB0; // Initialized to 0
    undefined4 field_0xB4; // Initialized to 0
    undefined field_0xB8;
    undefined field_0xB9;
    undefined field_0xBA;
    undefined field_0xBB;
    undefined field_0xBC;
    undefined field_0xBD;
    undefined field_0xBE;
    undefined field_0xBF;
};
ASSERT_SIZE(struct thread, 192); // Last 8 bytes are not confirmed

// Seems to be used to hold some global state about threads
struct thread_info {
    undefined field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    undefined field_0x4;
    undefined field_0x5;
    undefined field_0x6;
    undefined field_0x7;
    undefined* field_0x8;
    undefined field_0xC;
    undefined field_0xD;
    undefined field_0xE;
    undefined field_0xF;
    undefined field_0x10;
    undefined field_0x11;
    undefined field_0x12;
    undefined field_0x13;
    undefined field_0x14;
    undefined field_0x15;
    undefined field_0x16;
    undefined field_0x17;
    undefined field_0x18;
    undefined field_0x19;
    undefined field_0x1A;
    undefined field_0x1B;
    undefined field_0x1C;
    undefined field_0x1D;
    undefined field_0x1E;
    undefined field_0x1F;
    int thread_count; // 0x20: Total amount of threads (or maybe just the active ones?)
    undefined field_0x24;
    undefined field_0x25;
    undefined field_0x26;
    undefined field_0x27;
    undefined* field_0x28;
    // 0x2C: Points to the first thread on the linked list sorted by thread::sort_order
    struct thread* thread_list_head;
    undefined field_0x30;
    undefined field_0x31;
    undefined field_0x32;
    undefined field_0x33;
};
ASSERT_SIZE(struct thread_info, 52); // Could be shorter

#endif
