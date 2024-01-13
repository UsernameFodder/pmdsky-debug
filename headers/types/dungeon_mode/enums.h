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

// Moves that Nature Power can turn into
enum nature_power_variant {
    NATURE_POWER_SURF = 0,
    NATURE_POWER_STUN_SPORE = 1,
    NATURE_POWER_SHADOW_BALL = 2,
    NATURE_POWER_SWIFT = 3,
    NATURE_POWER_EARTHQUAKE = 4,
    NATURE_POWER_RAZOR_LEAF = 5,
    NATURE_POWER_BUBBLEBEAM = 6,
    NATURE_POWER_ROCK_SLIDE = 7,
    // This is exactly the same as NATURE_POWER_EARTHQUAKE,
    // except the move effect handler checks whether the defender
    // is digging and doubles the damage if so.
    NATURE_POWER_EARTHQUAKE_2 = 8,
    NATURE_POWER_TRI_ATTACK = 9,
    NATURE_POWER_HYDRO_PUMP = 10,
    NATURE_POWER_BLIZZARD = 11,
    NATURE_POWER_ICE_BEAM = 12,
    NATURE_POWER_SEED_BOMB = 13,
    NATURE_POWER_MUD_BOMB = 14,
};

// This is usually stored as a 16-bit integer
#pragma pack(push, 2)
ENUM_16_BIT(nature_power_variant);
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
    // Also used to indicate a random non-wonder tile trap should be selected instead during play
    TRAP_NONE = 25,
    TRAP_0x1A = 26,
    TRAP_0x1B = 27,
    TRAP_0x1C = 28,
    TRAP_0x1D = 29,
    TRAP_0x1E = 30,
    TRAP_0x1F = 31,
    // Behaves identically to a normal pitfall trap, but uses the sprite where the grate is
    // already broken. TRAP_PITFALL_TRAP becomes this one when something falls through.
    TRAP_BROKEN_PITFALL_TRAP = 32,
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

// This is can be stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(status_id);
#pragma pack(pop)

// Values for the two-turn move status group
// Corresponds to values 28 to 40 in the status_id enum
enum status_two_turn_id {
    STATUS_TWO_TURN_BIDE = 1,
    STATUS_TWO_TURN_SOLARBEAM = 2,
    STATUS_TWO_TURN_SKY_ATTACK = 3,
    STATUS_TWO_TURN_RAZOR_WIND = 4,
    STATUS_TWO_TURN_FOCUS_PUNCH = 5,
    STATUS_TWO_TURN_SKULL_BASH = 6,
    STATUS_TWO_TURN_FLYING = 7,
    STATUS_TWO_TURN_BOUNCING = 8,
    STATUS_TWO_TURN_DIVING = 9,
    STATUS_TWO_TURN_DIGGING = 10,
    STATUS_TWO_TURN_CHARGING = 11,
    STATUS_TWO_TURN_ENRAGED = 12,
    STATUS_TWO_TURN_SHADOW_FORCE = 13,
};

// This is usually stored as a 16-bit integer
#pragma pack(push, 2)
ENUM_16_BIT(status_two_turn_id);
#pragma pack(pop)

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
    TACTIC_NONE = 11, // used as an empty placeholder in menus (and maybe other things).
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

