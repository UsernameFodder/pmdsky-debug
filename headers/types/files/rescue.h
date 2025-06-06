#ifndef HEADERS_TYPES_FILES_RESCUE_H_
#define HEADERS_TYPES_FILES_RESCUE_H_

#include "../common/util.h" // (?)
#include "../common/common.h"
#include "../common/enums.h"

// 0x0 in rescue.bin
struct rescue_header {
    struct rescue_str_variant_table* variant_table_pointer; // 0x00
    struct rescue_str_continuity_table* continuity_table_pointer; // 0x04
    struct rescue_item_tables* item_tables_pointer; // 0x08
    struct rescue_monster_tables* monster_tables_pointer; // 0x0C
    struct mission_templates_table* template_table_pointer; // 0x10
    struct mission_categories_table* category_table_pointer; // 0x14
    // Likely Padding
    undefined field1_0x18;
    undefined field1_0x19;
    undefined field1_0x1A;
    undefined field1_0x1B;
    undefined field1_0x1C;
    undefined field1_0x1D;
    undefined field1_0x1E;
    undefined field1_0x1F;
};

ASSERT_SIZE(struct rescue_header, 32);


// 0x20 in rescue.bin
struct rescue_str_variant_table {
    struct rescue_str_variant_group string_groups[481];
    uint32_t padding[7]; // 0x00000000
}

ASSERT_SIZE(struct rescue_str_variant_table, 1952);



struct rescue_str_variant_group {
    // 0x0: Index of MISSION_STRING_IDS the group starts from. 
    uint16_t starting_index;
    // 0x2: Number of other MISSION_STRING_ID entries the group contains. A random int on the range 
    // [0, group_size) is added to starting_index to produce the final MISSION_STRING_ID entry.
    // This final index is then used again to select an index in rescue_str_continuity_table
    uint16_t group_size;
}
ASSERT_SIZE(struct rescue_str_variant_group, 4);


// 0x7C0 in rescue.bin
struct rescue_str_continuity_table {
    struct rescue_str_continuity continuities[964];
}

ASSERT_SIZE(struct rescue_str_continuity_table, 1952);


// Determines the entry in rescue_str_variant_table for the next string in sequence, if any.
struct rescue_str_continuity {
    /* This field is interpreted as follows:
     * If 0xFFFF, there is no next string.
     * If 0x1NNN, index 0xNNN is for a summary string, NOT a title string.
     * If 0x0NNN, index 0xNNN is for a title string, NOT a summary string.
    */
    uint16_t next_variant_table_id;
}

ASSERT_SIZE(struct rescue_str_continuity, 4);


// Item tables used by certain mission templates. This could be considered as one table,
// though in the templates themselves, none of the ranges overlap. 
// 0xF48 in rescue.bin
struct rescue_item_tables {
    // [0, 22): The "Common Table" seems to be for easily obtainable items of low value. 
    // Used by TAKE_ITEM_FROM_OUTLAW and DELIVER_ITEM templates, as well as the FIND_ITEM 
    // template with the MISSION_ITEM_COMMON_TABLE subtype. Mostly Seeds, Berries, and Orbs.
    // Life Seeds, Sitrus Berries, and Reviver Seeds are on this table, for some reason.
    struct item_id_16 common_table[22];
    // [22, 42): The "Rare Table" seems to be for higher-value items, more suitable for their mission types.
    // Used only by the PROSPECT_WITH_CLIENT subtype, as well as the FIND_ITEM template with the 
    // MISSION_ITEM_RARE_TABLE subtype. Consists mostly of Gummis, but Lost Loot is also present.
    // Sitrus Berries and Reviver Seeds are ALSO on this table, for some reason.
    struct item_id_16 rare_table[20];
    // If this is an item table, it's always zero and unused by any template.
    uint16_t padding[2];
}

ASSERT_SIZE(struct rescue_item_tables, 176);

