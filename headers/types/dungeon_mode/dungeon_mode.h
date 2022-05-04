// Types used primarily in the context of dungeon mode

#ifndef HEADERS_TYPES_DUNGEON_MODE_H_
#define HEADERS_TYPES_DUNGEON_MODE_H_

#include "enums.h"

// Used in various contexts, like with entity positions in the dungeon
struct position {
    int16_t x;
    int16_t y;
};
ASSERT_SIZE(struct position, 4);

// Item info
struct item {
    // 0x0: flags: 1-byte bitfield
    bool f_exists : 1;  // Validity flag
    bool f_in_shop : 1; // In a Kecleon Shop
    uint8_t flags_unk2 : 1;
    bool f_sticky : 1; // Sticky
    bool f_set : 1;    // Usable by L+R
    uint8_t flags_unk5 : 3;

    // 0x1: For bag items. 0 for none, 1 if held by the leader, 2 for the second party member, etc.
    uint8_t held_by;
    // 0x2: Only for stackable items. Will be 0 if unapplicable. For Poké, this is an "amount code"
    // rather than the literal amount
    uint16_t quantity;
    struct item_id_16 id; // 0x4
};
ASSERT_SIZE(struct item, 6);

// Trap info
struct trap {
    struct trap_id_8 id;
    // If 0 or 2, the trap will activate only when a team member steps on it. If 1, the trap will
    // activate only when an enemy steps on it. Naturally, this seems to be 0 for traps and 2 for
    // Wonder Tiles
    uint8_t team;
    uint8_t flags; // First bit determines if the trap can be broken? (needs testing)
    undefined field_0x3;
};
ASSERT_SIZE(struct trap, 4);

// Monster move info
struct move {
    // 0x0: flags0: 1-byte bitfield
    bool f_exists : 1;                   // This move will show up in the move list
    bool f_subsequent_in_link_chain : 1; // This move is in a link chain, but not the first move
    bool f_enabled_for_ai : 1;           // AI can use this move
    bool f_set : 1;                      // Usable by L+A
    bool f_last_used : 1; // This move was the last one used (for things like Torment?)
    bool f_disabled : 1;  // For Torment (and maybe others)
    uint8_t flags0_unk6 : 2;

    undefined field_0x1;

    // 0x2: flags2: 2-byte bitfield
    bool f_sealed : 1; // Sealed by a Seal Trap. Also prevents AI from using this move
    uint16_t flags2_unk1 : 2;
    bool f_consume_pp : 1;   // This move will consume PP this turn
    bool f_consume_2_pp : 1; // Consume 2 PP this turn. No effect unless f_consume_pp is set
    uint16_t flags2_unk5 : 1;
    // Consume 4 PP this turn. No effect unless f_consume_pp is set, overrides f_consume_2_pp
    bool f_consume_4_pp : 1;
    uint16_t flags2_unk7 : 1;
    uint16_t flags2_unk8 : 1;
    bool f_exclusive_item_pp_boost : 1; // A PP-boosting exclusive item is in effect
    uint16_t flags3_unk10 : 6;

    struct move_id_16 id; // 0x4
    uint8_t pp;           // 0x6: Current PP
    uint8_t ginseng;      // 0x7: Ginseng boost
};
ASSERT_SIZE(struct move, 8);

// Monster stat modifier info
struct monster_stat_modifiers {
    // Stages go from 0-20 inclusive, with normal being 10
    int16_t offensive_stages[2];  // 0x0: {atk, sp_atk}
    int16_t defensive_stages[2];  // 0x4: {def, sp_def}
    int16_t hit_chance_stages[2]; // 0x8: {accuracy, evasion}
    bool flash_fire_boost;        // 0xC
    undefined field_0xd;
    undefined field_0xe;
    undefined field_0xf;
    // Some moves like Screech affect the damage calculation differently than, e.g., Leer
    // 0x10: binary fixed-point (8 fraction bits), {atk, sp_atk}; from Charm, Memento, etc.
    int offensive_multipliers[2];
    // 0x18: binary fixed-point (8 fraction bits), {def, sp_def}; from Screech, etc.
    int defensive_multipliers[2];
};
ASSERT_SIZE(struct monster_stat_modifiers, 32);