// Identifies a message displayed when a monster takes damage
enum damage_message {
    DAMAGE_MESSAGE_MOVE = 0, // "Took X damage"
    DAMAGE_MESSAGE_BURN = 1,
    DAMAGE_MESSAGE_CONSTRICTION = 2, // "Was squeezed for X damage"
    DAMAGE_MESSAGE_POISON = 3,
    DAMAGE_MESSAGE_RECOIL_1 = 4, // User deals damage to itself because of their own recoil move
    DAMAGE_MESSAGE_WRAP = 5,     // "Was wrapped for X damage"
    DAMAGE_MESSAGE_COUNTER = 6,  // Damage taken from a counterattack
    DAMAGE_MESSAGE_CURSE = 7,
    DAMAGE_MESSAGE_NIGHTMARE = 8, // Damage taken when awakening from a nightmare
    DAMAGE_MESSAGE_LEECH_SEED = 9,
    DAMAGE_MESSAGE_SPIKES = 10,
    DAMAGE_MESSAGE_PERISH_SONG = 11,
    DAMAGE_MESSAGE_DESTINY_BOND = 12,
    DAMAGE_MESSAGE_SLUDGE = 13, // "Was showered with sludge for X damage"
    DAMAGE_MESSAGE_HUNGER = 14, // No string associated with hunger, uses 0x0.
    DAMAGE_MESSAGE_CHESTNUT_1 = 15,
    DAMAGE_MESSAGE_CHESTNUT_2 = 16,   // Same string as DAMAGE_MESSAGE_CHESTNUT_1
    DAMAGE_MESSAGE_PITFALL_TRAP = 17, // Same string as DAMAGE_MESSAGE_MOVE
    DAMAGE_MESSAGE_BAD_WEATHER = 18,
    DAMAGE_MESSAGE_MISSED_MOVE = 19, // Damage taken from moves that hurt the user when they miss
    DAMAGE_MESSAGE_RECOIL_2 = 20,    // Same string as DAMAGE_MESSAGE_RECOIL_1
    DAMAGE_MESSAGE_STEALTH_ROCK = 21,
    DAMAGE_MESSAGE_TOXIC_SPIKES = 22,
    DAMAGE_MESSAGE_ALMOST_FAINTED = 23, // "Is on the verge of fainting after using that move"
    DAMAGE_MESSAGE_BAD_DREAMS = 24,     // "Took X damage because of <ability>"
    DAMAGE_MESSAGE_SOLAR_POWER = 25,
    DAMAGE_MESSAGE_DRY_SKIN = 26,
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
    EXCLUSIVE_EFF_MAX_HP_BOOST_10 = 76,
    EXCLUSIVE_EFF_MAX_HP_BOOST_20 = 77, // Unused in-game
    EXCLUSIVE_EFF_MAX_HP_BOOST_30 = 78, // Unused in-game
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
    EXCLUSIVE_EFF_LAST = 129, // Used as a null-terminator in some places
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
    MISSION_TYPE_UNK_0xD = 13,
    MISSION_SPECIAL_EPISODE = 14,
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
    MISSION_EXPLORE_NEW_DUNGEON = 3,
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(mission_subtype_explore);
#pragma pack(pop)

// Mission subtype for MISSION_TAKE_ITEM_FROM_OUTLAW
enum mission_subtype_take_item {
    MISSION_TAKE_ITEM_NORMAL_OUTLAW = 0,
    MISSION_TAKE_ITEM_HIDDEN_OUTLAW = 1,
    MISSION_TAKE_ITEM_FLEEING_OUTLAW = 2,
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(mission_subtype_take_item);
#pragma pack(pop)

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

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(mission_subtype_outlaw);
#pragma pack(pop)

// Mission subtype for MISSION_CHALLENGE_REQUEST
enum mission_subtype_challenge {
    MISSION_CHALLENGE_NORMAL = 0,
    MISSION_CHALLENGE_MEWTWO = 1,
    MISSION_CHALLENGE_ENTEI = 2,
    MISSION_CHALLENGE_RAIKOU = 3,
    MISSION_CHALLENGE_SUICUNE = 4,
    MISSION_CHALLENGE_JIRACHI = 5,
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(mission_subtype_challenge);
#pragma pack(pop)

// The meaning of the mission subtype depends on the mission type
union mission_subtype {
    uint8_t none;
    struct mission_subtype_explore_8 explore;
    struct mission_subtype_take_item_8 take_item;
    struct mission_subtype_outlaw_8 outlaw;
    struct mission_subtype_challenge_8 challenge;
};

// Different types of rewards that a mission can have
enum mission_reward_type {
    MISSION_REWARD_MONEY = 0,
    MISSION_REWARD_MONEY_AND_MORE = 1, // Money + (?)
    MISSION_REWARD_ITEM = 2,
    MISSION_REWARD_ITEM_AND_MORE = 3, // Item + (?)
    MISSION_REWARD_ITEM_HIDDEN = 4,   // Item, displayed as "(?)"
    MISSION_REWARD_MONEY_HIDDEN = 5,  // Money, displayed as "(?)"
    // Either an egg or the client requests to join the team, displayed as "(?)"
    MISSION_REWARD_SPECIAL = 6,
};
// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(mission_reward_type);
#pragma pack(pop)

// Different types of restrictions that a mission can have
enum mission_restriction_type {
    MISSION_RESTRICTION_NONE = 0,
    MISSION_RESTRICTION_TYPE = 1,    // Requires a pokémon of a certain type on the team
    MISSION_RESTRICTION_MONSTER = 2, // Requires a certain pokémon on the team
};
// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(mission_restriction_type);
#pragma pack(pop)

// The restriction of a mission can be a monster ID or a type ID
union mission_restriction {
    struct monster_id_16 monster_id;
    struct type_id_8 type_id;
};

// Represents the different statuses that a mission can have
enum mission_status {
    MISSION_STATUS_INVALID = 0, // Used for empty mission slots
    MISSION_STATUS_UNK_1 = 1,   // The mission won't display a status
    MISSION_STATUS_UNK_2 = 2,   // The mission won't display a status
    MISSION_STATUS_UNK_3 = 3,   // The mission won't display a status
    MISSION_STATUS_SUSPENDED = 4,
    MISSION_STATUS_ACCEPTED = 5,
    MISSION_STATUS_DONE = 6,
    MISSION_STATUS_UNK_7 = 7, // Shows up as "accepted"
    MISSION_STATUS_UNK_8 = 8, // Shows up as "accepted"
};
// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(mission_status);
#pragma pack(pop)

// The cause of a monster taking damage, not including the move case.
// These codes should all be greater than any move ID.
// Some of the values are used as faint reasons or on the "The Last Outing" screen
// rather than as damage sources.
enum damage_source_non_move {
    DAMAGE_SOURCE_TRANSFORM_FRIEND = 563, // "was transformed into a friend"
    DAMAGE_SOURCE_NOT_BEFRIENDED = 564,   // "left without being befriended"
    DAMAGE_SOURCE_DEBUG_ATTACK = 565,
    DAMAGE_SOURCE_JUMP_KICK = 566,
    DAMAGE_SOURCE_HI_JUMP_KICK = 567,
    DAMAGE_SOURCE_DESTINY_BOND = 568,
    DAMAGE_SOURCE_SLUDGE = 569,
    DAMAGE_SOURCE_POWERFUL_MOVE_1 = 570,
    DAMAGE_SOURCE_POWERFUL_MOVE_2 = 571,
    DAMAGE_SOURCE_RECOIL = 572,
    DAMAGE_SOURCE_SPLASH = 573,
    DAMAGE_SOURCE_ENERGY = 574, // "an enemy's pent-up energy"
    DAMAGE_SOURCE_POWERFUL_MOVE_3 = 575,
    DAMAGE_SOURCE_POWERFUL_MOVE_4 = 576,
    DAMAGE_SOURCE_POWERFUL_MOVE_5 = 577,
    DAMAGE_SOURCE_POWERFUL_MOVE_6 = 578,
    DAMAGE_SOURCE_POWERFUL_MOVE_7 = 579,
    DAMAGE_SOURCE_POWERFUL_MOVE_8 = 580,
    DAMAGE_SOURCE_SPIKES = 581,
    DAMAGE_SOURCE_DEBUG_DAMAGE = 582,
    DAMAGE_SOURCE_BURN = 583,
    DAMAGE_SOURCE_CONSTRICTION = 584,
    DAMAGE_SOURCE_POISON = 585,
    DAMAGE_SOURCE_WRAP = 586,
    DAMAGE_SOURCE_CURSE = 587,
    DAMAGE_SOURCE_LEECH_SEED = 588,
    DAMAGE_SOURCE_PERISH_SONG = 589,
    DAMAGE_SOURCE_NIGHTMARE = 590,
    DAMAGE_SOURCE_THROWN_ROCK = 591,
    DAMAGE_SOURCE_HUNGER = 592,
    DAMAGE_SOURCE_EXPLODED = 593, // I think this one is only used for the monster that explodes.
                                  // Other monsters that are hit by the explosion use
                                  // DAMAGE_SOURCE_EXPLOSION.
    DAMAGE_SOURCE_CHESTNUT_TRAP = 594,
    DAMAGE_SOURCE_TRAP = 595,
    DAMAGE_SOURCE_PITFALL_TRAP = 596,
    DAMAGE_SOURCE_BLAST_SEED = 597,
    DAMAGE_SOURCE_THROWN_ITEM = 598,
    DAMAGE_SOURCE_TRANSFORM_ITEM = 599,
    DAMAGE_SOURCE_KNOCKED_FLYING = 600,
    DAMAGE_SOURCE_FLYING_MONSTER = 601,
    DAMAGE_SOURCE_GAVE_UP = 602,   // "gave up the exploration"
    DAMAGE_SOURCE_DELETED = 603,   // "was deleted for the sake of an event"
    DAMAGE_SOURCE_WENT_AWAY = 604, // "went away"
    DAMAGE_SOURCE_UNSEEN_FORCE = 605,
    DAMAGE_SOURCE_PARTNER_FAINTED = 606, // "returned with the fallen partner"
    DAMAGE_SOURCE_WEATHER = 607,
    DAMAGE_SOURCE_POSSESS = 608,
    DAMAGE_SOURCE_CLIENT_FAINTED = 609, // "failed to protect the client Pokémon"
    DAMAGE_SOURCE_ITEM_ORB = 610,
    DAMAGE_SOURCE_ITEM_NON_ORB = 611,
    DAMAGE_SOURCE_UNK612 = 612,                // "-"
    DAMAGE_SOURCE_ESCORT_FAINTED = 613,        // "failed to escort the client Pokémon"
    DAMAGE_SOURCE_OTHER_MONSTER_FAINTED = 614, // "returned with the fallen [string:2]"
    DAMAGE_SOURCE_BIDOOF_FAINTED = 615,
    DAMAGE_SOURCE_GROVYLE_FAINTED = 616,
    DAMAGE_SOURCE_CELEBI_FAINTED = 617,
    DAMAGE_SOURCE_CHATOT_FAINTED = 618,
    DAMAGE_SOURCE_CRESSELIA_FAINTED = 619,
    DAMAGE_SOURCE_TOXIC_SPIKES = 620,
    DAMAGE_SOURCE_STEALTH_ROCK = 621,
    DAMAGE_SOURCE_BAD_DREAMS = 622,
    DAMAGE_SOURCE_EXPLOSION = 623,
    DAMAGE_SOURCE_OREN_BERRY = 624,
    DAMAGE_SOURCE_DUMMY_625 = 625,
    DAMAGE_SOURCE_DUMMY_626 = 626,
    DAMAGE_SOURCE_DUMMY_627 = 627,
    DAMAGE_SOURCE_DUMMY_628 = 628,
    DAMAGE_SOURCE_DUMMY_629 = 629,
    DAMAGE_SOURCE_DUMMY_630 = 630,
    DAMAGE_SOURCE_DUMMY_631 = 631,
    DAMAGE_SOURCE_DUMMY_632 = 632,
    DAMAGE_SOURCE_ESCAPE = 633,           // "Escaped in the middle of exploration"
    DAMAGE_SOURCE_DUNGEON_CLEAR = 634,    // "cleared the dungeon"
    DAMAGE_SOURCE_RESCUE_SUCCESS = 635,   // "succeeded in the rescue mission"
    DAMAGE_SOURCE_MISSION_COMPLETE = 636, // "completed a mission! Impressive!"
    DAMAGE_SOURCE_BEFRIEND_MEW = 637,     // "befriended [CS:N]Mew[CR]!"
};

// This is usually stored as a 16-bit integer
#pragma pack(push, 2)
ENUM_16_BIT(damage_source_non_move);
#pragma pack(pop)

// Possible reasons why a monster can take damage or faint
union damage_source {
    enum move_id move;
    enum damage_source_non_move other;
};
union damage_source_16 {
    struct move_id_16 move;
    struct damage_source_non_move_16 other;
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

// Action IDs used to spawn tiles when generating fixed rooms
enum fixed_room_action_non_entity {
    FIXED_ACTION_FLOOR_ROOM = 0,
    FIXED_ACTION_WALL_HALLWAY_AM = 1, // Hallway wall, breakable with Absolute Mover IQ skill
    FIXED_ACTION_WALL_HALLWAY_IMPASSABLE = 2,
    FIXED_ACTION_WALL_HALLWAY = 3,
    FIXED_ACTION_LEADER_SPAWN = 4,
    FIXED_ACTION_SECONDARY_ROOM = 5, // Secondary terrain tile, part of a room
    FIXED_ACTION_CHASM_HALLWAY = 6,
    // If spawned, all tiles outside the fixed room are are turned into impassable chasm tiles
    FIXED_ACTION_CHASM_ALL_HALLWAY = 7,
    FIXED_ACTION_WARP_ZONE_ROOM = 8, // Spawns a warp zone
    FIXED_ACTION_FLOOR_HALLWAY = 9,
    FIXED_ACTION_CHASM_HALLWAY_IMPASSABLE = 10,
    FIXED_ACTION_FLOOR_HALLWAY_FLAG_10 = 11, // Enables tile::terrain_flags_unk10
    // Wall if fixed floor ID < FIXED_SEALED_CHAMBER, floor otherwise. Spawns a locked key door.
    FIXED_ACTION_FLOOR_WALL_ROOM_KEY_DOOR_LOCKED = 12,
    // Wall if fixed floor ID < FIXED_SEALED_CHAMBER, floor otherwise. Spawns a locked escort
    // key door.
    FIXED_ACTION_FLOOR_WALL_ROOM_KEY_DOOR_ESCORT = 13,
    FIXED_ACTION_WALL_HALLWAY_IMPASSABLE_14 = 14,
    FIXED_ACTION_WALL_HALLWAY_15 = 15,
    FIXED_ACTION_TEAM_MEMBER_2_SPAWN = 96,
    FIXED_ACTION_TEAM_MEMBER_3_SPAWN = 97,
    FIXED_ACTION_TEAM_MEMBER_4_SPAWN = 98,
    // Treated separately by the code, but doesn't seem to spawn anything special
    FIXED_ACTION_FLOOR_ROOM_99 = 99,
    FIXED_ACTION_WARP_ZONE_ROOM_107 = 107, // Same as FIXED_ACTION_WARP_ZONE_ROOM
    // Treated separately by the code, but doesn't seem to spawn anything special
    FIXED_ACTION_FLOOR_ROOM_108 = 108,
    // Treated separately by the code, but doesn't seem to spawn anything special
    FIXED_ACTION_FLOOR_ROOM_109 = 109,
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(fixed_room_action_non_entity);
#pragma pack(pop)

// Used to determine an action that will be performed when spawining a single tile during fixed
// room generation. Can spawn an entity or a tile.
union fixed_room_action {
    struct fixed_room_action_non_entity_8 tile_action;
    // If specified, this value - 16 represents the ID of the fixed entity to spawn.
    uint8_t entity_action;
};

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

// Used as a parameter when generating items
enum gen_item_stickiness {
    // Use the sticky item chance from the floor properties on the dungeon struct
    GEN_ITEM_STICKY_RANDOM = 0,
    GEN_ITEM_STICKY_ALWAYS = 1,
    GEN_ITEM_STICKY_NEVER = 2,
};

// Used to determine which version of a dungeon to load.
enum game_id {
    GAME_SKY = 0,
    GAME_TIME = 1,
    GAME_DARKNESS = 2,
};

// Used when determining how a monster's name should be displayed.
enum display_name_type {
    DISPLAY_NAME_NORMAL = 0,  // Display the name as usual
    DISPLAY_NAME_UNKNOWN = 1, // Display "(?)"
    DISPLAY_NAME_DECOY = 2,   // Display "Decoy"
};

// Used to more easily refer to the different item spawn lists of a floor
enum item_list_type {
    ITEM_LIST_REGULAR = 0,       // Standard list
    ITEM_LIST_SHOP = 1,          // Kecleon shop list
    ITEM_LIST_MONSTER_HOUSE = 2, // Monster house list
    ITEM_LIST_BURIED = 3,        // Buried items list
    ITEM_LIST_BAZAAR = 4,        // Bazaar grab bag list
    ITEM_LIST_SECRET_ROOM = 5,   // Secret room chests list
};

// Special values for union item_index
enum item_index_special {
    ITEM_INDEX_GROUND = 0x80,
    ITEM_INDEX_HELD = 0x81,
    ITEM_INDEX_UNK_0x82 = 0x82,
    ITEM_INDEX_UNK_0x83 = 0x83,
    ITEM_INDEX_UNK_0x84 = 0x84,
    ITEM_INDEX_UNK_0x85 = 0x85,
    ITEM_INDEX_UNK_0x86 = 0x86,
    ITEM_INDEX_UNK_0x87 = 0x87,
    ITEM_INDEX_UNK_0x88 = 0x88,
    ITEM_INDEX_UNK_0x89 = 0x89,
    ITEM_INDEX_UNK_0x8A = 0x8A,
    ITEM_INDEX_UNK_0x8B = 0x8B,
    ITEM_INDEX_UNK_0x8C = 0x8C,
    ITEM_INDEX_UNK_0x8D = 0x8D,
    ITEM_INDEX_UNK_0x8E = 0x8E,
    ITEM_INDEX_UNK_0x8F = 0x8F,
    ITEM_INDEX_HELD_TEAM_MEMBER_1 = 0x90,
    ITEM_INDEX_HELD_TEAM_MEMBER_2 = 0x91,
    ITEM_INDEX_HELD_TEAM_MEMBER_3 = 0x92,
    ITEM_INDEX_HELD_TEAM_MEMBER_4 = 0x93,
};

// Used to indicate the location of an item, which can be an item in the bag, on the floor or
// held by a certain monster.
union item_index {
    int bag_index;                         // For items in the bag
    enum item_index_special special_index; // For other items
};

// Determines the color of damage numbers displayed above a monster
// Values of this enum are actually passed to I/O registers, so the color probably depends on the
// color pallettes that are currently loaded in memory.
enum number_color {
    NUMBER_COLOR_AUTO = -1,
    NUMBER_COLOR_GREEN_0x0 = 0,
    NUMBER_COLOR_RED_0x1 = 1,
    NUMBER_COLOR_DARK_GREEN = 2,
    NUMBER_COLOR_YELLOW = 3,
    NUMBER_COLOR_UNK_0x4 = 4,
    NUMBER_COLOR_UNK_0x5 = 5,
    NUMBER_COLOR_RED_0x6 = 6,
    NUMBER_COLOR_UNK_0x7 = 7,
    NUMBER_COLOR_UNK_0x8 = 8,
    NUMBER_COLOR_UNK_0x9 = 9,
    NUMBER_COLOR_GREEN_0xA = 10,
    NUMBER_COLOR_LIGHT_BLUE = 11,
};

// Used in top_screen_status to store what kind of fade the screen is currently in, if any.
enum top_screen_fade_status {
    TOP_SCREEN_FADE_0x0 = 0,
    TOP_SCREEN_FADE_0x1 = 1,
    TOP_SCREEN_FADE_BLACK = 2,
    TOP_SCREEN_FADE_0x3 = 3,
    TOP_SCREEN_FADE_FADE_IN = 4,
    TOP_SCREEN_FADE_NO_FADE = 5,
    TOP_SCREEN_FADE_FADE_OUT = 6,
};

// Top screen type as set in the settings
enum top_screen_type {
    TOP_SCREEN_TYPE_TEXT_LOG = 0,
    TOP_SCREEN_TYPE_TEAM_STATS = 1,
    TOP_SCREEN_TYPE_CONTROLS = 2,
    TOP_SCREEN_TYPE_MAP_AND_TEAM = 3,
    TOP_SCREEN_TYPE_JOB = 4,
};

// Used in dungeon_fade to store what kind of fade a screen is currently in, if any.
enum fade_status_dungeon {
    DUNGEON_FADE_0x0 = 0,
    DUNGEON_FADE_NONE = 1,
    DUNGEON_FADE_IN = 2,
    DUNGEON_FADE_OUT = 3,
    DUNGEON_FADE_0x4 = 4,
    DUNGEON_FADE_0x5 = 5,
};

#endif
