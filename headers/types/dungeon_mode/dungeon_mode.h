// Types used primarily in the context of dungeon mode

#ifndef HEADERS_TYPES_DUNGEON_MODE_H_
#define HEADERS_TYPES_DUNGEON_MODE_H_

#include "enums.h"
#include "dungeon_mode_common.h"

// Used in various contexts, like with entity positions in the dungeon
struct position {
    int16_t x;
    int16_t y;
};
ASSERT_SIZE(struct position, 4);

// Used to indicate the pixel position of an entity. Values are shifted 8 bits to the left, so they
// are probably decimal numbers instead.
struct pixel_position {
    int32_t x;
    int32_t y;
};
ASSERT_SIZE(struct pixel_position, 8);

// Trap info
struct trap {
    struct trap_id_8 id;
    // If 0 or 2, the trap will activate only when a team member steps on it. If 1, the trap will
    // activate only when an enemy steps on it. Naturally, this seems to be 0 for traps and 2 for
    // Wonder Tiles
    uint8_t team;

    // 0x2: flags: 1-byte bitfield
    bool f_unbreakable : 1; // If true, the trap can't be broken (for example, using a Trapbust Orb)
    uint8_t flags_unk1 : 7;

    undefined field_0x3;
};
ASSERT_SIZE(struct trap, 4);

// Used to hold data during damage calculation
struct damage_data {
    int damage;                         // 0x0: Damage dealt by the move
    enum damage_message damage_message; // 0x4: Message to display
    enum type_matchup type_matchup;     // 0x8: Result of the type matchup calculation
    struct type_id_8 type;              // 0xC: Type of the move used
    struct move_category_8 category;    // 0xD: Category of the move used
    bool critical_hit;                  // 0xE
    // 0xF: Set to true if the type matchup calculation results in full immunity due to Flash Fire,
    // Levitate, or Magnet Rise. It seems like this is only used to skip the 30-damage override due
    // to the Set Damage status
    bool full_type_immunity;
    // 0x10: This can be set at various points to indicate that no damage should be dealt. This is
    // checked at the very end of CalcDamageFinal and overrides all other results of the damage
    // calculation.
    bool no_damage;
    // Most likely padding. These aren't set by the function that inits the struct.
    undefined field_0x11;
    undefined field_0x12;
    undefined field_0x13;
};
ASSERT_SIZE(struct damage_data, 20);

// Monster stat modifier info
struct monster_stat_modifiers {
    // Stages go from 0-20 inclusive, with normal being 10
    int16_t offensive_stages[2];  // 0x0: {atk, sp_atk}
    int16_t defensive_stages[2];  // 0x4: {def, sp_def}
    int16_t hit_chance_stages[2]; // 0x8: {accuracy, evasion}
    int16_t flash_fire_boost;     // 0xC: can be 0, 1, or 2
    undefined field_0xe;
    undefined field_0xf;
    // Some moves like Screech affect the damage calculation differently than, e.g., Leer
    // 0x10: binary fixed-point (8 fraction bits), {atk, sp_atk}; from Charm, Memento, etc.
    fx32_8 offensive_multipliers[2];
    // 0x18: binary fixed-point (8 fraction bits), {def, sp_def}; from Screech, etc.
    fx32_8 defensive_multipliers[2];
};
ASSERT_SIZE(struct monster_stat_modifiers, 32);

// Monster statuses
// Many fields are indexes that select from a group of status conditions. These fields are named
// by the FIRST status in the list (when the index is 1, since 0 usually means no status). For other
// statuses in the group, see the subsequent enum values in enum status_id after the first status.
#pragma pack(push, 1)
struct statuses {
    bool roost;
    // 0x1: The typing in entity::types before the flying type is removed for statuses::roost
    // and restored to entity::types after statuses::roost ends.
    struct type_id_8 original_types[2];
    // 0x3: The move id to be used if statuses::bide is 1.
    uint8_t bide_move_id;
    undefined field_0x4;
    undefined field_0x5;
    undefined field_0x6;
    // 0x7: Unique number given to the monster when spawning to differentiate it from other
    // monsters and to properly keep track of a monster. Likely used because a monster could be
    // spawned into the same slot as an old monster and using a pointer alone could cause some
    // issues. Used for Leech Seed, Destiny Bond, Storm Drain, Lightning Rod (probably more).
    uint32_t unique_id;
    // 0xB: Unique number between the wrapped and wrapping target to connect them.
    uint32_t wrap_pair_unique_id;
    // 0xF: Tracks the damage taken to deal when bide status ends. Max 0x3E7 (999).
    uint32_t bide_damage_tally;
    struct monster_behavior_8 monster_behavior; // 0x13
    uint8_t sleep;                              // 0x14: STATUS_SLEEP if 1
    uint8_t sleep_turns; // 0x15: Turns left for the status in statuses::sleep
    uint8_t burn;        // 0x16: STATUS_BURN if 1
    uint8_t burn_turns;  // 0x17: Turns left for the status in statuses::burn
    // 0x18: Turns left until residual damage for the status in statuses::burn, if applicable
    uint8_t burn_damage_countdown;
    // 0x19: The number of times the statuses::burn_damage_countdown has reached 0. Only used
    // when badly poisoned. Determines how much damage the badly poisoned status condition
    // will deal. There is no noticable difference because the table this value is looked up
    // on is filled with 0x6
    uint8_t badly_poisoned_damage_count;
    undefined field_0x1a;
    uint8_t freeze; // 0x1B: STATUS_FROZEN if 1
    undefined field_0x1c;
    undefined field_0x1d;
    undefined field_0x1e;
    // 0x1F: Controls the animation that plays when taking damage from the constriction status.
    // For some reason this is initalized to 0x22 (34)? Which is the animation used by
    // the exclusive item Nether Veil.
    uint32_t constriction_animation;
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
    // 0x2E: Turns left until residual healing for the status in statuses::reflect, if applicable
    uint8_t reflect_damage_countdown;
    uint8_t curse; // 0x2F: STATUS_CURSED if 1
    // 0x30: Set to monster::is_not_team_member of the attacker (the one causing the decoy status).
    uint8_t curse_applier_non_team_member_flag;
    // 0x31: Set to 1 on a Pokemon when inflicted with the Decoy status.
    undefined unk_decoy_tracker;
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
    // 0x3B: Used to track the statuses::statuses_unique_id of the relevant monster for
    // statuses like Leech Seed and Destiny Bond.
    uint32_t statuses_applier_id;
    // 0x3F: Index into entity_table_hdr::monster_slot_ptrs in the dungeon that the user
    // (drainer) is held.
    uint8_t leech_seed_source_monster_index;
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
    // 0x53: Set when initally spawning a team member with the ability Pickup.
    bool pickup_flag;
    bool grudge;       // 0x54: STATUS_GRUDGE
    bool exposed;      // 0x55: STATUS_EXPOSED (Foresight/Odor Sleuth)
    bool type_changed; // 0x56: Flag for if the monster's type has been changed
    bool boss_flag;    // 0x57: Seems to be true for boss monsters
    // 0x58: Appears to be a flag for when a monster increasces their speed. Maybe only used
    // by the RunLeaderTurn function to know if the leader has changed their speed stage partway
    // through the function?
    undefined unk_sped_up_tracker;
    // 0x59: Maybe related to being a team member and new recruit? Set to 1 in TryRecruit
    // and 0 in SpawnTeam. Also checked in EnemyEvolution to be 0 before evolving. Maybe to
    // prevent a recently recruited ally from evolving after and or to add a monster to the
    // assembly after the completion of a dungeon?
    undefined field_0x59;
#if PMDSKY_VERSION == PMDSKY_JP
#else
    // 0x5A: Possibly a flag while in action. Could also be a flag to cause the burn from
    // lava, heal a burn from water, and decrease hunger in the walls.
    bool in_action;
#endif
    // 0x5B: STATUS_TERRIFIED, interestingly, appears to use 0x1 for the Foe-Fear Orb but
    // 0x2 for the ability Stench. The distinction only seems to exist for the game to use
    // a special message for when terrified from stench ends.
    uint8_t terrified;
    uint8_t terrified_turns;   // 0x5C: Turns left for the terrified status
    uint8_t perish_song_turns; // 0x5D: Turns left before Perish Song takes effect
    // 0x5E: Increases progressively while the No-Slip Cap is held. Capped at 0x13
    // Used to calculate the chance of an item becoming sticky, resets to 0 when that happens
    uint8_t no_slip_cap_counter;
    // 0x5F: Determines how much experience the monster will reward after being defeated
    // 0 = 0.5x, 1 = 1.0x, 2 = 1.5x
    uint8_t exp_yield;
    // 0x60: Appears to be set when the held item of the monster is going to be used?
    bool unk_item_use_action;
    // 0x61: Is initalized to 0x63 (99). Changing it from this value causes the monster to
    // begin rendering differently? For example, it causes entity::0xB3 to be 1 and forces
    // entity::0x28 to be 0.
    undefined field_0x61;
    // 0x62: Flag for two-turn moves that haven't concluded yet. This is also a graphical flag.
    // A value of 1 mean "high up" (Fly/Bounce). A value of 2 means some other condition like
    // Dig, Shadow Force, etc. Other values are treated as invalid. Also used for the move
    // Seismic Toss when throwing up the target.
    uint8_t two_turn_move_invincible;
    // 0x63: Related to handling AI when a decoy is present on the floor?
    // Seems to only be 0, 1, 2
    undefined decoy_ai_tracker;
#if PMDSKY_VERSION == PMDSKY_JP
#else
    undefined field_0x64;
    undefined field_0x65;
    undefined field_0x66;
#endif
    // 0x67: 1 means normal. 0 means half speed. 2, 3, and 4 mean 2x, 3x, and 4x speed.
    int speed_stage;
    // Each counter ticks down to 0 turn by turn. The current speed_stage is calculated as:
    // min(max({# nonzero speed_up_counters} - {# nonzero speed_down_counters}, 0), 4)
    uint8_t speed_up_counters[5];   // 0x6B
    uint8_t speed_down_counters[5]; // 0x70
    uint8_t stockpile_stage;        // 0x75: Goes from 0-3. STATUS_STOCKPILING if nonzero
};
#if PMDSKY_VERSION == PMDSKY_JP
ASSERT_SIZE(struct statuses, 114);
#else
ASSERT_SIZE(struct statuses, 118);
#endif
#pragma pack(pop)