// Monster statuses
// Many fields are indexes that select from a group of status conditions. These fields are named
// by the FIRST status in the list (when the index is 1, since 0 usually means no status). For other
// statuses in the group, see the subsequent enum values in enum status_id after the first status.
#pragma pack(push, 1)
struct statuses {
    bool roost;
    uint8_t field_0x1; // Set by Roost to 0x2
    uint8_t field_0x2; // Set by Roost to 0xA
    undefined field_0x3;
    undefined field_0x4;
    undefined field_0x5;
    undefined field_0x6;
    undefined field_0x7;
    undefined field_0x8;
    undefined field_0x9;
    undefined field_0xa;
    undefined field_0xb;
    undefined field_0xc;
    undefined field_0xd;
    undefined field_0xe;
    undefined field_0xf;
    undefined field_0x10;
    undefined field_0x11;
    undefined field_0x12;
    struct monster_behavior_8 monster_behavior; // 0x13
    uint8_t sleep;                              // 0x14: STATUS_SLEEP if 1
    uint8_t sleep_turns; // 0x15: Turns left for the status in statuses::sleep
    uint8_t burn;        // 0x16: STATUS_BURN if 1
    uint8_t burn_turns;  // 0x17: Turns left for the status in statuses::burn
    // 0x18: Turns left until residual damage for the status in statuses::burn, if applicable
    uint8_t burn_damage_countdown;
    undefined field_0x19;
    undefined field_0x1a;
    uint8_t freeze; // 0x1B: STATUS_FROZEN if 1
    undefined field_0x1c;
    undefined field_0x1d;
    undefined field_0x1e;
    undefined field_0x1f;
    undefined field_0x20;
    undefined field_0x21;
    undefined field_0x22;
    uint8_t freeze_turns; // 0x23: Turns left for the status in statuses::freeze
    // 0x24: Turns left until residual damage for the status in statuses::freeze, if applicable
    uint8_t freeze_damage_countdown;
    undefined field_0x25;
    undefined field_0x26;
    uint8_t cringe;         // 0x27: STATUS_CRINGE if 1
    uint8_t cringe_turns;   // 0x28: Turns left for the status in statuses::cringe
    uint8_t bide;           // 0x29: STATUS_BIDE if 1
    uint8_t bide_turns;     // 0x2A: Turns left for the status in statuses::bide
    uint8_t bide_move_slot; // 0x2B: Slot in the user's move list
    uint8_t reflect;        // 0x2C: STATUS_REFLECT if 1
    uint8_t reflect_turns;  // 0x2D: Turns left for the status in statuses::reflect
    // 0x2E: Turns left until residual damage for the status in statuses::reflect, if applicable
    uint8_t reflect_damage_countdown;
    uint8_t curse; // 0x2F: STATUS_CURSED if 1
    undefined field_0x30;
    undefined field_0x31;
    uint8_t curse_turns; // 0x32: Turns left for the status in statuses::curse
    // 0x33: Turns left until residual damage for the status in statuses::curse, if applicable
    uint8_t curse_damage_countdown;
    undefined field_0x34;
    undefined field_0x35;
    undefined field_0x36;
    uint8_t leech_seed; // 0x37: STATUS_LEECH_SEED if 1
    undefined field_0x38;
    undefined field_0x39;
    undefined field_0x3a;
    undefined field_0x3b;
    undefined field_0x3c;
    undefined field_0x3d;
    undefined field_0x3e;
    undefined field_0x3f;
    uint8_t leech_seed_turns; // 0x40: Turns left for the status in statuses::leech_seed
    // 0x41: Turns left until residual damage for the status in statuses::leech_seed, if applicable.
    // Behaves weirdly without an afflictor
    uint8_t leech_seed_damage_countdown;
    undefined field_0x42;
    uint8_t sure_shot;         // 0x43: STATUS_SURE_SHOT if 1
    uint8_t sure_shot_turns;   // 0x44: Turns left for the status in statuses::sure_shot
    uint8_t long_toss;         // 0x45: STATUS_LONG_TOSS if 1
    uint8_t invisible;         // 0x46: STATUS_INVISIBLE if 1
    uint8_t invisible_turns;   // 0x47: Turns left for the status in statuses::invisible
    uint8_t blinded;           // 0x48: STATUS_BLINKER if 1
    uint8_t blinded_turns;     // 0x49: Turns left for the status in statuses::blinded
    uint8_t muzzled;           // 0x4A: STATUS_MUZZLED if 1
    uint8_t muzzled_turns;     // 0x4B: Turns left for the status in statuses::muzzled
    uint8_t miracle_eye;       // 0x4C: STATUS_MIRACLE_EYE if 1
    uint8_t miracle_eye_turns; // 0x4D: Turns left for the status in statuses::miracle_eye
    uint8_t magnet_rise;       // 0x4E: STATUS_MAGNET_RISE if 1
    uint8_t magnet_rise_turns; // 0x4F: Turns left for the status in statuses::magnet_rise
    bool power_ears;           // 0x50: STATUS_POWER_EARS
    bool scanning;             // 0x51: STATUS_SCANNING
    bool stair_spotter;        // 0x52: STATUS_STAIR_SPOTTER
    undefined field_0x53;
    bool grudge;  // 0x54: STATUS_GRUDGE
    bool exposed; // 0x55: STATUS_EXPOSED (Foresight/Odor Sleuth)
    undefined field_0x56;
    bool boss_flag; // 0x57: Seems to be true for boss monsters
    undefined field_0x58;
    undefined field_0x59;
    bool in_action;            // 0x5A: Possibly a flag while in action
    bool terrified;            // 0x5B: STATUS_TERRIFIED
    uint8_t terrified_turns;   // 0x5C: Turns left for the terrified status
    uint8_t perish_song_turns; // 0x5D: Turns left before Perish Song takes effect
    undefined field_0x5e;
    undefined field_0x5f;
    undefined field_0x60;
    undefined field_0x61;
    bool high_up; // 0x62: Graphical flag for Fly/Bounce
    undefined field_0x63;
    undefined field_0x64;
    undefined field_0x65;
    undefined field_0x66;
    // 0x67: 1 means normal. 0 means half speed. 2, 3, and 4 mean 2x, 3x, and 4x speed.
    int speed_stage;
    // Each counter ticks down to 0 turn by turn. The current speed_stage is calculated as:
    // max(min({# nonzero speed_up_counters} - {# nonzero speed_down_counters}, 0), 4)
    uint8_t speed_up_counters[5];   // 0x6B
    uint8_t speed_down_counters[5]; // 0x70
    uint8_t stockpile_stage;        // 0x75: Goes from 0-3. STATUS_STOCKPILING if nonzero
};
ASSERT_SIZE(struct statuses, 118);
#pragma pack(pop)

// Monster info
struct monster {
    // 0x0: flags: 2-byte bitfield
    uint16_t flags_unk0 : 5;
    bool f_swapping_places : 1; // Swapping places with another monster
    uint16_t flags_unk6 : 3;
    bool f_walking : 1; // Walking (but not dashing)
    uint16_t flags_unk10 : 5;
    bool f_swapping_places_petrified_ally : 1; // Swapping places with a petrified ally

