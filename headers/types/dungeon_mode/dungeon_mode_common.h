// These are types that logically belong to dungeon mode, but also need to be generally accessible
// for transitional code on the interface between dungeon mode and other modes.

#ifndef HEADERS_TYPES_DUNGEON_MODE_COMMON_H_
#define HEADERS_TYPES_DUNGEON_MODE_COMMON_H_

#include "enums.h"

// Item info
struct item {
    // 0x0: flags: 1-byte bitfield
    bool f_exists : 1;  // Validity flag
    bool f_in_shop : 1; // In a Kecleon Shop
    bool f_unpaid : 1;  // Picked up from a Kecleon Shop but not paid for yet
    bool f_sticky : 1;  // Sticky
    bool f_set : 1;     // Usable by L+R
    bool flag_unk5 : 1;
    // For stolen items to recover from outlaws (has red X)? Could be for other items for other
    // types of missions? (Uncertain)
    bool f_unk_mission_item1 : 1;
    // For stolen items to recover from outlaws (has red X)? Could be for other items for other
    // types of missions? (Uncertain) Definitely used temporarily when sorting the items in
    // storage.
    bool f_unk_mission_item2 : 1;
    // 0x1: For bag items. 0 for none, 1 if held by the leader, 2 for the second party member, etc.
    uint8_t held_by;
    // 0x2: Only for stackable items. Will be 0 if unapplicable. For Poké, this is an "amount code"
    // rather than the literal amount (see MONEY_QUANTITY_TABLE)
    uint16_t quantity;
    struct item_id_16 id; // 0x4
};
ASSERT_SIZE(struct item, 6);

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

// Used in menus and the post-dungeon summary.
struct monster_summary {
    struct monster_id_16 id; // 0x0
    char monster_name[10];   // 0x2
    undefined field_0xC;
    undefined field_0xD;
    undefined field_0xE;
    undefined field_0xF;
    undefined field_0x10;
    undefined field_0x11;
    undefined field_0x12;
    undefined field_0x13;
    undefined field_0x14;
    undefined field_0x15;
    struct type_id_8 types[2];        // 0x16
    struct ability_id_8 abilities[2]; // 0x18
    struct dungeon_id_8 joined_at;    // 0x1A
    uint8_t joined_at_floor;          // 0x1B
    struct item held_item;            // 0x1C
    undefined field_0x22;
    undefined field_0x23;
    int32_t hp;                    // 0x24: Current HP
    int32_t max_hp;                // 0x28: Actual max HP (hp + hp boost)
    uint32_t level;                // 0x2C
    int exp;                       // 0x30
    uint8_t offensive_stats[2];    // 0x34: {atk, sp_atk}
    uint8_t defensive_stats[2];    // 0x36: {def, sp_def}
    bool is_team_leader;           // 0x38
    uint8_t attack_boost;          // 0x39: from things like Power Band, Munch Belt
    uint8_t special_attack_boost;  // 0x3A
    uint8_t defense_boost;         // 0x3B
    uint8_t special_defense_boost; // 0x3C
    undefined field_0x3D;
    int16_t iq; // 0x3E
    undefined field_0x40;
    undefined field_0x41;
    // 0x42: Level upon first evolution. Set to 0 in dungeon mode.
    uint8_t level_at_first_evo;
    // 0x43: Level upon first evolution. Set to 0 in dungeon mode.
    uint8_t level_at_second_evo;
    // 0x44: Evolution status. In ground_mode, accounts for luminous spring being unlocked.
    uint8_t evo_status;
    bool inflicted_with_gastro_acid; // 0x45
    undefined field_0x46;
    undefined field_0x47;
    uint32_t iq_skill_flags[3]; // 0x48
    struct tactic_id_8 tactic;  // 0x54
    undefined field_0x55;
    undefined field_0x56;
    undefined field_0x57;
    // 0x58: Appears to be a list of all the currently inflicted statues in their enum form. The
    // last entry (30th) appears to always be STATUS_NONE to serve as a terminator for the list.
    // While in ground mode, it's always filled with STATUS_NONE.
    struct status_id_8 active_statuses[30];
    undefined2 _padding_0x76;
};
ASSERT_SIZE(struct monster_summary, 120);

#endif
