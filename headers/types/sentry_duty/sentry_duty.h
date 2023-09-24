// Types used primarily in the context of the Sentry Duty minigame (run by overlay 14)

#ifndef HEADERS_TYPES_SENTRY_DUTY_H_
#define HEADERS_TYPES_SENTRY_DUTY_H_

#include "../ground_mode/ground_mode.h"

typedef void (*state_handler_fn_t)(void);

enum sentry_completion_state {
    SENTRY_COMPLETION_IN_PROGRESS = 0,
    // Causes the main game loop to stop. Does some stuff, then transitions to FINALIZING.
    // This may also be used at the beginning of the minigame? Not sure
    SENTRY_COMPLETION_EXITING = 1,
    // Probably performing the last bit of cleanup? Then transitions to DONE
    SENTRY_COMPLETION_FINALIZING = 2,
    // The minigame has ended
    SENTRY_COMPLETION_DONE = 3,
};

enum sentry_control_state {
    // The default control state, which is to continue on in the current game state
    SENTRY_CTRL_CONTINUE = 0,
    // Triggers a game state transition with the necessary boilerplate, such as displaying
    // dialogue and setting up UI changes
    SENTRY_CTRL_STATE_TRANSITION = 1,
    // Always set directly after STATE_TRANSITION is handled. Seems to do some finalization
    // following a state change, before eventually transitioning back to CONTINUE
    SENTRY_CTRL_POST_TRANSITION = 2,
    // Set by a transition handler when another transition handler should be run immediately
    // after it, deferring the normal state transition boilerplate until after the second handler
    // has been run
    SENTRY_CTRL_INTERMEDIATE_TRANSITION = 3,
};

