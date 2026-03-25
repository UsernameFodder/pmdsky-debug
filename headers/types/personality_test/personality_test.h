// Types used primarily in the context of the personality test (run by overlay 13),
#ifndef HEADERS_TYPES_PERSONALITY_TEST_H_
#define HEADERS_TYPES_PERSONALITY_TEST_H_

enum personality_test_state {
    QUIZ_INIT = 0,
    QUIZ_INTRO = 1,
    QUIZ_START_QUIZ = 2,
    QUIZ_PREP_QUESTION = 3,
    QUIZ_QUESTION_PROMPT = 4,
    QUIZ_QUESTION_MENU = 5,
    QUIZ_ANSWER_QUESTION = 6,
    QUIZ_AFTER_QUESTION = 7,
    QUIZ_FINISH_QUESTIONS = 8,
    QUIZ_AURA_INFO = 9,
    QUIZ_AURA_INFO_AWAIT_DBOX_INACTIVE = 10,
    QUIZ_AURA_INFO_DBOX_INACTIVE = 11,
    QUIZ_HOLD_PROMPT = 12,
    QUIZ_HOLD_PROMPT_AWAIT_DBOX_INACTIVE = 13,
    QUIZ_HOLD_PROMPT_DBOX_INACTIVE = 14,
    QUIZ_AWAIT_BEGIN_HOLD = 15,
    QUIZ_BEGIN_HOLD = 16,
    QUIZ_CONTINUE_HOLD = 17,
    QUIZ_CONTINUE_HOLD_AWAIT_DBOX_INACTIVE = 18,
    QUIZ_CONTINUE_HOLD_DBOX_INACTIVE = 19,
    QUIZ_PRE_AURA_IDENTIFIED = 20,
    QUIZ_AURA_IDENTIFIED = 21,
    QUIZ_AURA_IDENTIFIED_AWAIT_DBOX_INACTIVE = 22,
    QUIZ_AURA_IDENTIFIED_DBOX_INACTIVE = 23,
    QUIZ_AWAIT_LIFT = 24,
    QUIZ_LIFTED_AWAIT_COUNTDOWN = 25,
    QUIZ_PREP_PRE_SHOW_AURA = 26,
    QUIZ_PRE_SHOW_AURA = 27,
    QUIZ_PRE_SHOW_AURA_AWAIT_DBOX_INACTIVE = 28,
    QUIZ_PRE_SHOW_AURA_DBOX_INACTIVE = 29,
    QUIZ_PREP_SHOW_AURA = 30,
    QUIZ_SHOW_AURA = 31,
    QUIZ_SHOW_AURA_AWAIT_DBOX_INACTIVE = 32,
    QUIZ_SHOW_AURA_DBOX_INACTIVE = 33,
    QUIZ_THANK_YOU = 34,
    QUIZ_UNUSED_35 = 35,
    QUIZ_EXPLAIN_PERSONALITY = 36,
    QUIZ_HERO_SPECIES = 37,
    QUIZ_HERO_SPECIES_AWAIT_DBOX_INACTIVE = 38,
    QUIZ_HERO_SPECIES_DBOX_INACTIVE = 39,
    QUIZ_PRE_CHOOSE_PARTNER = 40,
    QUIZ_PRE_CHOOSE_PARTNER_AWAIT_DBOX_INACTIVE = 41,
    QUIZ_CHOOSE_PARTNER_PROMPT = 42,
    QUIZ_CHOOSE_PARTNER_PROMPT_AWAIT_DBOX_INACTIVE = 43,
    QUIZ_START_CHOOSE_PARTNER = 44,
    QUIZ_CHOOSE_PARTNER = 45,
    QUIZ_FINISH_CHOOSE_PARTNER = 46,
    QUIZ_CONFIRM_PARTNER_SPECIES_PROMPT = 47,
    QUIZ_PARTNER_NAME_START = 48,
    QUIZ_AWAIT_PARTNER_NAME_FINISH = 49,
    QUIZ_PARTNER_NAME_FINISH = 50,
    QUIZ_CONFIRM_PARTNER_NAME_RESULT = 51,
    QUIZ_CONFIRM_PARTNER_NAME_YES = 52,
    QUIZ_CONFIRM_PARTNER_NAME_NO = 53,
    QUIZ_READY_TO_GO = 54,
    QUIZ_FINISH_1 = 55,
    QUIZ_FINISH_2 = 56,
    QUIZ_FINISH_3 = 57,
    QUIZ_WAIT_NEXT_STEP = 58,
    QUIZ_CONFIRM_PARTNER_SPECIES_MENU = 59,
    QUIZ_CONFIRM_PARTNER_SPECIES_MENU_RESULT = 60,
    QUIZ_EARLY_RELEASE = 61,
    QUIZ_EARLY_RELEASE_MESSAGE = 62,
    QUIZ_EARLY_RELEASE_RETRY = 63,
    QUIZ_UNK_64 = 64,
    QUIZ_UNK_65 = 65,
};

