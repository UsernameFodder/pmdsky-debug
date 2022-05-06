// Types used primarily in the context of ground mode (also sometimes called "script mode")

#ifndef HEADERS_TYPES_GROUND_MODE_H_
#define HEADERS_TYPES_GROUND_MODE_H_

#include "enums.h"

// Variables that track game state, available to the script engine.
struct script_var {
    struct script_var_type_16 type; // 0x0: type of data contained in this variable
    undefined2 field_0x1;           // 0x2
    // 0x4: value's offset into struct script_var_value_table, if type != VARTYPE_SPECIAL
    uint16_t mem_offset;
    uint16_t bitshift; // 0x6: bit position if type == VARTYPE_BIT
    uint16_t n_values; // 0x8: number of values (>1 means this variable is an array)
    // 0xA: 0 for every variable except VAR_VERSION, which has a default value of 1.
    int16_t default_val;
    char* name; // 0xC: variable name
};
ASSERT_SIZE(struct script_var, 16);

// Table of all predefined global variables available to the script engine.
struct script_var_table {
    struct script_var vars[115];
};
ASSERT_SIZE(struct script_var_table, 1840);

// Special "local" variables available to the script engine.
// All four local variables are of type VARTYPE_INT16.
struct script_local_var_table {
    struct script_var vars[4];
};
ASSERT_SIZE(struct script_local_var_table, 64);

// Slightly more convenient in-memory representation of script_var with mem_offset resolved.
// The game uses this struct when actually manipulating script variables.
struct script_var_desc {
    struct script_var* desc;
    void* value; // Pointer to the actual value in memory
};
ASSERT_SIZE(struct script_var_desc, 8);