struct sentry_duty {
    uint8_t field_0x0;
    uint8_t field_0x1;
    uint8_t field_0x2;
    uint8_t field_0x3;
    uint8_t field_0x4;
    uint8_t field_0x5;
    uint8_t field_0x6;
    uint8_t field_0x7;
    uint8_t field_0x8;
    uint8_t field_0x9;
    uint8_t field_0xa;
    uint8_t field_0xb;
    int field_0xc;
    int field_0x10;
    int field_0x14;
    int field_0x18;
    undefined field_0x1c;
    undefined field_0x1d;
    undefined field_0x1e;
    undefined field_0x1f;
    undefined field_0x20;
    undefined field_0x21;
    undefined field_0x22;
    undefined field_0x23;
    undefined field_0x24;
    undefined field_0x25;
    undefined field_0x26;
    undefined field_0x27;
    undefined field_0x28;
    undefined field_0x29;
    undefined field_0x2a;
    undefined field_0x2b;
    undefined field_0x2c;
    undefined field_0x2d;
    undefined field_0x2e;
    undefined field_0x2f;
    undefined field_0x30;
    undefined field_0x31;
    undefined field_0x32;
    undefined field_0x33;
    undefined field_0x34;
    undefined field_0x35;
    undefined field_0x36;
    undefined field_0x37;
    undefined field_0x38;
    undefined field_0x39;
    undefined field_0x3a;
    undefined field_0x3b;
    undefined field_0x3c;
    undefined field_0x3d;
    undefined field_0x3e;
    undefined field_0x3f;
    undefined field_0x40;
    undefined field_0x41;
    undefined field_0x42;
    undefined field_0x43;
    undefined field_0x44;
    undefined field_0x45;
    undefined field_0x46;
    undefined field_0x47;
    undefined field_0x48;
    undefined field_0x49;
    undefined field_0x4a;
    undefined field_0x4b;
    undefined field_0x4c;
    undefined field_0x4d;
    undefined field_0x4e;
    undefined field_0x4f;
    undefined field_0x50;
    undefined field_0x51;
    undefined field_0x52;
    undefined field_0x53;
    undefined field_0x54;
    undefined field_0x55;
    undefined field_0x56;
    undefined field_0x57;
    undefined field_0x58;
    undefined field_0x59;
    undefined field_0x5a;
    undefined field_0x5b;
    undefined field_0x5c;
    undefined field_0x5d;
    undefined field_0x5e;
    undefined field_0x5f;
    undefined field_0x60;
    undefined field_0x61;
    undefined field_0x62;
    undefined field_0x63;
    undefined field_0x64;
    undefined field_0x65;
    undefined field_0x66;
    undefined field_0x67;
    undefined field_0x68;
    undefined field_0x69;
    undefined field_0x6a;
    undefined field_0x6b;
    undefined field_0x6c;
    undefined field_0x6d;
    undefined field_0x6e;
    undefined field_0x6f;
    undefined field_0x70;
    undefined field_0x71;
    undefined field_0x72;
    undefined field_0x73;
    undefined field_0x74;
    undefined field_0x75;
    undefined field_0x76;
    undefined field_0x77;
    undefined field_0x78;
    undefined field_0x79;
    undefined field_0x7a;
    undefined field_0x7b;
    undefined field_0x7c;
    undefined field_0x7d;
    undefined field_0x7e;
    undefined field_0x7f;
    undefined field_0x80;
    undefined field_0x81;
    undefined field_0x82;
    undefined field_0x83;
    undefined field_0x84;
    undefined field_0x85;
    undefined field_0x86;
    undefined field_0x87;
    undefined field_0x88;
    undefined field_0x89;
    undefined field_0x8a;
    undefined field_0x8b;
    undefined field_0x8c;
    undefined field_0x8d;
    undefined field_0x8e;
    undefined field_0x8f;
    undefined field_0x90;
    undefined field_0x91;
    undefined field_0x92;
    undefined field_0x93;
    undefined field_0x94;
    undefined field_0x95;
    undefined field_0x96;
    undefined field_0x97;
    undefined field_0x98;
    undefined field_0x99;
    undefined field_0x9a;
    undefined field_0x9b;
    undefined field_0x9c;
    undefined field_0x9d;
    undefined field_0x9e;
    undefined field_0x9f;
    undefined field_0xa0;
    undefined field_0xa1;
    undefined field_0xa2;
    undefined field_0xa3;
    // 0xA4: Args for PreprocessString when displaying dialogue
    struct preprocessor_args dialogue_args;
    undefined field_0xf4;
    undefined field_0xf5;
    undefined field_0xf6;
    undefined field_0xf7;
    undefined field_0xf8;
    undefined field_0xf9;
    undefined field_0xfa;
    undefined field_0xfb;
    undefined field_0xfc;
    undefined field_0xfd;
    undefined field_0xfe;
    undefined field_0xff;
    undefined field_0x100;
    undefined field_0x101;
    undefined field_0x102;
    undefined field_0x103;
    // 0x104: Outermost game state, controls "game completion" sequence
    enum sentry_completion_state completion_state;
    int game_state; // 0x108: The innermost game state that controls most state transition logic
    // 0x10C: A middle "control" state that controls some events that span multiple game state
    // transitions, such as displaying dialogue.
    enum sentry_control_state control_state;
    // 0x110: The next game state to transition to. Transition handlers set this variable rather
    // than the actual game state, and the main game loop handles the transition internals properly
    int next_game_state;
    int prev_dialogue_str_id; // 0x114: previous string ID displayed in the dialogue window
    int field_0x118;
    int field_0x11c;
    struct animation field_0x120;
    struct animation field_0x1e4;
    struct animation field_0x2a8;
    struct animation field_0x36c[16];
    struct animation field_0xfac[16];
    struct animation field_0x1bec[16];
    struct animation field_0x282c[2];
    struct animation field_0x29b4;
    struct animation field_0x2a78[4];
    struct animation field_0x2d88[4];
    struct animation field_0x3098[4];
    struct animation field_0x33a8;
    struct animation field_0x346c;
    uint8_t field_0x3530;
    undefined field_0x3531;
    undefined field_0x3532;
    undefined field_0x3533;
    int field_0x3534;
    int field_0x3538;
    int field_0x353c;
    int field_0x3540;
    int field_0x3544[4];
    int field_0x3554[4];
    int round_display; // 0x3564: current round being displayed on the top screen
    int field_0x3568;
    int field_0x356c;
    // Interestingly, it seems like the game passes these buffers into PreprocessString with an
    // output capacity of 0x400 rather than 0x100... possible overflow bug?
    // A field with length 0x400 wouldn't even fit into the struct.
    // Unclear if these field is actually 0x100 long, just a guess that seems to make sense based
    // on access patterns
    char field_0x3570[256];
    char field_0x3670[256];
    char field_0x3770[256];
    uint8_t field_0x3870;
    uint8_t field_0x3871;
    undefined field_0x3872;
    undefined field_0x3873;
    int field_0x3874;
    int field_0x3878;
    int right_answer_slot; // 0x387C: index (0-3) of the right answer in the selection menu
    // 0x3880: index into SENTRY_DUTY_MONSTER_IDS for info on the right answer
    int right_answer_data_idx;
    int round; // 0x3884: Current round (0-indexed)
    int field_0x3888;
    int points; // 0x388C: point count
    int field_0x3890;
    int field_0x3894;
    int field_0x3898;
    uint8_t field_0x389c;
    uint8_t field_0x389d;
    uint8_t field_0x389e;
    uint8_t field_0x389f;
    uint8_t field_0x38a0;
    undefined field_0x38a1;
    // 0x38A2: species for each of the options in the selection menu
    struct monster_id_16 choices[4];
    undefined field_0x38aa;
    undefined field_0x38ab;
    int field_0x38ac;
    int field_0x38b0;
    int field_0x38b4;
    // 0x38B8: data indexes (see right_answer_data_idx) for previous rounds
    int prev_right_answer_data_idxs[6];
    int16_t hero_str_id;    // 0x38D0: string ID for the hero (should be 0)
    int16_t partner_str_id; // 0x38D2: string ID for the partner (should be 1)
};
ASSERT_SIZE(struct sentry_duty, 14548);

#endif
