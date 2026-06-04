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
    uint16_t weather_id; // 0xFFFF for none
    char* name;          // Level name as a null-terminated string
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

// Scripting coroutine located in unionall.ssb. Seems to represent coroutines when they are loaded
// in-RAM, unlike common_routine.
struct script_coroutine {
    // 0x0: Offset (in halfwords) where the coroutine starts, relative to the start of unionall
    uint16_t offset;
    uint16_t type;      // 0x2: Not confirmed
    uint16_t linked_to; // 0x4: From SkyTemple's source code. Purpose unknown.
};
ASSERT_SIZE(struct script_coroutine, 6);

// Contains additional info about a scripting coroutine loaded in RAM.
struct coroutine_info {
    void* unionall_start;  // 0x0: RAM address where unionall starts
    void* coroutine_start; // 0x4: RAM address where the coroutine starts
    undefined4 field_0x8;
    undefined field_0xc;
    undefined field_0xd;
    undefined field_0xe;
    undefined field_0xf;
    undefined field_0x10;
    undefined field_0x11;
    undefined field_0x12;
    undefined field_0x13;
    undefined2 field_0x14;
    undefined field_0x16;
    undefined field_0x17; // Likely padding
    undefined2 field_0x18;
    undefined padding[2];
};
ASSERT_SIZE(struct coroutine_info, 28);

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

// Data relating to animation
struct animation {
    undefined2 field_0x0[6]; // sometimes copied to field9_0x10 of animation_control
    struct animation_control ctrl;
    undefined fields[60];
};
ASSERT_SIZE(struct animation, 196);

// Holds data about an animation and how it should be played
#pragma pack(push, 2)
struct animation_data {
    uint8_t animation_id; // 0x0: ID of the animation in the monster's animation sheet
    // 0x1: speed + flags: 1-byte bitfield
    enum animation_speed animation_speed : 2;
    bool f_unk1 : 1;
    bool loop : 1;
    bool f_unk3 : 1;
    uint8_t f_unused : 3;
};
ASSERT_SIZE(struct animation_data, 2);
#pragma pack(pop)

// Runtime pointers of a currently-loaded ssb script in memory
struct ssb_runtime_info {
    void* file;                 // Pointer to start of ssb file in memory?
    void* opcodes;              // Pointer to start of ssb opcodes in memory?
    uint16_t* next_opcode_addr; // Address of next opcode to be run; this can also be used for local
                                // return addresses for opcodes like Call, Return, Flash, and more.
    void* strings; // A pointer to strings in a ssb file; halfword offsets followed by strings?
};
ASSERT_SIZE(struct ssb_runtime_info, 16);

// Keeps track of data used by script opcodes on a routine that's being executed
struct script_routine_state {
    int16_t execution_status; // Used in "WaitExecute" opcodes
    int16_t last_return_val;  // Return value of RunNextOpcode
    int16_t field_0x4;
    int8_t field_0x6;
    undefined field_0x7;
    struct ssb_runtime_info
        ssb_info[2]; // The first entry is used for the current ssb file being run. The
                     // second entry is used to store data due to a "call" operation, like Call,
                     // CallCommon, and others. When the "Return" opcode runs, the second entry
                     // gets copied back to the first.
    uint16_t* current_opcode_addr; // Address of current opcode
    uint16_t* current_param_addr;  // Address of current opcode parameter
    int16_t opcode_param_count;
    int16_t lock_id; // ID of the lock this routine is currently hanging on; -1 if no lock
    int16_t field_0x34;
    int8_t field_0x36;
    undefined field_0x37;
    int16_t field_0x38;
    int16_t timer; // Used in opcodes that require a frame timer, like Wait and PursueTurnLives
    int16_t field_0x3c;
    undefined field_0x3e;
    undefined field_0x3f;
    int current_variadic_param; // Maybe?
    undefined field_0x44;
    undefined field_0x45;
    undefined field_0x46;
    undefined field_0x47;
    undefined field_0x48;
    undefined field_0x49;
    undefined field_0x4a;
    undefined field_0x4b;
    undefined field_0x4c;
    undefined field_0x4d;
    undefined field_0x4e;
    undefined field_0x4f;
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
    undefined field_0x5a;
    undefined field_0x5b;
    undefined field_0x5c;
    undefined field_0x5d;
    undefined field_0x5e;
    undefined field_0x5f;
    undefined4 field_0x60[4];
};
ASSERT_SIZE(struct script_routine_state, 112);

// A routine executed at runtime, whether "targeted" on an entity or the main routine in a script
struct script_routine {
    struct ground_entity_function_table* function_table;
    void* parent_entity; // The parent entity of this routine: An actor, object, performer, or null
                         // (but NOT strictly the live_entity type). This means any entity
                         // self-references itself with this pointer; this is necessary because
                         // RunNextOpcode has a script_routine pointer as a parameter.
    struct script_routine_kind_16
        routine_kind; // Same as ground_entity_function_table::routine_kind
    int16_t id;       // Same as live_entity::id
    struct script_routine_state states[2];
};
ASSERT_SIZE(struct script_routine, 236);

// Represents a generic ground entity shared by actors, objects, and performers
struct live_entity {
    int16_t id; // The ID of this live entity, used to index within its statically-allocated list in
                // GROUND_STATE_PTRS
    int16_t kind; // The ID of the entity in its respective table (ENTITIES, OBJECTS, etc.)
    bool
        is_enabled; // True when the entity is loaded, false otherwise (should be checked if unsure)
    undefined field_0x5;
    uint16_t hanger;
    uint8_t sector;
    int8_t field_0x9;
    int16_t field_0xa;                    // Related to animation?
    struct uvec2 collision_box_size;      // The size of the collision box of the entity
    struct uvec2 collision_box_size_div2; // The size of the collision box divided by two
    struct direction_id_8 initial_direction;
    undefined field_0x1d;
    undefined field_0x1e;
    undefined field_0x1f;
    struct vec2 initial_pos;
    struct vec2 limit_min_pos; // minimum possible coordinates, for random move in free roam
    struct vec2 limit_max_pos; // maximum possible coordinates, for random move in free roam
    struct script_routine routine;
};
ASSERT_SIZE(struct live_entity, 292);