    struct monster_id_16 id;          // 0x2:
    struct monster_id_16 apparent_id; // 0x4: What's outwardly displayed if Transformed
    bool is_not_team_member; // 0x6: true for enemies and allied NPCs that aren't on the team
    bool is_team_leader;     // 0x7
    // 0x8: An ally is an NPC that isn't a normal team member, e.g. for story boss battles
    bool is_ally;
    struct shopkeeper_mode_8 shopkeeper; // 0x9
    uint8_t level;                       // 0xA
    undefined field_0xb;
    int16_t team_index;  // 0xC: In order by team lineup
    int16_t iq;          // 0xE
    int16_t hp;          // 0x10: Current HP
    int16_t max_hp_stat; // 0x12: Add to max_hp_boost for the actual max HP
    undefined field_0x14;
    undefined field_0x15;
    int16_t max_hp_boost; // 0x16: From Life Seeds, Sitrus Berries, etc.
    undefined field_0x18;
    undefined field_0x19;
    uint8_t atk;    // 0x1A
    uint8_t sp_atk; // 0x1B
    uint8_t def;    // 0x1C
    uint8_t sp_def; // 0x1D
    undefined field_0x1e;
    undefined field_0x1f;
    int exp;                                      // 0x20: Total Exp. Points
    struct monster_stat_modifiers stat_modifiers; // 0x24
    int16_t hidden_power_base_power;              // 0x44
    struct type_id_8 hidden_power_type;           // 0x46
    undefined field_0x47;
    struct dungeon_id_8 joined_at; // 0x48: Also used as a unique identifier for special monsters
    undefined field_0x49;
    uint16_t action_id;              // 0x4A: Changes as you do things
    struct direction_id_8 direction; // 0x4C: Current direction the monster is facing
    undefined field_0x4d;
    // 0x4E: Metadata for some action_id values.
    // E.g., this is the bag item index when using an item
    uint16_t action_use_idx;
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
    struct type_id_8 type1;       // 0x5E
    struct type_id_8 type2;       // 0x5F
    struct ability_id_8 ability1; // 0x60
    struct ability_id_8 ability2; // 0x61
    struct item held_item;        // 0x62
    uint16_t held_item_id;        // 0x68: Appears to be a mirror of held_item.id
    // Previous position data is used by the AI
    struct position prev_pos;  // 0x6A: Position 1 turn ago
    struct position prev_pos2; // 0x6E: Position 2 turns ago
    struct position prev_pos3; // 0x72: Position 3 turns ago
    struct position prev_pos4; // 0x76: Position 4 turns ago
    undefined field_0x7a;
    undefined field_0x7b;
    struct ai_objective_8 ai_objective; // 0x7C
    bool ai_not_next_to_target;         // 0x7D: This NPC monster is not next to its current target
    bool ai_targeting_enemy;            // 0x7E: This NPC monster is targeting an enemy monster
    bool ai_turning_around;             // 0x7F: This NPC monster has decided to turn around
    // 0x80: entity::spawn_genid of the entity currently being targeted
    uint16_t ai_target_spawn_genid;
    undefined field_0x82;
    undefined field_0x83;
    struct entity* ai_target; // 0x84: Current or most recent AI target
    undefined field_0x88;
    undefined field_0x89;
    undefined field_0x8a;
    undefined field_0x8b;
    struct position ai_target_pos; // 0x8C: Position of the entity currently being targeted
    // 0x90: Work array while updating skills in the menu. Same meaning as iq_skill_flags.
    uint32_t iq_skill_menu_flags[3];
    // 0x9C: First 9 bytes contain bitfield data; the rest is presumably padding.
    // Bitvector. See enum iq_skill_id for the meaning of each bit.
    uint32_t iq_skill_flags[3];
    struct tactic_id_8 tactic; // 0xA8
    struct statuses statuses;  // 0xA9
    undefined field_0x11f;
    undefined field_0x120;
    undefined field_0x121;
    undefined field_0x122;
    undefined field_0x123;
    struct move moves[4]; // 0x124
    uint8_t move_flags;   // 0x144: 1-byte bitfield
    undefined field_0x145;
    int16_t belly;                 // 0x146: Integer part
    int16_t belly_thousandths;     // 0x148
    int16_t max_belly;             // 0x14A: Integer part
    int16_t max_belly_thousandths; // 0x14C
    undefined field_0x14e;
    bool ai_next_to_target; // 0x14F: This NPC monster is next to its current target
    undefined field_0x150;
    undefined field_0x151;
    // 0x152: Seems to be true if the monster has already acted this turn: attacked, used an item,
    // or seemingly anything other than moving/resting. Also true when the monster faints.
    bool cannot_act;
    undefined field_0x153;
    undefined field_0x154;
    undefined field_0x155;
    undefined field_0x156;
    undefined field_0x157;
    undefined field_0x158;
    undefined field_0x159;
    undefined field_0x15a;
    undefined field_0x15b;
    undefined field_0x15c;
    undefined field_0x15d;
    undefined field_0x15e;
    undefined field_0x15f;
    undefined field_0x160;
    undefined field_0x161;
    undefined field_0x162;
    undefined field_0x163;
    undefined field_0x164;
    undefined field_0x165;
    undefined field_0x166;
    undefined field_0x167;
    undefined field_0x168;
    undefined field_0x169;
    undefined field_0x16a;
    undefined field_0x16b;
    undefined field_0x16c;
    undefined field_0x16d;
    undefined field_0x16e;
    undefined field_0x16f;
    undefined field_0x170;
    undefined field_0x171;
    undefined field_0x172;
    undefined field_0x173;
    undefined field_0x174;
    undefined field_0x175;
    undefined field_0x176;
    undefined field_0x177;
    undefined field_0x178;
    undefined field_0x179;
    undefined field_0x17a;
    undefined field_0x17b;
    undefined field_0x17c;
    undefined field_0x17d;
    struct position target_pixel_pos; // 0x17E: The AI's target's graphical position on screen?
    struct position pixel_pos;        // 0x182: The monster's graphical position on screen?
    undefined field_0x186;
    undefined field_0x187;
    undefined field_0x188;
    undefined field_0x189;
    undefined field_0x18a;
    undefined field_0x18b;
    undefined field_0x18c;
    undefined field_0x18d;
    undefined field_0x18e;
    undefined field_0x18f;
    undefined field_0x190;
    undefined field_0x191;
    undefined field_0x192;
    undefined field_0x193;
    undefined field_0x194;
    undefined field_0x195;
    undefined field_0x196;
    undefined field_0x197;
    undefined field_0x198;
    undefined field_0x199;
    undefined field_0x19a;
    undefined field_0x19b;
    struct position pos; // 0x19C: Mirror of the position on the entity struct
    undefined field_0x1a0;
    undefined field_0x1a1;
    undefined field_0x1a2;
    undefined field_0x1a3;
    undefined field_0x1a4;
    undefined field_0x1a5;
    undefined field_0x1a6;
    undefined field_0x1a7;
    undefined field_0x1a8;
    undefined field_0x1a9;
    undefined field_0x1aa;
    undefined field_0x1ab;
    undefined field_0x1ac;
    undefined field_0x1ad;
    undefined field_0x1ae;
    undefined field_0x1af;
    undefined field_0x1b0;
    undefined field_0x1b1;
    undefined field_0x1b2;
    undefined field_0x1b3;
    uint16_t walk_anim_frames_left; // 0x1B4: Number of frames left in walking animation?
    undefined field_0x1b6;
    undefined field_0x1b7;
    undefined field_0x1b8;
    undefined field_0x1b9;
    undefined field_0x1ba;
    undefined field_0x1bb;
    undefined field_0x1bc;
    undefined field_0x1bd;
    undefined field_0x1be;
    undefined field_0x1bf;
    undefined field_0x1c0;
    undefined field_0x1c1;
    undefined field_0x1c2;
    undefined field_0x1c3;
    undefined field_0x1c4;
    undefined field_0x1c5;
    undefined field_0x1c6;
    undefined field_0x1c7;
    undefined field_0x1c8;
    undefined field_0x1c9;
    undefined field_0x1ca;
    undefined field_0x1cb;
    undefined field_0x1cc;
    undefined field_0x1cd;
    undefined field_0x1ce;
    undefined field_0x1cf;
    undefined field_0x1d0;
    undefined field_0x1d1;
    undefined field_0x1d2;
    undefined field_0x1d3;
    undefined field_0x1d4;
    undefined field_0x1d5;
    undefined field_0x1d6;
    undefined field_0x1d7;
    undefined field_0x1d8;
    undefined field_0x1d9;
    undefined field_0x1da;
    undefined field_0x1db;
    undefined field_0x1dc;
    undefined field_0x1dd;
    undefined field_0x1de;
    undefined field_0x1df;
    undefined field_0x1e0;
    undefined field_0x1e1;
    undefined field_0x1e2;
    undefined field_0x1e3;
    undefined field_0x1e4;
    undefined field_0x1e5;
    undefined field_0x1e6;
    undefined field_0x1e7;
    undefined field_0x1e8;
    undefined field_0x1e9;
    undefined field_0x1ea;
    undefined field_0x1eb;
    undefined field_0x1ec;
    undefined field_0x1ed;
    undefined field_0x1ee;
    undefined field_0x1ef;
    undefined field_0x1f0;
    undefined field_0x1f1;
    undefined field_0x1f2;
    undefined field_0x1f3;
    undefined field_0x1f4;
    undefined field_0x1f5;
    undefined field_0x1f6;
    undefined field_0x1f7;
    undefined field_0x1f8;
    undefined field_0x1f9;
    undefined field_0x1fa;
    undefined field_0x1fb;
    undefined field_0x1fc;
    undefined field_0x1fd;
    undefined field_0x1fe;
    undefined field_0x1ff;
    undefined field_0x200;
    undefined field_0x201;
    undefined field_0x202;
    undefined field_0x203;
    undefined field_0x204;
    undefined field_0x205;
    undefined field_0x206;
    undefined field_0x207;
    undefined field_0x208;
    undefined field_0x209;
    undefined field_0x20a;
    undefined field_0x20b;
    undefined field_0x20c;
    undefined field_0x20d;
    undefined field_0x20e;
    undefined field_0x20f;
    uint8_t hp_fractional; // 0x210: 200 * fractional_part(HP)
    undefined field_0x211;
    undefined field_0x212;
    undefined field_0x213;
    undefined field_0x214;
    undefined field_0x215;
    undefined field_0x216;
    undefined field_0x217;
    undefined field_0x218;
    undefined field_0x219;
    undefined field_0x21a;
    undefined field_0x21b;
    undefined field_0x21c;
    undefined field_0x21d;
    undefined field_0x21e;
    undefined field_0x21f;
    undefined field_0x220;
    undefined field_0x221;
    undefined field_0x222;
    undefined field_0x223;
    // Stat boosts from exclusive items with EXCLUSIVE_EFF_STAT_BOOST
    uint8_t exclusive_item_atk_boost;    // 0x224
    uint8_t exclusive_item_sp_atk_boost; // 0x225
    uint8_t exclusive_item_def_boost;    // 0x226
    uint8_t exclusive_item_sp_def_boost; // 0x227
    // 0x228: Bitvector. See enum exclusive_item_effect_id for the meaning of each bit
    uint32_t exclusive_item_effect_flags[5];
    undefined field_0x23c;
    undefined field_0x23d;
    undefined field_0x23e;
    undefined field_0x23f;
};
ASSERT_SIZE(struct monster, 576);