// A bitfield where every bit controls one of the icons that can appear on top of a monster's sprite
// to represent status effects. If multiple bits are set, the shown icon cycles through them.
struct status_icon_flags {
    bool f_sleepless : 1; // Blue eye blinking yellow
    bool f_burn : 1;      // Red flame
    bool f_poison : 1;    // White skull
    bool f_toxic : 1;     // Purple skull
    bool f_confused : 1;  // Yellow birds
    bool f_cowering : 1;  // 2 green lines in circle (same as whiffer)
    bool f_taunt : 1;     // Fist icon
    bool f_encore : 1;    // Blue exclamation mark (same as low HP)
    // Blue shield with white sparks. Also for counter, mini counter, mist,
    // metal burst, aqua ring, and lucky chant
    bool f_reflect : 1;
    bool f_safeguard : 1;            // Pink shield. Also for mirror coat
    bool f_light_screen : 1;         // Golden shield. Also for magic coat
    bool f_protect : 1;              // Green shield. Also for mirror move and vital throw
    bool f_endure : 1;               // Blue shield with red sparks
    bool f_low_hp : 1;               // Blue exclamation mark (same as encore)
    bool f_curse : 1;                // Red skull
    bool f_embargo : 1;              // Yellow exclamation mark. Also for gastro acid and snatch
    bool f_sure_shot : 1;            // Blue sword blinking yellow
    bool f_whiffer : 1;              // 2 green lines in circle (same as cowering)
    bool f_set_damage : 1;           // Blue sword blinking red
    bool f_focus_energy : 1;         // Red sword blinking yellow
    bool f_blinded : 1;              // Blue eye with an X
    bool f_cross_eyed : 1;           // Blue question mark
    bool f_eyedrops : 1;             // Blue eye blinking yellow with a circular wave
    bool f_muzzled : 1;              // Blinking red cross
    bool f_grudge : 1;               // Purple shield
    bool f_exposed : 1;              // Blue eye blinking red with a circular wave
    bool f_sleep : 1;                // Red Z's
    bool f_lowered_stat : 1;         // Yellow arrow pointing down
    bool f_heal_block : 1;           // Blinking green cross
    bool f_miracle_eye : 1;          // Blinking orange cross
    bool f_red_exclamation_mark : 1; // Probably unused
    bool f_magnet_rise : 1;          // Purple arrow pointing up

    // The following 4 bytes appear to have a different meaning, maybe they are intended to
    // represent icons that are always displayed and do not cycle (inferred from the difference
    // between the ice block and the other icons). Except for the first bit, the others do not
    // seem to have an effect, but the code stores the full 4 bytes as a bitwise OR of some of the
    // flags (see UpdateStatusIconBitfield).
    bool f_freeze : 1; // Ice block
    uint8_t flags_unk2 : 7;
    undefined field_0x5;
    undefined field_0x6;
    undefined field_0x7;
};
ASSERT_SIZE(struct status_icon_flags, 8);

// Stores a parameter for an action taken by a monster
struct action_parameter {
    // 0x0: Metadata for the action.
    // E.g., this is the monster index when taking an action on a monster, the move index when
    // using a move or a union item_index value when using an item.
    uint8_t action_use_idx;
    undefined field_0x1;
    struct position item_pos; // 0x2: Position of the item to use when using an item on the floor
};
ASSERT_SIZE(struct action_parameter, 6);

// Contains data used to describe an action taken by a monster.
struct action_data {
    struct action_16 action_id;      // 0x0: Action ID
    struct direction_id_8 direction; // 0x2: Direction in which the action will be performed
    undefined field_0x3;
    struct action_parameter action_parameters[2]; // 0x4: Parameters for the action
    int16_t field_0x10;
    int16_t field_0x12;
};
ASSERT_SIZE(struct action_data, 20);

// Monster info
struct monster {
    // 0x0: flags: 2-byte bitfield
    // If true, the AI will skip this monster's turn. There's also an unresearched
    // check related to constriction that reads this flag
    bool f_ai_unk : 1;
    bool f_ai_skip_turn : 1; // If true, the AI will skip this monster's turn and reset the flag.
    uint16_t flags_unk2 : 3;
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
    uint8_t offensive_stats[2]; // 0x1A: {atk, sp_atk}
    uint8_t defensive_stats[2]; // 0x1C: {def, sp_def}
    uint8_t field_0x1e;
    uint8_t field_0x1f;
    int exp;                                      // 0x20: Total Exp. Points
    struct monster_stat_modifiers stat_modifiers; // 0x24
    int16_t hidden_power_base_power;              // 0x44
    struct type_id_8 hidden_power_type;           // 0x46
    undefined field_0x47;
    struct dungeon_id_8 joined_at; // 0x48: Also used as a unique identifier for special monsters
    uint8_t joined_at_floor;       // 0x49: Floor number of recruitment. 0 for special recruits
    struct action_data action;     // 0x4A: Determines the action the monster will take on this turn
    struct type_id_8 types[2];     // 0x5E
    struct ability_id_8 abilities[2]; // 0x60
    struct item held_item;            // 0x62
    // 0x68: Previous held item. Used for whenever taking the X-Ray Specs or Y-Ray Specs on or off
    // in order to update the camera and minimap. Changed to held_item.id after checking.
    struct item_id_16 previous_held_item_id;
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
    // 0x120: If zero, when the monster is standing in a room, the AI will make it head towards a
    // random exit. If nonzero, the monster will instead move in a random direction every turn.
    int random_movement;
    struct move moves[4]; // 0x124
    uint8_t move_flags;   // 0x144: 1-byte bitfield
    undefined field_0x145;
    int16_t belly;                 // 0x146: Integer part
    int16_t belly_thousandths;     // 0x148
    int16_t max_belly;             // 0x14A: Integer part
    int16_t max_belly_thousandths; // 0x14C
    // 0x14E: If true and the monster is an ally, the AI will skip it. False for enemies.
    bool ai_ally_skip;
    bool ai_next_to_target; // 0x14F: This NPC monster is next to its current target
    // 0x150: Set if monster::is_team_leader is true and belly is empty.
    bool famished;
    undefined field_0x151;
    // 0x152: Seems to be true if the monster has already acted this turn: attacked, used an item,
    // or seemingly anything other than moving/resting. Also true when the monster faints.
    bool already_acted;
    // 0x153: True if this enemy should evolve. It is not enough to set this flag to evolve
    // an enemy monster. You also need to set dungeon::should_enemy_evolve.
    bool should_evolve;
    // 0x154: True if using a charged move. Changed together with statuses::bide.
    bool using_charged_move;
    // 0x155: True if the target attacked a Pokemon that has STATUS_GRUDGE.
    bool hit_grudge_monster;
    undefined field_0x156; // 0 when the monster faints
    undefined field_0x157;
    // 0x158: General-purpose bitflags tracking different bits of volatile state.
    // Together with prev_state_bitflags, this is typically used to determine whether
    // to log a message on a state change.
    uint16_t state_flags;
    // 0x15A: The previous value of state_bitflags before the last update
    uint16_t prev_state_flags;
    // 0x15C: Appears to control if flash fire should activate.
    bool apply_flash_fire_boost;
    // 0x15D: Appears to be a counter for how many times rollout has hit. Likely to be able to
    // determine how much extra damage consecutive rollout hits should deal.
    uint8_t rollout_hit_counter;
    // 0x15E: If true, the monster is warped after completing its move.
    bool memento_warp_flag;
    // 0x15F: If true, the monster's special attack is dropped after the completing its move.
    bool overheat_special_attack_drop_flag;
    // 0x160: If true, the monster's shadow is drawn. This value is initalized to 1 for every
    // monster except Diglett and Dugtrio.
    bool display_shadow;
    // 0x161: If true, prevents giving items to this monster. Might have a broader meaning,
    // such as whether the monster is a guest pokémon.
    bool cannot_give_items;
    // 0x162: Related to using a move and either missing or fainting. Set to 1 right before
    // the function for a move is called and set to 0 (sometimes) in ApplyDamage. Gets set
    // when the monster faints sometimes with field 0x156. When false, causes random
    // outcomes with the monster to fail.
    bool field_0x162;
    // 0x163: Related to controlling the number of attacks per move use. Possibly to account
    // for two-turn moves?
    bool field_0x163;
    bool took_damage_flag; // 0x164: Set after the monster took damage.
    // 0x165: Appears to be some sort of validity check? Where 0 is valid and 1 is invalid.
    // HandleFaint sets this number to 1. Also set to 1 if IsMonsterIdInNormalRange is false.
    bool field_0x165;
    // 0x166: Set after the monster attacks (true if the attack missed, false otherwise). If true
    // when the monster attacks, Practice Swinger will activate.
    bool practice_swinger_flag;
    // 0x167: Set to true when the monster receives a critical hit. If true when the monster
    // attacks, Anger Point will activate. Set to false after the monster attacks.
    bool anger_point_flag;
    undefined field_0x168;
    undefined field_0x169;
    // 0x16A: When not DIR_NONE, monster will turn in the specified direction and
    // its AI will be forced to target the tile next to it in that direction.
    // Used to prevent bosses from turning towards team members the moment the boss fight
    // starts (which would override their intended starting facing direction).
    struct direction_id_8 force_turn;
    undefined field_0x16b;
    undefined field_0x16c;
    undefined field_0x16d;
    undefined field_0x16e;
    undefined field_0x16f;
    // 0x170: Set to make the monster disappear when using the move U-turn.
    bool uturn_hide_monster_flag;
    // 0x171: Some kind of visual flag? Gets set to 0 temporarily when changing Shaymin form
    // or when using the Gone Pebble? Also hardcoded to be set to 0 for monsters that generally
    // tend to float? Otherwise 1?
    bool field_0x171;
    // 0x172: Set when the leader and falling through a pitfall trap.
    bool pitfall_trap_flag_0x172;
    // 0x173: Some kind of visual flag?
    bool field_0x173;
    // 0x174: Set when the leader and falling through a pitfall trap.
    bool pitfall_trap_flag_0x174;
    undefined field_0x175;
    undefined field_0x176;
    // 0x177: Appears to be the direction for using sleep talk? Set to DIR_NONE when awake.
    struct direction_id_8 sleep_talk_direction;
    // 0x178: Appears to be the direction for using snore? Set to DIR_NONE when awake.
    struct direction_id_8 snore_direction;
    // 0x179: Seems to be set to 4 when the monster initally throws something and probably
    // related to direction somehow. Checked in a loop for every monster.
    uint8_t field_0x179;
    // 0x17A: Somehow related to sprite size?
    undefined field_0x17a;
    // 0x17B: Somehow related to sprite size?
    undefined field_0x17b;
    undefined field_0x17c;
    undefined field_0x17d;
    struct position target_pos; // 0x17E: The AI's target's position on screen
    struct position pixel_pos;  // 0x182: The monster's graphical position on screen?
    undefined field_0x186;
    undefined field_0x187;
    undefined field_0x188;
    undefined field_0x189;
    undefined field_0x18a;
    undefined field_0x18b;
    // 0x18C: Bitflags that cause non-damaging exclusive items to trigger on the
    // attacker after they have completed their move. For example, the Eclipse Robe
    // (Darkrai exclusive item) may afflict attacking enemies with the nightmare
    // status condition. (Only uses first 21 bits).
    uint32_t exclusive_item_trigger_bitflags;
    // 0x190: Appears to be related to the item name of the exclusive item that
    // caused the effect to trigger.
    int16_t field_0x190;
    // 0x192: Bitflags that cause non-damaging abilities to trigger on the attacker
    // after they have completed their move. Abilites like magnet pull, cute charm,
    // static, and flame body. (Only uses first 11 bits). One exception is the move
    // Rapid Spin which sets one of the flags for the user.
    uint16_t contact_ability_trigger_bitflags;
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
    // 0x214: Somehow related to gaining exp through a joy ribbon. Set to 0 after
    // gaining or losing a level. Also checked and set to 0 when an enemy evolves.
    // Maybe for a specific scenario of leveling up with exp from a joy ribbon?
    uint32_t unk_exp_tracker;
    // 0x218: Status icons displayed on top of the monster's sprite
    struct status_icon_flags status_icons;
    // 0x220: Seems to be related in some way to shadow size and being a water tileset. So
    // probably controls when the ripple effect when standing on water.
    uint8_t water_shadow_ripple_tracker;
    // 0x221: Set if the current move being used was copied by Me First
    bool me_first_flag;
    undefined field_0x222;
    undefined field_0x223;
    // Stat boosts from exclusive items with EXCLUSIVE_EFF_STAT_BOOST
    uint8_t exclusive_item_offense_boosts[2]; // 0x224: {atk, sp_atk}
    uint8_t exclusive_item_defense_boosts[2]; // 0x226: {def, sp_def}
    // 0x228: Bitvector. See enum exclusive_item_effect_id for the meaning of each bit
    uint32_t exclusive_item_effect_flags[5];
    // 0x23C: Initialized to 0. Probably menu related only, seems to be set to true through the
    // menu.
    bool field_0x23c;
    // 0x23F: When reviving a monster, temporarily set to true. Probably a visual indicator
    // of some kind?
    bool unk_revive_visual_tracker;
    // 0x23E: Gets set to 0 before using an attack and gets set to 1 in LevelUp. Seems to stop
    // the rest of the attacks (ie from Swift Swim) from continuing. Possibly to avoid the
    // the monster leveling up and trying to use a move that was just overwritten by a new move?
    undefined field_0x23e;
    // 0x23F: Gets set to 1 when the move used won't use up any PP. Used to check if the
    // monster should lose extra PP from the ability Pressure.
    bool should_not_lose_pp;
};
#if PMDSKY_VERSION == PMDSKY_JP
ASSERT_SIZE(struct monster, 572);
#else
ASSERT_SIZE(struct monster, 576);
#endif

