#ifndef HEADERS_DATA_RAM_H_
#define HEADERS_DATA_RAM_H_

extern struct rgb* DUNGEON_COLORMAP_PTR;
extern struct dungeon DUNGEON_STRUCT;
extern struct move_data_table MOVE_DATA_TABLE;
extern uint32_t FRAMES_SINCE_LAUNCH;
extern struct item BAG_ITEMS[50];
extern struct item* BAG_ITEMS_PTR;
extern struct item_id_16 STORAGE_ITEMS[1000];
extern uint16_t STORAGE_ITEM_QUANTITIES[1000];
extern struct bulk_item* KECLEON_SHOP_ITEMS_PTR;
extern struct bulk_item KECLEON_SHOP_ITEMS[8];
extern struct bulk_item UNUSED_KECLEON_SHOP_ITEMS[8];
extern struct bulk_item* KECLEON_WARES_ITEMS_PTR;
extern struct bulk_item KECLEON_WARES_ITEMS[4];
extern struct bulk_item UNUSED_KECLEON_WARES_ITEMS[4];
extern int32_t MONEY_CARRIED;
extern int32_t MONEY_STORED;
extern struct move LAST_NEW_MOVE;
extern struct script_var_value_table SCRIPT_VARS_VALUES;
extern uint8_t BAG_LEVEL;
extern struct special_episode_type_8 DEBUG_SPECIAL_EPISODE_NUMBER;
extern struct dungeon_id_8 PENDING_DUNGEON_ID;
extern uint8_t PENDING_STARTING_FLOOR;
extern uint32_t PLAY_TIME_SECONDS;
extern uint8_t PLAY_TIME_FRAME_COUNTER;
extern char TEAM_NAME[10];
extern struct ground_monster TEAM_MEMBER_LIST[555];
extern uint32_t LEADER_IQ_SKILLS[3];
extern char LEADER_NICKNAME[10];
extern uint32_t PARTY_MEMBER_2_IQ_SKILLS[3];
extern uint32_t FRAMES_SINCE_LAUNCH_TIMES_THREE;
extern bool TURNING_ON_THE_SPOT_FLAG;
extern struct floor_generation_status FLOOR_GENERATION_STATUS;

#endif