// Represents an actor currently loaded in a script scene (both during cutscenes and free-roam)
struct live_actor {
    struct live_entity entity;
    int16_t field_0x124;
    undefined field_0x126;
    undefined field_0x127;
    uint32_t attribute_bitfield; // Changed by various "Attribute" opcodes
    int32_t field_0x12c;
    struct direction_id_8 current_direction; // not sure
    undefined field_0x131;
    undefined field_0x132;
    undefined field_0x133;
    undefined field_0x134;
    undefined field_0x135;
    undefined field_0x136;
    undefined field_0x137;
    struct vec2 field38_0x138;
    undefined field_0x140;
    undefined field_0x141;
    undefined field_0x142;
    undefined field_0x143;
    undefined field_0x144;
    undefined field_0x145;
    undefined field_0x146;
    undefined field_0x147;
    undefined field_0x148;
    undefined field_0x149;
    undefined field_0x14a;
    undefined field_0x14b;
    uint32_t field_0x14c;
    int16_t field_0x150;
    bool field_0x152;
    undefined field_0x153;
    int16_t field_0x154;
    bool field_0x156;
    undefined field_0x157;
    int16_t field_0x158;
    struct direction_id_8 direction;
    undefined field_0x15b;
    struct vec2 coord_min; // the top-left coordinate of the collision box of the actor
    struct vec2 coord_max; // the bottom-right coordinate of the collision box of the actor
    int32_t height;
    int32_t second_height;        // Unsure; GetHeightLiveActor references this field, but
                                  // SetHeightLiveActor doesn't
    bool direction_should_change; // Seems to be set to 1 whenever direction is changed; see
                                  // SetDirectionLiveActor and SetPositionInitialLiveActor for
                                  // examples
    int8_t field_0x175;
    int16_t field_0x176;
    int16_t field_0x178;
    undefined field_0x17a;
    undefined field_0x17b;
    // not sure. seem to be 0 when not moving and 3 when moving most of the time.
    int32_t movement_related;
    int16_t second_bitfield; // not sure
    int16_t effect_flag;     // not sure
    int16_t effect_id;
    undefined field_0x186;
    undefined field_0x187;
    int effect_position_marker;
    struct animation animation;
};
ASSERT_SIZE(struct live_actor, 592);

// Represents an object currently loaded in a script scene (both during cutscenes and free-roam)
struct live_object {
    undefined field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    struct live_entity entity;
    undefined field_0x128;
    undefined field_0x129;
    undefined field_0x12a;
    undefined field_0x12b;
    uint32_t attribute_bitfield; // Changed by various "Attribute" opcodes
    struct direction_id_8 direction;
    undefined field_0x131;
    undefined field_0x132;
    undefined field_0x133;
    struct vec2 coord_min; // the top-left coordinate of the collision box of the object
    struct vec2 coord_max; // the bottom-right coordinate of the collision box of the object
    int32_t height;
    int32_t second_height;
    bool direction_should_change; // Seems to be set to 1 in SetPositionInitialLiveObject if
                                  // initial_direction != direction
    undefined field_0x14d;
    undefined field_0x14e;
    undefined field_0x14f;
    int16_t effect_flag; // not sure
    int16_t effect_id;
    struct animation animation;
};
ASSERT_SIZE(struct live_object, 536);

// Represents a performer currently loaded in a script scene (both during cutscenes and free-roam)
struct live_performer {
    undefined field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    struct live_entity entity;
    uint32_t attribute_bitfield; // Changed by various "Attribute" opcodes
    struct direction_id_8 direction;
    undefined field_0x12d;
    undefined field_0x12e;
    undefined field_0x12f;
    struct vec2 coord_min; // the top-left coordinate of the collision box of the performer
    struct vec2 coord_max; // the bottom-right coordinate of the collision box of the performer
    int32_t height;
    int32_t second_height;
    bool direction_should_change; // Seems to be set to 1 in SetPositionInitialLivePerformer if
                                  // initial_direction != direction
    undefined field_0x149;
    undefined field_0x14a;
    undefined field_0x14b;
    int16_t effect_flag; // not sure
    int16_t effect_id;
    struct animation animation;
};
ASSERT_SIZE(struct live_performer, 532);

// Represents an event currently loaded in a script scene during free-roam
struct live_event {
    int16_t id;
    uint16_t coroutine_id;
    uint16_t hanger;
    uint8_t sector;
    undefined field_0x7;
    int16_t script_id; // If -1, the event will attempt loading the Unionall coroutine specified by
                       // coroutine_id
    undefined field_0xa;
    undefined field_0xb;
    int32_t attribute_bitfield; // Either 0x800 or 0x1800, dependent on the value of field_0x2 from
                                // the coroutine related to this event
    struct vec2 coord_min;      // the top-left coordinate of the collision box of the event
    struct vec2 coord_max;      // the bottom-right coordinate of the collision box of the event
};
ASSERT_SIZE(struct live_event, 32);

// A list of 24 actors, which is the maximum number of statically allocated live actors
struct live_actor_list {
    struct live_actor actors[24];
};
ASSERT_SIZE(struct live_actor_list, 14208);

// A list of 16 objects, which is the maximum number of statically allocated live objects
struct live_object_list {
    struct live_object objects[16];
};
ASSERT_SIZE(struct live_object_list, 8576);

// A list of 16 performers, which is the maximum number of statically allocated live performers
struct live_performer_list {
    struct live_performer performers[16];
};
ASSERT_SIZE(struct live_performer_list, 8512);

// A list of 32 events, which is the maximum number of statically allocated live events
struct live_event_list {
    struct live_event events[32];
};
ASSERT_SIZE(struct live_event_list, 1024);

// A global structure holding various pointers to important structures for ground mode
struct main_ground_data {
    struct script_routine* main_routine; // 0x0: pointer to script structure
    undefined*
        partner_follow_data; // 0x4: pointer to the data related to the partner following the player
    struct live_actor_list* actors;         // 0x8: pointer to the actors
    struct live_object_list* objects;       // 0xC: pointer to the objects
    struct live_performer_list* performers; // 0x10: pointer to the performers
    struct live_event_list* events;         // 0x14: pointer to the events
};
ASSERT_SIZE(struct main_ground_data, 24);

struct partner_talk_kind_table_entry {
    enum talk_kind talk_kind;
    enum monster_id id;
};
ASSERT_SIZE(struct partner_talk_kind_table_entry, 8);

struct bar_item {
    struct item_id_16 id;
    int16_t hp_increase;
    int16_t iq_increase;
    int16_t atk_increase;
    int16_t def_increase;
    int16_t spa_increase;
    int16_t spdef_increase;
    uint16_t normal_drink_wght;
    uint16_t good_drink_wght;
    uint16_t miracle_drink_wght;
    uint16_t bad_drink_wght;
};
ASSERT_SIZE(struct bar_item, 22);

struct stat_bitflag {
    bool f_attack : 1;
    bool f_special_attack : 1;
    bool f_defense : 1;
    bool f_special_defense : 1;
    // Probably padding/unsued
    uint16_t f_unused : 12;
};
ASSERT_SIZE(struct stat_bitflag, 2);

struct gummi_result {
    struct gummi_compatability_16 gummi_compatability;
    struct stat_bitflag stats_increased;
};
ASSERT_SIZE(struct gummi_result, 4);

struct ground_weather_entry {
    int16_t field_0x0;
    int16_t field_0x2;
};
ASSERT_SIZE(struct ground_weather_entry, 4);

