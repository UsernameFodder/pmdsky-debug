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
    // types of missions? (Uncertain)
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

#endif
