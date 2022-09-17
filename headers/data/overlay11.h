#ifndef HEADERS_DATA_OVERLAY11_H_
#define HEADERS_DATA_OVERLAY11_H_

extern struct script_opcode_table SCRIPT_OP_CODES;
extern struct common_routine_table C_ROUTINES;
extern struct script_object OBJECTS[0]; // Length differs by version
extern struct dungeon_id_16 RECRUITMENT_TABLE_LOCATIONS[22];
extern int16_t RECRUITMENT_TABLE_LEVELS[22];
extern struct monster_id_16 RECRUITMENT_TABLE_SPECIES[22];
extern struct level_tilemap_list_entry LEVEL_TILEMAP_LIST[81];
extern struct overlay_load_entry OVERLAY11_OVERLAY_LOAD_TABLE[21];
extern struct main_ground_data GROUND_STATE_PTRS;

#endif
