#ifndef HEADERS_TYPES_FILES_RESCUE_H_
#define HEADERS_TYPES_FILES_RESCUE_H_

#include "../common/util.h" // (?)
#include "../common/common.h"
#include "../common/enums.h"

// 0x0 in rescue.bin
struct rescue_header {
    struct rescue_str_variant_table* variant_table_pointer;       // 0x00
    struct rescue_str_continuity_table* continuity_table_pointer; // 0x04
    struct rescue_item_tables* item_tables_pointer;               // 0x08
    struct rescue_monster_tables* monster_tables_pointer;         // 0x0C
    struct mission_templates_table* template_table_pointer;       // 0x10
    struct mission_categories_table* category_table_pointer;      // 0x14
    // Likely Padding
    undefined field_0x18;
    undefined field_0x19;
    undefined field_0x1A;
    undefined field_0x1B;
    undefined field_0x1C;
    undefined field_0x1D;
    undefined field_0x1E;
    undefined field_0x1F;
};

ASSERT_SIZE(struct rescue_header, 32);

// Table of rescue_str_variant_group structs, corresponding to the "text_string_offset" field of
// a mission_template struct or equivalently of a rescue_str_continuity struct.
// 0x20 in rescue.bin
struct rescue_str_variant_table {
    struct rescue_str_variant_group string_groups[482];
    struct rescue_str_variant_group unused[6];
};

ASSERT_SIZE(struct rescue_str_variant_table, 1952);

// Table of rescue_str_continuity structs, corresponding to each string in MISSION_TEXT_STRINGS
// and determines what index of the rescue_str_variant_table should follow each one (if any).
// 0x7C0 in rescue.bin
struct rescue_str_continuity_table {
    struct rescue_str_continuity continuities[964];
};

ASSERT_SIZE(struct rescue_str_continuity_table, 1952);

// Item tables used by certain mission templates. This could be considered as one table,
// though in the templates themselves, none of the ranges overlap.
// 0xF48 in rescue.bin
struct rescue_item_tables {
    // [0, 22): The "Common Table" seems to be for easily obtainable items of low value.
    // Used by TAKE_ITEM_FROM_OUTLAW and DELIVER_ITEM templates, as well as the FIND_ITEM
    // template with the MISSION_ITEM_COMMON_TABLE subtype. Mostly Seeds, Berries, and Orbs.
    // Life Seeds, Sitrus Berries, and Reviver Seeds are on this table, for some reason.
    struct item_id_16 common_table[22];
    // [22, 42): The "Rare Table" seems to be for higher-value items, more suitable for their
    // mission types. Used only by the PROSPECT_WITH_CLIENT subtype, as well as the FIND_ITEM
    // template with the MISSION_ITEM_RARE_TABLE subtype. Consists mostly of Gummis, but Lost Loot
    // is also present. Sitrus Berries and Reviver Seeds are ALSO on this table, for some reason.
    struct item_id_16 rare_table[20];
    // If this is an item table, it's always zero and unused by any template.
    uint16_t unused_item_table_entries[2];
};

ASSERT_SIZE(struct rescue_item_tables, 176);