struct ground_entity_function_table {
    struct script_routine_kind_16 routine_kind;
    uint16_t padding;
    int16_t (*get_id)(void* ground_entity);
    void (*get_collision_box)(void* ground_entity, struct uvec2* collision_box);
    void (*get_collision_box_center)(void* ground_entity, struct uvec2* collision_box_center);
    void (*get_height)(void* ground_entity, int* height, int* second_height);
    void (*get_direction)(void* ground_entity, struct direction_id_8* target);
    void (*get_attribute_bitfield)(void* ground_entity, uint32_t* attribute_bitfield);
    void (*set_position_initial)(void* ground_entity, struct vec2* offset);
    void (*set_movement_range)(void* ground_entity, struct vec2* limit_min_pos,
                               struct vec2* limit_max_pos);
    void (*set_position)(void* ground_entity, struct vec2* pos);
    void (*set_height)(void* ground_entity, int height);
    void (*set_direction)(void* ground_entity, enum direction_id direction);
    void (*set_animation)(void* ground_entity, uint16_t setanimation_param);
    void (*set_effect)(void* ground_entity, bool flag, int16_t effect_id, int position_marker);
    void (*set_attribute_bitfield_wrapper)(void* ground_entity, uint32_t attribute_bitfield);
    void (*reset_attribute_bitfield_wrapper)(void* ground_entity, uint32_t attribute_bitfield);
    void (*set_blink)(void* ground_entity, uint16_t interval_blink_in, uint16_t interval_blink_out);
    void (*set_position_offset)(void* ground_entity, struct vec2* offset);
    int (*get_animation_status)(void* ground_entity);
    int (*get_effect_status)(void* ground_entity);
};
ASSERT_SIZE(struct ground_entity_function_table, 80);

struct swap_shop_menu_data {
    // 0x0: The switch case index shared by SwapShopMainManager and SwapShopDialogueManager.
    enum swap_shop_shared_case current_shared_case;
    // 0x4: The "next" switch case index to be shared, typically assigned by SwapShopMainManager.
    enum swap_shop_shared_case next_shared_case;
    // 0x8: Used exclusively by SwapShopMainManager. More research needed.
    int32_t unk_windows_counter;
    // 0xC: If true, croagunk will not ask the player if they want to continue swapping.
    bool croagunk_out_of_swaps;
    undefined field_0xd;
    // 0xE: The item the player will receive by swapping.
    struct bulk_item received_item;
    undefined field_0x12;
    undefined field_0x13;
    undefined field_0x14;
    undefined field_0x15;
    undefined field_0x16;
    undefined field_0x17;
    // 0x18: While selecting which items to give croagunk, this field is filled when selecting
    // "info" on an item in the list.
    struct bulk_item described_item;
    // 0x1C: Whenever a simple menu is run, this field is where the result is stored.
    int32_t simple_menu_result;
    // 0x20: Zeroed in SwapShopEntryPoint, but never modified
    undefined4 unk_int_1;
    // 0x24: Zeroed in SwapShopEntryPoint, but never modified
    undefined4 unk_int_2;
    // 0x28: Emotion for croagunk's portrait.
    enum portrait_emotion emotion_id;
    // 0x2C: The case menu for a smaller switch case at the top of SwapShopMainManager.
    enum swap_shop_main_manager_case main_manager_case;
    // 0x30: Stores preprocessor_args for the next window.
    struct preprocessor_args preprocessor_args;
    // 0x80: The window_id of the current dialogue box window. -2 if none are present.
    int8_t dialogue_window_id;
    // 0x81: The window_id of the current portrait window. -2 if none are present.
    int8_t portrait_window_id;
    // 0x82: The window_id of the current advanced textbox window. -2 if none are present.
    int8_t advanced_textbox_window_id;
    // 0x83: The window_id of the current simple menu window. -2 if none are present.
    int8_t simple_menu_window_id;
    // 0x84: The window_id of the current text box window. -2 if none are present.
    int8_t text_window_id;
    // 0x85: The window_id of the current scroll box window. -2 if none are present.
    int8_t scroll_box_window_id;
    // 0x86: A small buffer for preprocessed text strings.
    char text_string[66];
    // 0xC8: Stores portrait_params for croagunk's next portrait.
    struct portrait_params portrait_params;
    // 0xD8: Stores the number of items required to trade for the current trade.
    int16_t exc_item_trade_count;
    // 0xDA: The number of exclusive items currently in the bag.
    int16_t exc_bag_item_count;
    // 0xDC: The individual item IDs of the exclusive items present in the bag.
    struct item_id_16 exc_bag_items[50];
    // 0x140: The number of exclusive items currently in storage.
    int16_t exc_storage_item_count;
    // 0x142: The individual item IDs of the exclusive items in storage.
    struct item_id_16 exc_storage_items[1000];
    undefined field_0x912;
    undefined field_0x913;
    // 0x914: The team member slot whose exclusive items are about to be viewed.
    int32_t selected_team_member_slot;
    // 0x918: The move slot to create an info menu for.
    int16_t selected_move_slot;
    // 0x91A: The four moves known by selected_team_member_slot.
    struct move known_moves[4];
    undefined field_0x93a;
    undefined field_0x93b;
};
ASSERT_SIZE(struct swap_shop_menu_data, 2364);

struct swap_shop_inventory_ptrs {
    // 0x0: Seems to be a pointer to some kind of temporary variable?
    undefined* temp_variable;
    // 0x4: A pointer to the swap_shop_inventory_data struct.
    struct swap_shop_inventory_data* inventory_data;
};
ASSERT_SIZE(struct swap_shop_inventory_ptrs, 8);

struct exc_item_trade_slot {
    // 0x0: The trade type the item slot uses.
    int16_t trade_type;
    // 0x2: This is likely either the item_id of the slot, or the index of the slot. It might vary
    // depending on the trade_type.
    int16_t unk_0x2;
    // 0x4: If the player has enough exclusive items to swap for the item, this will be true.
    bool can_afford_item;
    undefined unk_0x5;
};
ASSERT_SIZE(struct exc_item_trade_slot, 6);

struct swap_shop_inventory_data {
    // 0x0: switch case id for SwapShopInventoryManager
    enum swap_shop_inventory_case inventory_case;
    // 0x4: A copy of num_valid_shop_items to be used by CreateCollectionMenu?
    int32_t num_valid_shop_items_temp;
    // 0x8: Seemingly unused, these are zeroed for some reason?
    int32_t field_0x8[1068];
    // 0x10B8: If the item is purchasable, this field will be 0x2. Otherwise, it will be 0x0.
    int8_t is_item_purchasable[1068];
    // 0x14E4: Seems to be a window_id for a collection menu.
    int8_t collection_menu_window_id;
    undefined field_0x14e5;
    // 0x14E6: Seems to be a struct for each item in the shop.
    struct exc_item_trade_slot exc_item_trade_slots[1068];
    // 0x2DEE: For each item in the swap list, is the number of that item available?
    int16_t item_quantities[1068];
    // 0x3646: List of every exclusive item the player owns.
    struct item_id_16 owned_exclusive_items[1000];
    // 0x3E16: List of how much of each owned_exclusive_item the player owns.
    int16_t owned_item_quantities[1000];
    // 0x45E6: The synth_templates matching today's rolled swap shop items.
    struct synth_template shop_synth_templates[8];
    // 0x4626: Total number of items for sale in the swap shop.
    int16_t num_valid_shop_items;
    // 0x4628: Number of items required to be traded to croagunk for the selected item.
    int16_t num_required_trade_items;
    undefined field_0x462a;
    undefined field_0x462b;
    // 0x462C
    struct window_extra_info window_extra_info;
    // 0x46C4: Whether or not the synth.bin file is open.
    bool synth_bin_open;
    undefined field_0x46c5;
    undefined field_0x46c6;
    undefined field_0x46c7;
    // 0x46C8
    int32_t textbox_window_id;
    struct text_box* textbox_ptr;
    // 0x46D0: The item ID the player selects when choosing items to give croagunk. Used to check
    // the item info.
    struct item_id_16 selected_item;
    undefined field_0x46d2;
    undefined field_0x46d3;
};
ASSERT_SIZE(struct swap_shop_inventory_data, 18132);

