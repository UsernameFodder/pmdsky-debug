// Shared types used throughout the game

#ifndef HEADERS_TYPES_COMMON_H_
#define HEADERS_TYPES_COMMON_H_

#include "enums.h"
#include "util.h"
#include "../dungeon_mode/dungeon_mode_common.h"
#include "file_io.h"
#include "graphics.h"
#include "../files/wan.h"
#include "window.h"

// Parameters used by the NitroSDK to read the ROM.
struct start_module_params {
    void* autoload_list;
    void* autoload_list_end;
    void* autoload_start;
    void* static_bss_start;
    void* static_bss_end;
    uint32_t compressed_static_end;
    uint32_t version_id;
    uint32_t nitrocode_be;
    uint32_t nitrocode_le;
};
ASSERT_SIZE(struct start_module_params, 36);

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
    // allocated. The least significant byte is reserved for specifying the memory arena to use,
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
    // The 4 known arenas are:
    // - The default arena (used for most things, including dungeon mode)
    // - Two ground mode arenas (used in some cases, but not all)
    // - The sound data arena (used by the DSE sound engine)
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

struct overlay_load_entry {
    enum overlay_group_id group;
    // These are function pointers, but not sure of the signature.
    void* entrypoint;
    void* destructor;
    void* frame_update; // Possibly?
};
ASSERT_SIZE(struct overlay_load_entry, 16);

// Struct containing information about an overlay. The entries are copied from y9.bin.
struct overlay_info_entry {
    int overlay_id;                // 0x0
    int ram_address;               // 0x4
    int size;                      // 0x8
    int bss_size;                  // 0xC: Size of BSS data region
    int static_init_start_address; // 0x10
    int static_init_end_address;   // 0x14
    int file_id;                   // 0x18: File ID of this overlay in the ROM's FAT
    int unused;                    // 0x1C: Always zero
};
ASSERT_SIZE(struct overlay_info_entry, 32);

// This seems to be a simple structure used with utility functions related to managing items
// in bulk, such as in the player's bag, storage, and Kecleon shops.
struct bulk_item {
    struct item_id_16 id;
    uint16_t quantity; // Definitely in some contexts, but not verified in all
};
ASSERT_SIZE(struct bulk_item, 4);

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

// Type matchup combinator table, for combining two type matchups into one.
// This table is symmetric, and maps (type_matchup, type_matchup) -> type_matchup
struct type_matchup_combinator_table {
    enum type_matchup combination[4][4];
};
ASSERT_SIZE(struct type_matchup_combinator_table, 64);

// In the move data, the target and range are encoded together in the first byte of a single
// two-byte field. The target is the lower half, and the range is the upper half.
#pragma pack(push, 2)
struct move_target_and_range {
    enum move_target target : 4;
    enum move_range range : 4;
    enum move_ai_condition ai_condition : 4;
    uint16_t unused : 4; // At least I'm pretty sure this is unused...
};
ASSERT_SIZE(struct move_target_and_range, 2);
#pragma pack(pop)

// Data for a single move
struct move_data {
    uint16_t base_power;                          // 0x0
    struct type_id_8 type;                        // 0x2
    struct move_category_8 category;              // 0x3
    struct move_target_and_range target_range;    // 0x4
    struct move_target_and_range ai_target_range; // 0x6: Target/range as seen by the AI
    uint8_t pp;                                   // 0x8
    uint8_t ai_weight; // 0x9: Possibly. Weight for AI's random move selection
    // 0xA: Both accuracy values are used to calculate the move's actual accuracy.
    // See the PMD Info Spreadsheet.
    uint8_t accuracy1;
    uint8_t accuracy2; // 0xB
    // 0xC: If this move has a random chance AI condition (see enum move_ai_condition),
    // this is the chance that the AI will consider a potential target as elegible
    uint8_t ai_condition_random_chance;
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
    struct move_id_16 id; // 0x16
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

// Reduced version of dungeon_mode::move that stores less info
// Dungeon mode might also use these entries sometimes
struct ground_move {
    // 0x0: flags: 1-byte bitfield
    // See move::flags0 for details
    bool f_exists : 1;
    bool f_subsequent_in_link_chain : 1;
    bool f_enabled_for_ai : 1;
    bool f_set : 1;
    bool f_last_used : 1; // unconfirmed, but probably the same as struct move
    bool f_disabled : 1;
    uint8_t flags_unk6 : 2;

