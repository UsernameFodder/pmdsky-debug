#ifndef HEADERS_DATA_OVERLAY13_H_
#define HEADERS_DATA_OVERLAY13_H_

extern uint8_t QUIZ_BORDER_COLOR_TABLE[2];
extern struct vec2 PARTNER_PORTRAIT_OFFSET;
extern uint8_t PLAY_OLD_GAME_BOOST[2][4];
extern struct overlay_load_entry OVERLAY13_LOAD_ENTRY;
extern struct window_params MAIN_DBOX_WINDOW_PARAMS;
extern struct window_params AURA_BOW_DBOX_WINDOW_PARAMS;
extern struct window_params DEBUG_WINDOW_PARAMS;
extern struct window_params QUIZ_INTRO_WINDOW_PARAMS;
extern struct simple_menu_id_item CONFIRM_PARTNER_MENU_ITEMS[2];
extern struct monster_id_16 STARTERS_PARTNER_IDS[21];
extern struct monster_id_16 STARTERS_HERO_IDS[16][2];
extern uint32_t STARTERS_TYPE_INCOMPATIBILITY_TABLE[18];
extern uint16_t STARTERS_STRINGS[16][3];
extern uint16_t QUIZ_QUESTION_STRINGS[66];
extern uint16_t QUIZ_ANSWER_STRINGS[175];
extern uint8_t QUIZ_ANSWER_POINTS[171][16];
extern undefined4 OV13_STATIC_INITIALIZER;
extern struct personality_test* PERSONALITY_TEST_PTR;
extern uint32_t PARTNER_SELECT_MENU_OPTION_TRACKER;
extern uint32_t PARTNER_SELECT_MENU_OPTION_TIMER;
extern struct window_params QUESTION_ANSWER_WINDOW_PARAMS;
extern struct window_params CHOOSE_PARTNER_WINDOW_PARAMS;
extern struct simple_menu_id_item QUIZ_QUESTION_MENU_ITEMS[9];
extern struct quiz_answer_association QUIZ_ANSWER_ASSOCIATIONS[66];

#endif