// Generic entity data
struct entity {
    enum entity_type type;      // 0x0
    struct position pos;        // 0x4
    struct position prev_pos;   // 0x8
    int pixel_x_shifted;        // 0xC: pixel_x << 8
    int pixel_y_shifted;        // 0x10: pixel_y << 8
    int pixel_x_shifted_mirror; // 0x14: Monsters only?
    int pixel_y_shifted_mirror; // 0x18: Monsters only?
    // 0x1C: Graphical parameter for evelation above the ground. Last byte behaves weirdly.
    int elevation;
    bool is_visible; // 0x20: For traps/hidden stairs
    undefined field_0x21;
    undefined field_0x22;
    // 0x23: Seems to be the animation frame counter for the 10-frame "shuffle" animation that
    // plays at the end of a walk sequence
    uint8_t end_walk_anim_frame;
    undefined field_0x24;
    uint8_t room_idx; // 0x25: Index of the room a monster is in. 0xFF for hall
    // 0x26:Unique index for each monster that spawns. Starts at 0xA for the leader, and each
    // subsequent monster to spawn is assigned the next number (0xB, 0xC, ...)
    uint16_t spawn_genid;
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
    uint8_t idle_anim_frame; // 0x38: animation frame counter for the idle animation?
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
    struct position pixel_offset;  // 0x4C: displacement from a monster's normal position
    struct position shadow_offset; // 0x50: displacement of monster shadow from its normal position
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
    uint16_t sprite_id;        // 0x66: Maybe?
    uint16_t sprite_id_mirror; // 0x68
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
    void* field_0x74;
    void* field_0x78;
    void* field_0x7c;
    void* field_0x80;
    void* field_0x84;
    void* field_0x88;
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
    struct direction_id_8 graphical_direction_mirror1; // 0xA4
    undefined field_0xa5;
    undefined field_0xa6;
    undefined field_0xa7;
    undefined field_0xa8;
    undefined field_0xa9;
    undefined field_0xaa;
    undefined field_0xab;
    undefined field_0xac;
    undefined field_0xad;
    uint8_t anim_id;                           // 0xAE: Maybe?
    uint8_t anim_id_mirror;                    // 0xAF
    struct direction_id_8 graphical_direction; // 0xB0: Direction a monster's sprite is facing
    struct direction_id_8 graphical_direction_mirror0; // 0xB1
    undefined field_0xb2;
    undefined field_0xb3;
    void* info; // 0xB4: Points to info struct for monster/item/trap
};
ASSERT_SIZE(struct entity, 184);