// Pokemon tables used by certain mission templates. This could be considered as one table,
// though barring one exception, none of the ranges overlap.
// 0xFA0 in rescue.bin
struct rescue_monster_tables {
    // [0, 65): Used exclusively for the OUTLAW_NORMAL_2 mission subtype, and consists mostly of
    // middle-stage evolutions, such as Flaafy and Pidgeotto, with a few of the stronger
    // fully-evolved pokemon like Masquerain and Beautifly. Seem to be stronger on average than
    // those in normal_outlaw_3_table.
    struct monster_id_16 normal_outlaw_2_table[65];
    // [65, 106): Used exclusively for the OUTLAW_NORMAL_3 mission subtype, and consists mostly of
    // basic Pokemon, such as Lotad, Teddiursa, and Abra. Also contains all 3 forms of Burmy and
    // Wormadam.
    struct monster_id_16 normal_outlaw_3_table[41];
    // [106, 167): Used exclusively for fleeing outlaws, such as OUTLAW_FLEEING and
    // TAKE_ITEM_FLEEING_OUTLAW. Consists mostly of cowardly Pokemon such as Magikarp, Dunsparce, or
    // Luvdisc, as well as fast Pokemon like Doduo, Ponyta, Ninjask, and Yanmega.
    struct monster_id_16 fleeing_outlaw_table[61];
    // [167, 211): Used exclusively for the OUTLAW_MONSTER_HOUSE mission subtype, and consists
    // mostly of evolved Pokemon such as Tyranitar, Nidoking, Honchkrow, and Metagross. Manetric,
    // Electrike, Chinchou, and Lanturn are also present.
    struct monster_id_16 monster_house_outlaw_table[44];
    // [211, 244): Used for the OUTLAW_ESCORT mission subtype, and consists mostly of intimidating
    // pokemon such as Arbok, Absol, Sharpedo, and Drapion, as well as pseudo-legendaries such as
    // Metagross, Dragonite, Garchomp, Tyranitar, and Salamence. Every Pokemon on this list is fully
    // evolved if it has evolutions. This list is the same regardless of Whether Magnezone or
    // Magnemite accompanies you. Overlaps with the range used by the GUIDE_CLIENT template.
    //
    // [227, 471): Used for the GUIDE_CLIENT mission type, and consists of a wide variety of Pokemon
    // defying categorization. Outside of the overlap, the list seems to contain mostly weaker
    // Pokemon. There is one invalid Pokemon entry in this table, 0x0CBE. Likely intended to be
    // Aipom (0xBE). Should be filtered out by mission generation. Because these two tables overlap,
    // it is most efficient to merge them in this struct.
    struct monster_id_16 magnet_escort_and_guide_client_table[260];
    // [471, 582): Used for the OUTLAW_NORMAL_0 mission type, as well as one of the
    // TAKE_ITEM_NORMAL_OUTLAW templates. Consists mostly of unevolved Pokemon, such as Anorith,
    // Weedle, Magby, and Buizel. Outliers exist, such as Victreebel, Hitmonlee, and Hitmonchan, but
    // the vast majority are unevolved. Seem to be weaker on average than the normal_outlaw_1_table.
    struct monster_id_16 normal_outlaw_0_table[111];
    // [582, 725): Used for the OUTLAW_NORMAL_1 mission type, as well as the other
    // TAKE_ITEM_NORMAL_OUTLAW template. Consists mostly of fully-evolved Pokemon, such as Blaziken,
    // Tyranitar, Torterra, and Gengar. Outliers exists, such as Gligar, Golbat, Cubone, and
    // Cranidos.
    struct monster_id_16 normal_outlaw_1_table[143];
    uint16_t unused_monster_table_entries[11];
};

ASSERT_SIZE(struct rescue_monster_tables, 2944);

// Table of mission templates used for mission generation. A template is chosen from range indicated
// by the selected mission_weighted_category, and GenerateMission converts the template into a
// mission struct. 0x1560 in rescue.bin
struct mission_templates_table {
    struct mission_template[600];
    // This is the perfect size for a mission_category, but not for a template...
    uint16_t unused[4];
};
ASSERT_SIZE(struct mission_templates_table, 20416);

// One of these categories is selected via weighted random whenever the game tries to generate a
// mission. Each category corresponds to a range of templates in mission_template_table, which
// contains the specifics of the missions themselves. This struct is primarily used for generation
// restrictions. 0x6520 in rescue.bin
struct mission_categories_table {
    struct mission_weighted_category mission_categories[39];
    // This category is never read, but that might be a one-instruction change.
    struct mission_weighted_category unused_category;
};

ASSERT_SIZE(struct mission_categories_table, 640);
