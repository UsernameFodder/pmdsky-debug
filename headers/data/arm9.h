#ifndef HEADERS_DATA_ARM9_H_
#define HEADERS_DATA_ARM9_H_

#include "arm9/itcm.h"

extern struct start_module_params* START_MODULE_PARAMS;
extern uint32_t DEFAULT_MEMORY_ARENA_SIZE;
extern int LOG_MAX_ARG;
extern enum damage_source_non_move DAMAGE_SOURCE_CODE_ORB_ITEM;
extern enum damage_source_non_move DAMAGE_SOURCE_CODE_NON_ORB_ITEM;
extern enum item_id AURA_BOW_ID_LAST;
extern uint32_t NUMBER_OF_ITEMS;
extern uint32_t MAX_MONEY_CARRIED;
extern uint32_t MAX_MONEY_STORED;
extern struct dialog_box_list* DIALOG_BOX_LIST_PTR;
extern struct script_var_value_table* SCRIPT_VARS_VALUES_PTR;
extern uint32_t MONSTER_ID_LIMIT;
extern uint32_t MAX_RECRUITABLE_TEAM_MEMBERS;
extern int16_t NATURAL_LOG_VALUE_TABLE[2048];
extern undefined CART_REMOVED_IMG_DATA[0]; // Length differs between versions
extern char STRING_DEBUG_EMPTY[1];
extern char STRING_DEBUG_FORMAT_LINE_FILE[27];
extern char STRING_DEBUG_NO_PROG_POS[21];
extern char STRING_DEBUG_SPACED_PRINT[10];
extern char STRING_DEBUG_FATAL[19];
extern char STRING_DEBUG_NEWLINE[2];
extern char STRING_DEBUG_LOG_NULL[8];
extern char STRING_DEBUG_STRING_NEWLINE[4];
extern char STRING_EFFECT_EFFECT_BIN[18];
extern char STRING_MONSTER_MONSTER_BIN[20];
extern char STRING_BALANCE_M_LEVEL_BIN[20];
extern char STRING_DUNGEON_DUNGEON_BIN[20];
extern char STRING_MONSTER_M_ATTACK_BIN[21];
extern char STRING_MONSTER_M_GROUND_BIN[21];
extern char STRING_FILE_DIRECTORY_INIT[37];
extern enum item_id KECLEON_SHOP_ITEM_TABLE_LISTS_1[4];
extern enum item_id KECLEON_SHOP_ITEM_TABLE_LISTS_2[4];
extern struct exclusive_item_stat_boost_entry EXCLUSIVE_ITEM_STAT_BOOST_DATA[15];
extern struct exclusive_item_effect_entry EXCLUSIVE_ITEM_EFFECT_DATA[956];
extern struct item_id_16 TYPE_SPECIFIC_EXCLUSIVE_ITEMS[17][4];
extern struct move_id_16 RECOIL_MOVE_LIST[11];
extern struct move_id_16 PUNCH_MOVE_LIST[16];
extern int MOVE_POWER_STARS_TABLE[6];
extern int MOVE_ACCURACY_STARS_TABLE[8];
extern struct partner_talk_kind_table_entry PARTNER_TALK_KIND_TABLE[11];
extern struct script_local_var_table SCRIPT_VARS_LOCALS;
extern struct script_var_table SCRIPT_VARS;
extern struct portrait_layout PORTRAIT_LAYOUTS[32];
extern char KAOMADO_FILEPATH[20];
extern uint8_t WONDER_MAIL_BITS_MAP[32];
extern uint8_t WONDER_MAIL_BITS_SWAP[36];
extern uint8_t WONDER_MAIL_ENCRYPTION_TABLE[256];
extern struct dungeon_data_list_entry DUNGEON_DATA_LIST[180];
extern struct monster_id_16 ADVENTURE_LOG_ENCOUNTERS_MONSTER_IDS[38];
extern int16_t TACTIC_NAME_STRING_IDS[12];
extern int16_t STATUS_NAME_STRING_IDS[102];
extern struct dungeon_return_status DUNGEON_RETURN_STATUS_TABLE[91];
extern struct status_description STATUSES_FULL_DESCRIPTION_STRING_IDS[103];
extern struct mission_floors_forbidden MISSION_FLOORS_FORBIDDEN[100];
extern undefined* MISSION_FLOOR_RANKS_PTRS[100];
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
extern int16_t EGG_HP_BONUS;
extern int16_t EVOLUTION_HP_BONUS;
extern int DAMAGE_FORMULA_FLV_SHIFT;
extern int16_t EVOLUTION_PHYSICAL_STAT_BONUSES[2];
extern int DAMAGE_FORMULA_CONSTANT_SHIFT;
extern int DAMAGE_FORMULA_FLV_DEFICIT_DIVISOR;
extern int16_t EGG_STAT_BONUSES[4];
extern int16_t EVOLUTION_SPECIAL_STAT_BONUSES[2];
extern int DAMAGE_FORMULA_NON_TEAM_MEMBER_MODIFIER;
extern int DAMAGE_FORMULA_LN_PREFACTOR;
extern int DAMAGE_FORMULA_DEF_PREFACTOR;
extern int DAMAGE_FORMULA_AT_PREFACTOR;
extern int DAMAGE_FORMULA_LN_ARG_PREFACTOR;
extern struct forbidden_forgot_move_entry FORBIDDEN_FORGOT_MOVE_LIST[3];
extern int16_t TACTICS_UNLOCK_LEVEL_TABLE[12];
extern int16_t CLIENT_LEVEL_TABLE[16];
extern int16_t OUTLAW_LEVEL_TABLE[16];
extern int16_t OUTLAW_MINION_LEVEL_TABLE[16];
extern int HIDDEN_POWER_BASE_POWER_TABLE[10];
extern struct version_exclusive_monster VERSION_EXCLUSIVE_MONSTERS[23];
extern int16_t IQ_SKILL_RESTRICTIONS[69];
extern struct secondary_terrain_type_8 SECONDARY_TERRAIN_TYPES[200];
extern struct monster_id_16 SENTRY_DUTY_MONSTER_IDS[102];
extern int32_t IQ_SKILLS[69];
extern uint8_t IQ_GROUP_SKILLS[400];
extern int MONEY_QUANTITY_TABLE[100];
extern int16_t IQ_GUMMI_GAIN_TABLE[18][18];
extern int16_t GUMMI_BELLY_RESTORE_TABLE[18][18];
extern uint32_t BAG_CAPACITY_TABLE_SPECIAL_EPISODES[5];
extern uint32_t BAG_CAPACITY_TABLE[8];
extern struct monster_id_16 SPECIAL_EPISODE_MAIN_CHARACTERS[100];
extern struct guest_monster GUEST_MONSTER_DATA[18];
extern struct rankup_table_entry RANK_UP_TABLE[13];
extern struct monster_id_16 DS_DOWNLOAD_TEAMS[56];
extern enum monster_id UNOWN_SPECIES_ADDITIONAL_CHARS[28];
extern undefined MONSTER_SPRITE_DATA[1200];
extern int16_t MISSION_MENU_STRING_IDS_1[8];
extern int16_t MISSION_MENU_STRING_IDS_2[8];
extern struct dungeon_unlock_entry MISSION_DUNGEON_UNLOCK_TABLE[3];
extern struct item_id_16 NO_SEND_ITEM_TABLE[3];
extern struct monster_id_16 MISSION_BANNED_STORY_MONSTERS[21];
extern struct item_id_16 ITEM_DELIVERY_TABLE[23];
extern int MISSION_RANK_POINTS[16];
extern struct monster_id_16 MISSION_BANNED_MONSTERS[124];
extern int16_t MISSION_STRING_IDS[964];
extern struct script_level EVENTS[0]; // Length differs between versions
extern struct monster_id_16 DEMO_TEAMS[18][2];
extern struct script_entity ENTITIES[386];
extern struct dungeon_id_8 DUNGEON_SWAP_ID_TABLE[212];
extern struct map_marker MAP_MARKER_PLACEMENTS[310];
extern struct trig_values TRIG_TABLE[4096];
extern struct mem_arena_getters MEMORY_ALLOCATION_ARENA_GETTERS;
extern uint16_t PRNG_SEQUENCE_NUM;
extern enum overlay_group_id LOADED_OVERLAY_GROUP_0;
extern enum overlay_group_id LOADED_OVERLAY_GROUP_1;
extern enum overlay_group_id LOADED_OVERLAY_GROUP_2;
extern bool DEBUG_IS_INITIALIZED;
extern struct pack_file_opened* PACK_FILES_OPENED;
extern const char* PACK_FILE_PATHS_TABLE[6];
extern struct item* BAG_ITEMS_PTR_MIRROR;
extern void* ITEM_DATA_TABLE_PTRS[3];
extern struct move_data_table* MOVE_DATA_TABLE_PTR;
extern struct wan_table* WAN_TABLE;
extern struct render_3d_global RENDER_3D;
extern render_3d_element_64_fn_t RENDER_3D_FUNCTIONS_64[8];
extern int16_t TBL_TALK_GROUP_STRING_ID_START[6];
extern int16_t KEYBOARD_STRING_IDS[30];
extern bool NOTIFY_NOTE;
extern struct monster_id_16 DEFAULT_HERO_ID;
extern struct monster_id_16 DEFAULT_PARTNER_ID;
extern uint8_t GAME_MODE;
extern struct global_progress* GLOBAL_PROGRESS_PTR;
extern struct adventure_log* ADVENTURE_LOG_PTR;
extern void* ITEM_TABLES_PTRS_1[26];
extern enum monster_id* UNOWN_SPECIES_ADDITIONAL_CHAR_PTR_TABLE[28];
extern struct team_member_table* TEAM_MEMBER_TABLE_PTR;
extern undefined* MISSION_LIST_PTR;
extern const char* REMOTE_STRING_PTR_TABLE[7];
extern const char* RANK_STRING_PTR_TABLE[16];
extern void* SMD_EVENTS_FUN_TABLE[127];
extern int16_t MUSIC_DURATION_LOOKUP_TABLE_1[128];
extern int32_t MUSIC_DURATION_LOOKUP_TABLE_2[128];

#endif
