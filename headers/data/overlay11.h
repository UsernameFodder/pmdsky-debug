#ifndef HEADERS_DATA_OVERLAY11_H_
#define HEADERS_DATA_OVERLAY11_H_

extern undefined SCRIPT_COMMAND_PARSING_DATA[32];
extern char SCRIPT_OP_CODE_NAMES[0]; // Length might differ by version; needs verification
extern struct script_opcode_table SCRIPT_OP_CODES;
extern char OVERLAY11_DEBUG_STRINGS[0]; // Length might differ by version; needs verification
extern char C_ROUTINE_NAMES[0];         // Length might differ by version; needs verification
extern struct common_routine_table C_ROUTINES;
extern struct ground_weather_entry GROUND_WEATHER_TABLE[12];
extern char GROUND_WAN_FILES_TABLE[343][12];
extern struct script_object OBJECTS[0]; // Length differs by version
extern struct dungeon_id_16 RECRUITMENT_TABLE_LOCATIONS[22];
extern int16_t RECRUITMENT_TABLE_LEVELS[22];
extern struct monster_id_16 RECRUITMENT_TABLE_SPECIES[22];
extern struct level_tilemap_list_entry LEVEL_TILEMAP_LIST[81];
extern struct animation_data SETANIMATION_TABLE[84];
extern struct window_params TEAM_INFO_BOX_DEFAULT_WINDOW_PARAMS;
extern struct overlay_load_entry OVERLAY11_OVERLAY_LOAD_TABLE[21];
extern struct main_ground_data GROUND_STATE_PTRS;
extern uint32_t WORLD_MAP_MODE;

#endif