struct ground_bg_sub_struct_4 {
    int16_t field_0x0;
    int16_t field_0x2;
    undefined* field_0x4;
    undefined* field_0x8;
};
ASSERT_SIZE(struct ground_bg_sub_struct_4, 12);

struct ground_bg_sub_struct_c4 {
    uint8_t field_0x0;
    uint8_t field_0x1;
    int16_t field_0x2;
    int16_t field_0x4;
    uint16_t field_0x6;
    struct iovec bpa_file;
    struct bpa_header* bpa_header;
    undefined* field_0x14;
    undefined* field_0x18;
    undefined* field_0x1c;
    undefined* field_0x20;
    undefined* field_0x24;
    uint32_t field_0x28;
};
ASSERT_SIZE(struct ground_bg_sub_struct_c4, 44);

struct ground_bg_sub_struct_194 {
    uint8_t field_0x0;
    uint8_t field_0x1;
    uint8_t field_0x2;
    uint8_t field_0x3;
    uint8_t field_0x4;
    uint8_t field_0x5;
    uint8_t field_0x6;
    uint8_t field_0x7;
    int field_0x8;
};
ASSERT_SIZE(struct ground_bg_sub_struct_194, 12);

struct ground_bg_sub_struct_1a0 {
    uint8_t field_0x0;
    uint8_t field_0x1;
    uint8_t field_0x2;
    uint8_t field_0x3;
    uint8_t field_0x4;
    uint8_t field_0x5;
    uint8_t field_0x6;
    uint8_t field_0x7;
    uint8_t field_0x8;
    uint8_t field_0x9;
    uint8_t field_0xa;
    uint8_t field_0xb;
    uint8_t field_0xc;
    uint8_t field_0xd;
    uint8_t field_0xe;
    uint8_t field_0xf;
    uint8_t field_0x10;
    uint8_t field_0x11;
    uint8_t field_0x12;
    uint8_t field_0x13;
    uint8_t field_0x14;
    uint8_t field_0x15;
    uint8_t field_0x16;
    uint8_t field_0x17;
};
ASSERT_SIZE(struct ground_bg_sub_struct_1a0, 24);

struct layer_specs {
    int16_t num_tiles; // The number of tiles stored in the data + 1. The +1 is the null tile at the
                       // beginning of tiles.
    int16_t bpa_slot_num_tiles[4]; // The number of tiles in the BPA on this slot. 0 if no BPA is
                                   // assigned.
    int16_t num_chunks; // Number of chunks in the tilemap + 1. The +1 is the null chunk at the
                        // beginning of tile mappings, that is not stored.
};
ASSERT_SIZE(struct layer_specs, 12);

struct bpa_header {
    uint8_t num_tiles;         // The number of individual tiles.
    int16_t num_frames;        // The number of frames for each tile.
    int duration_per_frame[0]; // Settings for each frame. One entry per frame.
};

struct bma_header {
    // Map width/height that the camera in game will travel in tiles. Also the width/height of the
    // collision and unknown data layers! For most maps (Map Width Chunks) * (Tiling Width) = (Map
    // Width Camera).
    uint8_t map_width_tiles;
    uint8_t map_height_tiles;

    // Map width/height in chunks. Also the width/height of the chunk mappings.
    uint8_t map_width_chunks;
    uint8_t map_height_chunks;

    uint16_t num_layers; // Number of layers in this map. Must match BPC layer size. Allowed values
                         // are only 1 or 2.
    uint16_t has_data_layer; // Seems to be a boolean flag (0 or 1). If >0, the Unknown Data Layer
                             // exists.
    uint16_t has_collision;  // Number of Collision layers. 0, 1 or 2.
};
ASSERT_SIZE(struct bma_header, 10);

struct bpl_header {
    int16_t num_palettes;
    int16_t has_pal_animations;
};
ASSERT_SIZE(struct bpl_header, 4);

struct animation_specification {
    int16_t duration_per_frame; // Time in game frames to hold a single palette frame for
    int16_t num_frames; // Number of frames. This is also usually the length of frames in animation
                        // palette, but it can also be less.
};
ASSERT_SIZE(struct animation_specification, 4);

struct map_render {
    int16_t chunk_dimensions;
    int16_t field_0x2;
    int16_t num_bgs;
    bool wrap_around;
    uint8_t field_0x7;
    int width_chunks;
    int height_chunks;
    struct pixel_position map_size_pixels;
    void (*tilemap_render_func)(struct map_render*);
};
ASSERT_SIZE(struct map_render, 28);

struct ground_bg_sub_struct_2bc {
    uint8_t field_0x0;
    uint8_t field_0x1;
    int16_t field_0x2;
    int16_t num_layers;
    int16_t field_0x6;
    int16_t field_0x8;
    int16_t field_0xa;
    int16_t field_0xc;
    int16_t field_0xe[2];
    int16_t field_0x12;
    int16_t field_0x14;
    int16_t field_0x16;
    void (*field_0x18)(void*, void*, void*, int);
};
ASSERT_SIZE(struct ground_bg_sub_struct_2bc, 28);

struct ground_bg {
    int16_t field_0x0;
    int16_t field_0x2;
    struct ground_bg_sub_struct_4 field_0x4[16];
    struct ground_bg_sub_struct_c4 field_0xc4[4];
    struct iovec bpl_file; // 0x174
    struct iovec field_0x17c;
    struct iovec bpc_file; // 0x184
    struct iovec bma_file; // 0x18c
    struct ground_bg_sub_struct_194 field_0x194;
    struct ground_bg_sub_struct_1a0 field_0x1a0;
    void* field_0x1b8;
    uint8_t field_0x1bc;
    uint8_t field_0x1bd;
    int16_t field_0x1be;
    uint8_t field_0x1c0;
    uint8_t field_0x1c1;
    uint8_t field_0x1c2;
    uint8_t field_0x1c3;
    int field_0x1c4;
    struct layer_specs layer_specs[2]; // 1c8
    struct bma_header bma_header;
    struct bpl_header bpl_header;
    uint8_t field_0x1ee;
    uint8_t field_0x1ef;
    undefined* field_0x1f0;
    struct animation_specification* animation_specifications; // 0x1F4
    uint8_t field_0x1f8;
    uint8_t field_0x1f9;
    uint8_t field_0x1fa;
    uint8_t field_0x1fb;
    uint8_t field_0x1fc;
    uint8_t field_0x1fd;
    uint8_t field_0x1fe;
    uint8_t field_0x1ff;
    struct pixel_position camera_pixel_position[2]; // 0x200
    struct map_render map_render[2];                // 0x210
    uint8_t field_0x248[112];
    int16_t field_0x2b8;
    uint8_t field_0x2ba;
    uint8_t field_0x2bb;
    struct ground_bg_sub_struct_2bc field_0x2bc; // 2bc
    uint16_t* field_0x2d8;
    uint16_t* field_0x2dc[2];
    uint16_t* field_0x2e4[2];
    uint16_t* field_0x2ec[2];
    uint8_t field_0x2f0[616];
};
ASSERT_SIZE(struct ground_bg, 1372);

