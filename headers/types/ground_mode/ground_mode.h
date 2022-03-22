// Types used primarily in the context of ground mode (also sometimes called "script mode")

#ifndef HEADERS_TYPES_GROUND_MODE_H_
#define HEADERS_TYPES_GROUND_MODE_H_

#include "enums.h"

// Variables that track game state, available to the script engine.
struct script_var {
    uint16_t type;        // 0x0
    undefined2 field_0x1; // 0x2
    uint16_t mem_offset;  // 0x4
    uint16_t bitshift;    // 0x6
    uint16_t n_values;    // 0x8
    int16_t default_val;  // 0xA
    char* name;           // 0xC: variable name
};
ASSERT_SIZE(struct script_var, 16);

// Table of all predefined global variables available to the script engine.
struct script_var_table {
    struct script_var vars[115];
};
ASSERT_SIZE(struct script_var_table, 1840);

// Special "local" variables available to the script engine.
struct script_local_var_table {
    struct script_var vars[4];
};
ASSERT_SIZE(struct script_local_var_table, 64);

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

#endif