// Pokemon tables used by certain mission templates. This could be considered as one table,
// though barring one exception, none of the ranges overlap. 
// 0xFA0 in rescue.bin
struct rescue_monster_tables {
    // [0, 65): Used exclusively for the OUTLAW_NORMAL_2 mission subtype, and consists mostly of middle-stage
    // evolutions, such as Flaafy and Pidgeotto, with a few of the stronger fully-evolved pokemon like Masquerain and Beautifly.
    // Seem to be stronger on average than those in normal_outlaw_3_table.
    struct monster_id_16 normal_outlaw_2_table[65];
    // [65, 106): Used exclusively for the OUTLAW_NORMAL_2 mission subtype, and consists mostly of basic
    // Pokemon, such as Lotad, Teddiursa, and Abra. Also contains all 3 forms of Burmy and Wormadam.
    struct monster_id_16 normal_outlaw_3_table[41];
    // [106, 167): Used exclusively for fleeing outlaws, such as OUTLAW_FLEEING and TAKE_ITEM_FLEEING_OUTLAW.
    // Consists mostly of cowardly Pokemon such as Magikarp, Dunsparce, or Luvdisc, as well as fast Pokemon like
    // Doduo, Ponyta, Ninjask, and Yanmega. 
    struct monster_id_16 fleeing_outlaw_table[61];
    // [167, 211): Used exclusively for the OUTLAW_MONSTER_HOUSE mission subtype, and consists mostly of evolved Pokemon
    // such as Tyranitar, Nidoking, Honchkrow, and Metagross. Manetric, Electrike, Chinchou, and Lanturn are also present.
    struct monster_id_16 monster_house_outlaw_table[44];
    // [211, 244): Used for the OUTLAW_ESCORT mission subtype, and consists mostly of intimidating pokemon such as
    // Arbok, Absol, Sharpedo, and Drapion, as well as pseudo-legendaries such as Metagross, Dragonite, Garchomp, Tyranitar,
    // and Salamence. Every Pokemon on this list is fully evolved if it has evolutions. This list is the same regardless of
    // Whether Magnezone or Magnemite accompanies you. Overlaps with the range used by the GUIDE_CLIENT template.
    
    // [227, 471): Used for the GUIDE_CLIENT mission type, and consists of a wide variety of Pokemon defying categorization.
    // Outside of the overlap, the list seems to contain mostly weaker Pokemon. There is one invalid Pokemon entry in this table, 0x0CBE. 
    // Likely intended to be Aipom (0xBE). Should be filtered out by mission generation. 
    // Because these two tables overlap, it is most efficient to merge them in this struct.
    struct monster_id_16 magnet_escort_and_guide_client_table[260];
    // [471, 582): Used for the OUTLAW_NORMAL_0 mission type, as well as one of the TAKE_ITEM_NORMAL_OUTLAW templates.
    // Consists mostly of unevolved Pokemon, such as Anorith, Weedle, Magby, and Buizel. Outliers exist, such as Victreebel,
    // Hitmonlee, and Hitmonchan, but the vast majority are unevolved. Seem to be weaker on average than the normal_outlaw_1_table.
    struct monster_id_16 normal_outlaw_0_table[111];
    // [582, 725): Used for the OUTLAW_NORMAL_1 mission type, as well as the other TAKE_ITEM_NORMAL_OUTLAW template.
    // Consists mostly of fully-evolved Pokemon, such as Blaziken, Tyranitar, Torterra, and Gengar. Outliers exists,
    // such as Gligar, Golbat, Cubone, and Cranidos.
    struct monster_id_16 normal_outlaw_1_table[143];
    uint16_t padding[11];
}

ASSERT_SIZE(struct rescue_monster_tables, 2944);




