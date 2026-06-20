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
extern struct pixel_position POSITION_ZERO;
extern struct level_tilemap_list_entry LEVEL_TILEMAP_LIST[81];
extern struct ground_entity_function_table ACTOR_FUNCTION_TABLE;
extern struct animation_data SETANIMATION_TABLE[84];
extern struct ground_entity_function_table OBJECT_FUNCTION_TABLE;
extern struct ground_entity_function_table PERFORMER_FUNCTION_TABLE;
extern struct window_params TEAM_INFO_BOX_DEFAULT_WINDOW_PARAMS;
extern char SWAP_SHOP_ONE_STAR_STR[7];
extern char SWAP_SHOP_TWO_STAR_STR[13];
extern char SWAP_SHOP_THREE_STAR_STR[19];
extern char SWAP_SHOP_RED_TEXT_TAG_STR[7];
extern char SWAP_SHOP_UNCENTER_TEXT_TAG_STR[5];
extern char SWAP_SHOP_CLUMSET_ONE_STAR_STR[21];
extern char SWAP_SHOP_CLUMSET_TWO_STAR_STR[27];
extern char SWAP_SHOP_CLUMSET_THREE_STAR_STR[33];
extern struct overlay_load_entry OVERLAY11_OVERLAY_LOAD_TABLE[21];
extern undefined4 OV11_STATIC_INITIALIZER;
extern struct main_ground_data GROUND_STATE_PTRS;
extern struct exclusive_item_requirements* EXCLUSIVE_ITEM_REQUIREMENTS_PTR;
extern struct swap_shop_inventory_ptrs SWAP_SHOP_INVENTORY_PTRS;
extern bool SPINDA_CAFE_OVERLAY_STATUS;
extern bool RECYCLE_SHOP_OVERLAY_STATUS;
extern uint32_t WORLD_MAP_MODE;

#endif
