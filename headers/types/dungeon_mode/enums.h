// Enums used primarily in the context of dungeon mode

#ifndef HEADERS_TYPES_DUNGEON_MODE_ENUMS_H_
#define HEADERS_TYPES_DUNGEON_MODE_ENUMS_H_

// Direction on the dungeon grid
enum direction_id {
    DIR_NONE = -1,
    DIR_DOWN = 0,
    DIR_DOWN_RIGHT = 1,
    DIR_RIGHT = 2,
    DIR_UP_RIGHT = 3,
    DIR_UP = 4,
    DIR_UP_LEFT = 5,
    DIR_LEFT = 6,
    DIR_DOWN_LEFT = 7,
    DIR_CURRENT = 8, // Current direction of an entity. Used as a special value in some functions
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(direction_id);
#pragma pack(pop)

// Terrain type for a tile
enum terrain_type {
    TERRAIN_WALL = 0,
    TERRAIN_NORMAL = 1,
    TERRAIN_SECONDARY = 2, // Water or lava
    TERRAIN_CHASM = 3,
};

// Secondary terrain type for a dungeon
enum secondary_terrain_type {
    SECONDARY_TERRAIN_WATER = 0,
    SECONDARY_TERRAIN_LAVA = 1,
    SECONDARY_TERRAIN_CHASM = 2,
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(secondary_terrain_type);
#pragma pack(pop)

// Mobility types for monsters
enum mobility_type {
    MOBILITY_NORMAL = 0,
    // This is unused within the monster data in monster.md, but is probably used in dungeons
    // with a meaning that's dependent on what the secondary terrain type is
    // (either MOBILITY_LAVA or MOBILITY_WATER).
    MOBILITY_SECONDARY = 1,
    MOBILITY_HOVERING = 2,
    MOBILITY_INTANGIBLE = 3,
    MOBILITY_LAVA = 4,
    MOBILITY_WATER = 5,
};

// Entity type. This is used to tag generic entity pointers.
enum entity_type {
    ENTITY_NOTHING = 0,
    ENTITY_MONSTER = 1,
    ENTITY_TRAP = 2,
    ENTITY_ITEM = 3,
    ENTITY_HIDDEN_STAIRS = 5,
    // Used when a temporary instance of this struct is created to pass it to some
    // function that requires it as a parameter
    ENTITY_TEMPORARY = 6,
};

// Trap ID
enum trap_id {
    TRAP_NULL_TRAP = 0,
    TRAP_MUD_TRAP = 1,
    TRAP_STICKY_TRAP = 2,
    TRAP_GRIMY_TRAP = 3,
    TRAP_SUMMON_TRAP = 4,
    TRAP_PITFALL_TRAP = 5,
    TRAP_WARP_TRAP = 6,
    TRAP_GUST_TRAP = 7,
    TRAP_SPIN_TRAP = 8,
    TRAP_SLUMBER_TRAP = 9,
    TRAP_SLOW_TRAP = 10,
    TRAP_SEAL_TRAP = 11,
    TRAP_POISON_TRAP = 12,
    TRAP_SELFDESTRUCT_TRAP = 13,
    TRAP_EXPLOSION_TRAP = 14,
    TRAP_PP_ZERO_TRAP = 15,
    TRAP_CHESTNUT_TRAP = 16,
    TRAP_WONDER_TILE = 17,
    TRAP_POKEMON_TRAP = 18,
    TRAP_SPIKED_TILE = 19,
    TRAP_STEALTH_ROCK = 20,
    TRAP_TOXIC_SPIKES = 21,
    TRAP_TRIP_TRAP = 22,
    TRAP_RANDOM_TRAP = 23,
    TRAP_GRUDGE_TRAP = 24,
    // Used in fixed room data to indicate that a trap shouldn't be placed on the current tile
    TRAP_NONE = 25,
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(trap_id);
#pragma pack(pop)

// The type of hidden stairs (i.e. where it leads), if present
enum hidden_stairs_type {
    HIDDEN_STAIRS_NONE = 0,
    HIDDEN_STAIRS_SECRET_BAZAAR = 1,
    HIDDEN_STAIRS_SECRET_ROOM = 2,
    HIDDEN_STAIRS_RANDOM_SECRET_BAZAAR_OR_SECRET_ROOM = 255,
};

// Different types of warp effects
enum warp_type {
    WARP_RANDOM = 0,         // Warp to a random position
    WARP_STAIRS_2 = 1,       // Warp within 2 tiles of the stairs
    WARP_POSITION_FUZZY = 2, // Warp within 2 tiles of a specified position
    WARP_POSITION_EXACT = 3, // Warp to an exact position
    WARP_STAIRS_3 = 4,       // Warp within 3 tiles of the stairs
    WARP_LEADER = 5,         // Warp within 2 tiles of the leader
};

// Status IDs
// While there seems to be a global status order, this enum isn't used exactly that way in
// practice. See the comments for struct status.
enum status_id {
    STATUS_NONE = 0, // Called "-" internally
    STATUS_SLEEP = 1,
    STATUS_SLEEPLESS = 2, // Won't get sleepy
    STATUS_NIGHTMARE = 3, // Caught in a nightmare
    STATUS_YAWNING = 4,
    STATUS_NAPPING = 5,
    STATUS_LOW_HP = 6, // Low HP! Situation critical!
    STATUS_BURN = 7,   // Burned
    STATUS_POISONED = 8,
    STATUS_BADLY_POISONED = 9,
    STATUS_PARALYSIS = 10, // Paralyzed
    STATUS_IDENTIFYING = 11,
    STATUS_FROZEN = 12,
    STATUS_SHADOW_HOLD = 13, // Immobilized
    STATUS_WRAP = 14,        // Wrapped around foe
    STATUS_WRAPPED = 15,     // Wrapped by foe
    STATUS_INGRAIN = 16,     // Using Ingrain
    STATUS_PETRIFIED = 17,
    STATUS_CONSTRICTION = 18, // Being squeezed
    STATUS_FAMISHED = 19,     // About to drop from hunger
    STATUS_CRINGE = 20,       // Cringing
    STATUS_CONFUSED = 21,
    STATUS_PAUSED = 22, // Pausing
    STATUS_COWERING = 23,
    STATUS_TAUNTED = 24,
    STATUS_ENCORE = 25, // Afflicted with Encore
    STATUS_INFATUATED = 26,
    STATUS_DOUBLE_SPEED = 27, // Sped up
    STATUS_BIDE = 28,         // Biding
    STATUS_SOLARBEAM = 29,    // Charging SolarBeam
    STATUS_SKY_ATTACK = 30,   // Charging Sky Attack
    STATUS_RAZOR_WIND = 31,   // Charging Razor Wind
    STATUS_FOCUS_PUNCH = 32,  // Charging Focus Punch
    STATUS_SKULL_BASH = 33,   // Charging Skull Bash
    STATUS_FLYING = 34,       // Flying high up
    STATUS_BOUNCING = 35,
    STATUS_DIVING = 36,       // Hiding underwater
    STATUS_DIGGING = 37,      // Burrowing underground
    STATUS_CHARGING = 38,     // Using Charge
    STATUS_ENRAGED = 39,      // Shaking with rage
    STATUS_SHADOW_FORCE = 40, // Hiding among shadows
    STATUS_HALF_SPEED = 41,   // Slowed down
    STATUS_REFLECT = 42,      // Protected by Reflect
    STATUS_SAFEGUARD = 43,    // Protected by Safeguard
    STATUS_LIGHT_SCREEN = 44, // Protected by Light Screen
    STATUS_COUNTER = 45,      // Ready to counter
    STATUS_MAGIC_COAT = 46,   // Protected by Magic Coat
    STATUS_WISH = 47,         // Making a wish
    STATUS_PROTECT = 48,      // Protecting itself
    STATUS_MIRROR_COAT = 49,  // Protected by Mirror Coat
    STATUS_ENDURING = 50,     // Set to endure
    STATUS_MINI_COUNTER = 51, // Ready to deliver mini counters
    STATUS_MIRROR_MOVE = 52,  // Using Mirror Move
    STATUS_CONVERSION2 = 53,  // Alt Using Conversion 2
    STATUS_VITAL_THROW = 54,  // Ready to use Vital Throw
    STATUS_MIST = 55,         // Protected by Mist
    STATUS_METAL_BURST = 56,  // Protected by Metal Burst
    STATUS_AQUA_RING = 57,    // Cloaked by Aqua Ring
    STATUS_LUCKY_CHANT = 58,  // Lucky Chant in effect
    STATUS_WEAKENED = 59,
    STATUS_CURSED = 60,
    STATUS_DECOY = 61,
    STATUS_SNATCH = 62,       // Ready to snatch moves
    STATUS_GASTRO_ACID = 63,  // Drenched with Gastro Acid
    STATUS_HEAL_BLOCK = 64,   // Prevented from healing
    STATUS_EMBARGO = 65,      // Under Embargo
    STATUS_HUNGRY_PAL = 66,   // Immobilized by hunger
    STATUS_LEECH_SEED = 67,   // Afflicted with Leech Seed
    STATUS_DESTINY_BOND = 68, // Using Destiny Bond
    STATUS_POWERED_UP = 69,
    STATUS_SURE_SHOT = 70,    // Total accuracy for moves
    STATUS_WHIFFER = 71,      // Afflicted with Smokescreen
    STATUS_SET_DAMAGE = 72,   // Inflicts set damage
    STATUS_FOCUS_ENERGY = 73, // Enhanced critical-hit rate
    STATUS_UNNAMED_0x4A = 74, // Called "-" internally
    STATUS_LONG_TOSS = 75,    // Throws thrown items far
    STATUS_PIERCE = 76,       // Pierces walls w/ thrown items
    STATUS_UNNAMED_0x4D = 77,
    STATUS_INVISIBLE = 78,
    STATUS_TRANSFORMED = 79, // Transformed Pokemon
    STATUS_MOBILE = 80,      // Travel anywhere
    STATUS_SLIP = 81,        // Walk on water
    STATUS_UNNAMED_0x52 = 82,
    STATUS_BLINKER = 83,    // Blinded
    STATUS_CROSS_EYED = 84, // Hallucinating
    STATUS_EYEDROPS = 85,   // Seeing the unseeable
    STATUS_DROPEYE = 86,    // Poor vision
    STATUS_UNNAMED_0x57 = 87,
    STATUS_MUZZLED = 88, // Unable to use its mouth
    STATUS_UNNAMED_0x59 = 89,
    STATUS_MIRACLE_EYE = 90, // Exposed by Miracle Eye
    STATUS_UNNAMED_0x5B = 91,
    STATUS_MAGNET_RISE = 92, // Levitating with Magnet Rise
    STATUS_STOCKPILING = 93,
    STATUS_POWER_EARS = 94, // Can locate other Pokemon
    STATUS_SCANNING = 95,   // Can locate items
    STATUS_GRUDGE = 96,     // Bearing a grudge
    STATUS_EXPOSED = 97,    // Exposed to sight
    STATUS_TERRIFIED = 98,
    STATUS_PERISH_SONG = 99,     // Received Perish Song
    STATUS_DOUBLED_ATTACK = 100, // Has sped-up attacks
    STATUS_STAIR_SPOTTER = 101,  // Can locate stairs
};

// Tactic ID. These are usually encoded as bitvectors.
enum tactic_id {
    TACTIC_LETS_GO_TOGETHER = 0,
    TACTIC_YOU_GO_THE_OTHER_WAY = 1,
    TACTIC_GO_AFTER_FOES = 2,
    TACTIC_AVOID_THE_FIRST_HIT = 3,
    TACTIC_ALL_FOR_ONE = 4,
    TACTIC_GROUP_SAFETY = 5,
    TACTIC_AVOID_TROUBLE = 6,
    TACTIC_BE_PATIENT = 7,
    TACTIC_KEEP_YOUR_DISTANCE = 8,
    TACTIC_WAIT_THERE = 9,
    TACTIC_GET_AWAY_FROM_HERE = 10,
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(tactic_id);
#pragma pack(pop)

// Modes related to shopkeeper behavior
enum shopkeeper_mode {
    SHOPKEEPER_MODE_NORMAL = 0,
    SHOPKEEPER_MODE_SHOPKEEPER = 1,
    SHOPKEEPER_MODE_ATTACK_ENEMIES = 2,
    SHOPKEEPER_MODE_ATTACK_TEAM = 3,
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(shopkeeper_mode);
#pragma pack(pop)

// Behavior type of NPC monsters
enum monster_behavior {
    BEHAVIOR_NORMAL_ENEMY_0x0 = 0,
    BEHAVIOR_OUTLAW = 1,
    BEHAVIOR_HIDDEN_OUTLAW = 2,
    BEHAVIOR_FLEEING_OUTLAW = 3,
    BEHAVIOR_OUTLAW_TEAM_LEADER = 4,
    BEHAVIOR_OUTLAW_TEAM_MINION = 5,
    BEHAVIOR_FIXED_ENEMY = 6,
    BEHAVIOR_RESCUE_TARGET = 7,
    BEHAVIOR_WANDERING_ENEMY_0x8 = 8,
    BEHAVIOR_NORMAL_ENEMY_0x9 = 9,
    BEHAVIOR_ALLY = 10,
    BEHAVIOR_EXPLORER_MAZE_1 = 11,
    BEHAVIOR_EXPLORER_MAZE_2 = 12,
    BEHAVIOR_EXPLORER_MAZE_3 = 13,
    BEHAVIOR_EXPLORER_MAZE_4 = 14,
    BEHAVIOR_INVALID = 15,
    BEHAVIOR_SECRET_BAZAAR_KIRLIA = 16,
    BEHAVIOR_SECRET_BAZAAR_MIME_JR = 17,
    BEHAVIOR_SECRET_BAZAAR_SWALOT = 18,
    BEHAVIOR_SECRET_BAZAAR_LICKILICKY = 19,
    BEHAVIOR_SECRETE_BAZAAR_SHEDINJA = 20,
    BEHAVIOR_WANDERING_ENEMY_0x15 = 21,
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(monster_behavior);
#pragma pack(pop)

// AI objective that controls how an AI acts in the moment.
// These were probably taken from Rescue Team; need to confirm validity in Explorers.
enum ai_objective {
    AI_UNKNOWN = 0,
    AI_CHASE_TARGET = 1,
    AI_CHASE_REMEMBERED_TARGET = 2, // Chase a target using a previous position
    AI_ROAM = 3,                    // Roam hallways
    AI_LEAVE_ROOM = 4,              // Get out of a room
    AI_RUN_AWAY = 5,
    AI_STAND_STILL = 6,
    AI_TAKE_ITEM = 7,
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(ai_objective);
#pragma pack(pop)

// Actions that a monster can take on its turn
enum action {
    ACTION_NOTHING = 0, // 0x0
    // 0x1: Pass turn. Enemies will print the "is watching carefully" message when performing this
    // action
    ACTION_PASS_TURN = 1,
    ACTION_WALK = 2, // 0x2: Walk
    ACTION_UNK_3 = 3,
    ACTION_UNK_4 = 4,
    ACTION_UNK_5 = 5,
    ACTION_NEXT = 6,           // 0x6: Switch to next Pokémon when viewing moves
    ACTION_PREVIOUS = 7,       // 0x7: Switch to previous Pokémon when viewing moves
    ACTION_PLACE_ITEM = 8,     // 0x8: Place an item on the floor
    ACTION_PICK_UP_PLAYER = 9, // 0x9: Pick up an item from the floor (player)
    ACTION_UNK_A = 10,
    ACTION_THROW_ITEM_PLAYER = 11,      // 0xB: Throw an item
    ACTION_CHECK_ITEM_DESCRIPTION = 12, // 0xC: Check the description of an item
    ACTION_EAT_BERRY_SEED_DRINK = 13,   // 0xD: Each a berry or seed, or drink something
    ACTION_EAT_GUMMI = 14,              // 0xE: Eat a gummi
    ACTION_UNK_F = 15,
    ACTION_USE_TM = 16, // 0x10: Use a TM
    ACTION_UNK_11 = 17,
    ACTION_USE_ITEM = 18,        // 0x12: Use an item
    ACTION_TALK_FIELD = 19,      // 0x13: Talk to a teammate by facing them and pressing A
    ACTION_USE_MOVE_PLAYER = 20, // 0x14: Use a move (player)
    ACTION_USE_MOVE_AI = 21,     // 0x15: Use a move (AI)
    ACTION_UNK_16 = 22,
    ACTION_STRUGGLE = 23, // 0x17: Use Struggle
    ACTION_UNK_18 = 24,
    ACTION_CHECK_MOVES = 25,    // 0x19: Check the list of moves
    ACTION_CHANGE_TACTICS = 26, // 0x1A: Change a teammate's tactic
    ACTION_CHECK_SUMMARY = 27,  // 0x1B: Check a pokémon's summary
    ACTION_TALK_MENU = 28,      // 0x1C: Talk to a teammate by using the in-menu option
    ACTION_UNK_1D = 29,
    ACTION_SET_MOVE = 30,    // 0x1E: Set a move
    ACTION_SWITCH_MOVE = 31, // 0x1F: Switch a move on or off
    ACTION_UNK_20 = 32,
    ACTION_UNK_21 = 33,
    ACTION_UNK_22 = 34,
    ACTION_EAT_AI = 35,        // 0x23: Eat item (AI)
    ACTION_THROW_ITEM_AI = 36, // 0x24: Throw an item (AI)
    ACTION_UNK_25 = 37,
    ACTION_USE_STAIRS = 38,            // 0x26: Use the stairs
    ACTION_THROW_STRAIGHT_PLAYER = 39, // 0x27: Throw a straight-line item (player)
    ACTION_UNK_28 = 40,
    ACTION_UNK_29 = 41,
    ACTION_UNK_2A = 42,
    ACTION_QUICKSAVE = 43,    // 0x2B: Quicksave
    ACTION_USE_LINK_BOX = 44, // 0x2C: Use a Link Box
    ACTION_UNK_2D = 45,
    ACTION_GIVE_UP = 46, // 0x2E: Give up
    ACTION_UNK_2F = 47,
    ACTION_VIEW_IQ = 48,        // 0x30: Open the IQ skills menu
    ACTION_USE_ORB = 49,        // 0x31: Use an orb
    ACTION_REGULAR_ATTACK = 50, // 0x32: Regular Attack
    ACTION_UNSET_MOVE = 51,     // 0x33: Unset a move
    ACTION_SEND_HOME = 52,      // 0x34: Send a pokémon home
    ACTION_UNK_35 = 53,
    ACTION_GIVE_ITEM = 54, // 0X36: Give held item
    ACTION_TAKE_ITEM = 35, // 0x37: Take a held item
    ACTION_UNK_38 = 36,
    // 0x39: <pokémon> had second thoughts! - Used as a failsafe when trying to take an illegal
    // action
    ACTION_SECOND_THOUGHTS = 37,
    ACTION_SWAP_ITEM = 38,     // 0x3A: Swapping an item
    ACTION_CHANGE_LEADER = 39, // 0x3B: Change the team's leader
    ACTION_SET_ITEM = 40,      // 0x3C: Set item
    ACTION_UNSET_ITEM = 41,    // 0x3D: Unset item
    ACTION_UNK_3E = 42,
    ACTION_PICK_UP_AI = 43, // 0x3F: Pick up an item (AI)
    ACTION_UNK_40 = 44,
    ACTION_THROW_ARC_PLAYER = 45, // 0x41: Throw a Gravelerock or a similar item (player)
};

// This is usually stored as a 16-bit integer
#pragma pack(push, 2)
ENUM_16_BIT(action);
#pragma pack(pop)

// Potential sources of damage dealt to monsters
enum damage_source {
    DAMAGE_SOURCE_MOVE = 0, // "Took X damage"
    DAMAGE_SOURCE_BURN = 1,
    DAMAGE_SOURCE_CONSTRICTION = 2, // "Was squeezed for X damage"
    DAMAGE_SOURCE_POISON = 3,
    DAMAGE_SOURCE_RECOIL_1 = 4, // User deals damage to itself because of their own recoil move
    DAMAGE_SOURCE_WRAP = 5,     // "Was wrapped for X damage"
    DAMAGE_SOURCE_COUNTER = 6,  // Damage taken from a conunterattack
    DAMAGE_SOURCE_CURSE = 7,
    DAMAGE_SOURCE_NIGHTMARE = 8, // Damage taken when awakening from a nightmare
    DAMAGE_SOURCE_LEECH_SEED = 9,
    DAMAGE_SOURCE_SPIKES = 10,
    DAMAGE_SOURCE_PERISH_SONG = 11,
    DAMAGE_SOURCE_DESTINY_BOND = 12,
    DAMAGE_SOURCE_SLUDGE = 13, // "Was showered with sludge for X damage"
    DAMAGE_SOURCE_CHESTNUT_1 = 14,
    DAMAGE_SOURCE_CHESTNUT_2 = 15, // Same string as DAMAGE_SOURCE_CHESTNUT_1
    DAMAGE_SOURCE_UNK16 = 16,      // Same string as DAMAGE_SOURCE_MOVE
    DAMAGE_SOURCE_BAD_WEATHER = 17,
    DAMAGE_SOURCE_MISSED_MOVE = 18, // Damage taken from moves that hurt the user when they miss
    DAMAGE_SOURCE_RECOIL_2 = 19,    // Same string as DAMAGE_SOURCE_RECOIL_1
    DAMAGE_SOURCE_STEALTH_ROCK = 20,
    DAMAGE_SOURCE_TOXIC_SPIKES = 21,
    DAMAGE_SOURCE_ALMOST_FAINTED = 22, // "Is on the verge of fainting after using that move"
    DAMAGE_SOURCE_UNK_ABILITY = 23,    // "Took X damage because of <ability>"
    DAMAGE_SOURCE_SOLAR_POWER = 24,
    DAMAGE_SOURCE_DRY_SKIN = 25,
};

// Exclusive effect ID. These are usually encoded as bitvectors.
// Some of these are unused in-game but still labeled if easy to infer.
// See https://docs.google.com/spreadsheets/d/1XfE0x6JtLKDlCCvp5doNwyFpMX0C2HZCUCiDc1eMb1g
// for a list of exclusive items that map to each effect.
enum exclusive_item_effect_id {
    EXCLUSIVE_EFF_STAT_BOOST = 0, // Normal stat boosts; don't have a unique effect
    EXCLUSIVE_EFF_NO_PARALYSIS = 1,
    EXCLUSIVE_EFF_NO_CONFUSION = 2,
    EXCLUSIVE_EFF_NO_INFATUATION = 3,
    EXCLUSIVE_EFF_NO_FREEZE = 4,
    EXCLUSIVE_EFF_NO_CRITICAL_HITS = 5,
    EXCLUSIVE_EFF_HALVED_EXPLOSION_DAMAGE = 6,
    EXCLUSIVE_EFF_NO_EXPLOSION_DAMAGE = 7,
    EXCLUSIVE_EFF_NO_MOVE_DISABLING = 8,
    EXCLUSIVE_EFF_NO_WEATHER_DAMAGE = 9,
    EXCLUSIVE_EFF_NO_SLEEP = 10,
    EXCLUSIVE_EFF_MAY_POISON_PARALYZE_SLEEP_ATTACKERS = 11,
    EXCLUSIVE_EFF_UNUSED_0xC = 12, // Probably "may afflict <some status> on attackers"
    EXCLUSIVE_EFF_MAY_SLEEP_ATTACKERS = 13,
    EXCLUSIVE_EFF_MAY_NIGHTMARE_ATTACKERS = 14,
    EXCLUSIVE_EFF_MAY_BURN_ATTACKERS = 15,
    EXCLUSIVE_EFF_MAY_PARALYZE_ATTACKERS = 16,
    EXCLUSIVE_EFF_MAY_CONFUSE_ATTACKERS = 17,
    EXCLUSIVE_EFF_MAY_INFATUATE_ATTACKERS = 18,
    EXCLUSIVE_EFF_MAY_FREEZE_ATTACKERS = 19,
    EXCLUSIVE_EFF_MAY_SHADOW_HOLD_ATTACKERS = 20,
    EXCLUSIVE_EFF_MAY_CONSTRICT_ATTACKERS = 21,
    EXCLUSIVE_EFF_MAY_CRINGE_ATTACKERS = 22,
    EXCLUSIVE_EFF_MAY_BLINKER_ATTACKERS = 23,
    EXCLUSIVE_EFF_MAY_SEAL_ATTACKER_MOVES = 24,
    EXCLUSIVE_EFF_MAY_GO_INVISIBLE_WHEN_ATTACKED = 25,
    EXCLUSIVE_EFF_MAY_BOOST_MOVEMENT_SPEED_WHEN_ATTACKED = 26,
    EXCLUSIVE_EFF_MAY_WARP_WHEN_ATTACKED = 27,
    EXCLUSIVE_EFF_MAY_PERISH_SONG_ATTACKERS = 28,
    EXCLUSIVE_EFF_MAY_SLOW_ATTACKERS = 29,
    EXCLUSIVE_EFF_HALVED_PHYSICAL_DAMAGE = 30,
    EXCLUSIVE_EFF_HALVED_SPECIAL_DAMAGE = 31,
    EXCLUSIVE_EFF_COUNTER_PHYSICAL_DAMAGE = 32,
    EXCLUSIVE_EFF_MAY_BOUNCE_STATUS_MOVES = 33,
    EXCLUSIVE_EFF_MAY_ENDURE = 34,
    EXCLUSIVE_EFF_COUNTER_25_PCT_PHYSICAL_DAMAGE = 35,
    EXCLUSIVE_EFF_LONG_TOSS = 36,
    EXCLUSIVE_EFF_MAY_BOUNCE_MOVES = 37,
    EXCLUSIVE_EFF_NO_STAT_DROPS = 38,
    EXCLUSIVE_EFF_CONVERSION_2_WHEN_HIT = 39,
    EXCLUSIVE_EFF_NO_STATUS_WHEN_CLEAR = 40,
    EXCLUSIVE_EFF_NO_STATUS_WHEN_SUNNY = 41,
    EXCLUSIVE_EFF_NO_STATUS_WHEN_SANDSTORM = 42,
    EXCLUSIVE_EFF_NO_STATUS_WHEN_CLOUDY = 43,
    EXCLUSIVE_EFF_NO_STATUS_WHEN_RAINY = 44,
    EXCLUSIVE_EFF_NO_STATUS_WHEN_HAIL = 45,
    EXCLUSIVE_EFF_NO_STATUS_WHEN_FOGGY = 46,
    EXCLUSIVE_EFF_MOVEMENT_SPEED_BOOST_WHEN_CLEAR = 47,
    EXCLUSIVE_EFF_MOVEMENT_SPEED_BOOST_WHEN_SUNNY = 48,
    EXCLUSIVE_EFF_MOVEMENT_SPEED_BOOST_WHEN_SANDSTORM = 49,
    EXCLUSIVE_EFF_MOVEMENT_SPEED_BOOST_WHEN_CLOUDY = 50,
    EXCLUSIVE_EFF_MOVEMENT_SPEED_BOOST_WHEN_RAINY = 51,
    EXCLUSIVE_EFF_MOVEMENT_SPEED_BOOST_WHEN_HAIL = 52,
    EXCLUSIVE_EFF_MOVEMENT_SPEED_BOOST_WHEN_FOGGY = 53,
    EXCLUSIVE_EFF_ATTACK_SPEED_BOOST_WHEN_CLEAR = 54,
    EXCLUSIVE_EFF_ATTACK_SPEED_BOOST_WHEN_SUNNY = 55,
    EXCLUSIVE_EFF_ATTACK_SPEED_BOOST_WHEN_SANDSTORM = 56, // Unused in-game
    EXCLUSIVE_EFF_ATTACK_SPEED_BOOST_WHEN_CLOUDY = 57,    // Unused in-game
    EXCLUSIVE_EFF_ATTACK_SPEED_BOOST_WHEN_RAINY = 58,
    EXCLUSIVE_EFF_ATTACK_SPEED_BOOST_WHEN_HAIL = 59,
    EXCLUSIVE_EFF_ATTACK_SPEED_BOOST_WHEN_FOGGY = 60, // Unused in-game
    EXCLUSIVE_EFF_EVASION_BOOST_WHEN_CLEAR = 61,
    EXCLUSIVE_EFF_EVASION_BOOST_WHEN_SUNNY = 62,
    EXCLUSIVE_EFF_EVASION_BOOST_WHEN_SANDSTORM = 63,
    EXCLUSIVE_EFF_EVASION_BOOST_WHEN_CLOUDY = 64,
    EXCLUSIVE_EFF_EVASION_BOOST_WHEN_RAINY = 65,
    EXCLUSIVE_EFF_EVASION_BOOST_WHEN_HAIL = 66, // Unused in-game
    EXCLUSIVE_EFF_EVASION_BOOST_WHEN_FOGGY = 67,
    EXCLUSIVE_EFF_BYPASS_REFLECT_LIGHT_SCREEN = 68,
    EXCLUSIVE_EFF_SCRAPPY = 69,
    EXCLUSIVE_EFF_MIRACLE_EYE = 70,
    EXCLUSIVE_EFF_RESTORE_PP_ON_NEW_FLOORS = 71,
    EXCLUSIVE_EFF_RESTORE_HP_ON_NEW_FLOORS = 72,
    EXCLUSIVE_EFF_INCREASED_HP_RECOVERY = 73,
    EXCLUSIVE_EFF_MAX_PP_BOOST = 74,
    EXCLUSIVE_EFF_UNUSED_0x4B = 75,
    EXCLUSIVE_EFF_MAX_HP_BOOST = 76,
    EXCLUSIVE_EFF_UNUSED_0x4D = 77,
    EXCLUSIVE_EFF_UNUSED_0x4E = 78,
    EXCLUSIVE_EFF_EXP_BOOST = 79,
    EXCLUSIVE_EFF_EXP_FROM_DAMAGE = 80,
    EXCLUSIVE_EFF_MAY_RESTORE_PP_FROM_DAMAGE = 81,
    EXCLUSIVE_EFF_MAY_NOT_CONSUME_PP = 82,
    EXCLUSIVE_EFF_THROWN_ITEM_PROTECTION = 83,
    EXCLUSIVE_EFF_BOUNCE_THROWN_ITEMS = 84,
    EXCLUSIVE_EFF_EXTEND_SELF_EFFECTS_TO_TEAM = 85,
    EXCLUSIVE_EFF_MORE_TREASURE_DROPS = 86,
    EXCLUSIVE_EFF_HP_DRAIN_RECOVERY_BOOST = 87,
    EXCLUSIVE_EFF_PRESSURE_BOOST = 88,
    EXCLUSIVE_EFF_NO_STATUS = 89,
    EXCLUSIVE_EFF_HALVED_DAMAGE = 90,
    EXCLUSIVE_EFF_DAMAGE_BOOST_50_PCT = 91,
    EXCLUSIVE_EFF_ABSORB_TEAMMATE_POISON = 92,
    EXCLUSIVE_EFF_RECOVER_HP_FROM_APPLES_AND_BERRIES = 93,
    EXCLUSIVE_EFF_MORE_KECLEON_SHOPS = 94,
    EXCLUSIVE_EFF_MORE_HIDDEN_STAIRS = 95,
    EXCLUSIVE_EFF_NO_FRIENDLY_FIRE = 96,
    EXCLUSIVE_EFF_PICKUP_BOOST = 97,
    EXCLUSIVE_EFF_MORE_MONEY_DROPS = 98,
    EXCLUSIVE_EFF_UNUSED_0x63 = 99,
    EXCLUSIVE_EFF_RECOVER_HP_FROM_WATERY_TERRAIN = 100,
    EXCLUSIVE_EFF_HEAL_STATUS_FROM_WATERY_TERRAIN = 101,
    EXCLUSIVE_EFF_NO_FIRE_DAMAGE = 102,
    EXCLUSIVE_EFF_NO_WATER_DAMAGE = 103,
    EXCLUSIVE_EFF_NO_GRASS_DAMAGE = 104,
    EXCLUSIVE_EFF_NO_ELECTRIC_DAMAGE = 105,
    EXCLUSIVE_EFF_NO_FIGHTING_DAMAGE = 106,
    EXCLUSIVE_EFF_NO_GROUND_DAMAGE = 107,
    EXCLUSIVE_EFF_NO_FLYING_DAMAGE = 108,
    EXCLUSIVE_EFF_NO_PSYCHIC_DAMAGE = 109,
    EXCLUSIVE_EFF_NO_GHOST_DAMAGE = 110,
    EXCLUSIVE_EFF_NO_DRAGON_DAMAGE = 111,
    EXCLUSIVE_EFF_NO_DARK_DAMAGE = 112,
    EXCLUSIVE_EFF_NO_STEEL_DAMAGE = 113,
    EXCLUSIVE_EFF_ABSORB_FIRE_DAMAGE = 114,
    EXCLUSIVE_EFF_ABSORB_WATER_DAMAGE = 115,
    EXCLUSIVE_EFF_ABSORB_GRASS_DAMAGE = 116,
    EXCLUSIVE_EFF_ABSORB_ELECTRIC_DAMAGE = 117,
    EXCLUSIVE_EFF_ABSORB_ICE_DAMAGE = 118,
    EXCLUSIVE_EFF_ABSORB_FIGHTING_DAMAGE = 119,
    EXCLUSIVE_EFF_ABSORB_GROUND_DAMAGE = 120,
    EXCLUSIVE_EFF_ABSORB_FLYING_DAMAGE = 121,
    EXCLUSIVE_EFF_ABSORB_PSYCHIC_DAMAGE = 122,
    EXCLUSIVE_EFF_ABSORB_BUG_DAMAGE = 123,
    EXCLUSIVE_EFF_ABSORB_ROCK_DAMAGE = 124,
    EXCLUSIVE_EFF_ABSORB_GHOST_DAMAGE = 125,
    EXCLUSIVE_EFF_ABSORB_DRAGON_DAMAGE = 126,
    EXCLUSIVE_EFF_ABSORB_DARK_DAMAGE = 127,
    EXCLUSIVE_EFF_ABSORB_STEEL_DAMAGE = 128,
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(exclusive_item_effect_id);
#pragma pack(pop)

// Affects the chance of items spawning on each tile in a Kecleon Shop
// The chances (in %) of each tile (in a 3x3 area at the center of the shop)
// having an item are listed below.
enum shop_item_positions {
    // 30  30  30
    // 100 100 30
    // 30  30  30
    SHOP_POSITION_0 = 0,
    // 30  30  30
    // 100 30  30
    // 30  100 30
    SHOP_POSITION_1 = 1,
    // 30  30  30
    // 100 30  30
    // 30  30  100
    SHOP_POSITION_2 = 2,
    // 100 30  30
    // 30  30  100
    // 30  30  30
    SHOP_POSITION_3 = 3,
    // 100 30  100
    // 30  30  30
    // 30  30  30
    SHOP_POSITION_4 = 4,
    // 30  30  30
    // 30  30  100
    // 100 30  30
    SHOP_POSITION_5 = 5,
    // 30  30  30
    // 100 30  100
    // 30  30  30
    // The rest of the entries all have the same layout
    SHOP_POSITION_6 = 6,
    SHOP_POSITION_7 = 7,
    SHOP_POSITION_8 = 8,
    SHOP_POSITION_9 = 9,
    SHOP_POSITION_A = 10,
    SHOP_POSITION_B = 11,
    SHOP_POSITION_C = 12,
    SHOP_POSITION_D = 13,
    SHOP_POSITION_E = 14,
    SHOP_POSITION_F = 15,
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(shop_item_positions);
#pragma pack(pop)

// Weather ID
enum weather_id {
    WEATHER_CLEAR = 0,
    WEATHER_SUNNY = 1,
    WEATHER_SANDSTORM = 2,
    WEATHER_CLOUDY = 3,
    WEATHER_RAIN = 4,
    WEATHER_HAIL = 5,
    WEATHER_FOG = 6,
    WEATHER_SNOW = 7,
    WEATHER_RANDOM = 8,
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(weather_id);
#pragma pack(pop)

// Dungeon floor type
enum floor_type {
    FLOOR_TYPE_NORMAL = 0,
    FLOOR_TYPE_FIXED = 1,  // Fixed room
    FLOOR_TYPE_RESCUE = 2, // Rescuing another player
};

// Objective while in a dungeon
enum dungeon_objective {
    OBJECTIVE_STORY = 0, // This dungeon visit is part of the story
    OBJECTIVE_NORMAL = 1,
    OBJECTIVE_RESCUE = 2,          // Rescuing another player
    OBJECTIVE_UNK_GAME_MODE_5 = 3, // $GAME_MODE == 5 when entering the dungeon
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(dungeon_objective);
#pragma pack(pop)

// Mission type on a floor
enum mission_type {
    MISSION_RESCUE_CLIENT = 0,
    MISSION_RESCUE_TARGET = 1,
    MISSION_ESCORT_TO_TARGET = 2,
    MISSION_EXPLORE_WITH_CLIENT = 3,
    MISSION_PROSPECT_WITH_CLIENT = 4,
    MISSION_GUIDE_CLIENT = 5,
    MISSION_FIND_ITEM = 6,
    MISSION_DELIVER_ITEM = 7,
    MISSION_SEARCH_FOR_TARGET = 8,
    MISSION_TAKE_ITEM_FROM_OUTLAW = 9,
    MISSION_ARREST_OUTLAW = 10,
    MISSION_CHALLENGE_REQUEST = 11,
    MISSION_TREASURE_MEMO = 12,
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(mission_type);
#pragma pack(pop)

// Mission subtype for MISSION_EXPLORE_WITH_CLIENT
enum mission_subtype_explore {
    MISSION_EXPLORE_NORMAL = 0,
    MISSION_EXPLORE_SEALED_CHAMBER = 1,
    MISSION_EXPLORE_GOLDEN_CHAMBER = 2,
};

// Mission subtype for MISSION_TAKE_ITEM_FROM_OUTLAW
enum mission_subtype_take_item {
    MISSION_TAKE_ITEM_NORMAL_OUTLAW = 0,
    MISSION_TAKE_ITEM_HIDDEN_OUTLAW = 1,
    MISSION_TAKE_ITEM_FLEEING_OUTLAW = 2,
};

// Mission subtype for MISSION_ARREST_OUTLAW
// 0-3 all occur naturally in-game, but don't seem to have any obvious differences?
enum mission_subtype_outlaw {
    MISSION_OUTLAW_NORMAL_0 = 0,
    MISSION_OUTLAW_NORMAL_1 = 1,
    MISSION_OUTLAW_NORMAL_2 = 2,
    MISSION_OUTLAW_NORMAL_3 = 3,
    MISSION_OUTLAW_ESCORT = 4,
    MISSION_OUTLAW_FLEEING = 5,
    MISSION_OUTLAW_HIDEOUT = 6,
    MISSION_OUTLAW_MONSTER_HOUSE = 7,
};

// Mission subtype for MISSION_CHALLENGE_REQUEST
enum mission_subtype_challenge {
    MISSION_CHALLENGE_NORMAL = 0,
    MISSION_CHALLENGE_MEWTWO = 1,
    MISSION_CHALLENGE_ENTEI = 2,
    MISSION_CHALLENGE_RAIKOU = 3,
    MISSION_CHALLENGE_SUICUNE = 4,
    MISSION_CHALLENGE_JIRACHI = 5,
};

// The meaning of the mission subtype depends on the mission type
union mission_subtype {
    int none;
    enum mission_subtype_explore explore;
    enum mission_subtype_take_item take_item;
    enum mission_subtype_outlaw outlaw;
    enum mission_subtype_challenge challenge;
};

// List of reasons why you can get forcefully kicked out of a dungeon
enum forced_loss_reason {
    FORCED_LOSS_NONE = 0,            // You don't have to get kicked out of the dungeon
    FORCED_LOSS_PARTNER_FAINTED = 1, // Your partner fainted (before postgame)
    FORCED_LOSS_CLIENT_FAINTED = 2,  // Your client fainted
    FORCED_LOSS_ESCORT_FAINTED = 3,  // The client you had to escort fainted
    // "Your client [name:0] couldn't join you. Let's return to Treasure Town."
    FORCED_LOSS_CLIENT_CANT_JOIN = 4,
};

// Controls whether the loop that runs until the current floor ends should continue
// iterating or not and why
enum floor_loop_status {
    FLOOR_LOOP_CONTINUE = 0,       // The floor loop keeps executing as normal
    FLOOR_LOOP_LEADER_FAINTED = 1, // The floor loop exits because the leader fainted
    FLOOR_LOOP_NEXT_FLOOR = 2,     // The floor loop exits because the floor is over
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(floor_loop_status);
#pragma pack(pop)

// Fixed room ID. Fixed rooms can be full floor layouts, like most end-of-dungeon boss chambers,
// multi-room layouts in the case of Treasure Memos, or single rooms within a larger floor layout
// like with key chambers.
enum fixed_room_id {
    FIXED_NONE = 0,
    FIXED_BEACH_CAVE_PIT = 1,
    FIXED_MT_BRISTLE_PEAK = 2,
    FIXED_STEAM_CAVE_PEAK = 3,
    FIXED_AMP_CLEARING = 4,
    FIXED_UNDERGROUND_LAKE = 5,
    FIXED_CRYSTAL_LAKE = 6,
    FIXED_SEALED_RUIN_PIT = 7,
    FIXED_BRINE_CAVE_PIT = 8,
    FIXED_OLD_RUINS = 9,
    FIXED_TEMPORAL_PINNACLE = 10,
    FIXED_MYSTIFYING_FOREST_CLEARING = 11,
    FIXED_CREVICE_CAVE_PIT = 12,
    FIXED_MIRACLE_SEABED = 13,
    FIXED_REGICE_CHAMBER = 14,
    FIXED_REGIROCK_CHAMBER = 15,
    FIXED_REGISTEEL_CHAMBER = 16,
    FIXED_REGIGIGAS_CHAMBER = 17,
    FIXED_SPACIAL_RIFT_BOTTOM = 18,
    FIXED_DARK_CRATER_PIT = 19,
    FIXED_GIANT_VOLCANO_PEAK = 20,
    FIXED_SHIMMER_DESERT_PIT = 21,
    FIXED_MT_AVALANCHE_PEAK = 22,
    FIXED_BOTTOMLESS_SEA_DEPTHS = 23,
    FIXED_WORLD_ABYSS_PIT = 24,
    FIXED_DEEP_MYSTERY_JUNGLE = 25,
    FIXED_SKY_STAIRWAY_APEX = 26,
    FIXED_EMPTY_OCTAGON = 27,
    FIXED_SKY_PEAK_5TH_STATION_CLEARING = 28,
    FIXED_SKY_PEAK_SUMMIT = 29,
    FIXED_DEEP_STAR_CAVE_SE1 = 30,
    FIXED_STAR_CAVE_PIT_SE1 = 31,
    FIXED_FORTUNE_RAVINE_PIT = 32,
    FIXED_BARREN_VALLEY_CLEARING = 33,
    FIXED_BLACK_SWAMP = 34,
    FIXED_DARK_ICE_MOUNTAIN_PINNACLE = 35,
    FIXED_VAST_ICE_MOUNTAIN_PINNACLE = 36,
    FIXED_BOULDER_QUARRY_CLEARING = 37,
    FIXED_LIMESTONE_CAVERN_DEPTHS = 38,
    FIXED_UPPER_SPRING_CAVE = 39,
    FIXED_MIDDLE_SPRING_CAVE = 40,
    FIXED_SPRING_CAVE_PIT = 41,
    FIXED_DEEP_STAR_CAVE = 42,
    FIXED_BARREN_VALLEY_CLEARING_REVISIT = 43,
    FIXED_UNUSED_0x2C = 44,
    FIXED_UNUSED_0x2D = 45,
    FIXED_UNUSED_0x2E = 46,
    FIXED_UNUSED_0x2F = 47,
    FIXED_UNUSED_0x30 = 48,
    FIXED_UNUSED_0x31 = 49,
    FIXED_UNUSED_0x32 = 50,
    FIXED_UNUSED_0x33 = 51,
    FIXED_UNUSED_0x34 = 52,
    FIXED_UNUSED_0x35 = 53,
    FIXED_UNUSED_0x36 = 54,
    FIXED_UNUSED_0x37 = 55,
    FIXED_UNUSED_0x38 = 56,
    FIXED_UNUSED_0x39 = 57,
    FIXED_UNUSED_0x3A = 58,
    FIXED_UNUSED_0x3B = 59,
    FIXED_UNUSED_0x3C = 60,
    FIXED_UNUSED_0x3D = 61,
    FIXED_UNUSED_0x3E = 62,
    FIXED_TEST = 63,
    FIXED_EMPTY_RECTANGLE = 64,
    FIXED_BEACH_CAVE_PIT_REVISIT = 65,
    FIXED_MT_BRISTLE_PEAK_REVISIT = 66,
    FIXED_STEAM_CAVE_PEAK_REVISIT = 67,
    FIXED_AMP_CLEARING_REVISIT = 68,
    FIXED_UNDERGROUND_LAKE_REVISIT = 69,
    FIXED_CRYSTAL_LAKE_REVISIT = 70,
    FIXED_SEALED_RUIN_PIT_REVISIT = 71,
    FIXED_BRINE_CAVE_PIT_REVISIT = 72,
    FIXED_OLD_RUINS_REVISIT = 73,
    FIXED_TEMPORAL_PINNACLE_REVISIT = 74,
    FIXED_REGIGIGAS_CHAMBER_REVISIT = 75,
    FIXED_SPACIAL_RIFT_BOTTOM_REVISIT = 76,
    FIXED_DARK_CRATER_PIT_REVISIT = 77,
    FIXED_UNUSED_0x4E = 78,
    FIXED_UNUSED_0x4F = 79,
    FIXED_LABYRINTH_CAVE = 80,
    FIXED_ZERO_ISLE_NORTH = 81,
    FIXED_ZERO_ISLE_EAST = 82,
    FIXED_ZERO_ISLE_WEST = 83,
    FIXED_ZERO_ISLE_SOUTH = 84,
    FIXED_ORAN_FOREST = 85,
    FIXED_MARINE_RESORT = 86,
    FIXED_SERENITY_RIVER = 87,
    FIXED_LANDSLIDE_CAVE = 88,
    FIXED_LUSH_PRAIRIE = 89,
    FIXED_TINY_MEADOW = 90,
    FIXED_SURROUNDED_SEA = 91,
    FIXED_CONCEALED_RUINS = 92,
    FIXED_LAKE_AFAR = 93,
    FIXED_HAPPY_OUTLOOK = 94,
    FIXED_MT_MISTRAL = 95,
    FIXED_SHIMMER_HILL = 96,
    FIXED_LOST_WILDERNESS = 97,
    FIXED_MIDNIGHT_FOREST = 98,
    FIXED_ZERO_ISLE_CENTER = 99,
    FIXED_OBLIVION_FOREST = 100,
    FIXED_TREACHEROUS_WATERS = 101,
    FIXED_SOUTHEASTERN_ISLANDS = 102,
    FIXED_INFERNO_CAVE = 103,
    FIXED_MIDNIGHT_FOREST_COPY = 104,
    FIXED_UNUSED_0x69 = 105,
    FIXED_UNUSED_0x6A = 106,
    FIXED_UNUSED_0x6B = 107,
    FIXED_UNUSED_0x6C = 108,
    FIXED_UNUSED_0x6D = 109,
    FIXED_SUBSTITUTE_ROOM = 110,
    FIXED_GOLDEN_CHAMBER = 111,
    FIXED_SECRET_BAZAAR = 112,
    FIXED_SECRET_ROOM = 113,
    FIXED_TREASURE_MEMO_0 = 114,
    FIXED_TREASURE_MEMO_1 = 115,
    FIXED_TREASURE_MEMO_2 = 116,
    FIXED_TREASURE_MEMO_3 = 117,
    FIXED_TREASURE_MEMO_4 = 118,
    FIXED_TREASURE_MEMO_5 = 119,
    FIXED_TREASURE_MEMO_6 = 120,
    FIXED_TREASURE_MEMO_7 = 121,
    FIXED_TREASURE_MEMO_8 = 122,
    FIXED_TREASURE_MEMO_9 = 123,
    FIXED_TREASURE_MEMO_10 = 124,
    FIXED_TREASURE_MEMO_11 = 125,
    FIXED_TREASURE_MEMO_12 = 126,
    FIXED_TREASURE_MEMO_13 = 127,
    FIXED_TREASURE_MEMO_14 = 128,
    FIXED_TREASURE_MEMO_15 = 129,
    FIXED_TREASURE_MEMO_16 = 130,
    FIXED_TREASURE_MEMO_17 = 131,
    FIXED_TREASURE_MEMO_18 = 132,
    FIXED_TREASURE_MEMO_19 = 133,
    FIXED_TREASURE_MEMO_20 = 134,
    FIXED_TREASURE_MEMO_21 = 135,
    FIXED_TREASURE_MEMO_22 = 136,
    FIXED_TREASURE_MEMO_23 = 137,
    FIXED_TREASURE_MEMO_24 = 138,
    FIXED_TREASURE_MEMO_25 = 139,
    FIXED_TREASURE_MEMO_26 = 140,
    FIXED_TREASURE_MEMO_27 = 141,
    FIXED_TREASURE_MEMO_28 = 142,
    FIXED_TREASURE_MEMO_29 = 143,
    FIXED_TREASURE_MEMO_30 = 144,
    FIXED_MEWTWO_CHALLENGE_FLOOR = 145,
    FIXED_ENTEI_CHALLENGE_FLOOR = 146,
    FIXED_RAIKOU_CHALLENGE_FLOOR = 147,
    FIXED_SUICUNE_CHALLENGE_FLOOR = 148,
    FIXED_STAR_CAVE_PIT = 149,
    FIXED_CHALLENGE_LETTER_FLOOR_1 = 150,
    FIXED_CHALLENGE_LETTER_FLOOR_2 = 151,
    FIXED_CHALLENGE_LETTER_FLOOR_3 = 152,
    FIXED_CHALLENGE_LETTER_FLOOR_4 = 153,
    FIXED_CHALLENGE_LETTER_FLOOR_5 = 154,
    FIXED_UNUSED_0x9B = 155,
    FIXED_UNUSED_0x9C = 156,
    FIXED_UNUSED_0x9D = 157,
    FIXED_UNUSED_0x9E = 158,
    FIXED_UNUSED_0x9F = 159,
    FIXED_OUTLAW_TEAM_FLOOR_1 = 160,
    FIXED_OUTLAW_TEAM_FLOOR_2 = 161,
    FIXED_OUTLAW_TEAM_FLOOR_3 = 162,
    FIXED_OUTLAW_TEAM_FLOOR_4 = 163,
    FIXED_OUTLAW_TEAM_FLOOR_5 = 164,
    FIXED_SEALED_CHAMBER = 165,
    FIXED_UNUSED_0xA6 = 166,
    FIXED_UNUSED_0xA7 = 167,
    FIXED_UNUSED_0xA8 = 168,
    FIXED_UNUSED_0xA9 = 169,
    FIXED_CLEAR_SILK_CHAMBER = 170,
    FIXED_ORANGE_SILK_CHAMBER = 171,
    FIXED_GREEN_SILK_CHAMBER = 172,
    FIXED_IRON_SILK_CHAMBER = 173,
    FIXED_JOY_SEED_CHAMBER_6X = 174,
    FIXED_FRIEND_BOW_CHAMBER = 175,
    FIXED_PINK_SILK_CHAMBER = 176,
    FIXED_GRAY_SILK_CHAMBER = 177,
    FIXED_PURPLE_SILK_CHAMBER = 178,
    FIXED_JOY_SEED_CHAMBER_1X = 179,
    FIXED_ROYAL_SILK_CHAMBER = 180,
    FIXED_MIRACLE_CHEST_CHAMBER = 181,
    FIXED_BROWN_SILK_CHAMBER = 182,
    FIXED_SKY_SILK_CHAMBER = 183,
    FIXED_GOLD_SILK_CHAMBER = 184,
    FIXED_BLACK_SILK_CHAMBER = 185,
    FIXED_JOY_SEED_CHAMBER_2X = 186,
    FIXED_WONDER_GUMMI_CHAMBER = 187,
    FIXED_ITEM_CHAMBER_1 = 188,
    FIXED_ITEM_CHAMBER_2 = 189,
    FIXED_WONDER_CHEST_CHAMBER = 190,
    FIXED_RED_SILK_CHAMBER = 191,
    FIXED_BLUE_SILK_CHAMBER = 192,
    FIXED_GRASS_SILK_CHAMBER = 193,
    FIXED_YELLOW_SILK_CHAMBER = 194,
    FIXED_REVIVER_SEED_CHAMBER_1 = 195,
    FIXED_REVIVER_SEED_CHAMBER_2 = 196,
    FIXED_EMPTY_SEALED_CHAMBER = 197,
    FIXED_UNUSED_0xC6 = 198,
    FIXED_UNUSED_0xC7 = 199,
    FIXED_UNUSED_0xC8 = 200,
    FIXED_UNUSED_0xC9 = 201,
    FIXED_UNUSED_0xCA = 202,
    FIXED_UNUSED_0xCB = 203,
    FIXED_UNUSED_0xCC = 204,
    FIXED_UNUSED_0xCD = 205,
    FIXED_UNUSED_0xCE = 206,
    FIXED_UNUSED_0xCF = 207,
    FIXED_UNUSED_0xD0 = 208,
    FIXED_UNUSED_0xD1 = 209,
    FIXED_UNUSED_0xD2 = 210,
    FIXED_UNUSED_0xD3 = 211,
    FIXED_UNUSED_0xD4 = 212,
    FIXED_UNUSED_0xD5 = 213,
    FIXED_UNUSED_0xD6 = 214,
    FIXED_UNUSED_0xD7 = 215,
    FIXED_UNUSED_0xD8 = 216,
    FIXED_UNUSED_0xD9 = 217,
    FIXED_UNUSED_0xDA = 218,
    FIXED_UNUSED_0xDB = 219,
    FIXED_LEGENDARY_EXCLUSIVE_CHAMBER_1 = 220,
    FIXED_LEGENDARY_EXCLUSIVE_CHAMBER_2 = 221,
    FIXED_LEGENDARY_EXCLUSIVE_CHAMBER_3 = 222,
    FIXED_LEGENDARY_EXCLUSIVE_CHAMBER_4 = 223,
    FIXED_LEGENDARY_EXCLUSIVE_CHAMBER_5 = 224,
    FIXED_LEGENDARY_EXCLUSIVE_CHAMBER_6 = 225,
    FIXED_LEGENDARY_EXCLUSIVE_CHAMBER_7 = 226,
    FIXED_LEGENDARY_EXCLUSIVE_CHAMBER_8 = 227,
    FIXED_LEGENDARY_EXCLUSIVE_CHAMBER_9 = 228,
    FIXED_LEGENDARY_EXCLUSIVE_CHAMBER_10 = 229,
    FIXED_UNUSED_0xE6 = 230,
    FIXED_UNUSED_0xE7 = 231,
    FIXED_UNUSED_0xE8 = 232,
    FIXED_UNUSED_0xE9 = 233,
    FIXED_UNUSED_0xEA = 234,
    FIXED_UNUSED_0xEB = 235,
    FIXED_UNUSED_0xEC = 236,
    FIXED_UNUSED_0xED = 237,
    FIXED_UNUSED_0xEE = 238,
    FIXED_UNUSED_0xEF = 239,
    FIXED_UNUSED_0xF0 = 240,
    FIXED_UNUSED_0xF1 = 241,
    FIXED_UNUSED_0xF2 = 242,
    FIXED_UNUSED_0xF3 = 243,
    FIXED_UNUSED_0xF4 = 244,
    FIXED_UNUSED_0xF5 = 245,
    FIXED_UNUSED_0xF6 = 246,
    FIXED_UNUSED_0xF7 = 247,
    FIXED_UNUSED_0xF8 = 248,
    FIXED_UNUSED_0xF9 = 249,
    FIXED_UNUSED_0xFA = 250,
    FIXED_UNUSED_0xFB = 251,
    FIXED_UNUSED_0xFC = 252,
    FIXED_UNUSED_0xFD = 253,
    FIXED_UNUSED_0xFE = 254,
    FIXED_UNUSED_0xFF = 255,
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(fixed_room_id);
#pragma pack(pop)

// Floor layout size during floor generation
enum floor_size {
    FLOOR_SIZE_LARGE = 0,
    FLOOR_SIZE_SMALL = 1,
    FLOOR_SIZE_MEDIUM = 2,
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(floor_size);
#pragma pack(pop)

// Floor layout type during floor generation
enum floor_layout {
    LAYOUT_LARGE = 0,
    LAYOUT_SMALL = 1,
    LAYOUT_ONE_ROOM_MONSTER_HOUSE = 2,
    LAYOUT_OUTER_RING = 3,
    LAYOUT_CROSSROADS = 4,
    LAYOUT_TWO_ROOMS_WITH_MONSTER_HOUSE = 5,
    LAYOUT_LINE = 6,
    LAYOUT_CROSS = 7,
    LAYOUT_LARGE_0x8 = 8,
    LAYOUT_BEETLE = 9,
    LAYOUT_OUTER_ROOMS = 10,
    LAYOUT_MEDIUM = 11,
    LAYOUT_UNUSED_0xC = 12,
    LAYOUT_UNUSED_0xD = 13,
    LAYOUT_UNUSED_0xE = 14,
    LAYOUT_UNUSED_0xF = 15,
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(floor_layout);
#pragma pack(pop)

#endif