    undefined field_0x1;  // Probably padding since it doesn't get initialized
    struct move_id_16 id; // 0x2
    uint8_t ginseng;      // 0x4: Ginseng boost
    undefined field_0x5;  // Probably padding since it doesn't get initialized
};
ASSERT_SIZE(struct ground_move, 6);

// Used to store monster data in ground mode
// (Team members in the assembly, guest pokémon, etc.)
// Dungeon mode might also use these entries sometimes
struct ground_monster {
    bool is_valid;                 // 0x0: True if the entry is valid
    int8_t level;                  // 0x1: Monster level
    struct dungeon_id_8 joined_at; // 0x2
    uint8_t joined_at_floor;       // 0x3: See struct monster::joined_at_floor
    struct monster_id_16 id;       // 0x4: Monster ID
    int8_t level_at_first_evo;     // 0x6: Level upon first evolution, or 0 if not applicable
    int8_t level_at_second_evo;    // 0x7: Level upon second evolution, or 0 if not applicable
    uint16_t iq;                   // 0x8
    uint16_t max_hp;               // 0xA
    uint8_t offensive_stats[2];    // 0xC: {atk, sp_atk}
    uint8_t defensive_stats[2];    // 0xE: {def, sp_def}
    int exp;                       // 0x10
    // 0x14: Bitvector that keeps track of which IQ skills the monster has enabled.
    // See enum iq_skill_id for the meaning of each bit.
    uint32_t iq_skill_flags[3];
    struct tactic_id_8 tactic; // 0x20
    undefined field_0x21;
    struct ground_move moves[4]; // 0x22
    char name[10];               // 0x3A: Display name of the monster
};
ASSERT_SIZE(struct ground_monster, 68);

// Stores information about active team members, including those from special episodes.
// A lot of the fields seem to be analogous to fields on struct monster.
struct team_member {
    // 0x0: flags: 1-byte bitfield
    bool f_is_valid : 1;
    uint8_t flags_unk1 : 7;

