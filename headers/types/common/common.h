// Shared types used throughout the game

#ifndef HEADERS_TYPES_COMMON_H_
#define HEADERS_TYPES_COMMON_H_

#include "enums.h"

// Based on the code for vsprintf(3), it seems like va_list is implemented in the ARM9 binary
// by just passing a pointer into the stack, so define va_list to be void*.
typedef void* va_list;

// A slice in the usual programming sense: a pointer, length, and capacity.
// Used for the implementation of vsprintf(3), but maybe it's used elsewhere as well.
struct slice {
    void* data;        // Pointer to the data buffer
    uint32_t capacity; // How much space is available in total
    uint32_t length;   // How much space is currently filled
};
ASSERT_SIZE(struct slice, 12);

// Function to append data to a struct slice, and return a success flag.
typedef bool (*slice_append_fn_t)(struct slice* slice, void* data, uint32_t data_len);

// This is essentially the standard (32-bit) Unix I/O vector struct.
// It's used for file I/O and represents a buffer of data with a pointer and a length.
struct iovec {
    void* iov_base;
    uint32_t iov_len;
};
ASSERT_SIZE(struct iovec, 8);

// Program position info (basically stack trace info) for debug logging.
struct prog_pos_info {
    char* file; // file name
    int line;   // line number
};
ASSERT_SIZE(struct prog_pos_info, 8);

// Metadata describing a single memory block. A block is a chunk of dynamically allocated memory.
// It can contain nothing, an allocated object, or a memory arena that itself contains blocks.
struct mem_block {
    // 0x0: The type of content in this block (see enum memory_alloc_flag).
    uint32_t f_in_use : 1; // Block is reserved and cannot be split to accomodate new allocations
    uint32_t f_object : 1; // Block contains a normal object
    uint32_t f_arena : 1;  // Block contains a memory arena
    uint32_t content_flags_unused : 29;

    // 0x4: Flags passed internally to the memory allocator when this block was allocated
    // (see enum memory_alloc_flag).
    uint32_t f_alloc_in_use : 1; // Block was reserved when allocated
    uint32_t f_alloc_object : 1; // Block was allocated as an object
    // Block was allocated as a memory arena.
    // f_alloc_arena is always used in tandem with f_alloc_in_use, and influences how the
    // allocator tries to locate a free block of memory to allocate. However, the arena will
    // be for private use only, and will be viewed as in use by the allocator until the arena
    // is freed.
    uint32_t f_alloc_arena : 1;
    // Block was allocated as a subarena.
    // An allocation with f_alloc_subarena is used when creating a new global memory arena,
    // and guarantees that f_arena will be set and that f_in_use will NOT be set, which allows
    // blocks to be carved out when the allocator needs memory.
    uint32_t f_alloc_subarena : 1;
    uint32_t allocator_flags_unused : 28;

    // 0x8: Flags passed by the user to the memory allocator API functions when this block was
    // allocated. The least significant byte are reserved for specifying the memory arena to use,
    // and have functionality determined by the arena locator function currently in use by the
    // game. The upper bytes are the same as the internal memory allocator flags
    // (just left-shifted by 8).
    uint32_t user_flags : 8;
    uint32_t f_user_alloc_in_use : 1;   // See f_alloc_in_use
    uint32_t f_user_alloc_object : 1;   // See f_alloc_object
    uint32_t f_user_alloc_arena : 1;    // See f_alloc_arena
    uint32_t f_user_alloc_subarena : 1; // See f_alloc_subarena
    uint32_t user_flags_unused : 20;

    void* data;         // 0xC: Pointer to the start of the memory block
    uint32_t available; // 0x10: Number of free bytes in the memory block. Always a multiple of 4.
    uint32_t used;      // 0x14: Number of used bytes in the memory block. Always a multiple of 4.
};
ASSERT_SIZE(struct mem_block, 24);

// Metadata for a memory arena. A memory arena is a large, contiguous region of memory that can
// be carved up into chunks by the memory allocator as needed (when dynamic allocations are
// requested). An arena starts with one large, vacant block, which gradually gets subdivided as
// allocations are made within the arena.
struct mem_arena {
    // 0x0: The type of content in this arena, as a bitfield (see enum memory_alloc_flag).
    // Always seems to be 2 (corresponding to MEM_OBJECT), which makes sense since an arena
    // is by definition a region where objects can be allocated.
    uint32_t content_flags;
    // 0x4: Pointer to the parent arena if this is a subarena, or null otherwise.
    struct mem_arena* parent;
    struct mem_block* blocks; // 0x8: Array of memory blocks in the arena
    uint32_t n_blocks;        // 0xC: Number of memory blocks in the arena
    uint32_t max_blocks;      // 0x10: Maximum number of memory blocks the arena can hold
    void* data;               // 0x14: Pointer to the start of the memory arena
    uint32_t len;             // 0x18: Total length of the memory arena. Always a multiple of 4.
};
ASSERT_SIZE(struct mem_arena, 28);