struct link_shop {
    int8_t window_id_0x0;
    int8_t portrait_window_id; // 0x1
    int8_t window_id_0x2;
    int8_t window_id_0x3;
    int8_t window_id_0x4;
    int8_t window_id_0x5;
    int8_t window_id_0x6;
    int8_t window_id_0x7;
    int8_t window_id_0x8;
    int8_t scroll_box_window_id; // 0x9
    int8_t window_id_0xa;
    int8_t window_id_0xb;
    int8_t window_id_0xc;
    int8_t window_id_0xd;
    int8_t window_id_0xe;
    int8_t window_id_0xf;
    bool unk_0x10;
    undefined padding_0x11[3];
    struct preprocessor_args preprocessor_args; // 0x14
    struct portrait_params portrait_params;     // 0x64
    undefined4
        unk_0x74;   // Potentially also some state value; used in a switch in LinkShopMainManager.
    uint32_t state; // 0x78: Current state of the link shop menu; used in a switch in
                    // LinkShopMainManager. Could be an enum.
    undefined4
        unk_0x7c; // Potentially also some state value; used in a switch in LinkShopMainManager.
    uint32_t next_state; // 0x80: Next value for state.
    undefined4 unk_0x84;
    undefined4 unk_0x88;
    undefined2 unk_0x8c;
    undefined2 padding_0x8e;
    struct window_extra_info extra_info_1; // 0x90
    struct window_extra_info extra_info_2; // 0x128
    struct window_extra_info extra_info_3; // 0x1C0
    struct window_extra_info extra_info_4; // 0x258
    struct window_extra_info extra_info_5; // 0x2F0
    undefined2 unk_0x388;
    undefined2 unk_0x38a;
    int16_t member_idx; // 0x38C: Index in TEAM_MEMBER_TABLE for the selected monster.
    undefined2 padding_0x38e;
    undefined4 unk_0x390;
    uint8_t unk_0x394[8];
    undefined4 unk_0x39c;
    undefined4 unk_0x3a0;
    undefined2 unk_0x3a4;
    undefined1 unk_0x3a6; // This through 0x3AD is filled with a memcpy; may be a smaller struct.
    undefined1 unk_0x3a7; // Could not find direct use of this; may be padding.
    undefined2 unk_0x3a8;
    undefined2 unk_0x3aa;
    undefined1 unk_0x3ac;
    undefined1 unk_0x3ad;
    char name[10];           // 0x3AE: Name of the selected monster.
    undefined unk_0x3b8[12]; // Could not find direct use of this, probably not padding though.
};
ASSERT_SIZE(struct link_shop, 964);

struct bar_stats_data {
    int16_t stat_modifiers[6];
    int16_t new_stats[6];
    int8_t actual_stat_changes[6];
    undefined unk_field_0x1e;
    undefined unk_field_0x1f;
    int stat_drink_event_kind; // 0x0: stat_down, 0x1: surprise_stat_up, 0x2: expected_stat_up
    int num_iq_skills_pre;
    int num_iq_skills_post;
    struct iq_skill_id_8 iq_skills_pre[69];
    struct iq_skill_id_8 iq_skills_post[69];
    undefined unk_field_0xb6;
    undefined unk_field_0xb7;
};
ASSERT_SIZE(struct bar_stats_data, 184);

// Pointer lives at 0x23258F0 EU, which is squarely part of overlay_11. Other overlays likely share
// this address too.
struct spinda_cafe {
    enum bar_update_case main_case;
    enum bar_subcase curr_subcase;
    enum bar_subcase next_subcase;
    int8_t portrait_window_id;
    undefined1 unk_0xd;
    undefined1 unk_0xe;
    undefined1 unk_0xf;
    struct portrait_params portrait_params;
    int8_t inventory_window_id;
    int8_t team_window_id;
    int8_t scroll_box_window_id;
    bool item_validities[50];
    undefined1 unk_0x55;
    undefined1 unk_0x56;
    undefined1 unk_0x57;
    int sel_item_index;
    bool team_member_valid[4];
    undefined1 unk_0x60;
    undefined1 unk_0x61;
    undefined1 unk_0x62;
    undefined1 unk_0x63;
    int number_of_items;
    struct item_id_16 bag_items[50];
    struct item selected_item;
    undefined1 unk_0xd2;
    undefined1 unk_0xd3;
    uint32_t team_member_index;
    int16_t selected_move_index;
    struct move_id_16 team_move_ids[16];
    int16_t drink_type_index; // random [0,11] and determines what spinda calls the drink (IE: Mix,
                              // Juice, Dew, Soda, Shake)
    enum drink_event_type drink_event;
    uint16_t frame_delay;
    struct dungeon_id_8 unlocked_dungeon;
    undefined1 unk_0x103;
    struct monster_id_16 cafe_recruit_species;
    struct monster_id_16 egg_giver_species;
    int drink_stat_kind;
    struct bar_stats_data bar_stats;
    int return_case;
    struct preprocessor_flags preprocessor_flags_0;
    struct preprocessor_flags preprocessor_flags_1;
    char string_buffer_0[1024];
    char string_buffer_1[1024];
    int unknown_flags_1;
    struct window_extra_info window_extra_info;
};
ASSERT_SIZE(struct spinda_cafe, 2664);

struct recycle_item {
    enum item_id id;
    int32_t trade_type; // 0x0 = Normal, 0x1 = Ticket, 0x2 = Offer
    enum rank min_rank; // Vanilla prize tickets require a guild rank to unlock
    int32_t min_recycles;
    // Some kind of key for which item to offer as a bonus after trading.
    // 0x5: Prize Ticket, 0x8: Silver Ticket, 0xD: Gold Ticket, 0x14: Prism Ticket
    int16_t bonus_item;
    // Odds out of 100 to get that bonus item after trading.
    int16_t bonus_odds;
    struct bulk_item traded_items[4];
};
ASSERT_SIZE(struct recycle_item, 36);

// This struct seems to only exist in RAM. More research into it's purpose may be necessary.
// Note that it has all the same fields as recycle_item, but compressed into far fewer bytes.
struct short_recycle_item {
    struct item_id_16 id;
    int8_t trade_type; // 0x0 = Normal, 0x1 = Ticket, 0x2 = Offer
    struct rank_8 min_rank;
    int16_t min_recycles;
    // Some kind of key for which item to offer as a bonus after trading.
    // 0x5: Prize Ticket, 0x8: Silver Ticket, 0xD: Gold Ticket, 0x14: Prism Ticket
    int8_t bonus_item;
    // Odds out of 100 to get that bonus item after trading.
    int8_t bonus_odds;
    int8_t traded_item_quantities[4];
    struct item_id_16 traded_item_ids[4];
};
ASSERT_SIZE(struct short_recycle_item, 20);