    bool is_leader;                // 0x1
    uint8_t level;                 // 0x2
    struct dungeon_id_8 joined_at; // 0x3
    uint8_t joined_at_floor;       // 0x4
    undefined field_0x5;
    uint16_t iq;          // 0x6
    int16_t member_index; // 0x8: Index in the list of all team members (not just the active ones)
    int16_t team_index;   // 0xA: In order by team lineup
    struct monster_id_16 id;    // 0xC
    uint16_t current_hp;        // 0xE
    uint16_t max_hp;            // 0x10
    uint8_t offensive_stats[2]; // 0x12: {atk, sp_atk}
    uint8_t defensive_stats[2]; // 0x14: {def, sp_def}
    undefined field_0x16;
    undefined field_0x17;
    int exp;              // 0x18
    struct move moves[4]; // 0x1C
    undefined field_0x3C;
    undefined field_0x3D;
    struct item held_item;         // 0x3E
    int16_t belly;                 // 0x44: Integer part
    int16_t belly_thousandths;     // 0x46
    int16_t max_belly;             // 0x48: Integer part
    int16_t max_belly_thousandths; // 0x4A
    // 0x4C: Bitvector that keeps track of which IQ skills the monster has enabled.
    // See enum iq_skill_id for the meaning of each bit.
    uint32_t iq_skill_flags[3];
    struct tactic_id_8 tactic; // 0x58
    undefined field_0x59;
    undefined field_0x5A;
    undefined field_0x5B;
    undefined field_0x5C;
    undefined field_0x5D;
    char name[10]; // 0x5E: Display name of the monster
};
ASSERT_SIZE(struct team_member, 104);

// Table with information about all team members, which are active, and on which teams
struct team_member_table {
    // 0x0: List of all recruited team members. Appears to be in chronological order of recruitment.
    //
    // The first two entries are fixed to the hero and partner. The next three entries are reserved
    // for special episode main characters, which differ (and will be updated here) depending on the
    // special episode. For example, in SE5, the third entry becomes Grovyle, with the fourth and
    // fifth entries becoming Dusknoir after progressing far enough into the special episode.
    //
    // Subsequent entries are normal recruits. If a member is released, all subsequent members will
    // be shifted up, so there should be no gaps in the list.
    struct ground_monster members[555];
    // 0x936C: Currently active team members for each team, listed in team order. The first index is
    // the team ID (see enum team_id), the second is the roster index within the given team.
    //
    // This struct is updated relatively infrequently. For example, in dungeon mode, it's typically
    // only updated at the start of the floor; refer to DUNGEON_STRUCT instead for live data.
    struct team_member active_team_rosters[3][4];
    // 0x984C: Pointer into active_team_rosters for the currently active team, i.e.,
    // &active_team_rosters[active_team]
    struct team_member* active_roster;
    // 0x9850: Number of active members on TEAM_MAIN
    int16_t number_active_team_members_main;
    // 0x9852: Number of active members on TEAM_SPECIAL_EPISODE
    int16_t number_active_team_members_se;
    // 0x9854: Number of active members on TEAM_RESCUE
    int16_t number_active_team_members_rescue;
    // 0x9856: member indexes (into the members array) for the active rosters of each team
    int16_t active_team_roster_member_idxs[3][4];
    undefined field_0x986e;
    undefined field_0x986f;
    // 0x9870: Pointer into active_team_roster_member_idxs for the currently active team, i.e.,
    // &active_team_roster_member_idxs[active_team]
    int16_t* active_roster_member_idxs;
    undefined field_0x9874;       // Related to TEAM_MAIN (Guess)
    undefined field_0x9875;       // Related to TEAM_SPECIAL_EPISODE
    undefined field_0x9876;       // Related to TEAM_RESCUE
    struct team_id_8 active_team; // 0x9877: Currently active team
    undefined4 field_0x9878;      // Somehow related to explorer maze team.
    undefined4 field_0x987C;      // Somehow related to explorer maze team.
    // 0x9880: language type of explorer maze team
    int8_t explorer_maze_team_native_language;
    undefined field_0x9881; // Somehow related to explorer maze team.
    // 0x9882: Name of the explorer maze team. If the native language of the team doesn't match
    // our native language, use the default team name ("Pokémones" for NA) for the explorer
    // maze team. When initally saving the team name, it will use all 20 bytes, but when
    // copying the name to use in game, it will only use the first 10 bytes.
    char explorer_maze_team_name[20];
    undefined field_0x9896; // Padding?
    undefined field_0x9897; // Padding?
    // 0x9898: The 4 explorer maze monsters from selecting 'Team Trade'.
    struct ground_monster explorer_maze_monsters[4];
};
ASSERT_SIZE(struct team_member_table, 39336);

// Contains information about a monster's level-up data at a certain level
struct level_up_entry {
    uint32_t total_exp; // 0x0: Total EXP required to reach this level
    uint16_t hp;        // 0x4: HP increase
    uint8_t atk;        // 0x6: Atk increase
    uint8_t sp_atk;     // 0x7: Sp. Atk increase
    uint8_t def;        // 0x8: Def increase
    uint8_t sp_def;     // 0x9: Sp. Def increase
    undefined field_0xA;
    undefined field_0xB;
};
ASSERT_SIZE(struct level_up_entry, 12);

// A common structure for pairs of dungeon/floor values
struct dungeon_floor_pair {
    struct dungeon_id_8 dungeon_id;
    uint8_t floor_id;
};
ASSERT_SIZE(struct dungeon_floor_pair, 2);

// Unknown struct included in the dungeon_init struct (see below)
struct unk_dungeon_init {
    undefined field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    undefined field_0x4;
    undefined field_0x5;
    undefined field_0x6;
    undefined field_0x7;
    undefined field_0x8;
    undefined field_0x9;
    undefined field_0xA;
    undefined field_0xB;
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
    undefined field_0x20;
    undefined field_0x21;
    undefined field_0x22;
    undefined field_0x23;
    undefined field_0x24;
    undefined field_0x25;
    undefined field_0x26;
    undefined field_0x27;
    undefined field_0x28;
    undefined field_0x29;
    undefined field_0x2A;
    undefined field_0x2B;
    undefined field_0x2C;
    undefined field_0x2D;
    undefined field_0x2E;
    undefined field_0x2F;
    undefined field_0x30;
    undefined field_0x31;
    undefined field_0x32;
    undefined field_0x33;
    undefined field_0x34;
    undefined field_0x35;
    undefined field_0x36;
    undefined field_0x37;
    undefined field_0x38;
    undefined field_0x39;
    undefined field_0x3A;
    undefined field_0x3B;
    undefined field_0x3C;
    undefined field_0x3D;
    undefined field_0x3E;
    undefined field_0x3F;
    undefined field_0x40;
    undefined field_0x41;
    undefined field_0x42;
    undefined field_0x43;
    undefined field_0x44;
    undefined field_0x45;
    undefined field_0x46;
    undefined field_0x47;
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
    undefined field_0x64;
    undefined field_0x65;
    undefined field_0x66;
    undefined field_0x67;
    undefined field_0x68;
    undefined field_0x69;
    undefined field_0x6A;
    undefined field_0x6B;
    undefined field_0x6C;
    undefined field_0x6D;
    undefined field_0x6E;
    undefined field_0x6F;
    undefined field_0x70;
    undefined field_0x71;
    undefined field_0x72;
    undefined field_0x73;
    undefined field_0x74;
    undefined field_0x75;
    undefined field_0x76;
    undefined field_0x77;
    undefined field_0x78;
    undefined field_0x79;
    undefined field_0x7A;
    undefined field_0x7B;
    undefined field_0x7C;
    undefined field_0x7D;
    undefined field_0x7E;
    undefined field_0x7F;
    undefined field_0x80;
    undefined field_0x81;
    undefined field_0x82;
    undefined field_0x83;
    undefined field_0x84;
    undefined field_0x85;
    undefined field_0x86;
    undefined field_0x87;
    undefined field_0x88;
    undefined field_0x89;
    undefined field_0x8A;
    undefined field_0x8B;
    undefined field_0x8C;
    undefined field_0x8D;
    undefined field_0x8E;
    undefined field_0x8F;
    undefined field_0x90;
    undefined field_0x91;
    undefined field_0x92;
    undefined field_0x93;
    undefined field_0x94;
    undefined field_0x95;
    undefined field_0x96;
    undefined field_0x97;
    undefined field_0x98;
    undefined field_0x99;
    undefined field_0x9A;
    undefined field_0x9B;
    undefined field_0x9C;
    undefined field_0x9D;
    undefined field_0x9E;
    undefined field_0x9F;
    undefined field_0xA0;
    undefined field_0xA1;
    undefined field_0xA2;
    undefined field_0xA3;
    undefined field_0xA4;
    undefined field_0xA5;
    undefined field_0xA6;
    undefined field_0xA7;
    undefined field_0xA8;
    undefined field_0xA9;
    undefined field_0xAA;
    undefined field_0xAB;
    undefined field_0xAC;
    undefined field_0xAD;
    undefined field_0xAE;
    undefined field_0xAF;
    undefined field_0xB0;
    undefined field_0xB1;
    undefined field_0xB2;
    undefined field_0xB3;
    undefined field_0xB4;
    undefined field_0xB5;
    undefined field_0xB6;
    undefined field_0xB7;
    undefined field_0xB8;
    undefined field_0xB9;
    undefined field_0xBA;
    undefined field_0xBB;
    undefined field_0xBC;
    undefined field_0xBD;
    undefined field_0xBE;
    undefined field_0xBF;
    undefined field_0xC0;
    undefined field_0xC1;
    undefined field_0xC2;
    undefined field_0xC3;
    undefined field_0xC4;
    undefined field_0xC5;
    undefined field_0xC6;
    undefined field_0xC7;
    undefined field_0xC8;
    undefined field_0xC9;
    undefined field_0xCA;
    undefined field_0xCB;
    undefined field_0xCC;
    undefined field_0xCD;
    undefined field_0xCE;
    undefined field_0xCF;
    undefined field_0xD0;
    undefined field_0xD1;
    undefined field_0xD2;
    undefined field_0xD3;
    undefined field_0xD4;
    undefined field_0xD5;
    undefined field_0xD6;
    undefined field_0xD7;
    undefined field_0xD8;
    undefined field_0xD9;
    undefined field_0xDA;
    undefined field_0xDB;
    undefined field_0xDC;
    undefined field_0xDD;
    undefined field_0xDE;
    undefined field_0xDF;
    undefined field_0xE0;
    undefined field_0xE1;
    undefined field_0xE2;
    undefined field_0xE3;
    undefined field_0xE4;
    undefined field_0xE5;
    undefined field_0xE6;
    undefined field_0xE7;
};
ASSERT_SIZE(struct unk_dungeon_init, 232);

// A struct used to init certain values in the dungeon struct when entering dungeon mode.
// Gets initialized in ground mode.
struct dungeon_init {
    struct dungeon_id_8 id; // 0x0: Copied into dungeon::id
    uint8_t floor;          // 0x1: Copied into dungeon::floor
    // Copied into dungeon::field_0x74C, might be related to the dungeon being conquered or
    // the fixed room overrides.
    undefined2 field_0x2;
    undefined field_0x4;
    bool nonstory_flag;      // 0x5: Copied into dungeon::nonstory_flag
    bool recruiting_enabled; // 0x6: Copied into dungeon::recruiting_enabled
    // 0x7: If true, dungeon::recruiting_enabled gets set to false. Overrides recruiting_enabled.
    bool force_disable_recruiting;
    undefined field_0x8; // Copied into dungeon::field_0x75A
    // 0x9: Copied into dungeon::field_0x75B, but the value is immediately overwritten with TRUE.
    bool field_0x9;
    bool send_home_disabled;        // 0xA: Copied into dungeon::send_home_disabled
    bool hidden_land_flag;          // 0xB: Copied into dungeon::hidden_land_flag
    bool skip_faint_animation_flag; // 0xC: Copied into dungeon::skip_faint_animation_flag
    // 0xD: Copied into dungeon::dungeon_objective. Read as a signed byte (?).
    struct dungeon_objective_8 dungeon_objective;
    int8_t field_0xE;
    bool has_guest_pokemon; // 0xF: If true, a guest pokémon will be added to your team
    bool send_help_item;    // 0x10: If true, you recive an item at the start of the dungeon
    bool show_rescues_left; // 0x11: If true, you get a message saying how many rescue chances you
                            // have left
    undefined field_0x12;
    undefined field_0x13;
    // 0x14
    // [EU]0x22DF920 loads this as a word
    // [EU]0x22DFBAC loads this as a signed byte
    // ???
    undefined4 field_0x14; // Copied into dungeon::field_0x750
    // 0x18: The dungeon PRNG preseed? Copied into dungeon::prng_preseed_23_bit and
    // dungeon::rescue_prng_preseed_23_bit.
    uint32_t prng_preseed_23_bit;
    // 0x1C: Array containing the list of quest pokémon that will join the team in the dungeon
    // (max 2)
    struct ground_monster guest_pokemon[2];
    // 0xA4: Used as a base address at [EU]0x22E0354 and [EU]0x22E03AC.
    // It's probably a separate struct.
    undefined field_0xA4;
    undefined field_0xA5;
    undefined field_0xA6;
    undefined field_0xA7;
    // 0xA8: ID of the item to give to the player if send_help_item is true
    struct item_id_16 help_item;
    undefined field_0xAA;
    undefined field_0xAB;
    // 0xAC: Controls which version of the dungeon to load. Gets copied into
    // dungeon::dungeon_game_version_id. Uncertain when the game decides to load the
    // Time/Darkness version of dungeons.
    enum game_id dungeon_game_version_id;
    undefined4 field_0xB0;
    undefined field_0xB4; // Gets set to dungeon::id during dungeon init
    undefined field_0xB5; // Gets set to dungeon::floor during dungeon init
    undefined field_0xB6;
    undefined field_0xB7;
    // 0xB8: Used as a base address at [EU]0x22E0ABC.
    // It's probably a separate struct.
    undefined field_0xB8; // Gets set to dungeon::id during dungeon init
    undefined field_0xB9; // Gets set to dungeon::floor during dungeon init
    undefined field_0xBA;
    undefined field_0xBB;
    undefined4 field_0xBC;
    // 0xC0: Used as a base address at [EU]0x22E0A4C
    struct unk_dungeon_init field_0xC0;
    undefined field_0x1A8;
    // Probably padding, these bytes aren't accessed by the funtion that inits this struct
    undefined field_0x1A9;
    undefined field_0x1AA;
    undefined field_0x1AB;
};
ASSERT_SIZE(struct dungeon_init, 428);

// Unverified, ported from Irdkwia's notes
struct dungeon_unlock_entry {
    struct dungeon_id_8 dungeon_id;
    uint8_t scenario_balance_min;
};
ASSERT_SIZE(struct dungeon_unlock_entry, 2);

// Unverified, ported from Irdkwia's notes
struct dungeon_return_status {
    bool flag;
    uint8_t _padding;
    int16_t string_id;
};
ASSERT_SIZE(struct dungeon_return_status, 4);

// Structure describing various player progress milestones?
// Ported directly from Irdkwia's notes. The only confirmed thing is the struct size.
struct global_progress {
    undefined unk_pokemon_flags1[148];        // 0x0: unused
    undefined field_0x94[4];                  // 0x94
    undefined unk_pokemon_flags2[148];        // 0x98: used
    undefined exclusive_pokemon_flags[23];    // 0x12C: partially used, only for Time/Darkness
    undefined dungeon_max_reached_floor[180]; // 0x143: used
    undefined field_0x1f7;                    // unused
    undefined4 nb_adventures;                 // 0x1F8: used
    undefined field_0x1fc[16];                // unknown/unused
};
ASSERT_SIZE(struct global_progress, 524);

// The adventure log structure.
struct adventure_log {
    uint32_t completion_flags[4];            // 0x0
    uint32_t nb_dungeons_cleared;            // 0x10
    uint32_t nb_friend_rescues;              // 0x14
    uint32_t nb_evolutions;                  // 0x18
    uint32_t nb_eggs_hatched;                // 0x1C
    uint32_t successful_steals;              // 0x20: Unused in Sky
    uint32_t nb_faints;                      // 0x24
    uint32_t nb_victories_on_one_floor;      // 0x28
    uint32_t pokemon_joined_counter;         // 0x2C
    uint32_t pokemon_battled_counter;        // 0x30
    uint32_t moves_learned_counter;          // 0x34
    uint32_t nb_big_treasure_wins;           // 0x38
    uint32_t nb_recycled;                    // 0x3C
    uint32_t nb_gifts_sent;                  // 0x40
    uint32_t pokemon_joined_flags[37];       // 0x44
    uint32_t pokemon_battled_flags[37];      // 0xD8
    uint32_t moves_learned_flags[17];        // 0x16C
    uint32_t items_acquired_flags[44];       // 0x1B0
    uint32_t special_challenge_flags;        // 0x260
    uint32_t sentry_duty_game_points[5];     // 0x264
    struct dungeon_floor_pair current_floor; // 0x278
    uint16_t padding;                        // 0x27A
};
ASSERT_SIZE(struct adventure_log, 636);

struct exclusive_item_stat_boost_entry {
    int8_t atk;
    int8_t def;
    int8_t sp_atk;
    int8_t sp_def;
};
ASSERT_SIZE(struct exclusive_item_stat_boost_entry, 4);

struct exclusive_item_effect_entry {
    struct exclusive_item_effect_id_8 effect_id;
    uint8_t foreign_idx; // Index into other tables
};
ASSERT_SIZE(struct exclusive_item_effect_entry, 2);

struct rankup_table_entry {
    undefined field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    int field_0x4;
    int field_0x8;
    int16_t field_0xc;
    undefined field_0xe;
    undefined field_0xf;
};
ASSERT_SIZE(struct rankup_table_entry, 16);

// Contains the data for a single mission
struct mission {
    struct mission_status_8 status; // 0x0
    struct mission_type_8 type;     // 0x1
    union mission_subtype subtype;  // 0x2
    undefined field_0x3;
    struct dungeon_id_8 dungeon_id; // 0x4
    uint8_t floor;                  // 0x5
    undefined field_0x6;            // Likely padding
    undefined field_0x7;            // Likely padding
    int field_0x8;                  // 0x8, changing it seems to affect the text of the mission
    undefined field_0xc;
    undefined field_0xd;
    struct monster_id_16 client; // 0xE
    struct monster_id_16 target; // 0x10
    int16_t field_0x12;
    int16_t field_0x14;
    struct mission_reward_type_8 reward_type; // 0x16
    undefined field_0x17;
    struct item_id_16 item_reward;                      // 0x18
    struct mission_restriction_type_8 restriction_type; // 0x1A
    undefined field_0x1b;
    union mission_restriction restriction; // 0x1C
    undefined field_0x1e;
    undefined field_0x1f;
};
ASSERT_SIZE(struct mission, 32);

struct mission_floors_forbidden {
    uint8_t field_0x0;
    uint8_t field_0x1;
};
ASSERT_SIZE(struct mission_floors_forbidden, 2);

// Unverified, ported from Irdkwia's notes
struct quiz_answer_points_entry {
    undefined field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    undefined field_0x4;
    undefined field_0x5;
    undefined field_0x6;
    undefined field_0x7;
    undefined field_0x8;
    undefined field_0x9;
    undefined field_0xa;
    undefined field_0xb;
    undefined field_0xc;
    undefined field_0xd;
    undefined field_0xe;
    undefined field_0xf;
};
ASSERT_SIZE(struct quiz_answer_points_entry, 16);

// Unverified, ported from Irdkwia's notes
struct status_description {
    int16_t name_str_id;
    int16_t desc_str_id;
};
ASSERT_SIZE(struct status_description, 4);

// Unverified, ported from Irdkwia's notes
struct forbidden_forgot_move_entry {
    struct monster_id_16 monster_id;
    struct dungeon_id_16 origin_id;
    struct move_id_16 move_id;
};
ASSERT_SIZE(struct forbidden_forgot_move_entry, 6);

struct version_exclusive_monster {
    struct monster_id_16 id;
    bool in_eot; // In Explorers of Time
    bool in_eod; // In Explorers of Darkness
};
ASSERT_SIZE(struct version_exclusive_monster, 4);

// An entry correspond to sprite loaded in memory and ready to be displayed
struct wan_table_entry {
    char path[32];                  // 0x0: Needs to be null-terminated. Only used for direct file.
    bool file_externally_allocated; // 0x20: True if the iov_base shouldn’t be freed by this struct.
    struct wan_source_type_8 source_type; // 0x21: 1 = direct file, 2 = pack file
    int16_t pack_id;                      // 0x22: for wan in pack file
    int16_t file_index;                   // 0x24: for wan in pack file
    undefined field5_0x26;
    undefined field6_0x27;
    uint32_t iov_len;
    // 0x2C: When removing a reference, if it reaches 0, the entry is removed (unless
    // file_externally_allocated is true, as it is always removed even if there are remaining
    // references)
    int16_t reference_counter;
    undefined field9_0x2e;
    undefined field10_0x2f;
    // 0x30: pointer to the beginning of the data section of iov_base.
    struct wan_header* sprite_start;
    void* iov_base; // 0x34: points to a sirO
};
ASSERT_SIZE(struct wan_table_entry, 56);

// Global structure used to deduplicate loading of wan sprites. Loaded sprites are
// reference-counted.
struct wan_table {
    struct wan_table_entry sprites[96]; // 0x0
    void* at_decompress_scratch_space;  // 0x1500
    undefined field2_0x1504;
    undefined field3_0x1505;
    undefined field4_0x1506;
    undefined field5_0x1507;
    int16_t total_nb_of_entries;  // 0x1508: The total number of entries. Should be equal to 0x60.
    int16_t next_alloc_start_pos; // 0x150A
    int16_t field8_0x150c;
    undefined field9_0x150e;
    undefined field10_0x150f;
};
ASSERT_SIZE(struct wan_table, 5392);

// Store one boolean per vram bank
struct vram_banks_set {
    bool vram_A : 1;
    bool vram_B : 1;
    bool vram_C : 1;
    bool vram_D : 1;
    bool vram_E : 1;
    bool vram_F : 1;
    bool vram_G : 1;
    bool vram_H : 1;
    bool vram_I : 1;
    uint8_t _unused : 7;
};
ASSERT_SIZE(struct vram_banks_set, 2);

// Used as a parameter to SendAudioCommand. Includes data on which audio to play and how.
struct audio_command {
    // 0x0: Seems to be a value that marks the status of this entry. It's probably an enum, maybe a
    // command ID. Seems to be 0 when the entry is not in use.
    int status;
    struct music_id_16 music_id; // 0x4: ID of the music to play
    uint16_t volume;             // 0x6: Volume (0-255)
    undefined2 field_0x8;
    undefined field_0xA;
    undefined field_0xB;
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
};
ASSERT_SIZE(struct audio_command, 32);

// Contains data for an audio track that is being played
struct track_data {
    undefined field_0x0;
    undefined field_0x1;
    bool active; // 0x2: True if the track is active
    undefined field_0x3;
    undefined field_0x4;
    undefined field_0x5;
    undefined field_0x6;
    undefined field_0x7;
    int play_amount; // 0x8: Number of times the track has been played so far
    // 0xC: Delay (in frames, probably) before the next DSE event on this track begins
    int event_delay;
    undefined field_0x10;
    undefined field_0x11;
    undefined field_0x12;
    undefined field_0x13;
    undefined field_0x14;
    undefined field_0x15;
    undefined field_0x16;
    undefined field_0x17;
    void* track_data_start; // 0x18: Pointer to the start of the track's audio data
    void* current_event;    // 0x1C: Pointer to the current DSE event
};
ASSERT_SIZE(struct track_data, 32); // Exact size hasn't been confirmed

// Data about a wavi container
struct wavi_data {
    undefined field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    undefined field_0x4;
    undefined field_0x5;
    undefined field_0x6;
    undefined field_0x7;
    int16_t num_entries; // 0x8: Number of entries in the container
    undefined field_0xA;
    undefined field_0xB;
    undefined field_0xC;
    undefined field_0xD;
    undefined field_0xE;
    undefined field_0xF;
    void* pointer_table_start; // 0x10: Pointer to the start of the pointer table
};
ASSERT_SIZE(struct wavi_data, 20); // Likely longer

// TODO: Add more data file structures, as convenient or needed, especially if the load address
// or pointers to the load address are known.

#endif