// Dungeon entity table header
struct entity_table_hdr {
    // 0x0: A list of all monster pointers, whether they're used or not
    struct entity* monster_slot_ptrs[20];
    // 0x50: Null-terminated array of pointers to actually active monsters
    struct entity* active_monster_ptrs[20];
    struct entity* item_ptrs[64];     // 0xA0
    struct entity* trap_ptrs[64];     // 0x1A0
    struct entity* hidden_stairs_ptr; // 0x2A0
};
ASSERT_SIZE(struct entity_table_hdr, 676);

// A table of all entities in a dungeon.
// The header contains pointers that point into the subsequent entity array.
struct entity_table {
    struct entity_table_hdr header; // 0x0
    struct entity entities[149];    // 0x2A4
};
ASSERT_SIZE(struct entity_table, 28092);

// Used during floor generation to keep track of what entities should be spawned where
struct spawn_flags {
    bool f_stairs : 1;
    bool f_item : 1;
    bool f_trap : 1;
    bool f_monster : 1;
    uint16_t spawn_flags_unk4 : 12;
};
ASSERT_SIZE(struct spawn_flags, 2);

// Used during dungeon play to record the visibility of a tile.
struct visibility_flags {
    // If f_revealed == true and f_visited == false, the tile will appear as gray on the map.
    // This happens, e.g., when a Luminous Orb is used.
    bool f_revealed : 1; // Revealed on the map.
    bool f_visited : 1;  // Visited by the player
    uint16_t visibility_flags_unk2 : 14;
};
ASSERT_SIZE(struct visibility_flags, 2);

// These flags seem to occupy the same memory location, so the meaning is context-dependent.
union spawn_or_visibility_flags {
    struct spawn_flags spawn;
    struct visibility_flags visibility;
};
ASSERT_SIZE(union spawn_or_visibility_flags, 2);

// Tile data
struct tile {
    // 0x0: terrain_flags: 2-byte bitfield
    enum terrain_type terrain_type : 2;
    // This tile can be corner-cut when walking. Seemingly only used during dungeon generation.
    bool f_corner_cuttable : 1;
    // Includes room tiles right next to a hallway, and branching points within corridors.
    // Only applies to natural halls, not ones made by Absolute Mover, not "hallways" made of
    // secondary terrain, etc. Used by the AI for navigation.
    bool f_natural_junction : 1;
    // This tile is impassable, even with Absolute Mover/Mobile Scarf. Used for the map border,
    // key chamber walls, walls in boss battle rooms, etc.
    bool f_impassable_wall : 1;
    bool f_in_kecleon_shop : 1;  // In a Kecleon Shop
    bool f_in_monster_house : 1; // In a Monster House
    uint16_t terrain_flags_unk7 : 1;
    // Cannot be broken by Absolute Mover. Set naturally on key doors.
    bool f_unbreakable : 1;
    // Tile is any type of "stairs" (normal stairs, Hidden Stairs, Warp Zone)
    bool f_stairs : 1;
    uint16_t terrain_flags_unk10 : 1;
    bool f_key_door : 1;            // Tile is a key door
    bool f_key_door_key_locked : 1; // Key door is locked and requires a Key to open
    // Key door is locked and requires an escort to open (for Sealed Chamber missions)
    bool f_key_door_escort_locked : 1;
    uint16_t terrain_flags_unk14 : 1;
    // Tile is open terrain but unreachable from the stairs spawn point. Only set during dungeon
    // generation.
    bool f_unreachable_from_stairs : 1;

    // 0x2: Seems to be used for spawning entities during dungeon generation, and for visibility
    // during dungeon play
    union spawn_or_visibility_flags spawn_or_visibility_flags;
    uint16_t texture_id; // 0x4: Maybe? Changing this causes the tile texture to change
    undefined field_0x6;
    uint8_t room; // 0x7: Room index. 0xFF if not in a room.
    // 0x8: Where a monster standing on this tile is allowed to move.
    // Each element is a bitflag that corresponds to one of the first four values of
    // enum mobility_type. Each bit in the bitflag corresponds to the values of enum direction,
    // where 1 means a monster with that mobility type is allowed to walk in that direction.
    uint8_t walkable_neighbor_flags[4];
    struct entity* monster; // 0xC: Pointer to a monster on this tile, if any
    // 0x10: Pointer to an entity other than a monster on this tile (item/trap)
    struct entity* object;
};
ASSERT_SIZE(struct tile, 20);

