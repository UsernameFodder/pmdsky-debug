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
    int16_t field_0x2;
    int16_t field_0x4;
    int16_t field_0x6;
    int16_t field_0x8;
    int16_t field_0xa;
    int16_t field_0xc;
    // Probably padding
    undefined field_0xe[8];
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

#endif