// Table of runtime values for global script variables.
//
// The structure of this table is controlled by the metadata in struct script_var_table.
// Variables of type VARTYPE_NONE and VARTYPE_SPECIAL are not stored in this table. The script
// engine handles these variables specially.
//
// The fields are out of order relative to enum script_var_id and the script var table.
// Evidently the layout was chosen to be optimally packed, with the fields rearranged so the
// widest ones come first.
struct script_var_value_table {
    int32_t version;                            // 0x0: VAR_VERSION
    int32_t condition;                          // 0x4: VAR_CONDITION
    uint32_t position_x[3];                     // 0x8: VAR_POSITION_X
    uint32_t position_y[3];                     // 0x14: VAR_POSITION_Y
    uint32_t position_height[3];                // 0x20: VAR_POSITION_HEIGHT
    uint32_t recycle_count;                     // 0x2C: VAR_RECYCLE_COUNT
    int16_t ground_enter;                       // 0x30: VAR_GROUND_ENTER
    int16_t ground_getout;                      // 0x32: VAR_GROUND_GETOUT
    int16_t ground_map;                         // 0x34: VAR_GROUND_MAP
    int16_t ground_place;                       // 0x36: VAR_GROUND_PLACE
    int16_t ground_enter_backup[5];             // 0x38: VAR_GROUND_ENTER_BACKUP
    int16_t ground_getout_backup[5];            // 0x42: VAR_GROUND_GETOUT_BACKUP
    int16_t ground_map_backup[5];               // 0x4C: VAR_GROUND_MAP_BACKUP
    int16_t ground_place_backup[5];             // 0x56: VAR_GROUND_PLACE_BACKUP
    int16_t dungeon_select;                     // 0x60: VAR_DUNGEON_SELECT
    int16_t dungeon_enter;                      // 0x62: VAR_DUNGEON_ENTER
    int16_t dungeon_enter_mode;                 // 0x64: VAR_DUNGEON_ENTER_MODE
    int16_t dungeon_enter_index;                // 0x66: VAR_DUNGEON_ENTER_INDEX
    int16_t dungeon_enter_backup[5];            // 0x68: VAR_DUNGEON_ENTER_BACKUP
    int16_t dungeon_enter_mode_backup[5];       // 0x72: VAR_DUNGEON_ENTER_MODE_BACKUP
    int16_t dungeon_enter_index_backup[5];      // 0x7C: VAR_DUNGEON_ENTER_INDEX_BACKUP
    int16_t hero_first_kind;                    // 0x86: VAR_HERO_FIRST_KIND
    int16_t partner_first_kind;                 // 0x88: VAR_PARTNER_FIRST_KIND
    int16_t random_request_npc03_kind;          // 0x8A: VAR_RANDOM_REQUEST_NPC03_KIND
    int16_t event_local;                        // 0x8C: VAR_EVENT_LOCAL
    int16_t dungeon_event_local;                // 0x8E: VAR_DUNGEON_EVENT_LOCAL
    int16_t item_backup[2];                     // 0x90: VAR_ITEM_BACKUP
    int16_t item_backup_kurekure[2];            // 0x94: VAR_ITEM_BACKUP_KUREKURE
    int16_t item_backup_take[2];                // 0x98: VAR_ITEM_BACKUP_TAKE
    int16_t item_backup_get[2];                 // 0x9C: VAR_ITEM_BACKUP_GET
    int16_t request_thanks_result_kind;         // 0xA0: VAR_REQUEST_THANKS_RESULT_KIND
    int16_t request_thanks_result_variation;    // 0xA2: VAR_REQUEST_THANKS_RESULT_VARIATION
    uint16_t dungeon_enter_frequency;           // 0xA4: VAR_DUNGEON_ENTER_FREQUENCY
    uint16_t dungeon_enter_frequency_backup[5]; // 0xA6: VAR_DUNGEON_ENTER_FREQUENCY_BACKUP
    int8_t scenario_balance_flag;               // 0xB0: VAR_SCENARIO_BALANCE_FLAG
    int8_t scenario_balance_debug;              // 0xB1: VAR_SCENARIO_BALANCE_DEBUG
    int8_t hero_talk_kind;                      // 0xB2: VAR_HERO_TALK_KIND
    int8_t partner_talk_kind;                   // 0xB3: VAR_PARTNER_TALK_KIND
    int8_t config_color_kind;                   // 0xB4: VAR_CONFIG_COLOR_KIND
    int8_t rom_variation;                       // 0xB5: VAR_ROM_VARIATION
    int8_t special_episode_type;                // 0xB6: VAR_SPECIAL_EPISODE_TYPE
    int8_t position_direction[3];               // 0xB7: VAR_POSITION_DIRECTION
    uint8_t scenario_select[2];                 // 0xBA: VAR_SCENARIO_SELECT
    uint8_t scenario_main[2];                   // 0xBC: VAR_SCENARIO_MAIN
    uint8_t scenario_side[2];                   // 0xBE: VAR_SCENARIO_SIDE
    uint8_t scenario_sub1[2];                   // 0xC0: VAR_SCENARIO_SUB1
    uint8_t scenario_sub2[2];                   // 0xC2: VAR_SCENARIO_SUB2
    uint8_t scenario_sub3[2];                   // 0xC4: VAR_SCENARIO_SUB3
    uint8_t scenario_sub4[2];                   // 0xC6: VAR_SCENARIO_SUB4
    uint8_t scenario_sub5[2];                   // 0xC8: VAR_SCENARIO_SUB5
    uint8_t scenario_sub6[2];                   // 0xCA: VAR_SCENARIO_SUB6
    uint8_t scenario_sub7[2];                   // 0xCC: VAR_SCENARIO_SUB7
    uint8_t scenario_sub8[2];                   // 0xCE: VAR_SCENARIO_SUB8
    uint8_t crystal_color_01;                   // 0xD0: VAR_CRYSTAL_COLOR_01
    uint8_t crystal_color_02;                   // 0xD1: VAR_CRYSTAL_COLOR_02
    uint8_t crystal_color_03;                   // 0xD2: VAR_CRYSTAL_COLOR_03
    uint8_t compulsory_save_point;              // 0xD3: VAR_COMPULSORY_SAVE_POINT
    uint8_t compulsory_save_point_side;         // 0xD4: VAR_COMPULSORY_SAVE_POINT_SIDE
    uint8_t scenario_select_backup[8];          // 0xD5: VAR_SCENARIO_SELECT_BACKUP
    uint8_t ground_enter_link;                  // 0xDD: VAR_GROUND_ENTER_LINK
    uint8_t ground_enter_link_backup[5];        // 0xDE: VAR_GROUND_ENTER_LINK_BACKUP
    uint8_t dungeon_result;                     // 0xE3: VAR_DUNGEON_RESULT
    uint8_t ground_start_mode;                  // 0xE4: VAR_GROUND_START_MODE
    uint8_t dungeon_result_backup[5];           // 0xE5: VAR_DUNGEON_RESULT_BACKUP
    uint8_t ground_start_mode_backup[5];        // 0xEA: VAR_GROUND_START_MODE_BACKUP
    uint8_t request_clear_count;                // 0xEF: VAR_REQUEST_CLEAR_COUNT
    uint8_t player_kind;                        // 0xF0: VAR_PLAYER_KIND
    uint8_t attendant1_kind;                    // 0xF1: VAR_ATTENDANT1_KIND
    uint8_t attendant2_kind;                    // 0xF2: VAR_ATTENDANT2_KIND
    uint8_t player_kind_backup[5];              // 0xF3: VAR_PLAYER_KIND_BACKUP
    uint8_t attendant1_kind_backup[5];          // 0xF8: VAR_ATTENDANT1_KIND_BACKUP
    uint8_t attendant2_kind_backup[5];          // 0xFD: VAR_ATTENDANT2_KIND_BACKUP
    uint8_t world_map_level;                    // 0x102: VAR_WORLD_MAP_LEVEL
    uint8_t lottery_result;                     // 0x103: VAR_LOTTERY_RESULT
    uint8_t sub30_spot_level;                   // 0x104: VAR_SUB30_SPOT_LEVEL
    uint8_t team_rank_event_level;              // 0x105: VAR_TEAM_RANK_EVENT_LEVEL
    uint8_t play_old_game;                      // 0x106: VAR_PLAY_OLD_GAME
    char hero_first_name[10];                   // 0x107: VAR_HERO_FIRST_NAME
    char partner_first_name[10];                // 0x111: VAR_PARTNER_FIRST_NAME