struct recycle_shop_main {
    enum recycle_case_id case_id;
    enum recycle_main_menu_state mmenu_case;
    undefined field_0x8;
    undefined field_0x9;
    undefined field_0xa;
    undefined field_0xb;
    int unk_subcase;
    undefined field_0x10;
    undefined field_0x11;
    undefined field_0x12;
    undefined field_0x13;
    int unk_counter_0x14;
    int8_t portrait_window_id;
    undefined field_0x19;
    undefined field_0x1a;
    undefined field_0x1b;
    struct portrait_params portrait_params;
    int8_t window_id_2;
    int8_t window_id_3;
    int8_t window_id_4;
    undefined field_0x2f;
    struct preprocessor_args preprocessor_args;
    int recycle_trade_type;
    struct recycle_item* recycle_items_ptr;
    undefined field16_0x88;
    undefined field17_0x89;
    undefined field18_0x8a;
    undefined field19_0x8b;
    undefined field20_0x8c;
    undefined field21_0x8d;
    undefined field22_0x8e;
    undefined field23_0x8f;
    undefined field24_0x90;
    undefined field25_0x91;
    undefined field26_0x92;
    undefined field27_0x93;
    undefined field28_0x94;
    undefined field29_0x95;
    undefined field30_0x96;
    undefined field31_0x97;
    undefined field32_0x98;
    undefined field33_0x99;
    undefined field34_0x9a;
    undefined field35_0x9b;
    undefined field_0x9c;
    undefined field_0x9d;
    undefined field_0x9e;
    undefined field_0x9f;
    undefined field_0xa0;
    undefined field_0xa1;
    undefined field_0xa2;
    undefined field_0xa3;
    bool portrait_active;
    bool resuming_menu;
    bool portrait_hidden;
    undefined field_0xa7;
    struct item_id_16 item_id;
    undefined field_0xaa;
    undefined field_0xab;
    undefined field_0xac;
    undefined field_0xad;
    undefined field_0xae;
    undefined field_0xaf;
    int8_t related_to_trade_type;
    undefined field_0xb1;
    undefined field_0xb2;
    undefined field_0xb3;
};
ASSERT_SIZE(struct recycle_shop_main, 180);

struct recycle_offer_items {
    struct recycle_item** item_ids_ptr;
    int num_offers;
};

struct recycle_unk_struct_0x84 {
    int case_id;
    int subcase_id;
    int field2_0x8;
    int field3_0xc;
    int field4_0x10;
    int8_t text_box_window_ids[3];
    undefined fields_0x15[81];
    undefined field7_0x68;
    undefined field8_0x69;
    undefined field9_0x6a;
    undefined field10_0x6b;
    struct recycle_item* recycle_ite_ptr;
    int16_t matching_items_bag[4];
    int16_t matching_items_storage[4];
    int16_t num_items_to_trade;
    undefined field15_0x82;
    undefined field16_0x83;
};
ASSERT_SIZE(struct recycle_unk_struct_0x84, 132);