// Generic entity data
struct entity {
    enum entity_type type;                  // 0x0
    struct position pos;                    // 0x4
    struct position prev_pos;               // 0x8
    struct pixel_position pixel_pos;        // 0xC
    struct pixel_position pixel_pos_mirror; // 0x14: Monsters only?
    // 0x1C: Graphical parameter for evelation above the ground. Last byte behaves weirdly.
    int elevation;
    bool is_visible; // 0x20: For traps/hidden stairs
    undefined field_0x21;
    // 0x22: If true, the sprite will be shown with a certain degree of transparency
    bool transparent;
    // 0x23: Seems to be the animation frame counter for the 10-frame "shuffle" animation that
    // plays at the end of a walk sequence
    uint8_t end_walk_anim_frame;
    undefined field_0x24;
    uint8_t room_idx; // 0x25: Index of the room a monster is in. 0xFF for hall
    // 0x26: Unique index for each monster that spawns. Starts at 0xA for the leader, and each
    // subsequent monster to spawn is assigned the next number (0xB, 0xC, ...)
    uint16_t spawn_genid;
    undefined field_0x28;
    undefined field_0x29;
    undefined field_0x2a;
    undefined field_0x2b;
    struct animation_control anim_ctrl;
    uint16_t sprite_index; // 0xA8
    undefined field_0xaa;
    undefined field_0xab;
    undefined field_0xac;
    undefined field_0xad;
    uint8_t animation_group_id;        // 0xAE
    uint8_t animation_group_id_mirror; // 0xAF
    uint8_t animation_id;              // 0xB0
    uint8_t animation_id_mirror0;      // 0xB1
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
    // 0x7: Room index. 0xFF if not in a room, 0xFE on junctions during map generation (it gets set
    // to 0xFF later).
    uint8_t room;
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

// Information about the rooms on the current floor
struct room_data {
    uint8_t room_id;
    undefined field_0x1;                 // Initialized to 0
    struct position bottom_right_corner; // 0x2
    struct position top_left_corner;     // 0x6
    undefined field_0xa;                 // Doesn't get initialized, likely padding
    undefined field_0xb;                 // Doesn't get initialized, likely padding
    undefined4 field_0xc;                // Initialized to (bottom_right_corner.x - 1) * 0x1C
    undefined4 field_0x10;               // Initialized to (bottom_right_corner.y - 1) * 0x1C
    undefined4 field_0x14;               // Initialized to (top_left_corner.x + 1) * 0x1C
    undefined4 field_0x18;               // Initialized to (top_left_corner.y + 1) * 0x1C
};
ASSERT_SIZE(struct room_data, 28);

// Struct that seems to hold data related to the map, the camera and the touchscreen numbers
struct display_data {
    struct position camera_pos; // 0x0: Position of the tile the camera is currently pointing to
    // 0x4: Copy of 0x0. Used to detect changes in the camera's position.
    struct position camera_pos_mirror;
    // 0x8: Pixel position of the tile the camera is currently pointing to
    struct position camera_pixel_pos;
    struct position camera_pixel_pos_mirror; // 0xC: Mirror of 0x8
    // 0x10: Entity currently being pointed by the camera, mostly used to
    // control rendering of the dungeon, GUI, minimap, etc.
    struct entity* camera_target;
    // 0x14: Appears to be used to determine the offset to render the screen from normal when
    // shaking.
    uint32_t screen_shake_offset;
    // 0x18: Appears to be the intensity value for when the screen shakes. Decremented by
    // 0x1 until 0x0.
    uint32_t screen_shake_intensity;
    // 0x1C: Appears to be the value to set to display_data::screen_shake_intensity when it
    // reaches 0x0. (This number is usually 0x0 so the screen stops shaking after.)
    uint32_t screen_shake_intensity_reset;
    undefined field_0x20; // 0x20: Initialized to 0x3.
    // 0x21: Same as floor_properties::visibility_range
    // Affects the number of map tiles around the player's position that get marked as
    // "visited" while exploring, as well as how far away you can see enemies under non-illuminated
    // conditions (outside of this range, enemies will not be visible on screen).
    uint8_t visibility_range;
    // 0x22: True if the pokémon currently pointed by the camera has
    // the status_id::STATUS_BLINKER effect
    bool blinded;
    // 0x23: True after using a Luminous Orb or in floors where darkness is forcefully disabled
    bool luminous;
    // 0x24: If false and luminous is false as well, darkness will be displayed graphically.
    // This is set in dungeons that aren't naturally dark, and also in some fixed room floors.
    bool natural_lighting;
    // 0x25: True if the pokémon currently pointed by the camera has the Map Surveyor IQ skill
    // active
    bool map_surveyor;
    // 0x26: True if enemies should be shown on the map.
    // Seems to be a dependent property computed as
    // (leader has Power Ears status OR leader has X-Ray Specs equipped).
    // This is NOT affected by the luminous flag.
    bool can_see_enemies;
    // 0x27: True if items are being shown on the map. Similar to can_see_enemies.
    bool can_see_items;
    // 0x28: True if traps are being shown on the map. Similar to can_see_enemies.
    bool can_see_traps;
    // 0x29: True if the pokémon currently pointed by the camera has the
    // status_id::STATUS_CROSS_EYED effect.
    // Causes all entities to be displayed as green circles on the map.
    bool hallucinating;
    bool can_see_stairs;  // 0x2A: True if stairs are being shown on the map
    undefined field_0x2B; // 0x2B: Initialized to 0
    undefined field_0x2C;
    bool darkness;        // 0x2D: True if there's darkness on the floor
    undefined field_0x2E; // 0x2E: Initialized to 1
    // 0x2F: True if the leader is being pointed by the camera right now. If false, UI digits will
    // be displayed in green.
    bool leader_pointed;
    undefined field_0x30; // 0x30: Initialized to 1
    // 0x31: Set to 1 when losing in a dungeon. Seems to cause display_data::0x38 to
    // display_data::leader_max_hp_touch_screen to become 0xFFFF (-1).
    bool unk_fade_to_black_tracker;
    undefined field_0x32;   // 0x32: Initialized to 0
    undefined field_0x33;   // 0x33: Initialized to 0
    undefined field_0x34;   // 0x34: Is used, related to lighting?
    bool team_menu_or_grid; // 0x35: True when the team menu is opened or while Y is being held
    // Derived from internal direction in leader info block
    struct direction_id_8 leader_target_direction;        // 0x36
    struct direction_id_8 leader_target_direction_mirror; // 0x37
    undefined2 field_0x38;                                // 0x38: Initialized to 0xFFFF (-1).
    undefined2 field_0x3A;                                // 0x3A: Initialized to 0xFFFF (-1).
    int16_t floor_touch_screen;         // 0x3C: Floor number displayed on the touch screen
    int16_t leader_level_touch_screen;  // 0x3E: Leader's level displayed on the touch screen
    int16_t leader_hp_touch_screen;     // 0x40: Leader's current HP displayed on the touch screen
    int16_t leader_max_hp_touch_screen; // 0x42: Leader's max HP displayed on the touch screen
    undefined2 field_0x44;
    // Padding?
    undefined field_0x46;
    undefined field_0x47;
};
ASSERT_SIZE(struct display_data, 72);

// Contains the data required to display a tile on the minimap
struct minimap_display_tile {
    undefined4 field_0x0;
    undefined4 field_0x4;
    undefined4 field_0x8;
    undefined4 field_0x0C;
    undefined4 field_0x10;
    undefined4 field_0x14;
    undefined4 field_0x18;
    undefined4 field_0x1C;
    undefined4 field_0x20;
    undefined4 field_0x24;
    undefined4 field_0x28;
    undefined4 field_0x2C;
    undefined4 field_0x30;
    undefined4 field_0x34;
    undefined4 field_0x38;
    undefined4 field_0x3C;
};
ASSERT_SIZE(struct minimap_display_tile, 64);

// Contains the graphical representation of minimap tiles
struct minimap_display_data {
    // 0x0: Matrix that contains tile display data. Each chunk of 4x4 tiles shares
    // a single entry in this matrix. (To calculate which entry corresponds to a
    // given (x,y) coordinate, simply divide both x and y by 2 and drop decimals)
    struct minimap_display_tile tile_matrix_1[16][28];
    // 0x7000: Another matrix just like the first one
    struct minimap_display_tile tile_matrix_2[16][28];
    // 0xE000: Weird byte matrix.
    // The code that initializes this matrix fills 56 bytes
    // per row, which seems like an oversight.
    // On top of that, starting at 0xE1C0 there's a pointer table that gets overwritten every time
    // this matrix is rebuilt, only to be restored later. There's probably
    // an union involved somewhere, but right now there's not enough information
    // to know where exactly.
    undefined field_0xE000[32][28];
    undefined overwritten_extra_bytes[28]; // 0xE380
    undefined4 field_0xE39C[41];           // 0xE39C: Array of pointers
    undefined4 field_0xE440;
    undefined field_0xE444;
    undefined field_0xE445;
    undefined field_0xE446;
    undefined field_0xE447;
    undefined field_0xE448;
    // Padding?
    undefined field_0xE449;
    undefined field_0xE44A;
    undefined field_0xE44B;
};
ASSERT_SIZE(struct minimap_display_data, 58444);

// Data related to dungeon generation
struct dungeon_generation_info {
    // 0x0: Set if the floor layout is guaranteed to be a Monster House, or the dungeon generation
    // algorithm fails
    bool force_create_monster_house;
    // 0x1: Set if the locked door on the floor has already been opened.
    bool locked_door_opened;
    // 0x2: Set if a kecleon shop was properly spawned.
    bool kecleon_shop_spawned;
    // 0x3: When a non-zero value, the one-room orb will fail.
    undefined unk_one_room_flag;
    bool dough_seed_extra_poke_flag;
    // 0x5: Room index of Monster House on the floor. 0xFF if there's no Monster House
    uint8_t monster_house_room;
    // 0x6: Related to when a monster from a fixed room faints. Maybe to check if the floor
    // should be over after knocking them out?
    undefined unk_fixed_room_static_monster_tracker;
    undefined field_0x7;
    // 0x8: The type of the hidden stairs on the current floor.
    enum hidden_stairs_type hidden_stairs_type;
    // 0xC: Used to check to load the corresponding hidden fixed room and information for
    // the corresponding type of hidden floor.
    enum hidden_stairs_type hidden_floor_type;
    int16_t tileset_id; // 0x10
    // 0x12: Music table index (see the same field in struct floor_properties)
    uint16_t music_table_idx;
    // 0x14: Controls which trap graphics to use for the staircase. Usually 0x2B (27) and
    // 0x2C (28) for down and up respectively.
    uint16_t staircase_visual_idx;
    struct fixed_room_id_8 fixed_room_id; // 0x16
    undefined field_0x17;
    undefined field_0x18;
    undefined field_0x19;
    uint16_t floor_generation_attempts; // 0x1A: Number of attempts at floor layout generation
    struct tile tiles[32][56];          // 0x1C
    struct position team_spawn_pos;     // 0x8C1C: Position of the team spawn
    struct position stairs_pos;         // 0x8C20: Position of the stairs spawn
    // 0x8C24: Position of the Hidden Stairs spawn, or (-1, -1) if no Hidden Stairs
    struct position hidden_stairs_pos;
    // 0x8C28: Spawn position of each of the team members
    struct position individual_team_spawn_positions[4];
    // There's another 6 words that look like spawn positions right after these 4
};
ASSERT_SIZE(struct dungeon_generation_info, 35896);

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
    bool has_secondary_structure; // 0x9: Whether this cell has generated a secondary structure
    bool is_room;                 // 0xA: Whether this cell contains a room
    bool is_connected;            // 0xB: Whether this cell is connected to a neighboring grid cell
    bool is_kecleon_shop;         // 0xC: Whether this cell has a Kecleon Shop
    undefined field_0xd;          // 0xD: This field is entirely unused
    bool is_monster_house;        // 0xE: Whether this cell has a Monster House
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
    undefined field_0x1b;          // 0x1B: Potentially flag_fixed ?
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
    // 0x1C: Where the Hidden Stairs spawn is. Values of -1 indicate no Hidden Stairs.
    struct position hidden_stairs_spawn;
    // 0x20: Where the middle of the Kecleon Shop is, if applicable
    struct position kecleon_shop_middle;
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
    // 0x1: Affects the number of rooms to be generated. If it's positive, a slight random variation
    // (between -2 and +1) is applied to the value (the final value must be at least 1). If it's
    // negative, its absolute value is used without adding a random variation.
    int8_t room_density;
    uint8_t tileset; // 0x2
    // 0x3: Indexes into the music ID table in overlay 10 to determine the floor's music track.
    // See the relevant descriptions in the overlay 10 symbols for more information.
    uint8_t music_table_idx;
    struct weather_id_8 weather; // 0x4
    // 0x5: Controls how many connections will be made between grid cells
    uint8_t floor_connectivity;
    // 0x6: Controls how many enemies will be spawned. If 0, no enemies will spawn, even as the
    // player walks.
    // The final value will be 0 in the special case that density is 0. Otherwise, the final value
    // will be randomized between density / 2 and density - 1, but no lower than 1.
    // The code seems to imply that the value was meant to be signed, since it contains a section
    // that takes the absolute value of the density while skipping the random variation, similar to
    // room_density.
    uint8_t enemy_density;
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
    // 0x16: Vision range (in tiles) when standing on a hallway.
    // A value of 0 means no limit, a value > 0 enables darkness on the floor.
    uint8_t visibility_range;
    uint8_t max_money_amount_div_5; // 0x17: 1/5 the maximum amount for Poké spawns
    // 0x18: Chance of an item spawning on each tile in a Kecleon shop
    struct shop_item_positions_8 shop_item_positions;
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

struct tileset_property {
    int32_t field_0x0;
    uint8_t field_0x4;
    uint8_t field_0x5;
    uint8_t field_0x6;
    uint8_t _padding;
    struct nature_power_variant_16 nature_power_variant;
    uint8_t field_0xa;
    // 0xB: True if this is a water tileset. If so, a different kind of shadow will be displayed
    // under monsters in dungeons, drought orbs won't work and Dive can be used anywhere.
    bool is_water_tileset;
};
ASSERT_SIZE(struct tileset_property, 12);

struct level_tilemap_list_entry {
    int16_t field_0x0;
    uint8_t field_0x2;
    uint8_t field_0x3;
    undefined4 field_0x4;
};
ASSERT_SIZE(struct level_tilemap_list_entry, 8);

struct move_animation {
    int16_t field_0x0;
    int16_t field_0x2;
    int16_t field_0x4;
    int16_t field_0x6;
    uint8_t field_0x8;
    undefined field_0x9;
    undefined field_0xa;
    undefined field_0xb;
    undefined field_0xc;
    undefined field_0xd;
    undefined field_0xe;
    undefined field_0xf;
    undefined field_0x10;
    int8_t field_0x11;
    uint16_t field_0x12;
    int16_t field_0x14;
    uint16_t field_0x16;
};
ASSERT_SIZE(struct move_animation, 24);

// Unverified, ported from Irdkwia's notes
struct special_monster_move_animation {
    int16_t field_0x0;
    undefined field_0x2;
    int8_t field_0x3;
    int16_t field_0x4;
};
ASSERT_SIZE(struct special_monster_move_animation, 6);

// Unverified, ported from Irdkwia's notes
struct item_animation {
    int16_t field_0x0;
    int16_t field_0x2;
};
ASSERT_SIZE(struct item_animation, 4);

// Unverified, ported from Irdkwia's notes
struct trap_animation {
    int16_t field_0x0;
};
ASSERT_SIZE(struct trap_animation, 2);

// Unverified, ported from Irdkwia's notes
struct effect_animation {
    int field_0x0;
    int field_0x4;
    int field_0x8;
    int field_0xc;
    int field_0x10;
    int field_0x14;
    uint8_t field_0x18;
    int8_t field_0x19;
    uint8_t field_0x1a;
    uint8_t field_0x1b;
};
ASSERT_SIZE(struct effect_animation, 28);

// Contains data about a monster that spawns in a dungeon
struct monster_spawn_entry {
    uint16_t level_mult_512; // 0x0: Spawn level << 9
    // 0x2: Incremental spawn weight of this entry for normal spawns
    uint16_t incremental_spawn_weight;
    // 0x4: Incremental spawn weight of this entry for monster house spawns
    uint16_t incremental_spawn_weight_monster_house;
    // 0x6: Monster id. Depending on where this struct is used, it can have values above 600
    // to list secondary gender entries.
    struct monster_id_16 id;
};
ASSERT_SIZE(struct monster_spawn_entry, 8);

// Info about a mission destination floor
struct mission_destination_info {
    bool is_destination_floor;  // 0x0: Whether or not the current floor is a mission destination
    struct mission_type_8 type; // 0x1:
    // 0x2: The meaning of this field depends on the type field; see union mission_subtype.
    uint8_t subtype;
    // 0x3: The index of the mission in the job list?
    uint8_t mission_job_list_idx;
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
    // 0x17: Related to missions where you have to obtain an item? Possibly related to the item
    // being picked up and/or destroyed?
    bool unk_mission_item_tracker1;
    undefined field_0x18;
    undefined field_0x19;
    // 0x1A: Related to missions where you have to obtain an item? Possibly related to the item
    // being picked up and/or destroyed?
    bool unk_mission_item_tracker2;
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

struct dungeon_group_and_group_floor {
    struct dungeon_group_id_8 group_id;
    uint8_t group_floor;
};
ASSERT_SIZE(struct dungeon_group_and_group_floor, 2);

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
    // 0xA: Chance of setting the monster::random_movement field to 1 when spawning an enemy
    int16_t random_movement_chance;
};
ASSERT_SIZE(struct dungeon_restriction, 12);

// Entry in the fixed room item spawn table
struct fixed_room_item_spawn_entry {
    struct bulk_item item;
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
    bool illuminated;           // 0x4: Floor will be fully illuminated (darkness level 0)
    bool enable_lategame_traps; // 0x5: Allows Summon, Pitfall, and Pokémon traps to work
    // 0x6: Monsters can use moves on this floor. Does not affect the regular attack.
    // If false, NPCs won't use any moves at all (not even the regular attack)
    bool moves_enabled;
    bool orbs_allowed; // 0x7: Orbs can be used. Ignored for IDs past FIXED_SEALED_CHAMBER.
    // 0x8: Warping, being blown away, and leaping are allowed on this floor.
    // Ignored for IDs past FIXED_SEALED_CHAMBER.
    bool tile_jumps_allowed;
    // 0x9: Trawl Orbs work on this floor. Ignored for IDs past FIXED_SEALED_CHAMBER.
    bool trawl_orbs_allowed;
    // 0xA: This floor will be exited immediately after all enemies have been defeated.
    // If false, the fixed room must have stairs placed somewhere to be considered valid.
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

// Struct that contains some data used when spawning new enemies
struct enemy_spawn_stats {
    struct monster_id_16 id; // 0x0
    uint16_t level;          // 0x2
    // 0x4: Spawn moves. Useless since each individual enemy gets its own when spawning.
    struct move_id_16 moves[4];
    uint16_t max_hp; // 0xC
    uint8_t atk;     // 0xE
    uint8_t def;     // 0xF
    uint8_t sp_atk;  // 0x10
    uint8_t sp_def;  // 0x11
};
ASSERT_SIZE(struct enemy_spawn_stats, 18);

// Data for guest monsters that join you during certain story dungeons.
// These all directly correspond to fields in struct monster.
struct guest_monster {
    char* name;                    // 0x0: Points to an array containing the monster's name
    struct monster_id_16 id;       // 0x4
    struct dungeon_id_8 joined_at; // 0x6
    uint8_t joined_at_floor;       // 0x7
    struct move_id_16 moves[4];    // 0x8
    int16_t max_hp;                // 0x10
    uint8_t level;                 // 0x12
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

// Used by the AI to keep track of which moves it can use
struct ai_possible_move {
    bool can_be_used;                // 0x0
    struct direction_id_8 direction; // 0x1: Direction in which the move should be used
    undefined field_0x2;
    undefined field_0x3;
    int weight; // 0x4: Affects the chance of the AI using this move
};
ASSERT_SIZE(struct ai_possible_move, 8);

struct castform_weather_attributes {
    struct type_id_8 castform_type;
    uint8_t _padding;
    struct monster_id_16 castform_male_id;   // monster ID for male Castform in this weather
    struct monster_id_16 castform_female_id; // monster ID for female Castform in this weather
};
ASSERT_SIZE(struct castform_weather_attributes, 6);

// Performs the effect of a move used by the attacker on the defender, with the item ID associated
// with the move (or ITEM_NOTHING if not applicable). Returns whether or not the move was
// successfully used.
typedef bool (*move_effect_fn_t)(struct entity* attacker, struct entity* defender,
                                 struct move* move, enum item_id item_id);

// Describes one possible variant that a wildcard move like Nature Power or Metronome can turn into
struct wildcard_move_desc {
    // Move ID that the wildcard move turns into
    struct move_id_16 move_id;
    uint16_t _padding;
    // Effect handler that executes the move that the wildcard move turns into
    move_effect_fn_t do_move;
};
ASSERT_SIZE(struct wildcard_move_desc, 8);

struct natural_gift_item_info {
    struct item_id_16 item_id;
    struct type_id_8 type_id;
    uint8_t _padding;
    // This value is the boost that's added to Natural Gift's default base power of 1
    // during damage calculation
    int16_t base_power_boost;
};
ASSERT_SIZE(struct natural_gift_item_info, 6);

// Used to store data about a menu entry for in-dungeon menus
// Might be also used outside of dungeons.
struct dungeon_menu_entry {
    uint16_t id; // 0x0: Used to identify each entry in the menu
    // 0x2: Used to store some additional data about menu entries.
    // For example, if the entry represents an item, this stores the item ID.
    uint16_t parameter;
    undefined field_0x4;
    undefined field_0x5;
    uint16_t string_id; // 0x6: ID of the string shown for this entry
};
ASSERT_SIZE(struct dungeon_menu_entry, 8);

// Represents a message containing a dungeon tip that is displayed at the start of a floor
struct message_tip {
    int tip_id;     // 0x0: Each tip message has its own ID
    int message_id; // 0x4: String ID of the message to display
};
ASSERT_SIZE(struct message_tip, 8);

// Map marker entry in the map marker placements list. These determine where a dungeon
// appears on the Wonder Map.
struct map_marker {
    // Which map this marker belongs to. For example, the main world, the Hidden Land
    // map, maps in the Special Episodes, etc.
    // The game only uses it to check if the marker is correct, but not to decide which map to
    // display
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

// Contains the necessary information to spawn a normal monster.
// Allocated on the stack and passed via a pointer to SpawnMonster.
struct spawned_monster_data {
    struct monster_id_16 monster_id;    // 0x0: The id of the monster to spawn
    struct monster_behavior_8 behavior; // 0x2: NPC behavior of the monster
    undefined field_0x3;
    undefined field_0x4;
    undefined field_0x5;
    undefined field_0x6;
    undefined field_0x7;
    uint16_t level;      // 0x8: The level of the spawned monster
    struct position pos; // 0xA: Position on the floor where the monster will be spawned
    // 0xE: True if the monster cannot be spawned asleep.
    // If false, there is a random chance that the monster will be asleep.
    bool cannot_be_asleep;
    undefined field_0xf;
};
ASSERT_SIZE(struct spawned_monster_data, 16);

// Contains the necessary information to spawn a mission target.
#pragma pack(push, 1)
struct spawned_target_data {
    struct monster_id_16 monster_id;    // 0x0: The id of the monster to spawn
    struct monster_behavior_8 behavior; // 0x2: NPC behavior of the monster
    uint8_t _padding_0x3;
    uint16_t level; // 0x4: The level of the spawned monster
    bool valid;     // 0x6: Indicates that this spawn data is valid
    bool visible; // 0x7: Set to false if this is a "hidden outlaw" that behaves like a normal enemy
    bool pos_assigned; // 0x8: Whether or not a spawn position has been assigned to this monster
    uint8_t _padding_0x9;
    struct position pos; // 0xA: Position on the floor where the monster will be spawned
    undefined field_0xe;
};
ASSERT_SIZE(struct spawned_target_data, 15);
#pragma pack(pop)

// Contains the necessary information to spawn a Kecleon shopkeeper.
struct spawned_shopkeeper_data {
    struct monster_id_16 monster_id;    // 0x0: The id of the monster to spawn
    struct monster_behavior_8 behavior; // 0x2: NPC behavior of the monster
    bool valid;                         // 0x3: Indicates that this spawn data is valid
    uint8_t pos_x;                      // 0x4
    uint8_t pos_y;                      // 0x5
};
ASSERT_SIZE(struct spawned_shopkeeper_data, 6);

// Appears to contain diagnostic information related to the damage calculation routines.
struct damage_calc_diag {
    struct type_id_8 move_type; // 0x0: The type of the last move used
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    enum move_category move_category; // 0x4: The category of the last move used
    // 0x8: The type matchup of the last move used against the individual types of the defender
    struct type_matchup_8 move_indiv_type_matchups[2];
    // 0xA: The modified offensive stat stage of the attacker for the last move used
    uint8_t offensive_stat_stage;
    // 0xB: The modified defensive stat stage of the defender for the last move used
    uint8_t defensive_stat_stage;
    // 0xC: The base offensive stat of the attacker for the last move used
    uint16_t offensive_stat;
    // 0xE: The base defensive stat of the defender for the last move used
    uint16_t defensive_stat;
    // 0x10: The Flash Fire boost of the attacker when a Fire move was last used
    uint16_t flash_fire_boost;
    // 0x12: The modified offense value calculated for the attacker for the last move used,
    // prior to being clamped between 0 and 999
    uint16_t offense_calc;
    // 0x14: The modified defense value calculated for the defender for the last move used
    uint16_t defense_calc;
    uint16_t attacker_level; // 0x16: The level of the attacker for the last move used
    // 0x18: The intermediate quantity in the damage calculation called "AT" in debug logging,
    // which corresponds to: round[ min(max(offense_calc, 0), 999) + power_calc ],
    // where power_calc is a modified move power calculated as (intermediate rounding omitted):
    // GetMovePower(...) * (offensive stat stage multipliers) * (offensive multipliers)
    uint16_t damage_calc_at;
    // 0x1A: An identical copy of defense_calc. This is probably a relic of development,
    // when the final defense contribution to the damage formula might have been a different
    // quantity computed from defense_calc, like how damage_calc_at is computed
    // from offense_calc
    uint16_t damage_calc_def;
    // 0x1C: The intermediate quantity in the damage calculation called "FLV" in debug logging
    // (effective level?), which corresponds to: round[ (offense_calc - defense_calc)/8 + level ]
    uint16_t damage_calc_flv;
    undefined field_0x1e;
    undefined field_0x1f;
    // 0x20: The result of the damage calculation after multiplying the base value by multipliers,
    // but before applying random variation. There are also a couple stray multipliers applied
    // after this result, including multipliers specific to the projectile move (the static 50%,
    // and the Power Pitcher multiplier) and the Air Blade multiplier.
    int damage_calc;
    // 0x24: The intermediate quantity in the damage calculation resulting from the "base" damage
    // calculation: the sum of the power, attack, defense, and level terms, modified by the
    // non-team-member multiplier if relevant, and clamped between 1 and 999.
    int damage_calc_base;
    // 0x28: The random multiplier applied to the result of the damage calculation, as a
    // percentage (so the actual factor, multiplied by 100), rounded to an integer.
    int damage_calc_random_mult_pct;
    // 0x2C: The calculated "static" damage multiplier applied to the output of the base damage
    // calculation. "Static" in the sense that this part of the multiplier doesn't depend on
    // variables like type-based effects, critical hits, and Reflect/Light Screen. Factors in
    // the static damage multiplier include the argument to CalcDamage, the multiplier due to
    // monster::me_first_flag, Reckless, and Iron Fist.
    int static_damage_mult;
    // 0x30: The net number of attack boosts to an attacker due to a Power Band or Munch Belt.
    // It seems like there's a bug in the code; aura bows do not contribute to this field.
    int8_t item_atk_modifier;
    // 0x31: The net number of special attack boosts to an attacker due to a Special Band,
    // Munch Belt, or aura bow. It seems like there's a bug in the code; physical attack boosts
    // from aura bows also contribute to this field.
    int8_t item_sp_atk_modifier;
    // 0x32: The net number of offense boosts to an attacker due to Download, Rivalry,
    // Flower Gift, and Solar Power
    int8_t ability_offense_modifier;
    // 0x33: The net number of defense boosts to a defender due to Flower Gift
    int8_t ability_defense_modifier;
    // 0x34: The net number of offense boosts to an attacker due to Aggressor, Defender, and
    // Practice Swinger
    int8_t iq_skill_offense_modifier;
    // 0x35: The net number of defense boosts to a defender due to Counter Basher, Aggressor, and
    // Defender
    int8_t iq_skill_defense_modifier;
    // 0x36: The net number of defense boosts to a defender due to a Def Scarf or aura bow.
    // It seems like there's a bug in the code; special defense boosts from aura bows also
    // contribute to this field.
    int8_t item_def_modifier;
    // 0x37: The net number of special defense boosts to a defender due to a Zinc Band.
    // It seems like there's a bug in the code; aura bows do not contribute to this field.
    int8_t item_sp_def_modifier;
    // 0x38: Whether or not Scope Lens or Sharpshooter boosted the critical hit rate of a move
    bool scope_lens_or_sharpshooter_activated;
    // 0x39: Whether or not the Patsy Band boosted the critical hit rate of a move
    bool patsy_band_activated;
    // 0x3A: Whether or not Reflect or the Time Shield halved the damage from a physical move
    bool half_physical_damage_activated;
    // 0x3B: Whether or not Light Screen or the Aqua Mantle halved the damage from a special move
    bool half_special_damage_activated;
    // 0x3C: Whether or not the Enhanced critical-hit rate status maxed out the critical hit rate
    // of a move
    bool focus_energy_activated;
    // 0x3D: Whether or not Type-Advantage Master boosted the critical hit rate of a move
    bool type_advantage_master_activated;
    // 0x3E: Whether or not a non-Normal-type move was dampened by Cloudy weather
    bool cloudy_drop_activated;
    // 0x3F: Whether or not a Fire or Water move was affected by Rainy weather
    bool rain_multiplier_activated;
    // 0x40: Whether or not a Fire or Water move was affected by Sunny weather
    bool sunny_multiplier_activated;
    // 0x41: Whether or a Fire move was dampened by Thick Fat or Heatproof
    bool fire_move_ability_drop_activated;
    // 0x42: Whether or not Flash Fire was activated at some point for Fire immunity
    bool flash_fire_activated;
    // 0x43: Whether or not Levitate was activated at some point for Ground immunity
    bool levitate_activated;
    bool torrent_boost_activated;  // 0x44: Whether or not a Water move was boosted by Torrent
    bool overgrow_boost_activated; // 0x45: Whether or not a Grass move was boosted by Overgrow
    bool swarm_boost_activated;    // 0x46: Whether or not a Bug move was boosted by Swarm
    // 0x47: Whether or not a Fire move was boosted by either Blaze or Dry Skin
    bool fire_move_ability_boost_activated;
    // 0x48: Whether or not Scrappy was activated at some point to bypass immunity
    bool scrappy_activated;
    // 0x49: Whether or not Super Luck boosted the critical hit rate for a move
    bool super_luck_activated;
    // 0x4A: Whether or not Sniper boosted the critical hit damage multiplier for a move
    bool sniper_activated;
    bool stab_boost_activated; // 0x4B: Whether or not STAB was activated for a move
    // 0x4C: Whether or not an Electric move was dampened by either Mud Sport or Fog
    bool electric_move_dampened;
    // 0x4D: Whether or not Water Sport was activated by a Fire move
    bool water_sport_drop_activated;
    bool charge_boost_activated; // 0x4E: Whether or not Charge was activated by an Electric move
    undefined field_0x4f;
    // 0x50: Whether or not a Ghost type's immunity to Normal/Fighting was activated at some point
    bool ghost_immunity_activated;
    // 0x51: Whether or not a defender took less damage due to the Charging Skull Bash status
    bool skull_bash_defense_boost_activated;
    undefined field_0x52;
    undefined field_0x53;
};
ASSERT_SIZE(struct damage_calc_diag, 84);

// Used during ApplyDamage to negate damage from certain types due to exclusive items
struct damage_negating_exclusive_eff_entry {
    enum type_id type;
    enum exclusive_item_effect_id effect;
};
ASSERT_SIZE(struct damage_negating_exclusive_eff_entry, 8);

// Represents a two-turn move and its corresponding status_two_turn_id value
struct two_turn_move_and_status {
    struct move_id_16 move;
    struct status_two_turn_id_16 status;
};
ASSERT_SIZE(struct two_turn_move_and_status, 4);

// Represents an area on the touchscreen
// Touchscreen coordinates range from (1,1) (top left corner) to (254, 190) (bottom right corner)
struct touchscreen_area {
    int y1; // 0x0: Lower Y coordinate
    int y2; // 0x4: Upper Y coordinate
    int x1; // 0x8: Lower X coordinate
    int x2; // 0xC: Upper X coordinate
};
ASSERT_SIZE(struct touchscreen_area, 16);

// Store the currently loaded attack sprite
// It’s a unpacked WAN file
struct loaded_attack_sprite_data {
    undefined1 file_content[106496];
    uint16_t loaded_sprite_id; // 0 for none
};
ASSERT_SIZE(struct loaded_attack_sprite_data, 106498);

typedef int (*top_screen_status_fn_t)(void);

// Stores the state of the top screen in dungeon mode
struct top_screen_status {
    undefined field0_0x0;
    undefined field1_0x1;
    uint8_t field2_0x2;
    undefined field3_0x3;
    enum top_screen_fade_status fade_status; // 0x4: Status of the top screen fade
    undefined field5_0x8;
    undefined field6_0x9;
    undefined field7_0xa;
    undefined field8_0xb;
    int field9_0xc;
    enum top_screen_type top_screen_type; // 0x10: Top screen type setting
    // 0x14: Function that is used to initialize the given type of top screen layout
    top_screen_status_fn_t init_func;
    // 0x18: Function that is used to update the given type of top screen layout
    top_screen_status_fn_t update_func;
    void* field13_0x1c;
    // 0x20: Function that is used to close the given type of top screen layout
    top_screen_status_fn_t free_func;
    undefined field15_0x24;
    undefined field16_0x25;
    undefined field17_0x26;
    undefined field18_0x27;
    undefined field19_0x28;
    undefined field20_0x29;
    undefined field21_0x2a;
    uint8_t field22_0x2b;
    undefined field23_0x2c;
    undefined field24_0x2d;
    undefined field25_0x2e;
    undefined field26_0x2f;
    undefined field27_0x30;
    undefined field28_0x31;
    undefined field29_0x32;
    undefined field30_0x33;
    undefined field31_0x34;
    undefined field32_0x35;
    undefined field33_0x36;
    undefined field34_0x37;
    undefined field35_0x38;
    undefined field36_0x39;
    undefined field37_0x3a;
    undefined field38_0x3b;
    undefined field39_0x3c;
    undefined field40_0x3d;
    undefined field41_0x3e;
    undefined field42_0x3f;
    undefined field43_0x40;
    undefined field44_0x41;
    undefined field45_0x42;
    undefined field46_0x43;
    undefined field47_0x44;
    undefined field48_0x45;
    undefined field49_0x46;
    undefined field50_0x47;
    undefined field51_0x48;
    undefined field52_0x49;
    undefined field53_0x4a;
    undefined field54_0x4b;
    undefined field55_0x4c;
    undefined field56_0x4d;
    undefined field57_0x4e;
    undefined field58_0x4f;
    undefined field59_0x50;
    undefined field60_0x51;
    undefined field61_0x52;
    undefined field62_0x53;
    undefined field63_0x54;
    undefined field64_0x55;
    undefined field65_0x56;
    undefined field66_0x57;
    undefined field67_0x58;
    undefined field68_0x59;
    undefined field69_0x5a;
    undefined field70_0x5b;
    undefined field71_0x5c;
    undefined field72_0x5d;
    undefined field73_0x5e;
    undefined field74_0x5f;
    undefined field75_0x60;
    undefined field76_0x61;
    undefined field77_0x62;
    undefined field78_0x63;
    undefined field79_0x64;
    undefined field80_0x65;
    undefined field81_0x66;
    undefined field82_0x67;
    undefined field83_0x68;
    undefined field84_0x69;
    undefined field85_0x6a;
    undefined field86_0x6b;
    undefined field87_0x6c;
    undefined field88_0x6d;
    undefined field89_0x6e;
    undefined field90_0x6f;
    undefined field91_0x70;
    undefined field92_0x71;
    undefined field93_0x72;
    undefined field94_0x73;
    undefined field95_0x74;
    undefined field96_0x75;
    undefined field97_0x76;
    undefined field98_0x77;
    undefined field99_0x78;
    undefined field100_0x79;
    undefined field101_0x7a;
    undefined field102_0x7b;
    undefined field103_0x7c;
    undefined field104_0x7d;
    undefined field105_0x7e;
    undefined field106_0x7f;
    undefined field107_0x80;
    undefined field108_0x81;
    undefined field109_0x82;
    undefined field110_0x83;
    undefined field111_0x84;
    undefined field112_0x85;
    undefined field113_0x86;
    undefined field114_0x87;
    undefined field115_0x88;
    undefined field116_0x89;
    undefined field117_0x8a;
    undefined field118_0x8b;
    undefined field119_0x8c;
    undefined field120_0x8d;
    undefined field121_0x8e;
    undefined field122_0x8f;
    undefined field123_0x90;
    undefined field124_0x91;
    undefined field125_0x92;
    undefined field126_0x93;
    undefined field127_0x94;
    undefined field128_0x95;
    undefined field129_0x96;
    undefined field130_0x97;
    undefined field131_0x98;
    undefined field132_0x99;
    undefined field133_0x9a;
    undefined field134_0x9b;
    undefined field135_0x9c;
    undefined field136_0x9d;
    undefined field137_0x9e;
    undefined field138_0x9f;
    undefined field139_0xa0;
    undefined field140_0xa1;
    undefined field141_0xa2;
    undefined field142_0xa3;
    undefined field143_0xa4;
    undefined field144_0xa5;
    undefined field145_0xa6;
    undefined field146_0xa7;
    undefined field147_0xa8;
    undefined field148_0xa9;
    undefined field149_0xaa;
    undefined field150_0xab;
    undefined field151_0xac;
    undefined field152_0xad;
    undefined field153_0xae;
    undefined field154_0xaf;
    undefined field155_0xb0;
    undefined field156_0xb1;
    undefined field157_0xb2;
    undefined field158_0xb3;
    undefined field159_0xb4;
    undefined field160_0xb5;
    undefined field161_0xb6;
    undefined field162_0xb7;
    undefined field163_0xb8;
    undefined field164_0xb9;
    undefined field165_0xba;
    undefined field166_0xbb;
    undefined field167_0xbc;
    undefined field168_0xbd;
    undefined field169_0xbe;
    undefined field170_0xbf;
    undefined field171_0xc0;
    undefined field172_0xc1;
    undefined field173_0xc2;
    undefined field174_0xc3;
    undefined field175_0xc4;
    undefined field176_0xc5;
    undefined field177_0xc6;
    undefined field178_0xc7;
    undefined field179_0xc8;
    undefined field180_0xc9;
    undefined field181_0xca;
    undefined field182_0xcb;
    undefined field183_0xcc;
    undefined field184_0xcd;
    undefined field185_0xce;
    undefined field186_0xcf;
    undefined field187_0xd0;
    undefined field188_0xd1;
    undefined field189_0xd2;
    undefined field190_0xd3;
    undefined field191_0xd4;
    undefined field192_0xd5;
    undefined field193_0xd6;
    undefined field194_0xd7;
    undefined field195_0xd8;
    undefined field196_0xd9;
    undefined field197_0xda;
    undefined field198_0xdb;
    undefined field199_0xdc;
    undefined field200_0xdd;
    undefined field201_0xde;
    undefined field202_0xdf;
    undefined field203_0xe0;
    undefined field204_0xe1;
    undefined field205_0xe2;
    undefined field206_0xe3;
    undefined field207_0xe4;
    undefined field208_0xe5;
    undefined field209_0xe6;
    undefined field210_0xe7;
    undefined field211_0xe8;
    undefined field212_0xe9;
    undefined field213_0xea;
    undefined field214_0xeb;
    undefined field215_0xec;
    undefined field216_0xed;
    undefined field217_0xee;
    undefined field218_0xef;
    undefined field219_0xf0;
    undefined field220_0xf1;
    undefined field221_0xf2;
    undefined field222_0xf3;
    undefined field223_0xf4;
    undefined field224_0xf5;
    undefined field225_0xf6;
    undefined field226_0xf7;
    undefined field227_0xf8;
    undefined field228_0xf9;
    undefined field229_0xfa;
    undefined field230_0xfb;
    undefined field231_0xfc;
    undefined field232_0xfd;
    undefined field233_0xfe;
    undefined field234_0xff;
    undefined field235_0x100;
    undefined field236_0x101;
    undefined field237_0x102;
    undefined field238_0x103;
    undefined field239_0x104;
    undefined field240_0x105;
    undefined field241_0x106;
    undefined field242_0x107;
    undefined field243_0x108;
    undefined field244_0x109;
    undefined field245_0x10a;
    undefined field246_0x10b;
    undefined field247_0x10c;
    undefined field248_0x10d;
    undefined field249_0x10e;
    undefined field250_0x10f;
    undefined field251_0x110;
    undefined field252_0x111;
    undefined field253_0x112;
    undefined field254_0x113;
    undefined field255_0x114;
    undefined field256_0x115;
    undefined field257_0x116;
    undefined field258_0x117;
    undefined field259_0x118;
    undefined field260_0x119;
    undefined field261_0x11a;
    undefined field262_0x11b;
    undefined field263_0x11c;
    undefined field264_0x11d;
    undefined field265_0x11e;
    undefined field266_0x11f;
    undefined field267_0x120;
    undefined field268_0x121;
    undefined field269_0x122;
    undefined field270_0x123;
    undefined field271_0x124;
    undefined field272_0x125;
    undefined field273_0x126;
    undefined field274_0x127;
    undefined field275_0x128;
    undefined field276_0x129;
    undefined field277_0x12a;
    undefined field278_0x12b;
    undefined field279_0x12c;
    undefined field280_0x12d;
    undefined field281_0x12e;
    undefined field282_0x12f;
    undefined field283_0x130;
    undefined field284_0x131;
    undefined field285_0x132;
    undefined field286_0x133;
    undefined field287_0x134;
    undefined field288_0x135;
    undefined field289_0x136;
    undefined field290_0x137;
    undefined field291_0x138;
    undefined field292_0x139;
    undefined field293_0x13a;
    undefined field294_0x13b;
    undefined field295_0x13c;
    undefined field296_0x13d;
    undefined field297_0x13e;
    undefined field298_0x13f;
    undefined field299_0x140;
    undefined field300_0x141;
    undefined field301_0x142;
    undefined field302_0x143;
    undefined field303_0x144;
    undefined field304_0x145;
    undefined field305_0x146;
    undefined field306_0x147;
    undefined field307_0x148;
    undefined field308_0x149;
    undefined field309_0x14a;
    undefined field310_0x14b;
    undefined field311_0x14c;
    undefined field312_0x14d;
    undefined field313_0x14e;
    undefined field314_0x14f;
    undefined field315_0x150;
    undefined field316_0x151;
    undefined field317_0x152;
    undefined field318_0x153;
    undefined field319_0x154;
    undefined field320_0x155;
    undefined field321_0x156;
    undefined field322_0x157;
    undefined field323_0x158;
    undefined field324_0x159;
    undefined field325_0x15a;
    undefined field326_0x15b;
    undefined field327_0x15c;
    undefined field328_0x15d;
    undefined field329_0x15e;
    undefined field330_0x15f;
    undefined field331_0x160;
    undefined field332_0x161;
    undefined field333_0x162;
    undefined field334_0x163;
    undefined field335_0x164;
    undefined field336_0x165;
    undefined field337_0x166;
    undefined field338_0x167;
    undefined field339_0x168;
    undefined field340_0x169;
    undefined field341_0x16a;
    undefined field342_0x16b;
    undefined field343_0x16c;
    undefined field344_0x16d;
    undefined field345_0x16e;
    undefined field346_0x16f;
    undefined field347_0x170;
    undefined field348_0x171;
    undefined field349_0x172;
    undefined field350_0x173;
    undefined field351_0x174;
    undefined field352_0x175;
    undefined field353_0x176;
    undefined field354_0x177;
    undefined field355_0x178;
    undefined field356_0x179;
    undefined field357_0x17a;
    undefined field358_0x17b;
    undefined field359_0x17c;
    undefined field360_0x17d;
    undefined field361_0x17e;
    undefined field362_0x17f;
    undefined field363_0x180;
    undefined field364_0x181;
    undefined field365_0x182;
    undefined field366_0x183;
    undefined field367_0x184;
    undefined field368_0x185;
    undefined field369_0x186;
    undefined field370_0x187;
    undefined field371_0x188;
    undefined field372_0x189;
    undefined field373_0x18a;
    undefined field374_0x18b;
    undefined field375_0x18c;
    undefined field376_0x18d;
    undefined field377_0x18e;
    undefined field378_0x18f;
    undefined field379_0x190;
    undefined field380_0x191;
    undefined field381_0x192;
    undefined field382_0x193;
    undefined field383_0x194;
    undefined field384_0x195;
    undefined field385_0x196;
    undefined field386_0x197;
    undefined field387_0x198;
    undefined field388_0x199;
    undefined field389_0x19a;
    undefined field390_0x19b;
    undefined field391_0x19c;
    undefined field392_0x19d;
    undefined field393_0x19e;
    undefined field394_0x19f;
    undefined field395_0x1a0;
    undefined field396_0x1a1;
    undefined field397_0x1a2;
    undefined field398_0x1a3;
    undefined field399_0x1a4;
    undefined field400_0x1a5;
    undefined field401_0x1a6;
    undefined field402_0x1a7;
    undefined field403_0x1a8;
    undefined field404_0x1a9;
    undefined field405_0x1aa;
    undefined field406_0x1ab;
    undefined field407_0x1ac;
    undefined field408_0x1ad;
    undefined field409_0x1ae;
    undefined field410_0x1af;
    undefined field411_0x1b0;
    undefined field412_0x1b1;
    undefined field413_0x1b2;
    undefined field414_0x1b3;
    undefined field415_0x1b4;
    undefined field416_0x1b5;
    undefined field417_0x1b6;
    undefined field418_0x1b7;
    undefined field419_0x1b8;
    undefined field420_0x1b9;
    undefined field421_0x1ba;
    undefined field422_0x1bb;
    undefined field423_0x1bc;
    undefined field424_0x1bd;
    undefined field425_0x1be;
    undefined field426_0x1bf;
    undefined field427_0x1c0;
    undefined field428_0x1c1;
    undefined field429_0x1c2;
    undefined field430_0x1c3;
    undefined field431_0x1c4;
    undefined field432_0x1c5;
    undefined field433_0x1c6;
    undefined field434_0x1c7;
    undefined field435_0x1c8;
    undefined field436_0x1c9;
    undefined field437_0x1ca;
    undefined field438_0x1cb;
    undefined field439_0x1cc;
    undefined field440_0x1cd;
    undefined field441_0x1ce;
    undefined field442_0x1cf;
    undefined field443_0x1d0;
    undefined field444_0x1d1;
    undefined field445_0x1d2;
    undefined field446_0x1d3;
    undefined field447_0x1d4;
    undefined field448_0x1d5;
    undefined field449_0x1d6;
    undefined field450_0x1d7;
    undefined field451_0x1d8;
    undefined field452_0x1d9;
    undefined field453_0x1da;
    undefined field454_0x1db;
    undefined field455_0x1dc;
    undefined field456_0x1dd;
    undefined field457_0x1de;
    undefined field458_0x1df;
    undefined field459_0x1e0;
    undefined field460_0x1e1;
    undefined field461_0x1e2;
    undefined field462_0x1e3;
    undefined field463_0x1e4;
    undefined field464_0x1e5;
    undefined field465_0x1e6;
    undefined field466_0x1e7;
    undefined field467_0x1e8;
    undefined field468_0x1e9;
    undefined field469_0x1ea;
    undefined field470_0x1eb;
    undefined field471_0x1ec;
    undefined field472_0x1ed;
    undefined field473_0x1ee;
    undefined field474_0x1ef;
    undefined field475_0x1f0;
    undefined field476_0x1f1;
    undefined field477_0x1f2;
    undefined field478_0x1f3;
    undefined field479_0x1f4;
    undefined field480_0x1f5;
    undefined field481_0x1f6;
    undefined field482_0x1f7;
    undefined field483_0x1f8;
    undefined field484_0x1f9;
    undefined field485_0x1fa;
    undefined field486_0x1fb;
    undefined field487_0x1fc;
    undefined field488_0x1fd;
    undefined field489_0x1fe;
    undefined field490_0x1ff;
    undefined field491_0x200;
    undefined field492_0x201;
    undefined field493_0x202;
    undefined field494_0x203;
    undefined field495_0x204;
    undefined field496_0x205;
    undefined field497_0x206;
    undefined field498_0x207;
    undefined field499_0x208;
    undefined field500_0x209;
    undefined field501_0x20a;
    undefined field502_0x20b;
    undefined field503_0x20c;
    undefined field504_0x20d;
    undefined field505_0x20e;
    undefined field506_0x20f;
    undefined field507_0x210;
    undefined field508_0x211;
    undefined field509_0x212;
    undefined field510_0x213;
    undefined field511_0x214;
    undefined field512_0x215;
    undefined field513_0x216;
    undefined field514_0x217;
    undefined field515_0x218;
    undefined field516_0x219;
    undefined field517_0x21a;
    undefined field518_0x21b;
    undefined field519_0x21c;
    undefined field520_0x21d;
    undefined field521_0x21e;
    undefined field522_0x21f;
    undefined field523_0x220;
    undefined field524_0x221;
    undefined field525_0x222;
    undefined field526_0x223;
    undefined field527_0x224;
    undefined field528_0x225;
    undefined field529_0x226;
    undefined field530_0x227;
    undefined field531_0x228;
    undefined field532_0x229;
    undefined field533_0x22a;
    undefined field534_0x22b;
    undefined field535_0x22c;
    undefined field536_0x22d;
    undefined field537_0x22e;
    undefined field538_0x22f;
};
ASSERT_SIZE(struct top_screen_status, 560);

// Stores the state of the screen fade for one screen in dungeon mode. Probably also does something
// else given how many unknowns there are.
struct dungeon_fade {
    undefined field0_0x0;
    undefined field1_0x1;
    undefined field2_0x2;
    undefined field3_0x3;
    undefined field4_0x4;
    undefined field5_0x5;
    undefined field6_0x6;
    undefined field7_0x7;
    undefined field8_0x8;
    undefined field9_0x9;
    undefined field10_0xa;
    undefined field11_0xb;
    undefined field12_0xc;
    undefined field13_0xd;
    undefined field14_0xe;
    undefined field15_0xf;
    undefined field16_0x10;
    undefined field17_0x11;
    undefined field18_0x12;
    undefined field19_0x13;
    undefined field20_0x14;
    undefined field21_0x15;
    undefined field22_0x16;
    undefined field23_0x17;
    undefined field24_0x18;
    undefined field25_0x19;
    undefined field26_0x1a;
    undefined field27_0x1b;
    undefined field28_0x1c;
    undefined field29_0x1d;
    undefined field30_0x1e;
    undefined field31_0x1f;
    undefined field32_0x20;
    undefined field33_0x21;
    undefined field34_0x22;
    undefined field35_0x23;
    undefined field36_0x24;
    undefined field37_0x25;
    undefined field38_0x26;
    undefined field39_0x27;
    undefined field40_0x28;
    undefined field41_0x29;
    undefined field42_0x2a;
    undefined field43_0x2b;
    undefined field44_0x2c;
    undefined field45_0x2d;
    undefined field46_0x2e;
    undefined field47_0x2f;
    undefined field48_0x30;
    undefined field49_0x31;
    undefined field50_0x32;
    undefined field51_0x33;
    undefined field52_0x34;
    undefined field53_0x35;
    undefined field54_0x36;
    undefined field55_0x37;
    undefined field56_0x38;
    undefined field57_0x39;
    undefined field58_0x3a;
    undefined field59_0x3b;
    undefined field60_0x3c;
    undefined field61_0x3d;
    undefined field62_0x3e;
    undefined field63_0x3f;
    undefined field64_0x40;
    undefined field65_0x41;
    undefined field66_0x42;
    undefined field67_0x43;
    undefined field68_0x44;
    undefined field69_0x45;
    undefined field70_0x46;
    undefined field71_0x47;
    undefined field72_0x48;
    undefined field73_0x49;
    undefined field74_0x4a;
    undefined field75_0x4b;
    undefined field76_0x4c;
    undefined field77_0x4d;
    undefined field78_0x4e;
    undefined field79_0x4f;
    undefined field80_0x50;
    undefined field81_0x51;
    undefined field82_0x52;
    undefined field83_0x53;
    undefined field84_0x54;
    undefined field85_0x55;
    undefined field86_0x56;
    undefined field87_0x57;
    undefined field88_0x58;
    undefined field89_0x59;
    undefined field90_0x5a;
    undefined field91_0x5b;
    undefined field92_0x5c;
    undefined field93_0x5d;
    undefined field94_0x5e;
    undefined field95_0x5f;
    undefined field96_0x60;
    undefined field97_0x61;
    undefined field98_0x62;
    undefined field99_0x63;
    undefined field100_0x64;
    undefined field101_0x65;
    undefined field102_0x66;
    undefined field103_0x67;
    undefined field104_0x68;
    undefined field105_0x69;
    undefined field106_0x6a;
    undefined field107_0x6b;
    undefined field108_0x6c;
    undefined field109_0x6d;
    undefined field110_0x6e;
    undefined field111_0x6f;
    undefined field112_0x70;
    undefined field113_0x71;
    undefined field114_0x72;
    undefined field115_0x73;
    undefined field116_0x74;
    undefined field117_0x75;
    undefined field118_0x76;
    undefined field119_0x77;
    undefined field120_0x78;
    undefined field121_0x79;
    undefined field122_0x7a;
    undefined field123_0x7b;
    undefined field124_0x7c;
    undefined field125_0x7d;
    undefined field126_0x7e;
    undefined field127_0x7f;
    undefined field128_0x80;
    undefined field129_0x81;
    undefined field130_0x82;
    undefined field131_0x83;
    undefined field132_0x84;
    undefined field133_0x85;
    undefined field134_0x86;
    undefined field135_0x87;
    char field136_0x88;
    undefined field137_0x89;
    undefined field138_0x8a;
    undefined field139_0x8b;
    // 0x8C: What kind of fade the screen is currently in, if any
    enum fade_status_dungeon fade_type;
    // 0x90: Brightness change of screen at the current stage of the fade
    int delta_brightness;
    // 0x94 Change of brightness per step of the fade
    int delta_delta_brightness;
    bool is_fading; // 0x98
    undefined field144_0x99;
    undefined field145_0x9a;
    undefined field146_0x9b;
};
ASSERT_SIZE(struct dungeon_fade, 156);

// Stores the dungeon_fade struct for both screens and some associated status value that is used in
// a switch case in HandleFadesDungeon.
struct dungeon_fades {
    struct dungeon_fade bottom_screen_dungeon_fade; // 0x0
    struct dungeon_fade top_screen_dungeon_fade;    // 0x4
    int bottom_screen_status;                       // 0x8
    int top_screen_status;                          // 0xC
};
ASSERT_SIZE(struct dungeon_fades, 320);

// Separate this out into its own file because it's massive
#include "dungeon.h"

#endif