// Data related to dungeon generation
struct dungeon_generation_info {
    // 0x0: Set if the floor layout is guaranteed to be a Monster House, or the dungeon generation
    // algorithm fails
    bool force_create_monster_house;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    undefined field_0x4;
    // 0x5: Room index of Monster House on the floor. 0xFF if there's no Monster House
    uint8_t monster_house_room;
    undefined field_0x6;
    undefined field_0x7;
    enum hidden_stairs_type hidden_stairs_type; // 0x8
    undefined field_0xc;
    undefined field_0xd;
    undefined field_0xe;
    undefined field_0xf;
    uint8_t tileset_id; // 0xA
    undefined field_0x11;
    // 0x12: Music table index (see the same field in struct floor_properties)
    uint16_t music_table_idx;
    undefined field_0x14;
    undefined field_0x15;
    struct fixed_room_id_8 fixed_room_id; // 0x16
    undefined field_0x17;
    undefined field_0x18;
    undefined field_0x19;
    uint16_t floor_generation_attempts; // 0x1A: Number of attempts at floor layout generation
    struct tile tiles[32][56];          // 0x1C
    struct position player_spawn_pos;   // 0x8C1C: Position of the player spawn
    struct position stairs_pos;         // 0x8C20: Position of the stairs spawn
    // 0x8C24: Position of the Hidden Stairs spawn, or (-1, -1) if no Hidden Stairs
    struct position hidden_stairs_pos;
};
ASSERT_SIZE(struct dungeon_generation_info, 35880);

// Dungeon grid cell data during dungeon generation.
// A grid cell is a rectangular sector of the full 56x32 tile floor.
// Each grid cell becomes a room or a hallway anchor after dungeon generation.
struct dungeon_grid_cell {
    int16_t start_x; // 0x0: inclusive
    int16_t start_y; // 0x2: inclusive
    int16_t end_x;   // 0x4: exclusive
    int16_t end_y;   // 0x6: exclusive
    // 0x8: Grid cells can be invalid and not used in dungeon generation, depending on the
    // generation parameters for a particular floor/dungeon
    bool is_invalid;
    undefined field_0x9;
    bool is_room;      // 0xA: Whether this cell contains a room
    bool is_connected; // 0xB: Whether this cell is connected to a neighboring grid cell
    undefined field_0xc;
    undefined field_0xd;
    bool is_monster_house; // 0xE: Whether this cell has a Monster House
    undefined field_0xf;
    bool is_maze_room; // 0x10: Whether this cell has a maze room
    // 0x11: Room has been merged into another room and is no longer valid
    bool was_merged_into_other_room;
    bool is_merged_room; // 0x12: Room had another room merged into it
    // Connection flags set during connection assignment
    bool is_connected_to_top;    // 0x13
    bool is_connected_to_bottom; // 0x14
    bool is_connected_to_left;   // 0x15
    bool is_connected_to_right;  // 0x16
    // Work variables used while connections are being created
    bool should_connect_to_top;    // 0x17
    bool should_connect_to_bottom; // 0x18
    bool should_connect_to_left;   // 0x19
    bool should_connect_to_right;  // 0x1A
    undefined field_0x1b;
    // 0x1C: This room has been flagged to have imperfections generated
    bool flag_imperfect;
    // 0x1D: This room has been flagged to have secondary structures generated
    bool flag_secondary_structure;
};
ASSERT_SIZE(struct dungeon_grid_cell, 30);

// Floor generation status data, populated during dungeon generation
struct floor_generation_status {
    bool second_spawn;                    // 0x0
    bool has_monster_house;               // 0x1: This floor has a Monster House
    uint8_t stairs_room;                  // 0x2: The index of the room containing the stairs
    bool has_kecleon_shop;                // 0x3: This floor has a Kecleon Shop
    bool has_chasms_as_secondary_terrain; // 0x4: Secondary terrain type is SECONDARY_TERRAIN_CHASM
    bool is_invalid;                      // 0x5: Set when floor generation fails
    struct floor_size_8 floor_size;       // 0x6
    bool has_maze;                        // 0x7: This floor has a maze room
    bool no_enemy_spawns;                 // 0x8: No enemies should spawn on this floor
    undefined field_0x9;
    undefined field_0xa;
    undefined field_0xb;
    int16_t kecleon_shop_spawn_chance; // 0xC: Percentage chance from 0-100
    undefined field_0xe;
    undefined field_0xf;
    int16_t monster_house_spawn_chance; // 0x10: Percentage chance from 0-100
    undefined field_0x12;
    undefined field_0x13;
    int n_rooms; // 0x14: Number of rooms this floor should have
    // 0x18: Maximum number of additional secondary structures to generate within rooms.
    // This is decremented as secondary structures are generated until it falls to 0.
    int secondary_structures_budget;
    // Where the Hidden Stairs spawn is
    uint16_t hidden_stairs_spawn_x; // 0x1C
    uint16_t hidden_stairs_spawn_y; // 0x1E
    // The middle of the Kecleon Shop is, if applicable
    uint16_t kecleon_shop_middle_x; // 0x20
    uint16_t kecleon_shop_middle_y; // 0x22
    // 0x24: The number of tiles that can be reached from the stairs, assuming normal mobility
    int n_tiles_reachable_from_stairs;
    enum floor_layout layout;                   // 0x28
    enum hidden_stairs_type hidden_stairs_type; // 0x2C
    // The limits of the Kecleon Shop, if applicable
    int kecleon_shop_min_x; // 0x30
    int kecleon_shop_min_y; // 0x34
    int kecleon_shop_max_x; // 0x38
    int kecleon_shop_max_y; // 0x3C
};
ASSERT_SIZE(struct floor_generation_status, 64);

// When generating spawns, the game builds up arrays of these structs for valid possibilities,
// then selects a subset of them at random.
struct spawn_position {
    uint8_t x;
    uint8_t y;
};
ASSERT_SIZE(struct spawn_position, 2);