// Global table of all heap allocations
struct mem_alloc_table {
    uint32_t n_arenas;              // 0x0: Number of global memory arenas (including subarenas)
    struct mem_arena default_arena; // 0x4: The default memory arena for allocations
    // Not actually sure how long this array is, but has at least 4 elements, and can't have
    // more than 8 because it would overlap with default_arena.data
    struct mem_arena* arenas[8]; // 0x20: Array of global memory arenas
};
ASSERT_SIZE(struct mem_alloc_table, 64);

// Functions to get the desired memory arena for (de)allocation, or null if there's no preference.
// flags has the same meaning as the flags passed to MemAlloc.
typedef struct mem_arena* (*get_alloc_arena_fn_t)(struct mem_arena* arena, uint32_t flags);
typedef struct mem_arena* (*get_free_arena_fn_t)(struct mem_arena* arena, void* ptr);

struct mem_arena_getters {
    get_alloc_arena_fn_t get_alloc_arena; // Arena to be used by MemAlloc and friends
    get_free_arena_fn_t get_free_arena;   // Arena to be used by MemFree
};
ASSERT_SIZE(struct mem_arena_getters, 8);

// This seems to be a simple structure used with utility functions related to managing items in
// the player's bag and storage.
struct owned_item {
    enum item_id id : 16;
    uint16_t amount; // Probably? This is a guess
};
ASSERT_SIZE(struct owned_item, 4);

struct rect16_xywh {
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
};
ASSERT_SIZE(struct rect16_xywh, 8);

struct rgb_24 {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};
ASSERT_SIZE(struct rgb_24, 3);

// The 4th byte may sometimes be used, and sometimes it is merely padding
struct rgb_32 {
    uint8_t r;
    uint8_t b;
    uint8_t g;
    uint8_t x;
};
ASSERT_SIZE(struct rgb_32, 4);

// A structure that represents a file stream for file I/O.
struct file_stream {
    undefined4 field_0x0;
    undefined4 field_0x4;
    undefined4 field_0x8;
    undefined4 field_0xc;
    undefined4 field_0x10;
    undefined4 field_0x14;
    undefined4 field_0x18;
    undefined4 field_0x1c;
    undefined4 field_0x20;
    undefined4 field_0x24;
    undefined4 field_0x28;
    undefined4 field_0x2c;
    undefined4 field_0x30;
    undefined4 field_0x34;
    undefined4 field_0x38;
    undefined4 field_0x3c;
    undefined4 field_0x40;
    undefined4 field_0x44;
};
ASSERT_SIZE(struct file_stream, 72);

/*  Handle to a memory-allocated WTE file.

    The WTE file format is a simple file format found both in file directories
    and in the ROM filesystem. This format specializes in storing texture data,
    and is closely linked to the 3D engine, as well as the 3D resource manager!

    While EoS is not a 3D game, the game still utilizes the 3D hardware to draw
    graphics onto the screen. Examples of its usage are the Dungeon Mode GUI
    (DUNGEON/dungeon.bin+0x3F4) and the fog (DUNGEON/dungeon.bin+0x401) */
struct wte_handle {
    void* content; // Pointer to the heap-allocated WTE data. Only stored for freeing the data
    struct wte_header* header;
};
ASSERT_SIZE(struct wte_handle, 8);

// These arguments are almost directly passed to the TEXIMAGE_PARAM register, just rearranged
// For more information see:
// https://problemkaputt.de/gbatek.htm#ds3dtextureattributes
struct wte_texture_params {
    uint8_t texture_smult : 3;
    uint8_t texture_tmult : 3;
    uint8_t unused6 : 2;
    enum texture_format format : 3;
    bool repeat_x : 1;
    bool repeat_y : 1;
    uint8_t unusedD : 3;
};
ASSERT_SIZE(struct wte_texture_params, 2);

