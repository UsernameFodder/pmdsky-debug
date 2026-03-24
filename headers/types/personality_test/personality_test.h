// Types used primarily in the context of the personality test (run by overlay 13),
#ifndef HEADERS_TYPES_PERSONALITY_TEST_H_
#define HEADERS_TYPES_PERSONALITY_TEST_H_

enum personality_test_state {
    INIT = 0,
    INTRO = 1,
    START_QUIZ = 2,
    PREP_QUESTION = 3,
    QUESTION_PROMPT = 4,
    QUESTION_MENU = 5,
    ANSWER_QUESTION = 6,
    AFTER_QUESTION = 7,
    FINISH_QUESTIONS = 8,
    AURA_INFO = 9,
    AURA_INFO_AWAIT_DBOX_INACTIVE = 10,
    AURA_INFO_DBOX_INACTIVE = 11,
    HOLD_PROMPT = 12,
    HOLD_PROMPT_AWAIT_DBOX_INACTIVE = 13,
    HOLD_PROMPT_DBOX_INACTIVE = 14,
    AWAIT_BEGIN_HOLD = 15,
    BEGIN_HOLD = 16,
    CONTINUE_HOLD = 17,
    CONTINUE_HOLD_AWAIT_DBOX_INACTIVE = 18,
    CONTINUE_HOLD_DBOX_INACTIVE = 19,
    PRE_AURA_IDENTIFIED = 20,
    AURA_IDENTIFIED = 21,
    AURA_IDENTIFIED_AWAIT_DBOX_INACTIVE = 22,
    AURA_IDENTIFIED_DBOX_INACTIVE = 23,
    AWAIT_LIFT = 24,
    LIFTED_AWAIT_COUNTDOWN = 25,
    PREP_PRE_SHOW_AURA = 26,
    PRE_SHOW_AURA = 27,
    PRE_SHOW_AURA_AWAIT_DBOX_INACTIVE = 28,
    PRE_SHOW_AURA_DBOX_INACTIVE = 29,
    PREP_SHOW_AURA = 30,
    SHOW_AURA = 31,
    SHOW_AURA_AWAIT_DBOX_INACTIVE = 32,
    SHOW_AURA_DBOX_INACTIVE = 33,
    THANK_YOU = 34,
    UNUSED_35 = 35,
    EXPLAIN_PERSONALITY = 36,
    HERO_SPECIES = 37,
    HERO_SPECIES_AWAIT_DBOX_INACTIVE = 38,
    HERO_SPECIES_DBOX_INACTIVE = 39,
    PRE_CHOOSE_PARTNER = 40,
    PRE_CHOOSE_PARTNER_AWAIT_DBOX_INACTIVE = 41,
    CHOOSE_PARTNER_PROMPT = 42,
    CHOOSE_PARTNER_PROMPT_AWAIT_DBOX_INACTIVE = 43,
    START_CHOOSE_PARTNER = 44,
    CHOOSE_PARTNER = 45,
    FINISH_CHOOSE_PARTNER = 46,
    CONFIRM_PARTNER_SPECIES_PROMPT = 47,
    PARTNER_NAME_START = 48,
    AWAIT_PARTNER_NAME_FINISH = 49,
    PARTNER_NAME_FINISH = 50,
    CONFIRM_PARTNER_NAME_RESULT = 51,
    CONFIRM_PARTNER_NAME_YES = 52,
    CONFIRM_PARTNER_NAME_NO = 53,
    READY_TO_GO = 54,
    FINISH_1 = 55,
    FINISH_2 = 56,
    FINISH_3 = 57,
    WAIT_NEXT_STEP = 58,
    CONFIRM_PARTNER_SPECIES_MENU = 59,
    CONFIRM_PARTNER_SPECIES_MENU_RESULT = 60,
    EARLY_RELEASE = 61,
    EARLY_RELEASE_MESSAGE = 62,
    EARLY_RELEASE_RETRY = 63,
    UNK_64 = 64,
    UNK_65 = 65,
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