    // Everything past here is of type VARTYPE_BIT
    uint8_t side02_talk : 3;                   // 0x11B: VAR_SIDE02_TALK
    uint8_t side06_room : 3;                   // 0x11B: VAR_SIDE06_ROOM
    uint8_t side08_boss2nd : 1;                // 0x11B: VAR_SIDE08_BOSS2ND
    uint8_t side01_boss2nd : 1;                // 0x11B: VAR_SIDE01_BOSS2ND
    uint8_t scenario_main_bit_flag[16];        // 0x11C: VAR_SCENARIO_MAIN_BIT_FLAG
    uint8_t scenario_talk_bit_flag[32];        // 0x12C: VAR_SCENARIO_TALK_BIT_FLAG
    uint8_t scenario_main_bit_flag_backup[64]; // 0x14C: VAR_SCENARIO_MAIN_BIT_FLAG_BACKUP
    uint8_t special_episode_open[1];           // 0x18C: VAR_SPECIAL_EPISODE_OPEN
    uint8_t special_episode_open_old[1];       // 0x18D: VAR_SPECIAL_EPISODE_OPEN_OLD
    uint8_t special_episode_conquest[1];       // 0x18E: VAR_SPECIAL_EPISODE_CONQUEST
    uint8_t performance_progress_list[8];      // 0x18F: VAR_PERFORMANCE_PROGRESS_LIST
    uint8_t dungeon_open_list[32];             // 0x197: VAR_DUNGEON_OPEN_LIST
    uint8_t dungeon_enter_list[32];            // 0x1B7: VAR_DUNGEON_ENTER_LIST
    uint8_t dungeon_arrive_list[32];           // 0x1D7: VAR_DUNGEON_ARRIVE_LIST
    uint8_t dungeon_conquest_list[32];         // 0x1F7: VAR_DUNGEON_CONQUEST_LIST
    uint8_t dungeon_present_list[32];          // 0x217: VAR_DUNGEON_PRESENT_LIST
    uint8_t dungeon_request_list[32];          // 0x237: VAR_DUNGEON_REQUEST_LIST
    uint8_t world_map_mark_list_normal[40];    // 0x257: VAR_WORLD_MAP_MARK_LIST_NORMAL
    uint8_t world_map_mark_list_special[40];   // 0x27F: VAR_WORLD_MAP_MARK_LIST_SPECIAL
    uint8_t station_item_static[4];            // 0x2A7: VAR_STATION_ITEM_STATIC
    uint8_t station_item_temp[4];              // 0x2AB: VAR_STATION_ITEM_TEMP
    uint8_t deliver_item_static[2];            // 0x2AF: VAR_DELIVER_ITEM_STATIC
    uint8_t deliver_item_temp[2];              // 0x2B1: VAR_DELIVER_ITEM_TEMP
    uint8_t bit_fuwarante_local : 5;           // 0x2B3: VAR_BIT_FUWARANTE_LOCAL
    uint8_t sub30_treasure_discover : 1;       // 0x2B3: VAR_SUB30_TREASURE_DISCOVER
    uint8_t sub30_spot_discover : 1;           // 0x2B3: VAR_SUB30_SPOT_DISCOVER
    uint8_t sub30_projectp : 1;                // 0x2B3: VAR_SUB30_PROJECTP