struct mission_templates_table {
    struct mission_template hideout_magnemite_1[8];
    struct mission_template hideout_magnemite_2[5];
    struct mission_template hideout_magnezone_1[9];
    struct mission_template hideout_magnezone_2[8];
    struct mission_template outlaw_monster_house[1];
    struct mission_template magnemite_escort[1];
    struct mission_template magnezone_escort[1];
    struct mission_template arrest_fleeing[1];
    struct mission_template arrest_normal_0[1];
    struct mission_template arrest_normal_1[1];
    struct mission_template arrest_normal_2[1];
    struct mission_template arrest_normal_3[1];
    struct mission_template escort_random[1];
    struct mission_template escort_specific[12];
    struct mission_template explore_normal[1];
    struct mission_template explore_sealed[1];
    struct mission_template explore_golden[1];
    struct mission_template explore_new[1];
    struct mission_template prospect_random[1];
    struct mission_template prospect_fixed[6];
    struct mission_template guide_client[1];
    struct mission_template rescue_client[1];
    struct mission_template rescue_target_random[1];
    struct mission_template rescue_target_child[18];
    struct mission_template rescue_friends[24];
    struct mission_template rescue_lovers_rivals[23];
    struct mission_template search_for_target[1];
    struct mission_template challenge_mewtwo[1];
    struct mission_template challenge_entei[1];
    struct mission_template challenge_raikou[1];
    struct mission_template challenge_suicune[1];
    struct mission_template challenge_jirachi[1];
    struct mission_template challenge_other[28];
    struct mission_template take_outlaw_0[1];
    struct mission_template take_outlaw_1[1];
    struct mission_template take_hidden_outlaw[1];
    struct mission_template take_fleeing_outlaw[1];
    struct mission_template find_item_common[1];
    struct mission_template find_item_rare[1];
    struct mission_template find_favorite_gummi[324];
    struct mission_template find_evo_item[43];
    struct mission_template deliver_item[1];
    struct mission_template seven_treasures[7];
    struct mission_template gabite_scale[1];
    struct mission_template treasure_memo[54];
    uint16 padding[4];
}
ASSERT_SIZE(struct mission_templates_table, 20416);


