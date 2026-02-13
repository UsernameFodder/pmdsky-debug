#ifndef HEADERS_TYPES_COMMON_UTIL_H_
#define HEADERS_TYPES_COMMON_UTIL_H_

#include "enums.h"

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

struct cp_context {
    uint64_t div_numer;
    uint64_t div_denom;
    uint64_t sqrt;
    uint16_t div_mode;
    uint16_t sqrt_mode;
};
ASSERT_SIZE(struct cp_context, 28);

struct os_context {
    uint32_t cpsr;
    uint32_t registers[13]; // Initialized to 0
    // 0x38: Points to the area of the allocated stack area that can actually be used to store data.
    // This is equal to stack_pointer - 0x44, with an additional - 4 if the resulting addess is not
    // aligned to 8 bytes.
    void* usable_stack_pointer;
    // 0x3C: Address of the function to return to once the thread exits. Usually ThreadExit.
    thread_exit_fn_t exit_function;
    void* function_address_plus_4; // 0x40: Pointer to the function to run + 4
    void* stack_pointer_minus_4;   // 0x44: Pointer to the start of the stack area - 4
    struct cp_context cp_context;
};
ASSERT_SIZE(struct os_context, 100);

typedef void (*os_alarm_handler)(void*);

struct osi_alarm {
    os_alarm_handler handler;
    void* arg;

    uint32_t tag;
    uint64_t fire;
    struct osi_alarm* prev;
    struct osi_alarm* next;

    uint64_t period;
    uint64_t start;
};
ASSERT_SIZE(struct osi_alarm, 44);

typedef void (*os_thread_destructor)(void*);

struct thread;
struct os_thread_queue;
struct os_thread_link;
struct os_mutex;
struct os_mutex_queue;
struct os_mutex_link;

struct os_thread_queue {
    struct thread* head; // Initialized to NULL
    struct thread* tail; // Initialized to NULL
};
ASSERT_SIZE(struct os_thread_queue, 8);

struct os_thread_link {
    struct thread* prev; // Initialized to NULL
    struct thread* next; // Initialized to NULL
};
ASSERT_SIZE(struct os_thread_link, 8);

struct os_mutex_link {
    struct os_mutex* next; // Initialized to NULL
    struct os_mutex* prev; // Initialized to NULL
};
ASSERT_SIZE(struct os_mutex_link, 8);

struct os_mutex_queue {
    struct os_mutex* head; // Initialized to NULL
    struct os_mutex* tail; // Initialized to NULL
};
ASSERT_SIZE(struct os_mutex_queue, 8);

struct os_mutex {
    struct os_thread_queue queue;
    struct thread* thread;     // Initialized to NULL
    int count;                 // Initialized to 0
    struct os_mutex_link link; // Initialized to NULL
};
ASSERT_SIZE(struct os_mutex, 24);

// Contains information about a running thread
struct thread {
    struct os_context context;
    enum os_thread_state state; // Initialized to OS_THREAD_STATE_WAITING
    // 0x68: Pointer to the next thread. This forms a linked list sorted in ascending order
    // according to sorting_order
    struct thread* next_thread;
    // 0x6C: Seems to be a thread ID that gets incremented for each new thread created.
    int thread_id;
    int sorting_order;             // 0x70: Used to sort threads.
    void* profiler;                // Initialized to NULL
    struct os_thread_queue* queue; // Initialized to NULL
    struct os_thread_link link;
    struct os_mutex* mutex; // Initialized to NULL
    struct os_mutex_queue mutex_queue;
    void* stack_end_pointer;       // 0x90: Pointer to the end of the stack area (exclusive)
    void* stack_pointer;           // 0x94: Pointer to the start of the stack area (inclusive)
    uint32_t stack_warning_offset; // Initialized to 0
    struct os_thread_queue join_queue;
    // Initialized to 0.
    void* specific[3];
    struct osi_alarm* alarm_for_sleep; // Initialized to 0
    os_thread_destructor destructor;   // Initialized to NULL
    void* user_parameter;
    uint32_t system_errno;
};
ASSERT_SIZE(struct thread, 192);

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

// Language and flags of the DS firmware user settings.
struct language_and_flags {
    uint8_t language : 3;
    bool gba_mode_screen : 1;
    uint8_t backlight_level : 2;
    bool bootmenu_disable : 1;
    undefined field_0x7 : 1;
};
ASSERT_SIZE(struct language_and_flags, 1);

// Various pieces of user settings originating from the DS firmware.
// See https://problemkaputt.de/gbatek.htm#dsfirmwareusersettings
struct user_settings {
    struct language_and_flags language_and_flags;
    uint8_t favorite_color;
    uint8_t birthday_month;
    uint8_t birthday_day;
    uint16_t nickname[10]; // UTF-16 string
    undefined2 field_0x18; // always 0; null terminator for nickname?
    uint16_t nickname_length;
    uint16_t message[26];  // UTF-16 string
    undefined2 field_0x50; // always 0; null terminator for message?
    uint16_t message_length;
};
ASSERT_SIZE(struct user_settings, 84);

// Fields of the DS system clock.
struct system_clock {
    undefined4 field_0x0; // always 0
    int second;
    int minute;
    int hour;
    int day;
    int month;
    int year;
};
ASSERT_SIZE(struct system_clock, 28);

// IPv4 Internet domain socket address.
struct sockaddr_in {
    uint8_t len; // always 8
    uint8_t family;
    uint16_t port;
    uint8_t ip[4];
};
ASSERT_SIZE(struct sockaddr_in, 8);

typedef void (*os_irq_function)(void);

typedef void (*lock_ctrl_funcp)(void);

typedef void (*os_switch_thread_callback)(struct thread* from, struct thread* to);

typedef void (*pxi_fifo_callback)(enum pxi_fifo_tag tag, uint32_t data, bool err);

struct os_lock_word {
    uint32_t lock_flag;
    uint16_t owner_id;
    uint16_t extension;
};
ASSERT_SIZE(struct os_lock_word, 8);

struct card_rom_stat {
    void (*read_func)(struct card_rom_stat*);
    uint32_t ctrl;
    uint8_t* cache_page;
    uint32_t dummy[5];
    uint8_t cache_buf[512];
};
ASSERT_SIZE(struct card_rom_stat, 544);

struct ctrdg_rom_cycle {
    enum mi_cartridge_rom_cycle_1st c1;
    enum mi_cartridge_rom_cycle_2nd c2;
};
ASSERT_SIZE(struct ctrdg_rom_cycle, 8);

struct ctrdg_lock_by_proc {
    bool locked;
    uint8_t dummy[3];
    enum os_intr_mode irq;
};
ASSERT_SIZE(struct ctrdg_lock_by_proc, 8);

#endif