// Dungeon floor properties
struct floor_properties {
    struct floor_layout_8 layout; // 0x0
    int8_t n_rooms;               // 0x1: Number of rooms to be generated
    uint8_t tileset;              // 0x2
    // 0x3: Indexes into the music ID table in overlay 10 to determine the floor's music track.
    // See the relevant descriptions in the overlay 10 symbols for more information.
    uint8_t music_table_idx;
    struct weather_id_8 weather; // 0x4
    // 0x5: Controls how many connections will be made between grid cells
    uint8_t floor_connectivity;
    uint8_t enemy_density;              // 0x6: Controls how many enemies will be spawned
    uint8_t kecleon_shop_spawn_chance;  // 0x7: Percentage chance from 0-100
    uint8_t monster_house_spawn_chance; // 0x8: Percentage chance from 0-100
    uint8_t maze_room_chance;           // 0x9: Percentage chance from 0-100
    uint8_t sticky_item_chance;         // 0xA
    // 0xB: Whether or not dead ends are allowed in the floor layout. If false, dead ends will be
    // corrected during floor generation (or rather, they should be, but the implementation is
    // buggy)
    bool allow_dead_ends;
    // 0xC: Maximum number of secondary structures that can be generated on the floor
    uint8_t max_secondary_structures;
    // 0xD: room_flags: 1-byte bitfield
    bool f_secondary_structures : 1; // Whether secondary structures are allowed
    uint8_t room_flags_unk1 : 1;
    bool f_room_imperfections : 1; // Whether room imperfections are allowed
    uint8_t room_flags_unk3 : 5;

    undefined field_0xe;
    uint8_t item_density; // 0xF: Controls how many items will be spawned
    uint8_t trap_density; // 0x10: Controls how many traps will be spawned
    uint8_t floor_number; // 0x11: The current floor number within the overall dungeon
    struct fixed_room_id_8 fixed_room_id; // 0x12
    uint8_t extra_hallways;               // 0x13: Number of extra hallways to generate
    uint8_t buried_item_density; // 0x14: Controls how many buried items (in walls) will be spawned
    // 0x15: Controls how much secondary terrain (water, lava, and this actually applies to chasms
    // too) will be spawned
    uint8_t secondary_terrain_density;
    struct darkness_level_8 darkness_level; // 0x16
    uint8_t max_money_amount_div_5;         // 0x17: 1/5 the maximum amount for Poké spawns
    undefined field_0x18;
    // 0x19: Chance that a Monster House will be an itemless one
    uint8_t itemless_monster_house_chance;
    // 0x1A: Values are shifted relative to enum hidden_stairs_type.
    // 0 means HIDDEN_STAIRS_SECRET_BAZAAR, 1 means HIDDEN_STAIRS_SECRET_ROOM, and
    // 255 still means HIDDEN_STAIRS_RANDOM_SECRET_BAZAAR_OR_SECRET_ROOM.
    uint8_t hidden_stairs_type;
    uint8_t hidden_stairs_spawn_chance; // 0x1B
    int16_t enemy_iq;                   // 0x1C: IQ stat of enemies
    int16_t iq_booster_value; // 0x1E: IQ increase from the IQ booster item upon entering the floor
};
ASSERT_SIZE(struct floor_properties, 32);

// Info about a mission destination floor
struct mission_destination_info {
    bool is_destination_floor;  // 0x0: Whether or not the current floor is a mission destination
    struct mission_type_8 type; // 0x1:
    // 0x2: The meaning of this field depends on the type field; see union mission_subtype.
    uint8_t subtype;
    undefined field_0x3;
    // 0x4: Item to retrieve, if this is an item-retrieval mission
    struct item_id_16 item_to_retrieve;
    struct item_id_16 item_to_deliver;     // 0x6: Item to deliver to the client, if relevant
    struct item_id_16 special_target_item; // 0x8: For Sealed Chamber and Treasure Memo missions
    struct monster_id_16 client;           // 0xA: The client on the mission listing
    struct monster_id_16 rescue_target;    // 0xC: The monster to be rescued
    // 0xE: Usually just the target to defeat. If an outlaw has minions, the monster IDs will be
    // listed in subsequent entries. Note that there can be multiple minions of the same species,
    // which is not reflected here.
    struct monster_id_16 enemy_species[3];
    uint8_t n_enemy_species; // 0x14: Length of the preceding array
    undefined field_0x15;
    // 0x16: Fixed room ID of the destination floor, if relevant
    // (e.g., Chambers, Challenge Letters, etc.)
    struct fixed_room_id_8 fixed_room_id;
    undefined field_0x17;
    undefined field_0x18;
    undefined field_0x19;
    undefined field_0x1a;
    // 0x1B: Will be set after the target enemy has been defeated.
    // If there are minions, this flag applies just to the main outlaw.
    bool target_enemy_is_defeated;
};
ASSERT_SIZE(struct mission_destination_info, 28);

// Entry for a single dungeon in the dungeon data list
struct dungeon_data_list_entry {
    uint8_t n_floors;                   // Number of floors in the dungeon
    uint8_t dungeon_data_index_mappa_s; // Index in mappa_s.bin
    // Combined floor count in earlier dungeon within the same group
    uint8_t n_preceding_floors_group;
    // Combined floor count of all dungeon within the same group (both before and after)
    uint8_t n_total_floors_group;
};
ASSERT_SIZE(struct dungeon_data_list_entry, 4);

// Entry for a single dungeon in the dungeon restriction list
struct dungeon_restriction {
    // 0x0: flags: 2-byte bitfield
    bool f_dungeon_goes_up : 1;  // Dungeon goes up (stairs ascend, floors are labeled 1F, 2F, ...)
    bool f_enemies_evolve : 1;   // Enemies evolve after they defeat another monster
    bool f_enemies_give_exp : 1; // Enemies give experience
    bool f_recruitment_allowed : 1;   // Recruitment is allowed
    bool f_reset_to_level_1 : 1;      // Team members are reset to level 1 for the dungeon
    bool f_money_allowed : 1;         // Money is allowed to be brought into the dungeon
    bool f_leader_change_enabled : 1; // The team leader can be changed within the dungeon
    bool f_enter_without_saving : 1;  // If false, the game will be saved upon entering
    bool f_disable_iq_skills : 1;     // IQ skills won't work in the dungeon
    bool f_no_trap_uncovering : 1;    // Traps cannot be revealed by attacking or throwing items
    bool f_treasure_box_drops : 1;    // Enemies can drop Treasure Boxes
    uint16_t flags_unused : 5;