struct wte_header {
    char signature[4];                // 0x0: Signature bytes (must be "\x57\x54\x45\x00")
    void* texture;                    // 0x4
    uint32_t texture_size;            // 0x8
    struct wte_texture_params params; // 0xC
    uint16_t _padding_0xe;
    /*  These bounds are NOT used by the game, but they prove useful to extract the texture out
        of the file. The offsets are redundant and should be zero

        The width specified here should always be the same as the one specified in the texture
        params, but the height may be lower. The reason is that just like the width, the height
        needs to be a power of 2 in the range of 8..1024. The actual texture can have a lower
        height, but not a lower width, as the width is required to properly read the image */
    struct rect16_xywh texture_bounds; // 0x10
    struct rgb_32* palette;            // 0x18
    uint16_t color_amt;                // 0x1C: How many colors are stored in the palette
    uint16_t _padding_0x1e;
};
ASSERT_SIZE(struct wte_header, 32);

// These flags are shared with the function to display text inside message boxes
// So they might need a rename once more information is found
struct preprocessor_flags {
    uint16_t unknown0 : 13;
    bool show_speaker : 1;
    uint32_t unknown18 : 18;
};
ASSERT_SIZE(struct preprocessor_flags, 4);

// Represents arguments that might be passed to the PreprocessString function
struct preprocessor_args {
    uint32_t flag_vals[4]; // These are usually IDs with additional flags attached to them
    uint32_t id_vals[5];
    int32_t number_vals[5];
    char* strings[5];
    // An optional argument that is used to insert the name of a Pokémon
    // When they're talking through a message box. It requires it's respective flag to be on
    uint32_t speaker_id;
};
ASSERT_SIZE(struct preprocessor_args, 80);

struct type_matchup_16 {
    enum type_matchup val : 16;
};
ASSERT_SIZE(struct type_matchup_16, 2);

// Type matchup table, not including TYPE_NEUTRAL.
// Note that Ghost's immunities seem to be hard-coded elsewhere. In this table, both Normal and
// Fighting are encoded as neutral against Ghost.
//
// Row index corresponds to the attack type and the column index corresponds to the defender type.
// C-style access: type_matchup_table[attack_type][target_type] or
// *(&type_matchup_table[0][0] + attack_type*18 + target_type)
struct type_matchup_table {
    struct type_matchup_16 matchups[18][18];
};
ASSERT_SIZE(struct type_matchup_table, 648);

// In the move data, the target and range are encoded together in the first byte of a single
// two-byte field. The target is the lower half, and the range is the upper half.
struct move_target_and_range {
    enum move_target : 4;
    enum move_range : 4;
    enum healing_move_type : 4;
    uint16_t unused : 4; // At least I'm pretty sure this is unused...
};
ASSERT_SIZE(struct move_target_and_range, 2);

// Data for a single move
struct move_data {
    uint16_t base_power;                          // 0x0
    enum type_id type : 8;                        // 0x2
    enum move_category category : 8;              // 0x3
    struct move_target_and_range target_range;    // 0x4
    struct move_target_and_range ai_target_range; // 0x6: Target/range as seen by the AI
    uint8_t pp;                                   // 0x8
    uint8_t ai_weight; // 0x9: Possibly. Weight for AI's random move selection
    // 0xA: Both accuracy values are used to calculate the move's actual accuracy.
    // See the PMD Info Spreadsheet.
    uint8_t accuracy1;
    uint8_t accuracy2;            // 0xB
    uint8_t field_0xc;            // unknown
    uint8_t strikes;              // 0xD: Number of times the move hits (i.e. for multi-hit moves)
    uint8_t max_ginseng_boost;    // 0xE: Maximum possible Ginseng boost for this move
    uint8_t crit_chance;          // 0xF: The base critical hit chance
    bool reflected_by_magic_coat; // 0x10
    bool can_be_snatched;         // 0x11
    bool fails_while_muzzled;     // 0x12
    // 0x13: Seems to be used by the AI with Status Checker for using moves against frozen monsters
    bool ai_can_use_against_frozen;
    bool usable_while_taunted; // 0x14
    // 0x15: Index in the string files of the range string to be displayed in the move info screen
    uint8_t range_string_idx;
    enum move_id id : 16; // 0x16
    // 0x18: Index in the string files of the message string to be displayed in the dungeon message
    // log when a move is used. E.g., the default (0) is "[User] used [move]!"
    uint16_t message_string_idx;
};
ASSERT_SIZE(struct move_data, 26);

// The move data table, as contained within /BALANCE/waza_p.bin, and when loaded into memory.
struct move_data_table {
    struct move_data entries[559];
};
ASSERT_SIZE(struct move_data_table, 14534);

// TODO: Add more data file structures, as convenient or needed, especially if the load address
// or pointers to the load address are known.

#endif
