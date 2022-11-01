#ifndef HEADERS_DATA_ARM9_H_
#define HEADERS_DATA_ARM9_H_

#include "arm9/itcm.h"

extern uint32_t DEFAULT_MEMORY_ARENA_SIZE;
extern enum item_id AURA_BOW_ID_LAST;
extern uint32_t NUMBER_OF_ITEMS;
extern uint32_t MAX_MONEY_CARRIED;
extern uint32_t MAX_MONEY_STORED;
extern struct script_var_value_table* SCRIPT_VARS_VALUES_PTR;
extern uint32_t MONSTER_ID_LIMIT;
extern uint32_t MAX_RECRUITABLE_TEAM_MEMBERS;
extern undefined CART_REMOVED_IMG_DATA[0]; // Length differs between versions
extern struct exclusive_item_stat_boost_entry EXCLUSIVE_ITEM_STAT_BOOST_DATA[15];
extern struct exclusive_item_effect_entry EXCLUSIVE_ITEM_EFFECT_DATA[956];
extern struct move_id_16 RECOIL_MOVE_LIST[11];
extern struct move_id_16 PUNCH_MOVE_LIST[16];
extern struct script_local_var_table SCRIPT_VARS_LOCALS;
extern struct script_var_table SCRIPT_VARS;
extern struct dungeon_data_list_entry DUNGEON_DATA_LIST[180];
extern struct dungeon_restriction DUNGEON_RESTRICTIONS[256];
extern int16_t SPECIAL_BAND_STAT_BOOST;
extern int16_t MUNCH_BELT_STAT_BOOST;
extern int16_t GUMMI_STAT_BOOST;
extern int32_t MIN_IQ_EXCLUSIVE_MOVE_USER;
extern int16_t WONDER_GUMMI_IQ_GAIN;
extern int16_t AURA_BOW_STAT_BOOST;
extern int32_t MIN_IQ_ITEM_MASTER;
extern int16_t DEF_SCARF_STAT_BOOST;
extern int16_t POWER_BAND_STAT_BOOST;
extern int16_t WONDER_GUMMI_STAT_BOOST;
extern int16_t ZINC_BAND_STAT_BOOST;
extern int16_t TACTICS_UNLOCK_LEVEL_TABLE[12];
extern int16_t OUTLAW_LEVEL_TABLE[16];
extern int16_t OUTLAW_MINION_LEVEL_TABLE[16];
extern int16_t IQ_SKILL_RESTRICTIONS[69];
extern struct secondary_terrain_type_8 SECONDARY_TERRAIN_TYPES[200];
extern int32_t IQ_SKILLS[69];
extern uint8_t IQ_GROUP_SKILLS[400];
extern int16_t IQ_GUMMI_GAIN_TABLE[18][18];
extern int16_t GUMMI_BELLY_RESTORE_TABLE[18][18];
extern uint32_t BAG_CAPACITY_TABLE[8];
extern struct monster_id_16 SPECIAL_EPISODE_MAIN_CHARACTERS[100];
extern struct guest_monster GUEST_MONSTER_DATA[18];
extern struct rankup_table_entry RANK_UP_TABLE[13];
extern undefined MONSTER_SPRITE_DATA[1200];
extern struct monster_id_16 MISSION_BANNED_STORY_MONSTERS[21];
extern struct monster_id_16 MISSION_BANNED_MONSTERS[124];
extern struct script_level EVENTS[0]; // Length differs between versions
extern struct script_entity ENTITIES[386];
extern struct map_marker MAP_MARKER_PLACEMENTS[310];
extern struct mem_arena_getters MEMORY_ALLOCATION_ARENA_GETTERS;
extern uint16_t PRNG_SEQUENCE_NUM;
extern enum overlay_group_id LOADED_OVERLAY_GROUP_0;
extern enum overlay_group_id LOADED_OVERLAY_GROUP_1;
extern enum overlay_group_id LOADED_OVERLAY_GROUP_2;
extern struct pack_file_opened* PACK_FILE_OPENED;
extern const char* PACK_FILE_PATHS_TABLE[6];
extern struct move_data_table* MOVE_DATA_TABLE_PTR;
extern bool NOTIFY_NOTE;
extern struct monster_id_16 DEFAULT_HERO_ID;
extern struct monster_id_16 DEFAULT_PARTNER_ID;
extern uint8_t GAME_MODE;
extern struct global_progress* GLOBAL_PROGRESS_PTR;
extern struct adventure_log* ADVENTURE_LOG_PTR;
extern void* ITEM_TABLES_PTRS_1[26];
extern void* SMD_EVENTS_FUN_TABLE[127];

#endif