    uint16_t unused;             // 0x2
    uint8_t max_rescue_attempts; // 0x4: 0xFF means no rescues allowed
    uint8_t max_items_allowed; // 0x5: Maximum number of items that can be brought into the dungeon
    uint8_t max_party_size;    // 0x6: Maximum number of team members that can enter the dungeon
    undefined field_0x7;
    uint16_t turn_limit_per_floor; // 0x8: Number of turns per floor before the wind blows you out
    undefined field_0xa;
    undefined field_0xb;
};
ASSERT_SIZE(struct dungeon_restriction, 12);

// Entry in the fixed room item spawn table
struct fixed_room_item_spawn_entry {
    struct item_id_16 id;
    undefined field_0x2;
    undefined field_0x3;
    undefined field_0x4;
    undefined field_0x5;
    undefined field_0x6;
    undefined field_0x7;
};
ASSERT_SIZE(struct fixed_room_item_spawn_entry, 8);

// Entry in the fixed room monster spawn table
struct fixed_room_monster_spawn_entry {
    struct monster_id_16 id;
    uint8_t stat_table_idx; // Index into the fixed room monster spawn stats table
    struct monster_behavior_8 behavior;
};
ASSERT_SIZE(struct fixed_room_monster_spawn_entry, 4);

// Entry in the fixed room monster spawn stats table
struct fixed_room_monster_spawn_stats_entry {
    uint16_t level;    // 0x0
    uint16_t hp;       // 0x2
    int16_t exp_yield; // 0x4
    uint8_t atk;       // 0x6
    uint8_t sp_atk;    // 0x7
    uint8_t def;       // 0x8
    uint8_t sp_def;    // 0x9
    undefined field_0xa;
    undefined field_0xb;
};
ASSERT_SIZE(struct fixed_room_monster_spawn_stats_entry, 12);

// Entry in the fixed room properties table
struct fixed_room_properties_entry {
    // 0x0: If MUSIC_NONE_0x0, the music will be taken from the mappa file for the floor
    struct music_id_16 music;
    undefined field_0x2;
    undefined field_0x3;
    bool illuminated; // 0x4: Floor will be fully illuminated (darkness level DARKNESS_BRIGHT)
    bool enable_lategame_traps; // 0x5: Allows Summon, Pitfall, and Pokémon traps to spawn
    bool ai_moves_enabled;      // 0x6: NPCs can use moves on this floor
    bool orbs_allowed;          // 0x7: Orbs can be used. Ignored for IDs past FIXED_SEALED_CHAMBER.
    // 0x8: Warping, being blown away, and leaping are allowed on this floor.
    // Ignored for IDs past FIXED_SEALED_CHAMBER.
    bool tile_jumps_allowed;
    // 0x9: Trawl Orbs work on this floor. Ignored for IDs past FIXED_SEALED_CHAMBER.
    bool trawl_orbs_allowed;
    // 0xA: This floor will be exited immediately after all enemies have been defeated.
    bool exit_after_enemies_defeated;
    undefined field_0xb;
};
ASSERT_SIZE(struct fixed_room_properties_entry, 12);

// Entry in the fixed room tile spawn table.
struct fixed_room_tile_spawn_entry {
    struct trap_id_8 id; // 0x0
    uint8_t flags;       // 0x1: Copied into trap::flags
    uint8_t room;        // 0x2: Room ID, or 0xFF for hallways
    // 0x3: flags3: 1-byte bitfield
    bool f_trap_visible : 1; // This trap will be visible
    uint8_t flags3_unk1 : 2;
    bool f_secondary_terrain : 1; // This tile will be secondary terrain
    uint8_t flags3_unk4 : 4;
};
ASSERT_SIZE(struct fixed_room_tile_spawn_entry, 4);

// Entry in the fixed room entity spawn table. Each field points into one of the subtables.
struct fixed_room_entity_spawn_entry {
    struct fixed_room_item_spawn_entry* item;
    struct fixed_room_monster_spawn_entry* monster;
    struct fixed_room_tile_spawn_entry* tile;
};
ASSERT_SIZE(struct fixed_room_entity_spawn_entry, 12);

// Data for guest monsters that join you during certain story dungeons.
// These all directly correspond to fields in struct monster.
struct guest_monster {
    undefined field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    struct monster_id_16 id;       // 0x4
    struct dungeon_id_8 joined_at; // 0x6
    undefined field_0x7;
    struct move_id_16 moves[4]; // 0x8
    int16_t max_hp;             // 0x10
    uint8_t level;              // 0x12
    undefined field_0x13;
    int16_t iq;  // 0x14
    uint8_t atk; // 0x16
    undefined field_0x17;
    uint8_t sp_atk; // 0x18
    undefined field_0x19;
    uint8_t def; // 0x1A
    undefined field_0x1b;
    uint8_t sp_def; // 0x1C
    undefined field_0x1d;
    undefined field_0x1e;
    undefined field_0x1f;
    int exp; // 0x20
};
ASSERT_SIZE(struct guest_monster, 36);

// Map marker entry in the map marker placements list. These determine where a dungeon
// appears on the Wonder Map.
struct map_marker {
    // [Untested] Which map this marker belongs to. For example, the main world, the Hidden Land
    // map, maps in the Special Episodes, etc.
    uint16_t map_id;
    // If not -1, this map marker is a reference to some other map marker, with an index indicated
    // by this field
    int16_t reference_id;
    // Coordinates on the map
    struct position pos;
};
ASSERT_SIZE(struct map_marker, 8);

// The LCG states for the dungeon PRNG. See the relevant functions in the overlay 29 symbols for an
// explanation of how the dungeon PRNG works.
struct prng_state {
    int use_secondary;        // 0x0: Flag for whether or not to use the secondary LCGs
    uint32_t seq_num_primary; // 0x4: Sequence number for the primary LCG sequence
    // 0x8: Second number in the default sequence (starting with 1),
    // used for calculating the actual seed
    uint32_t preseed;
    // 0xC: The last value generated by the PRNG, corresponding to seq_num_primary
    uint32_t last_value_primary;
    int idx_secondary; // 0x10: Index of the currently active secondary LCG
};
ASSERT_SIZE(struct prng_state, 20);

// Separate this out into its own file because it's massive
#include "dungeon.h"

#endif