struct recycle_unk_struct_0x340 {
    int case_id;
    int8_t simple_menu_wid_1;   // Created by retype action
    int8_t simple_menu_wid_2;   // Created by retype action
    int8_t scrollbox_window_id; // Created by retype action
    undefined field4_0x7;
    int simple_menu_1_result;
    int smenu_1_result_old;
    int starts_zero_1;
    int starts_zero_2;
    int8_t uhhhh;
    undefined field10_0x19;
    undefined field11_0x1a;
    undefined field12_0x1b;
    int arm9_unk_field; //  Arm9LoadUnkFieldNa0x2029EC8
    int is_0xd;
    int16_t text_string_id;
    undefined field16_0x26;
    undefined field17_0x27;
    int field18_0x28;
    undefined field19_0x2c;
    undefined field20_0x2d;
    undefined field21_0x2e;
    undefined field22_0x2f;
    undefined field23_0x30;
    undefined field24_0x31;
    undefined field25_0x32;
    undefined field26_0x33;
    undefined field27_0x34;
    undefined field28_0x35;
    undefined field29_0x36;
    undefined field30_0x37;
    undefined field31_0x38;
    undefined field32_0x39;
    undefined field33_0x3a;
    undefined field34_0x3b;
    undefined field35_0x3c;
    undefined field36_0x3d;
    undefined field37_0x3e;
    undefined field38_0x3f;
    undefined field39_0x40;
    undefined field40_0x41;
    undefined field41_0x42;
    undefined field42_0x43;
    undefined field43_0x44;
    undefined field44_0x45;
    undefined field45_0x46;
    undefined field46_0x47;
    undefined field47_0x48;
    undefined field48_0x49;
    undefined field49_0x4a;
    undefined field50_0x4b;
    undefined field51_0x4c;
    undefined field52_0x4d;
    undefined field53_0x4e;
    undefined field54_0x4f;
    undefined field55_0x50;
    undefined field56_0x51;
    undefined field57_0x52;
    undefined field58_0x53;
    undefined field59_0x54;
    undefined field60_0x55;
    undefined field61_0x56;
    undefined field62_0x57;
    undefined field63_0x58;
    undefined field64_0x59;
    undefined field65_0x5a;
    undefined field66_0x5b;
    undefined field67_0x5c;
    undefined field68_0x5d;
    undefined field69_0x5e;
    undefined field70_0x5f;
    undefined field71_0x60;
    undefined field72_0x61;
    undefined field73_0x62;
    undefined field74_0x63;
    undefined field75_0x64;
    undefined field76_0x65;
    undefined field77_0x66;
    undefined field78_0x67;
    undefined field79_0x68;
    undefined field80_0x69;
    undefined field81_0x6a;
    undefined field82_0x6b;
    undefined field83_0x6c;
    undefined field84_0x6d;
    undefined field85_0x6e;
    undefined field86_0x6f;
    undefined field87_0x70;
    undefined field88_0x71;
    undefined field89_0x72;
    undefined field90_0x73;
    undefined field91_0x74;
    undefined field92_0x75;
    undefined field93_0x76;
    undefined field94_0x77;
    undefined field95_0x78;
    undefined field96_0x79;
    undefined field97_0x7a;
    undefined field98_0x7b;
    undefined field99_0x7c;
    undefined field100_0x7d;
    undefined field101_0x7e;
    undefined field102_0x7f;
    undefined field103_0x80;
    undefined field104_0x81;
    undefined field105_0x82;
    undefined field106_0x83;
    undefined field107_0x84;
    undefined field108_0x85;
    undefined field109_0x86;
    undefined field110_0x87;
    undefined field111_0x88;
    undefined field112_0x89;
    undefined field113_0x8a;
    undefined field114_0x8b;
    undefined field115_0x8c;
    undefined field116_0x8d;
    undefined field117_0x8e;
    undefined field118_0x8f;
    int field119_0x90;
    undefined field120_0x94;
    undefined field121_0x95;
    undefined field122_0x96;
    undefined field123_0x97;
    undefined field124_0x98;
    undefined field125_0x99;
    undefined field126_0x9a;
    undefined field127_0x9b;
    undefined field128_0x9c;
    undefined field129_0x9d;
    undefined field130_0x9e;
    undefined field131_0x9f;
    undefined field132_0xa0;
    undefined field133_0xa1;
    undefined field134_0xa2;
    undefined field135_0xa3;
    undefined field136_0xa4;
    undefined field137_0xa5;
    undefined field138_0xa6;
    undefined field139_0xa7;
    undefined field140_0xa8;
    undefined field141_0xa9;
    undefined field142_0xaa;
    undefined field143_0xab;
    undefined field144_0xac;
    undefined field145_0xad;
    undefined field146_0xae;
    undefined field147_0xaf;
    undefined field148_0xb0;
    undefined field149_0xb1;
    undefined field150_0xb2;
    undefined field151_0xb3;
    struct window_input_ctx window_input_ctx;
    undefined field153_0x1ac;
    undefined field154_0x1ad;
    undefined field155_0x1ae;
    undefined field156_0x1af;
    undefined field157_0x1b0;
    undefined field158_0x1b1;
    undefined field159_0x1b2;
    undefined field160_0x1b3;
    undefined field161_0x1b4;
    undefined field162_0x1b5;
    undefined field163_0x1b6;
    undefined field164_0x1b7;
    undefined field165_0x1b8;
    undefined field166_0x1b9;
    undefined field167_0x1ba;
    undefined field168_0x1bb;
    undefined field169_0x1bc;
    undefined field170_0x1bd;
    undefined field171_0x1be;
    undefined field172_0x1bf;
    undefined field173_0x1c0;
    undefined field174_0x1c1;
    undefined field175_0x1c2;
    undefined field176_0x1c3;
    undefined field177_0x1c4;
    undefined field178_0x1c5;
    undefined field179_0x1c6;
    undefined field180_0x1c7;
    undefined field181_0x1c8;
    undefined field182_0x1c9;
    undefined field183_0x1ca;
    undefined field184_0x1cb;
    undefined field185_0x1cc;
    undefined field186_0x1cd;
    undefined field187_0x1ce;
    undefined field188_0x1cf;
    undefined field189_0x1d0;
    undefined field190_0x1d1;
    undefined field191_0x1d2;
    undefined field192_0x1d3;
    undefined field193_0x1d4;
    undefined field194_0x1d5;
    undefined field195_0x1d6;
    undefined field196_0x1d7;
    undefined field197_0x1d8;
    undefined field198_0x1d9;
    undefined field199_0x1da;
    undefined field200_0x1db;
    undefined field201_0x1dc;
    undefined field202_0x1dd;
    undefined field203_0x1de;
    undefined field204_0x1df;
    undefined field205_0x1e0;
    undefined field206_0x1e1;
    undefined field207_0x1e2;
    undefined field208_0x1e3;
    undefined field209_0x1e4;
    undefined field210_0x1e5;
    undefined field211_0x1e6;
    undefined field212_0x1e7;
    undefined field213_0x1e8;
    undefined field214_0x1e9;
    undefined field215_0x1ea;
    undefined field216_0x1eb;
    undefined field217_0x1ec;
    undefined field218_0x1ed;
    undefined field219_0x1ee;
    undefined field220_0x1ef;
    undefined field221_0x1f0;
    undefined field222_0x1f1;
    undefined field223_0x1f2;
    undefined field224_0x1f3;
    undefined field225_0x1f4;
    undefined field226_0x1f5;
    undefined field227_0x1f6;
    undefined field228_0x1f7;
    undefined field229_0x1f8;
    undefined field230_0x1f9;
    undefined field231_0x1fa;
    undefined field232_0x1fb;
    undefined field233_0x1fc;
    undefined field234_0x1fd;
    undefined field235_0x1fe;
    undefined field236_0x1ff;
    undefined field237_0x200;
    undefined field238_0x201;
    undefined field239_0x202;
    undefined field240_0x203;
    undefined field241_0x204;
    undefined field242_0x205;
    undefined field243_0x206;
    undefined field244_0x207;
    undefined field245_0x208;
    undefined field246_0x209;
    undefined field247_0x20a;
    undefined field248_0x20b;
    undefined field249_0x20c;
    undefined field250_0x20d;
    undefined field251_0x20e;
    undefined field252_0x20f;
    undefined field253_0x210;
    undefined field254_0x211;
    undefined field255_0x212;
    undefined field256_0x213;
    undefined field257_0x214;
    undefined field258_0x215;
    undefined field259_0x216;
    undefined field260_0x217;
    undefined field261_0x218;
    undefined field262_0x219;
    undefined field263_0x21a;
    undefined field264_0x21b;
    undefined field265_0x21c;
    undefined field266_0x21d;
    undefined field267_0x21e;
    undefined field268_0x21f;
    undefined field269_0x220;
    undefined field270_0x221;
    undefined field271_0x222;
    undefined field272_0x223;
    undefined field273_0x224;
    undefined field274_0x225;
    undefined field275_0x226;
    undefined field276_0x227;
    undefined field277_0x228;
    undefined field278_0x229;
    undefined field279_0x22a;
    undefined field280_0x22b;
    undefined field281_0x22c;
    undefined field282_0x22d;
    undefined field283_0x22e;
    undefined field284_0x22f;
    undefined field285_0x230;
    undefined field286_0x231;
    undefined field287_0x232;
    undefined field288_0x233;
    undefined field289_0x234;
    undefined field290_0x235;
    undefined field291_0x236;
    undefined field292_0x237;
    undefined field293_0x238;
    undefined field294_0x239;
    undefined field295_0x23a;
    undefined field296_0x23b;
    undefined field297_0x23c;
    undefined field298_0x23d;
    undefined field299_0x23e;
    undefined field300_0x23f;
    undefined field301_0x240;
    undefined field302_0x241;
    undefined field303_0x242;
    undefined field304_0x243;
    undefined field305_0x244;
    undefined field306_0x245;
    undefined field307_0x246;
    undefined field308_0x247;
    undefined field309_0x248;
    undefined field310_0x249;
    undefined field311_0x24a;
    undefined field312_0x24b;
    undefined field313_0x24c;
    undefined field314_0x24d;
    undefined field315_0x24e;
    undefined field316_0x24f;
    undefined field317_0x250;
    undefined field318_0x251;
    undefined field319_0x252;
    undefined field320_0x253;
    undefined field321_0x254;
    undefined field322_0x255;
    undefined field323_0x256;
    undefined field324_0x257;
    undefined field325_0x258;
    undefined field326_0x259;
    undefined field327_0x25a;
    undefined field328_0x25b;
    undefined field329_0x25c;
    undefined field330_0x25d;
    undefined field331_0x25e;
    undefined field332_0x25f;
    undefined field333_0x260;
    undefined field334_0x261;
    undefined field335_0x262;
    undefined field336_0x263;
    undefined field337_0x264;
    undefined field338_0x265;
    undefined field339_0x266;
    undefined field340_0x267;
    undefined field341_0x268;
    undefined field342_0x269;
    undefined field343_0x26a;
    undefined field344_0x26b;
    undefined field345_0x26c;
    undefined field346_0x26d;
    undefined field347_0x26e;
    undefined field348_0x26f;
    undefined field349_0x270;
    undefined field350_0x271;
    undefined field351_0x272;
    undefined field352_0x273;
    undefined field353_0x274;
    undefined field354_0x275;
    undefined field355_0x276;
    undefined field356_0x277;
    undefined field357_0x278;
    undefined field358_0x279;
    undefined field359_0x27a;
    undefined field360_0x27b;
    undefined field361_0x27c;
    undefined field362_0x27d;
    undefined field363_0x27e;
    undefined field364_0x27f;
    undefined field365_0x280;
    undefined field366_0x281;
    undefined field367_0x282;
    undefined field368_0x283;
    undefined field369_0x284;
    undefined field370_0x285;
    undefined field371_0x286;
    undefined field372_0x287;
    undefined field373_0x288;
    undefined field374_0x289;
    undefined field375_0x28a;
    undefined field376_0x28b;
    undefined field377_0x28c;
    undefined field378_0x28d;
    undefined field379_0x28e;
    undefined field380_0x28f;
    undefined field381_0x290;
    undefined field382_0x291;
    undefined field383_0x292;
    undefined field384_0x293;
    undefined field385_0x294;
    undefined field386_0x295;
    undefined field387_0x296;
    undefined field388_0x297;
    undefined field389_0x298;
    undefined field390_0x299;
    undefined field391_0x29a;
    undefined field392_0x29b;
    undefined field393_0x29c;
    undefined field394_0x29d;
    undefined field395_0x29e;
    undefined field396_0x29f;
    undefined field397_0x2a0;
    undefined field398_0x2a1;
    undefined field399_0x2a2;
    undefined field400_0x2a3;
    undefined field401_0x2a4;
    undefined field402_0x2a5;
    undefined field403_0x2a6;
    undefined field404_0x2a7;
    undefined field405_0x2a8;
    undefined field406_0x2a9;
    undefined field407_0x2aa;
    undefined field408_0x2ab;
    undefined field409_0x2ac;
    undefined field410_0x2ad;
    undefined field411_0x2ae;
    undefined field412_0x2af;
    undefined field413_0x2b0;
    undefined field414_0x2b1;
    undefined field415_0x2b2;
    undefined field416_0x2b3;
    undefined field417_0x2b4;
    undefined field418_0x2b5;
    undefined field419_0x2b6;
    undefined field420_0x2b7;
    undefined field421_0x2b8;
    undefined field422_0x2b9;
    undefined field423_0x2ba;
    undefined field424_0x2bb;
    undefined field425_0x2bc;
    undefined field426_0x2bd;
    undefined field427_0x2be;
    undefined field428_0x2bf;
    undefined field429_0x2c0;
    undefined field430_0x2c1;
    undefined field431_0x2c2;
    undefined field432_0x2c3;
    undefined field433_0x2c4;
    undefined field434_0x2c5;
    undefined field435_0x2c6;
    undefined field436_0x2c7;
    undefined field437_0x2c8;
    undefined field438_0x2c9;
    undefined field439_0x2ca;
    undefined field440_0x2cb;
    undefined field441_0x2cc;
    undefined field442_0x2cd;
    undefined field443_0x2ce;
    undefined field444_0x2cf;
    undefined field445_0x2d0;
    undefined field446_0x2d1;
    undefined field447_0x2d2;
    undefined field448_0x2d3;
    undefined field449_0x2d4;
    undefined field450_0x2d5;
    undefined field451_0x2d6;
    undefined field452_0x2d7;
    undefined field453_0x2d8;
    undefined field454_0x2d9;
    undefined field455_0x2da;
    undefined field456_0x2db;
    undefined field457_0x2dc;
    undefined field458_0x2dd;
    undefined field459_0x2de;
    undefined field460_0x2df;
    undefined field461_0x2e0;
    undefined field462_0x2e1;
    undefined field463_0x2e2;
    undefined field464_0x2e3;
    undefined field465_0x2e4;
    undefined field466_0x2e5;
    undefined field467_0x2e6;
    undefined field468_0x2e7;
    undefined field469_0x2e8;
    undefined field470_0x2e9;
    undefined field471_0x2ea;
    undefined field472_0x2eb;
    undefined field473_0x2ec;
    undefined field474_0x2ed;
    undefined field475_0x2ee;
    undefined field476_0x2ef;
    undefined field477_0x2f0;
    undefined field478_0x2f1;
    undefined field479_0x2f2;
    undefined field480_0x2f3;
    undefined field481_0x2f4;
    undefined field482_0x2f5;
    undefined field483_0x2f6;
    undefined field484_0x2f7;
    undefined field485_0x2f8;
    undefined field486_0x2f9;
    undefined field487_0x2fa;
    undefined field488_0x2fb;
    undefined field489_0x2fc;
    undefined field490_0x2fd;
    undefined field491_0x2fe;
    undefined field492_0x2ff;
    undefined field493_0x300;
    undefined field494_0x301;
    undefined field495_0x302;
    undefined field496_0x303;
    undefined field497_0x304;
    undefined field498_0x305;
    undefined field499_0x306;
    undefined field500_0x307;
    undefined field501_0x308;
    undefined field502_0x309;
    undefined field503_0x30a;
    undefined field504_0x30b;
    undefined field505_0x30c;
    undefined field506_0x30d;
    undefined field507_0x30e;
    undefined field508_0x30f;
    undefined field509_0x310;
    undefined field510_0x311;
    undefined field511_0x312;
    undefined field512_0x313;
    undefined field513_0x314;
    undefined field514_0x315;
    undefined field515_0x316;
    undefined field516_0x317;
    undefined field517_0x318;
    undefined field518_0x319;
    undefined field519_0x31a;
    undefined field520_0x31b;
    undefined field521_0x31c;
    undefined field522_0x31d;
    undefined field523_0x31e;
    undefined field524_0x31f;
    undefined field525_0x320;
    undefined field526_0x321;
    undefined field527_0x322;
    undefined field528_0x323;
    undefined field529_0x324;
    undefined field530_0x325;
    undefined field531_0x326;
    undefined field532_0x327;
    undefined field533_0x328;
    undefined field534_0x329;
    undefined field535_0x32a;
    undefined field536_0x32b;
    undefined field537_0x32c;
    undefined field538_0x32d;
    undefined field539_0x32e;
    undefined field540_0x32f;
    int simple_menu_2_result;
    struct recycle_offer_items offer_items;
    struct simple_menu_item* simple_menu_item_ptr; // Created by retype action
};
ASSERT_SIZE(struct recycle_unk_struct_0x340, 832);

// Likely used for selecting items you own between bag and storage. May be shared with the swap
// shop?
struct recycle_unk_struct_0x1cc {
    int case_id;
    int8_t collection_window_id;
    int8_t simple_menu_window_id;
    int8_t scroll_box_window_id;
    int8_t textbox_window_id_1;
    int8_t textbox_window_id_2;
    undefined field_0x9;
    undefined field_0xa;
    undefined field_0xb;
    int item_selection_index;
    int item_selection_index_backup;
    int starts_zero;
    int also_starts_zero;
    bool explain;
    undefined field12_0x1d;
    undefined field13_0x1e;
    undefined field14_0x1f;
    struct window_extra_info window_extra_info;
    struct window_input_ctx window_input_ctx;
    char* option_states;
    int simple_menu_result;
    struct owned_item** owned_items_list_ptr;
    int num_items_required;
    void* function_1;
    void* function_2;
    void* function_3;
};
ASSERT_SIZE(struct recycle_unk_struct_0x1cc, 460);

#endif