struct personality_test {
    uint8_t outer_state;      // 0x0: Used in a switch statement in PersonalityTestFrameUpdate. 0xD
                              // causes PersonalityTestMainManager to run.
    uint8_t next_outer_state; // 0x1: Educated guess.
    int8_t main_dbox_window_id;     // 0x2
    int8_t menu_window_id;          // 0x3
    int8_t aura_bow_dbox_window_id; // 0x4
    int8_t portrait_box_window_id;  // 0x5
    int8_t debug_window_id; // 0x6: Window ID for the debug window showing all point values. Only
                            // assigned if DEBUG_FLAG_EDIT_MODE is enabled.
    undefined1 padding_0x7;
    char* question_buf_ptr; // 0x8: Points to question_buf.
    undefined4 unk_0xc[2];
    char* answer_buf_ptr; // 0x14: Seems to be updated for each answer buf it fills in; as such,
                          // probably points the the buf for the last valid answer.
    undefined4 unk_0x18;  // 0x18
    undefined4 unk_0x1c;  // 0x1C
    enum personality_test_state state; // 0x20
    int32_t question_num;              // 0x24: Which question we're currently on.
    int32_t lift_continue_countdown;   // 0x28: After the "lift your finger" text shows, this begins
                                     // counting down by 1 each frame. The sequence will not advance
                                     // until touch is released AND this countdown has reached 0.
    undefined field_0x2c[4];
    enum personality_test_state future_state; // 0x30
    uint8_t personality_points[16]; // 0x34: Tracks points for each personality. The one with the
                                    // highest is the winning personality.
    uint8_t tiebreaker_bonus_points[16]; // 0x44: Indexes where points[i] is the highest point value
                                         // will get a random value between [0,19].
    uint8_t question_ids[10]; // 0x54: Question IDs that will be used throughout the quiz. Used to
                              // index in QUESTION_ANSWER_ASSOCIATIONS and QUIZ_QUESTION_STRINGS.
    bool not_play_old_game;   // 0x5E
    uint8_t gender;           // 0x5F
    uint16_t question_string_id; // 0x60: String ID for the current question.
    undefined field_0x62[10];
    char question_buf[512];  // 0x6C: Size is a guess
    char answer_bufs[4][64]; // 0x26C
    undefined field_0x36c[4];
    uint32_t n_partner_options;               // 0x370
    struct monster_id_16 partner_options[20]; // 0x374: Species that can be the partner, after
                                              // accounting for type incompatibilities.
    undefined field_0x39c[24];
    struct portrait_params portrait_params; // 0x3B4: For the portrait used in partner selection
    char charbuf_0x3c4[32]; // 0x3C4: Related to the keyboard for naming the partner.
    char charbuf_0x3e4[32]; // 0x3E4: Related to the keyboard for naming the partner. Size is an
                            // (educated) guess.
    char charbuf_0x404[32]; // 0x404: Related to the keyboard for naming the partner. Size is an
                            // (educated) guess.
    struct monster_id_16 partner_species; // 0x424
    uint16_t aura_color;                  // 0x426
};
ASSERT_SIZE(struct personality_test, 1064);

struct quiz_answer_association {
    uint8_t num_answers; // 0x0: How many answers there are to this question.
    uint8_t answer_idx;  // 0x1: Index of the first answer for this question in QUIZ_ANSWER_STRINGS
                         // and QUIZ_ANSWER_POINTS.
};
ASSERT_SIZE(struct quiz_answer_association, 2);

#endif