// One of these categories is selected via weighted random whenever the game tries to generate a mission.
// Each category corresponds to a range of templates in mission_template_table, which contains the specifics
// of the missions themselves. This struct is primarily used for generation restrictions. 
struct mission_categories_table {
    // [0]: Outlaw Hideout (0/10/10/0) Requirements: {None} Templates: [0, 30)
    struct mission_weighted_category cat_hideout;
    // [1]: Outlaw Monster House (0/10/5/0) Requirements: {None} Templates: [30, 31)
    struct mission_weighted_category cat_monster_house;
    // [2]: Outlaw Escort Magnezone/Magnemite (0/10/5/0) Requirements: {Rank >= Gold} Templates: [31, 33)
    struct mission_weighted_category cat_escort_magnet;
    // [3]: Outlaw Fleeing (0/10/10/0) Requirements: {None} Templates: [33, 34)
    struct mission_weighted_category cat_fleeing_outlaw;
    // [4]: Outlaw Normal 0 (0/10/10/0) Requirements: {None} Templates: [34, 35)
    struct mission_weighted_category cat_outlaw_norm_0;
    // [5]: Outlaw Normal 1 (0/10/10/0) Requirements: {None} Templates: [35, 36)
    struct mission_weighted_category cat_outlaw_norm_1;
    // [6]: Outlaw Normal 2 (0/10/10/0) Requirements: {None} Templates: [36, 37)
    struct mission_weighted_category cat_outlaw_norm_2;
    // [7]: Outlaw Normal 3 (0/10/10/0) Requirements: {None} Templates: [37, 38)
    struct mission_weighted_category cat_outlaw_norm_3;
    // [8]: Escort To Target (Random) (10/0/10/0) Requirements: {Rank >= Bronze} Templates: [38, 39)
    struct mission_weighted_category cat_escort_random;
    // [9]: Escort to Target (Specific) (10/0/10/0) Requirements: {Rank >= Silver} Templates: [39, 51)
    struct mission_weighted_category cat_escort_specific;
    // [10]: Explore Client Normal (10/0/10/0) Requirements: {Rank >= Gold} Templates: [51, 52)
    struct mission_weighted_category cat_explore_normal;
    // [11]: Explore Client Sealed Chamber (5/0/5/0) Requirements: {Rank >= Diamond} Templates: [52, 53)
    struct mission_weighted_category cat_explore_sealed;
    // [12]: Explore Client Gold Chamber (5/0/5/0) Requirements: {Rank >= Diamond} Templates: [53, 54)
    struct mission_weighted_category cat_explore_gold;
    // [13]: Explore Client New Dungeon (10/0/10/0) Requirements: {Rank >= Bronze} Templates: [54, 55)
    struct mission_weighted_category cat_explore_new;
    // [14]: Prospect With Client (5/0/5/0) Requirements: {Rank >= Diamond} Templates: [55, 62)
    struct mission_weighted_category cat_prospect;
    // [15]: Guide Client (10/0/10/0) Requirements: {Rank >= Silver} Templates: [62, 63)
    struct mission_weighted_category cat_guide_client;
    // [16]: Rescue Client (10/0/0/10) Requirements: {None} Templates: [63, 64)
    struct mission_weighted_category cat_rescue_random_client;
    // [17]: Rescue Target (Random) (10/0/10/0) Requirements: {None} Templates: [64, 65)
    struct mission_weighted_category cat_rescue_random_target;
    // [18]: Rescue Target (Child) (10/0/10/0) Requirements: {None} Templates: [65, 83)
    struct mission_weighted_category cat_rescue_child;
    // [19]: Rescue Species (10/0/10/0) Requirements: {None} Templates: [83, 107)
    struct mission_weighted_category cat_rescue_species;
    // [20]: Rescue Lover/Rival (10/0/10/0) Requirements: {None} Templates: [107, 130)
    struct mission_weighted_category cat_rescue_lover_rival;
    // [21]: Search For Target (10/0/10/0) Requirements: {None} Templates: [130, 131)
    struct mission_weighted_category cat_search_target;
    // [22]: Challenge Mewtwo (0/0/5/0) Requirements: {Secret Rank} Templates: [131, 132)
    struct mission_weighted_category cat_challenge_mewtwo;
    // [23]: Challenge Entei (0/0/5/0) Requirements: {Secret Rank} Templates: [132, 133)
    struct mission_weighted_category cat_challenge_entei;
    // [24]: Challenge Raikou (0/0/5/0) Requirements: {Secret Rank} Templates: [133, 134)
    struct mission_weighted_category cat_challenge_raikou;
    // [25]: Challenge Suicune (0/0/5/0) Requirements: {Secret Rank} Templates: [134, 135)
    struct mission_weighted_category cat_challenge_suicune;
    // [26]: Challenge Jirachi (0/0/5/0) Requirements: {Secret Rank} Templates: [135, 136)
    // Additional requirements are checked during generation!
    struct mission_weighted_category cat_challenge_jirachi;
    // [27]: Challenge Normal (0/0/10/0) Requirements: {None} Templates: [136, 164)
    struct mission_weighted_category cat_challenge_other;
    // [28]: Take From Outlaw (0/10/10/0) Requirements: {None} Templates: [164, 166)
    struct mission_weighted_category cat_take_outlaw_norm;
    // [29]: Take Hidden Outlaw (0/10/10/0) Requirements: {None} Templates: [166, 167)
    struct mission_weighted_category cat_take_outlaw_hidden;
    // [30]: Take Fleeing Outlaw (0/10/10/0) Requirements: {None} Templates: [167, 168)
    struct mission_weighted_category cat_take_outlaw_fleeing;
    // [31]: Find Item (Random) (10/0/10/0) Requirements: {None} Templates: [168, 169)
    struct mission_weighted_category cat_find_item_random;
    // [32]: Find Item (Table) (5/0/5/0) Requirements: {None} Templates: [169, 170)
    struct mission_weighted_category cat_find_table_item;
    // [33]: Find Item Gummi (10/0/10/0) Requirements: {None} Templates: [170, 494)
    struct mission_weighted_category cat_find_specific_gummi;
    // [34]: Find Item Evolve (3/0/5/0) Requirements: {None} Templates: [494, 537)
    struct mission_weighted_category cat_find_evo_item;
    // [35]: Deliver Item (Random) (10/0/0/10) Requirements: {None} Templates: [537, 538)
    struct mission_weighted_category cat_deliver_item_random;
    // [36]: 7 Treasure Mission (0/0/300/0) Requirements: {Secret Rank} Templates: [538, 545)
    struct mission_weighted_category cat_7_treasure_mission;
    // [37]: Gabite Scale (0/0/150/0) Requirements: {Balance Flag >= 2} Templates: [545, 546)
    struct mission_weighted_category cat_gabite_scale_mission;
    // [38]: Treasure Memo (0/0/0/10) Requirements: {None} Templates: [546, 600)
    struct mission_weighted_category cat_treasure_memo;
    // [39]: No Job (Failsafe) (0/0/0/0) Requirements: {None} Templates: [0, 0)
    // This category is used in the event that no other categories are available.
    struct mission_weighted_category cat_failsafe_job;
}

ASSERT_SIZE(struct mission_categories_table, 640);