    // The rest of the value table is unused
    uint8_t unused[332];
};
ASSERT_SIZE(struct script_var_value_table, 1024);

// A level is a location in which a scene can take place.
// Each level has an associated tileset and associated scripts.
struct script_level {
    uint16_t mapty;
    undefined2 field_0x2;
    uint16_t mapid;
    int16_t field_0x6;
    char* name; // Level name as a null-terminated string
};
ASSERT_SIZE(struct script_level, 12);

// An entity represents a live actor that can move around and do things within a scene.
struct script_entity {
    uint16_t type;
    uint16_t entid;
    char* name; // Entity name as a null-terminated string
    // Note: The following fields are usually the same across versions, but not always.
    // They differ for the following script_entity IDs:
    // - ENTITY_NPC_TEST005
    // - ENTITY_NPC_BIIDARU
    // - ENTITY_NPC_BIPPA_BRO
    // - ENTITY_NPC_BIPPA_SIS
    // - ENTITY_NPC_PUKURIN_PAPA
    // - ENTITY_NPC_PUKURIN_MAMA
    uint16_t field_0x8;
    uint8_t field_0xa;
    uint8_t field_0xb;
};
ASSERT_SIZE(struct script_entity, 12);

// Opcode for the script engine.
struct script_opcode {
    int8_t n_params; // Number of parameters for the script "function". -1 means variadic.
    int8_t string_idx;
    undefined field_0x2;
    undefined field_0x3;
    char* name; // Opcode name as a null-terminated string
};
ASSERT_SIZE(struct script_opcode, 8);

// Table of all opcodes for the script engine.
struct script_opcode_table {
    struct script_opcode ops[383];
};
ASSERT_SIZE(struct script_opcode_table, 3064);

// Common routines used within the unionall.ssb script (the master script).
struct common_routine {
    struct common_routine_id_16 id;
    int16_t field_0x2;
    char* name; // Routine name as a null-terminated string
};
ASSERT_SIZE(struct common_routine, 8);

struct common_routine_table {
    struct common_routine routines[701];
};
ASSERT_SIZE(struct common_routine_table, 5608);

// An object is a non-entity, usually inanimate object that can be statically placed in a scene.
struct script_object {
    int16_t field_0x0;
    uint8_t field_0x2;
    uint8_t field_0x3;
    char* name; // Object name as a null-terminated string, or a null pointer
    uint8_t field_0x8;
    uint8_t unused[3]; // Always 0
};
ASSERT_SIZE(struct script_object, 12);

// A global structure holding various pointer to important structure for ground mode
struct main_ground_data {
    undefined* script; // 0x0: pointer to script structure
    undefined*
        partner_follow_data; // 0x4: pointer to the data related to the partner following the player
    undefined* actors;       // 0x8: pointer to the actors
    undefined* objects;      // 0x12: pointer to the objects
    undefined* performers;   // 0x16: pointer to the performers
    undefined* events;       // 0x20: pointer to the events
};
ASSERT_SIZE(struct main_ground_data, 24);

#endif
