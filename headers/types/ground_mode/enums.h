// Enums used primarily in the context of ground mode

#ifndef HEADERS_TYPES_GROUND_MODE_ENUMS_H_
#define HEADERS_TYPES_GROUND_MODE_ENUMS_H_

// Predefined variables available to the script engine.
// Also see struct script_var_value_table, where the corresponding values for these variables are
// stored.
// TODO: replace raw address references with actual symbol names
enum script_var_id {
    // Its only purpose is to be checked at [NA]0x204C448, which compares the saved value to the
    // default value in the script variable table (which is 1 in base game). If the values are not
    // equal, then the save file is treated as corrupted, which thus gets deleted.
    VAR_VERSION = 0,
    // Seemingly unused?
    VAR_CONDITION = 1,
    // Two indexed values determined via the Top Menu. These are referenced by Unionall for general
    // control flow.
    // To be a bit more specific, the first coroutine run after the Top Menu is EVENT_DIVIDE, which
    // checks for these indexed values. In doing so, the game knows when to load scripts from
    // maingame, a Special Episode, or Rescue mode.
    VAR_SCENARIO_SELECT = 2,
    // Two indexed values used for maingame story progression. Typically, the first value represents
    // the chapter, and the second represents a subsection of that chapter.
    // This is referenced everywhere in maingame scripts and Unionall. It's especially notable in
    // deciding which NPCs to spawn in certain overworld sections, deciding which cutscene to load
    // after a dungeon loss, and various other control flow purposes. Used EXTENSIVELY in
    // EVENT_DIVIDE and other related coroutines.
    VAR_SCENARIO_MAIN = 3,
    // Two indexed values used for Special Episode story progression. Very similar to SCENARIO_MAIN,
    // but used on a much smaller scale.
    VAR_SCENARIO_SIDE = 4,
    VAR_SCENARIO_SUB1 = 5,
    VAR_SCENARIO_SUB2 = 6,
    VAR_SCENARIO_SUB3 = 7,
    VAR_SCENARIO_SUB4 = 8,
    VAR_SCENARIO_SUB5 = 9,
    VAR_SCENARIO_SUB6 = 10,
    VAR_SCENARIO_SUB7 = 11,
    VAR_SCENARIO_SUB8 = 12,
    // Three bitflags used in Special Episode 2. They're initialized to 0 when Special Episode 2
    // begins and set to 1 when Igglybuff talks to each of his neighbors (Wooper, Smoochum, and
    // Budew) at the start of the Special Episode.
    VAR_SIDE02_TALK = 13,
    // Three bitflags used in Special Episode 4. They're initialized to 0 when Special Episode 4
    // begins and set to 1 upon exiting the Limestone Cavern paths.
    VAR_SIDE06_ROOM = 14,
    // A bitflag used in Special Episode 5. It's initialized to 0 when Special Episode 5 begins and
    // is set to 1 after losing to the Barren Valley Clearing boss.
    VAR_SIDE08_BOSS2ND = 15,
    // A bitflag used in Special Episode 1. It's initialized to 0 when Special Episode 1 begins and
    // is set to 1 after losing to the Deep Star Cave boss.
    VAR_SIDE01_BOSS2ND = 16,
    // 128 bitflags used across maingame and Special Episodes. They all serve a variety of purposes,
    // but some of the most notable ones include:
    //   3: The player lost in a dungeon (SCENARIO)
    //   4: The player lost in a dungeon (REQUEST)
    //   6: The game was saved
    //   7: Overworld save
    //   8: Cutscene save
    //   10: Job day
    //   11: Sentry Duty day
    VAR_SCENARIO_MAIN_BIT_FLAG = 17,
    // 256 bitflags used across maingame for a variety of purposes. These are mostly used for
    // Treasure Town shopkeeper NPCs to make sure they don't repeat the same intro text before they
    // open their hard-coded menu.
    VAR_SCENARIO_TALK_BIT_FLAG = 18,
    // Despite the name, this is one integer! This controls a wide variety of game functions, such
    // as bag size, Treasure Box spawns, and spawn threshold (i.e. enemies can only spawn when this
    // variable is greater than or equal to a certain value).
    // Also see GetScenarioBalance.
    VAR_SCENARIO_BALANCE_FLAG = 19,
    // This shares the same features as SCENARIO_BALANCE_FLAG and actually takes priority over it in
    // maingame if this is greater than or equal to 0. Nevertheless, we're fairly certain this is
    // unused in the base game.
    // Also see GetScenarioBalance.
    VAR_SCENARIO_BALANCE_DEBUG = 20,
    // The following 3 variables are used in the Crystal Cave overworld puzzle when a crystal
    // changes color.
    VAR_CRYSTAL_COLOR_01 = 21,
    VAR_CRYSTAL_COLOR_02 = 22,
    VAR_CRYSTAL_COLOR_03 = 23,
    // Used to identify a cutscene save in the maingame. While SCENARIO_MAIN_BIT_FLAG[6] and
    // SCENARIO_MAIN_BIT_FLAG[8] together identify that the game has been saved mid-cutscene, this
    // integer marks which part of the game the cutscene save happened in.
    VAR_COMPULSORY_SAVE_POINT = 24,
    // Used to identify a cutscene save in a Special Episode. Acts very similar to
    // COMPULSORY_SAVE_POINT.
    VAR_COMPULSORY_SAVE_POINT_SIDE = 25,
    // The backup version of SCENARIO_SELECT saved in the function ScenarioFlagBackup.
    VAR_SCENARIO_SELECT_BACKUP = 26,
    // The backup version of SCENARIO_MAIN_BIT_FLAG saved in the function ScenarioFlagBackup.
    VAR_SCENARIO_MAIN_BIT_FLAG_BACKUP = 27,
    // The Level ID (see the version-dependent script_level_id_* enums) that the player is in in the
    // overworld.
    VAR_GROUND_ENTER = 28,
    // Set to 0 in Unionall, in the coroutine EVENT_FORMATION. Doesn't seem to update when
    // transitioning to other maps in the overworld, so its true purpose is a bit unknown.
    VAR_GROUND_ENTER_LINK = 29,
    // The Level ID that the player just arrived from. Notoriously relied upon in Enter00 scripts to
    // determine where the player and partner actors should spawn if a map has multiple
    // entrances/exits.
    VAR_GROUND_GETOUT = 30,
    // The Level ID of the currently loaded background. Typically, this shares the same value as
    // GROUND_ENTER, but you could have an instance where you load LEVEL_P01P01A's Enter00 script
    // and have it display LEVEL_P01P03A's background.
    VAR_GROUND_MAP = 31,
    // The World Map Marker ID that determines where the player appears on the Top Screen when
    // selecting "Map and team" in the overworld.
    VAR_GROUND_PLACE = 32,
    // The backup version of GROUND_ENTER saved in the function EventFlagBackup.
    VAR_GROUND_ENTER_BACKUP = 33,
    // The backup version of GROUND_ENTER_LINK saved in the function EventFlagBackup.
    VAR_GROUND_ENTER_LINK_BACKUP = 34,
    // The backup version of GROUND_GETOUT saved in the function EventFlagBackup.
    VAR_GROUND_GETOUT_BACKUP = 35,
    // The backup version of GROUND_MAP saved in the function EventFlagBackup.
    VAR_GROUND_MAP_BACKUP = 36,
    // The backup version of GROUND_PLACE saved in the function EventFlagBackup.
    VAR_GROUND_PLACE_BACKUP = 37,
    // The ID of the dungeon chosen from the crossroads menu. Definitely set in various places, but
    // for sure at [NA]0x22E1DC8 and [NA]0x22DDA78.
    VAR_DUNGEON_SELECT = 38,
    // The ID of the dungeon being entered...sorta. It gets set in various places, and when the
    // dungeon being entered has its mode set to DMODE_OPEN or DMODE_CLOSED (please see
    // DUNGEON_ENTER_MODE for an explanation of dungeon modes), this matches the value of
    // DUNGEON_SELECT. However, other dungeon modes result in it being set to 215 (written at
    // [NA]0x204E848) or even 214 (written at [NA]0x204E8BC, checked at [NA]0x204F1D0).
    // Also, be sure to check out [NA]0x22E8A58 and [NA]0x22DDA88.
    VAR_DUNGEON_ENTER = 39,
    // The "mode" of the dungeon being entered. Each dungeon has an associated "dungeon mode", which
    // if you're familiar with ExplorerScript, you'll know as DMODE_OPEN, DMODE_CLOSED,
    // DMODE_REQUEST, and DMODE_OPEN_AND_REQUEST. This variable stores the dungeon mode, but this is
    // in fact represented by other variables as well.
    // This determines which coroutine to run upon reloading Unionall after a dungeon. DMODE_OPEN
    // and DMODE_CLOSED will make the game start from GETOUT_SCENARIO_DUNGEON, while the other two
    // will result in GETOUT_REQUEST_DUNGEON being run.
    // These modes will also determine if the specified dungeon appears in the crossroads menu.
    // DMODE_OPEN will make the dungeon appear at the crossroads with an exclamation mark next to
    // its name. DMODE_OPEN_AND_REQUEST will make the dungeon visible in the menu, but that's it.
    // Any other modes will not show the dungeon in the menu.
    VAR_DUNGEON_ENTER_MODE = 40,
    // The ID of the dungeon being entered.
    VAR_DUNGEON_ENTER_INDEX = 41,
    // A counter of how many dungeons the player has entered. Incremented by one when a dungeon is
    // exited, regardless of win/loss.
    VAR_DUNGEON_ENTER_FREQUENCY = 42,
    // The result of the dungeon being exited. 8 is a success and anything else is a failure (10 is
    // when the Escape Orb is used). This is somewhat similar to GROUND_START_MODE, but also gets
    // set to different values in the crossroads menu. Mid-dungeon, this is also the ID of the
    // dungeon.
    VAR_DUNGEON_RESULT = 43,
    // The mode set upon entering ground mode. This is set after starting a new game or loading a
    // save, but it's also most notable for being checked in Unionall's GETOUT coroutines to see if
    // the player won or lost in a dungeon. 8 is a success and anything else is a failure (10 is
    // when the Escape Orb is used).
    // It's a bit unknown how this is used outside of exiting dungeons.
    VAR_GROUND_START_MODE = 44,
    // The backup version of DUNGEON_ENTER_BACKUP, which is NOT saved in the function
    // EventFlagBackup...
    VAR_DUNGEON_ENTER_BACKUP = 45,
    // The backup version of DUNGEON_ENTER_MODE saved in the function EventFlagBackup.
    VAR_DUNGEON_ENTER_MODE_BACKUP = 46,
    // The backup version of DUNGEON_ENTER_INDEX saved in the function EventFlagBackup.
    VAR_DUNGEON_ENTER_INDEX_BACKUP = 47,
    // The backup version of DUNGEON_ENTER_FREQUENCY saved in the function EventFlagBackup.
    VAR_DUNGEON_ENTER_FREQUENCY_BACKUP = 48,
    // The backup version of DUNGEON_RESULT saved in the function EventFlagBackup.
    VAR_DUNGEON_RESULT_BACKUP = 49,
    // The backup version of GROUND_START_MODE saved in the function EventFlagBackup.
    VAR_GROUND_START_MODE_BACKUP = 50,
    // The number of Job Requests cleared in a single day. In the base game, Unionall relies on this
    // to decide when to progress the story.
    // One notable read is at [NA]0x22E6D34, which gets this variable's value and checks if it's
    // less than 100. If so, it increments it by one.
    VAR_REQUEST_CLEAR_COUNT = 51,
    // Handles how the player actor (ACTOR_PLAYER and possibly other related actors) will display.
    //   0: Species of Chimecho Assembly entry 0
    //   1: Species of Chimecho Assembly entry 1
    //   2: Nothing
    //   3: Species of the current party leader
    //   4: Species of ACTOR_ATTENDANT_BIPPA
    //   5: Species of ACTOR_ATTENDANT_PUPURIN
    //   6: Species of ACTOR_ATTENDANT_KIMAWARI
    //   7: Species of ACTOR_ATTENDANT_FUTURE
    //   8: Species of ACTOR_ATTENDANT_CHARMS
    //   9+: Nothing
    // Checked at [NA]0x20650E8 and [NA]0x2065188.
    VAR_PLAYER_KIND = 52,
    // Handles how the partner actor (ACTOR_ATTENDANT1 and possibly other related actors) will
    // display. It also notably is checked in the Unionall coroutine TALK_ATTENDANT (when you press
    // R to talk to the partner).
    //   0: Nothing
    //   1: Species of Chimecho Assembly entry 0
    //   2: Species of Chimecho Assembly entry 1
    //   3: Nothing
    //   4: Species of the current party leader
    //   5-9: Nothing
    //   10: Species of ACTOR_ATTENDANT_MANAFI (who moves incredibly slowly)
    // Checked at [NA]0x20651BC and [NA]0x22F7EEC.
    VAR_ATTENDANT1_KIND = 53,
    // Handles how the second partner actor (ACTOR_ATTENDANT2 and possibly other related actors)
    // will display. If this variable is 10, then ACTOR_ATTENDANT2 will be the species of
    // ACTOR_ATTENDANT_MANAFI. Anything else is nothing.
    // Checked at [NA]0x20651CC.
    VAR_ATTENDANT2_KIND = 54,
    // The backup version of PLAYER_KIND saved in the function EventFlagBackup.
    VAR_PLAYER_KIND_BACKUP = 55,
    // The backup version of ATTENDANT1_KIND, saved in the function EventFlagBackup.
    VAR_ATTENDANT1_KIND_BACKUP = 56,
    // The backup version of ATTENDANT2_KIND, saved in the function EventFlagBackup.
    VAR_ATTENDANT2_KIND_BACKUP = 57,
    VAR_FRIEND_SUM = 58,
    VAR_UNIT_SUM = 59,
    // The amount of money the player has on hand.
    // TODO: This is probably backed by MONEY_CARRIED? Need to confirm.
    VAR_CARRY_GOLD = 60,
    // The amount of money stored in Duskull Bank.
    // TODO: This is probably backed by MONEY_STORED? Need to confirm.
    VAR_BANK_GOLD = 61,
    // The species ID of the initial starter. Saved at [NA]0x2048868, which is done by the special
    // process INIT_MAIN_TEAM_AFTER_QUIZ.
    // This remains unchanged even if Chimecho Assembly entry 0 is modified in any way.
    VAR_HERO_FIRST_KIND = 62,
    // The name of the initial starter. Saved at [NA]0x2048878, which is done by the special process
    // INIT_MAIN_TEAM_AFTER_QUIZ.
    // This remains unchanged even if Chimecho Assembly entry 0 is modified in any way.
    VAR_HERO_FIRST_NAME = 63,
    // The species ID of the initial partner. Saved at [NA]0x20488D8, which is done by the special
    // process INIT_MAIN_TEAM_AFTER_QUIZ.
    // This remains unchanged even if Chimecho Assembly entry 1 is modified in any way.
    VAR_PARTNER_FIRST_KIND = 64,
    // The name of the initial partner. Saved at [NA]0x20488E8, which is done by the special process
    // INIT_MAIN_TEAM_AFTER_QUIZ.
    // This remains unchanged even if Chimecho Assembly entry 1 is modified in any way.
    VAR_PARTNER_FIRST_NAME = 65,
    // The "talk kind" of the initial starter, typically used in scripts where the player speaks;
    // see enum talk_kind (4, 5). This is most likely done to differentiate speech patterns between
    // genders (this probably has a purpose in Japanese, just not in English).
    // Saved at [NA]0x204893C, which is done by the special process INIT_MAIN_TEAM_AFTER_QUIZ. This
    // remains unchanged even if Chimecho Assembly entry 0 is modified in any way.
    VAR_HERO_TALK_KIND = 66,
    // The "talk kind" of the initial partner, typically used in scripts where the partner speaks to
    // differentiate speech patterns; see enum talk_kind (1, 2, 3). Unlike HERO_TALK_KIND, there's
    // actually a table that maps starters to certain talk kinds ([NA]0x209CCE4). Each entry
    // consists of a talk kind and a species ID.
    // Saved at [NA]0x204897C, which is done by the special process INIT_MAIN_TEAM_AFTER_QUIZ. This
    // remains unchanged even if Chimecho Assembly entry 1 is modified in any way.
    VAR_PARTNER_TALK_KIND = 67,
    // Determines which species the actor ACTOR_RANDOM_REQUEST_NPC03 will appear as. Saved at
    // [NA]0x2065BA4.
    // True to its name, this variable is randomly generated in the function [NA]0x205EB5C, which is
    // called within GenerateDailyMissions. There also seems to be an exception where the game
    // doesn't generate the species randomly (for Magnemite, maybe).
    VAR_RANDOM_REQUEST_NPC03_KIND = 68,
    // The value of the word at [NA]0x20AFF28. Saved at [NA]0x2048990, which is done by the special
    // process INIT_MAIN_TEAM_AFTER_QUIZ. Changing the variable doesn't seem to do much, so its true
    // purpose is unknown.
    VAR_CONFIG_COLOR_KIND = 69,
    // Its purpose is unknown, but it's initialized to 0 at [NA]0x204B0E4.
    VAR_ROM_VARIATION = 70,
    // The language determined by the NDS firmware. Notably checked in some scripts to display
    // different backgrounds or objects, such as in the intro.
    //   0: Japanese
    //   1: English
    //   2: French
    //   3: German
    //   4: Italian
    //   5: Spanish
    VAR_LANGUAGE_TYPE = 71,
    // A value to distinguish the type of game being played; see enum game_mode and the GAME_MODE
    // data symbol. This mainly differentiates between maingame and Special Episodes, and the game
    // checks for this value in numerous places (not necessarily in scripts) to differentiate
    // maingame versus Special Episode behavior. This can include whether to display the green
    // window color, initializing the current party upon a new game/episode, and many other tasks.
    // The function [NA]0x204B018 sets GAME_MODE to a new value, which gets called in various places
    // within the function [NA]0x2065D1C. It seems dependent on the status code returned by
    // ReadSaveHeader.
    VAR_GAME_MODE = 72,
    // Used to distinguish which Special Episode is currently being played; see enum
    // special_episode_type. Unlike SPECIAL_EPISODE_TYPE, the base game scripts seem to prefer this
    // variable when checking which Special Episode is run.
    // This variable's value is backed by DEBUG_SPECIAL_EPISODE_NUMBER.
    VAR_EXECUTE_SPECIAL_EPISODE_TYPE = 73,
    // Used to distinguish which Special Episode is currently being played. Very similar to
    // EXECUTE_SPECIAL_EPISODE_TYPE, to the point where Adex doesn't know the intricate differences.
    // Probably saved in multiple places, but one notable instance is at [NA]0x204C910.
    VAR_SPECIAL_EPISODE_TYPE = 74,
    // Eight bitflags used to mark if a Special Episode is accessible through the Top Menu. These
    // are set at various points in the game's story in Acting scripts.
    //   0: SE1
    //   1: SE2
    //   2: SE3
    //   3: SE4
    //   4: SE5
    //   5: Unused
    //   6: Unused
    //   7: Unused
    VAR_SPECIAL_EPISODE_OPEN = 75,
    // Eight bitflags that are seemingly related to Special Episodes? We're fairly certain that
    // they're unused, but can't say anything for sure. Gets set as the hero is initalized for the
    // corresponding special episode.
    VAR_SPECIAL_EPISODE_OPEN_OLD = 76,
    // Eight bitflags used to mark if a Special Episode has been beaten. These are typically set in
    // an Acting script.
    //   0: SE1
    //   1: SE2
    //   2: SE3
    //   3: SE4
    //   4: SE5
    //   5: Unused
    //   6: Unused
    //   7: Unused
    // Checked at [NA]0x204CA30.
    VAR_SPECIAL_EPISODE_CONQUEST = 77,
    // 64 bitflags corresponding to various story-based unlocks. Known values:
    // https://docs.google.com/spreadsheets/d/1NK3wkC8mOCu-tVkxULLkhpgPR-glApiD2v3zp-mdW7w
    VAR_PERFORMANCE_PROGRESS_LIST = 78,
    // 256 bitflags used to determine if a dungeon will appear in the crossroads menu. Each bitflag
    // represents a dungeon ID (so 0 is Test Dungeon, 1 is Beach Cave, and so on).
    VAR_DUNGEON_OPEN_LIST = 79,
    // 256 bitflags that supposedly represent each dungeon. Checked at [NA]0x204CEE0. For what
    // purpose? No clue!
    // You'd think that this keeps track of which dungeons you've entered, but that doesn't seem to
    // be the case.
    VAR_DUNGEON_ENTER_LIST = 80,
    // 256 bitflags that supposedly represent each dungeon. For what purpose? No clue!
    VAR_DUNGEON_ARRIVE_LIST = 81,
    // 256 bitflags used to determine if a dungeon has been beaten before. Each bitflag represents a
    // dungeon ID (so 0 is Test Dungeon, 1 is Beach Cave, and so on).
    VAR_DUNGEON_CONQUEST_LIST = 82,
    // 256 bitflags that supposedly represent each dungeon. For what purpose? No clue!
    VAR_DUNGEON_PRESENT_LIST = 83,
    // 256 bitflags used to determine if beating a dungeon will result in the game branching to
    // GETOUT_REQUEST_DUNGEON or GETOUT_SCENARIO_DUNGEON. Each bitflag represents a dungeon ID (so 0
    // is Test Dungeon, 1 is Beach Cave, and so on).
    // To clarify, if a bitflag X from here is set to 0 and DUNGEON_OPEN_LIST[X] is 1, then the
    // dungeon will show with an exclamation mark next to its name. If beaten, the game will start
    // running code in the Unionall coroutine GETOUT_SCENARIO_DUNGEON.
    VAR_DUNGEON_REQUEST_LIST = 84,
    // 320 bitflags used for...something.
    VAR_WORLD_MAP_MARK_LIST_NORMAL = 85,
    // 320 bitflags used for...something.
    VAR_WORLD_MAP_MARK_LIST_SPECIAL = 86,
    // A value used to determine which clouds block certain areas in the level S01P01A. Or it might
    // be more accurate to say "which position of the map will be shown", but the clouds actually
    // have an animation, so there must also be some hard-coded aspect to that level.
    // Saved at [NA]0x204CDF8.
    // Read at [NA]0x204CDD8.
    VAR_WORLD_MAP_LEVEL = 87,
    // The reason why the POSITION variables are even read is because of another command:
    // LoadPosition. This updates an actor, object, or performer’s x, y, height, and direction using
    // those variables
    //
    // Three indexed values used to save the X position of an actor, object, or performer. This
    // value is stored as Pixel << 8.
    // This is only updated via the script command SavePosition, which also updates the other
    // POSITION variables.
    // Saved at [NA]0x22E0E10.
    // Read at [NA]0x22E0D18.
    VAR_POSITION_X = 88,
    // Three indexed values used to save the Y position of an actor, object, or performer. This
    // value is stored as Pixel << 8.
    // This is only updated via the script command SavePosition, which also updates the other
    // POSITION variables.
    // Saved at [NA]0x22E0E28.
    // Read at [NA]0x22E0D30.
    VAR_POSITION_Y = 89,
    // Three indexed values used to save the height of an actor, object, or performer. This value is
    // stored as Pixel << 8.
    // This is only updated via the script command SavePosition, which also updates the other
    // POSITION variables.
    // Saved at [NA]0x22E0E40.
    // Read at [NA]0x22E0D48.
    VAR_POSITION_HEIGHT = 90,
    // Three indexed values used to save the direction of an actor, object, or performer. See enum
    // direction_id.
    // This is only updated via the script command SavePosition, which also updates the other
    // POSITION variables.
    // Saved at [NA]0x22E0E58.
    // Read at [NA]0x22E0D60.
    VAR_POSITION_DIRECTION = 91,
    // A variable used for a variety of purposes. As the name suggests, this is somewhat of a
    // "local" variable, in that the game's convention with this is basically all over the place.
    // Cutscenes often use it when needing to loop an actor's action. Another notable use is
    // deciding which letter the player receives in S00P01A/DOPEN. There are also some writes/reads
    // scattered around the game's code, but they all probably do some equally random shenanigans.
    VAR_EVENT_LOCAL = 92,
    // A variable used for a variety of purposes. Seems to behave similarly to EVENT_LOCAL, but the
    // script engine doesn't use this as often. One notable use is determining the gender of the
    // Heatran you fight (and recruit) at Giant Volcano.
    VAR_DUNGEON_EVENT_LOCAL = 93,
    // 32 bitflags used for...something.
    VAR_STATION_ITEM_STATIC = 94,
    // 32 bitflags used for...something.
    VAR_STATION_ITEM_TEMP = 95,
    // 16 bitflags used for...something.
    VAR_DELIVER_ITEM_STATIC = 96,
    // 16 bitflags used for...something.
    VAR_DELIVER_ITEM_TEMP = 97,
    // 5 bitflags used to conditionally show menu options in Drifblim's Gondola in Sky Peak.
    VAR_BIT_FUWARANTE_LOCAL = 98,
    // The result of a Prize Ticket drawing, which is determined randomly in P01P04A/S30A0601.
    VAR_LOTTERY_RESULT = 99,
    VAR_ITEM_BACKUP = 100,
    VAR_ITEM_BACKUP_KUREKURE = 101,
    VAR_ITEM_BACKUP_TAKE = 102,
    VAR_ITEM_BACKUP_GET = 103,
    VAR_REQUEST_THANKS_RESULT_KIND = 104,
    VAR_REQUEST_THANKS_RESULT_VARIATION = 105,
    // A flag used to determine if Wynaut at Spinda's Cafe should include more recyclable items in
    // the Recycle Shop.
    // Set to 1 at [NA]0x2012078.
    // Set to 0 in the function [NA]0x2011AB8.
    VAR_SUB30_TREASURE_DISCOVER = 106,
    // A flag used to determine if Wynaut at Spinda's Cafe should unlock a dungeon.
    // Set to 1 at [NA]0x2012100.
    // Set to 0 in the function [NA]0x2011AB8.
    VAR_SUB30_SPOT_DISCOVER = 107,
    // Incremented at the Recycle Shop for every item you trade.
    // Read at [NA]0x2012004, which is involved in determining several of the SUB30 flags being set.
    // Set to 0 in the function [NA]0x2011AB8.
    VAR_RECYCLE_COUNT = 108,
    // This determines which dungeon will get unlocked by Wynaut after recycling various times.
    // Specifically, this gets checked in the script P01P04A/S30A0501.
    // Incremented by one at [NA]0x20120F0.
    // Set to 0 in the function [NA]0x2011AB8.
    VAR_SUB30_SPOT_LEVEL = 109,
    VAR_TEAM_RANK_EVENT_LEVEL = 110,
    // The value used to keep track of the player's answer to the Personality Quiz question "Did you
    // play 'Explorers of Time' or 'Explorers of Darkness'?"
    VAR_PLAY_OLD_GAME = 111,
    VAR_NOTE_MODIFY_FLAG = 112,
    VAR_SUB30_PROJECTP = 113,
    VAR_SUM = 114, // This seems to be an end marker or some sort of special variable?

    // Local variables start at ID 0x400
    VAR_LOCAL0 = 1024,
    VAR_LOCAL1 = 1025,
    VAR_LOCAL2 = 1026,
    VAR_LOCAL3 = 1027,
};

// Script variable types.
enum script_var_type {
    VARTYPE_NONE = 0,
    VARTYPE_BIT = 1,
    // VARTYPE_STRING is an array of characters, and implementation-wise are treated exactly the
    // same way as VARTYPE_UINT8
    VARTYPE_STRING = 2,
    VARTYPE_UINT8 = 3,
    VARTYPE_INT8 = 4,
    VARTYPE_UINT16 = 5,
    VARTYPE_INT16 = 6,
    VARTYPE_UINT32 = 7,
    VARTYPE_INT32 = 8,
    VARTYPE_SPECIAL = 9,
};

// This is usually stored as a 16-bit integer
#pragma pack(push, 2)
ENUM_16_BIT(script_var_type);
#pragma pack(pop)

// Opcodes available to the script engine.
enum script_opcode_id {
    OPCODE_NULL = 0,
    OPCODE_BACK_CHANGE_GROUND = 1,
    OPCODE_BACK_SET_BACK_EFFECT = 2,
    OPCODE_BACK_SET_BACK_SCROLL_OFFSET = 3,
    OPCODE_BACK_SET_BACK_SCROLL_SPEED = 4,
    OPCODE_BACK_SET_BANNER = 5,
    OPCODE_BACK_SET_BANNER2 = 6,
    OPCODE_BACK_SET_EFFECT = 7,
    OPCODE_BACK_SET_DUNGEON_BANNER = 8,
    OPCODE_BACK_SET_GROUND = 9,
    OPCODE_BACK_SET_SPECIAL_EPISODE_BANNER = 10,
    OPCODE_BACK_SET_SPECIAL_EPISODE_BANNER2 = 11,
    OPCODE_BACK_SET_SPECIAL_EPISODE_BANNER3 = 12,
    OPCODE_BACK_SET_TITLE_BANNER = 13,
    OPCODE_BACK_SET_WEATHER = 14,
    OPCODE_BACK_SET_WEATHER_EFFECT = 15,
    OPCODE_BACK_SET_WEATHER_SCROLL_OFFSET = 16,
    OPCODE_BACK_SET_WEATHER_SCROLL_SPEED = 17,
    OPCODE_BACK2_SET_BACK_EFFECT = 18,
    OPCODE_BACK2_SET_BACK_SCROLL_OFFSET = 19,
    OPCODE_BACK2_SET_BACK_SCROLL_SPEED = 20,
    OPCODE_BACK2_SET_DATA = 21,
    OPCODE_BACK2_SET_EFFECT = 22,
    OPCODE_BACK2_SET_GROUND = 23,
    OPCODE_BACK2_SET_MODE = 24,
    OPCODE_BACK2_SET_SPECIAL_ACTING = 25,
    OPCODE_BACK2_SET_WEATHER = 26,
    OPCODE_BACK2_SET_WEATHER_EFFECT = 27,
    OPCODE_BACK2_SET_WEATHER_SCROLL_OFFSET = 28,
    OPCODE_BACK2_SET_WEATHER_SCROLL_SPEED = 29,
    OPCODE_BGM_FADE_OUT = 30,
    OPCODE_BGM_PLAY = 31,
    OPCODE_BGM_PLAY_FADE_IN = 32,
    OPCODE_BGM_STOP = 33,
    OPCODE_BGM_CHANGE_VOLUME = 34,
    OPCODE_BGM2_FADE_OUT = 35,
    OPCODE_BGM2_PLAY = 36,
    OPCODE_BGM2_PLAY_FADE_IN = 37,
    OPCODE_BGM2_STOP = 38,
    OPCODE_BGM2_CHANGE_VOLUME = 39,
    OPCODE_BRANCH = 40,
    OPCODE_BRANCH_BIT = 41,
    OPCODE_BRANCH_DEBUG = 42,
    OPCODE_BRANCH_EDIT = 43,
    OPCODE_BRANCH_EXECUTE_SUB = 44,
    OPCODE_BRANCH_PERFORMANCE = 45,
    OPCODE_BRANCH_SCENARIO_NOW = 46,
    OPCODE_BRANCH_SCENARIO_NOW_AFTER = 47,
    OPCODE_BRANCH_SCENARIO_NOW_BEFORE = 48,
    OPCODE_BRANCH_SCENARIO_AFTER = 49,
    OPCODE_BRANCH_SCENARIO_BEFORE = 50,
    OPCODE_BRANCH_SUM = 51,
    OPCODE_BRANCH_VALUE = 52,
    OPCODE_BRANCH_VARIABLE = 53,
    OPCODE_BRANCH_VARIATION = 54,
    OPCODE_CALL = 55,
    OPCODE_CALL_COMMON = 56,
    OPCODE_CAMERA_MOVE2_DEFAULT = 57,
    OPCODE_CAMERA_MOVE2_MY_POSITION = 58,
    OPCODE_CAMERA_MOVE2_MYSELF = 59,
    OPCODE_CAMERA_MOVE2_POSITION_MARK_VARIADIC = 60,
    OPCODE_CAMERA_MOVE2_POSITION_MARK = 61,
    OPCODE_CAMERA_MOVE3_DEFAULT = 62,
    OPCODE_CAMERA_MOVE3_MY_POSITION = 63,
    OPCODE_CAMERA_MOVE3_MYSELF = 64,
    OPCODE_CAMERA_MOVE3_POSITION_MARK_VARIADIC = 65,
    OPCODE_CAMERA_MOVE3_POSITION_MARK = 66,
    OPCODE_CAMERA_MOVE_DEFAULT = 67,
    OPCODE_CAMERA_MOVE_MY_POSITION = 68,
    OPCODE_CAMERA_MOVE_MYSELF = 69,
    OPCODE_CAMERA_MOVE_POSITION_MARK_VARIADIC = 70,
    OPCODE_CAMERA_MOVE_POSITION_MARK = 71,
    OPCODE_CAMERA_SET_DEFAULT = 72,
    OPCODE_CAMERA_SET_EFFECT = 73,
    OPCODE_CAMERA_SET_MY_POSITION = 74,
    OPCODE_CAMERA_SET_MYSELF = 75,
    OPCODE_CAMERA_SET_POSITION_MARK = 76,
    OPCODE_CAMERA2_MOVE2_DEFAULT = 77,
    OPCODE_CAMERA2_MOVE2_MY_POSITION = 78,
    OPCODE_CAMERA2_MOVE2_MYSELF = 79,
    OPCODE_CAMERA2_MOVE2_POSITION_MARK_VARIADIC = 80,
    OPCODE_CAMERA2_MOVE2_POSITION_MARK = 81,
    OPCODE_CAMERA2_MOVE3_DEFAULT = 82,
    OPCODE_CAMERA2_MOVE3_MY_POSITION = 83,
    OPCODE_CAMERA2_MOVE3_MYSELF = 84,
    OPCODE_CAMERA2_MOVE3_POSITION_MARK_VARIADIC = 85,
    OPCODE_CAMERA2_MOVE3_POSITION_MARK = 86,
    OPCODE_CAMERA2_MOVE_DEFAULT = 87,
    OPCODE_CAMERA2_MOVE_MY_POSITION = 88,
    OPCODE_CAMERA2_MOVE_MYSELF = 89,
    OPCODE_CAMERA2_MOVE_POSITION_MARK_VARIADIC = 90,
    OPCODE_CAMERA2_MOVE_POSITION_MARK = 91,
    OPCODE_CAMERA2_SET_DEFAULT = 92,
    OPCODE_CAMERA2_SET_EFFECT = 93,
    OPCODE_CAMERA2_SET_MY_POSITION = 94,
    OPCODE_CAMERA2_SET_MYSELF = 95,
    OPCODE_CAMERA2_SET_POSITION_MARK = 96,
    OPCODE_CANCEL_CUT = 97,
    OPCODE_CANCEL_RECOVER_COMMON = 98,
    OPCODE_CASE = 99,
    OPCODE_CASE_MENU = 100,
    OPCODE_CASE_MENU2 = 101,
    OPCODE_CASE_SCENARIO = 102,
    OPCODE_CASE_TEXT = 103,
    OPCODE_CASE_VALUE = 104,
    OPCODE_CASE_VARIABLE = 105,
    OPCODE_DEBUG_ASSERT = 106,
    OPCODE_DEBUG_PRINT = 107,
    OPCODE_DEBUG_PRINT_FLAG = 108,
    OPCODE_DEBUG_PRINT_SCENARIO = 109,
    OPCODE_DEFAULT_TEXT = 110,
    OPCODE_DESTROY = 111,
    OPCODE_END = 112,
    OPCODE_END_ANIMATION = 113,
    OPCODE_EXECUTE_ACTING = 114,
    OPCODE_EXECUTE_COMMON = 115,
    OPCODE_FLAG_CALC_BIT = 116,
    OPCODE_FLAG_CALC_VALUE = 117,
    OPCODE_FLAG_CALC_VARIABLE = 118,
    OPCODE_FLAG_CLEAR = 119,
    OPCODE_FLAG_INITIAL = 120,
    OPCODE_FLAG_SET = 121,
    OPCODE_FLAG_RESET_DUNGEON_RESULT = 122,
    OPCODE_FLAG_RESET_SCENARIO = 123,
    OPCODE_FLAG_SET_ADVENTURE_LOG = 124,
    OPCODE_FLAG_SET_DUNGEON_MODE = 125,
    OPCODE_FLAG_SET_DUNGEON_RESULT = 126,
    OPCODE_FLAG_SET_PERFORMANCE = 127,
    OPCODE_FLAG_SET_SCENARIO = 128,
    OPCODE_FLASH = 129,
    OPCODE_HOLD = 130,
    OPCODE_ITEM_GET_VARIABLE = 131,
    OPCODE_ITEM_SET = 132,
    OPCODE_ITEM_SET_TABLE_DATA = 133,
    OPCODE_ITEM_SET_VARIABLE = 134,
    OPCODE_JUMP = 135,
    OPCODE_JUMP_COMMON = 136,
    OPCODE_LIVES = 137,
    OPCODE_LOAD_POSITION = 138,
    OPCODE_LOCK = 139,
    OPCODE_MAIN_ENTER_ADVENTURE = 140,
    OPCODE_MAIN_ENTER_DUNGEON = 141,
    OPCODE_MAIN_ENTER_GROUND = 142,
    OPCODE_MAIN_ENTER_GROUND_MULTI = 143,
    OPCODE_MAIN_ENTER_RESCUE_USER = 144,
    OPCODE_MAIN_ENTER_TRAINING = 145,
    OPCODE_MAIN_ENTER_TRAINING2 = 146,
    OPCODE_MAIN_SET_GROUND = 147,
    OPCODE_ME_PLAY = 148,
    OPCODE_ME_STOP = 149,
    OPCODE_MESSAGE_CLOSE = 150,
    OPCODE_MESSAGE_CLOSE_ENFORCE = 151,
    OPCODE_MESSAGE_EXPLANATION = 152,
    OPCODE_MESSAGE_FACE_POSITION_OFFSET = 153,
    OPCODE_MESSAGE_IMITATION_SOUND = 154,
    OPCODE_MESSAGE_KEY_WAIT = 155,
    OPCODE_MESSAGE_MAIL = 156,
    OPCODE_MESSAGE_MENU = 157,
    OPCODE_MESSAGE_MONOLOGUE = 158,
    OPCODE_MESSAGE_NARRATION = 159,
    OPCODE_MESSAGE_NOTICE = 160,
    OPCODE_MESSAGE_EMPTY_ACTOR = 161,
    OPCODE_MESSAGE_RESET_ACTOR = 162,
    OPCODE_MESSAGE_SET_ACTOR = 163,
    OPCODE_MESSAGE_SET_FACE = 164,
    OPCODE_MESSAGE_SET_FACE_EMPTY = 165,
    OPCODE_MESSAGE_SET_FACE_ONLY = 166,
    OPCODE_MESSAGE_SET_FACE_POSITION = 167,
    OPCODE_MESSAGE_SET_WAIT_MODE = 168,
    OPCODE_MESSAGE_SPECIAL_TALK = 169,
    OPCODE_MESSAGE_SWITCH_MENU = 170,
    OPCODE_MESSAGE_SWITCH_MENU2 = 171,
    OPCODE_MESSAGE_SWITCH_MONOLOGUE = 172,
    OPCODE_MESSAGE_SWITCH_TALK = 173,
    OPCODE_MESSAGE_TALK = 174,
    OPCODE_MOVE2_POSITION = 175,
    OPCODE_MOVE2_POSITION_LIVES = 176,
    OPCODE_MOVE2_POSITION_MARK_VARIADIC = 177,
    OPCODE_MOVE2_POSITION_MARK = 178,
    OPCODE_MOVE2_POSITION_OFFSET_VARIADIC = 179,
    OPCODE_MOVE2_POSITION_OFFSET = 180,
    OPCODE_MOVE2_POSITION_OFFSET_RANDOM = 181,
    OPCODE_MOVE3_POSITION = 182,
    OPCODE_MOVE3_POSITION_LIVES = 183,
    OPCODE_MOVE3_POSITION_MARK_VARIADIC = 184,
    OPCODE_MOVE3_POSITION_MARK = 185,
    OPCODE_MOVE3_POSITION_OFFSET_VARIADIC = 186,
    OPCODE_MOVE3_POSITION_OFFSET = 187,
    OPCODE_MOVE3_POSITION_OFFSET_RANDOM = 188,
    OPCODE_MOVE_DIRECTION = 189,
    OPCODE_MOVE_HEIGHT = 190,
    OPCODE_MOVE_POSITION = 191,
    OPCODE_MOVE_POSITION_LIVES = 192,
    OPCODE_MOVE_POSITION_LIVES_TIME = 193,
    OPCODE_MOVE_POSITION_MARK_VARIADIC = 194,
    OPCODE_MOVE_POSITION_MARK = 195,
    OPCODE_MOVE_POSITION_MARK_TIME = 196,
    OPCODE_MOVE_POSITION_OFFSET_VARIADIC = 197,
    OPCODE_MOVE_POSITION_OFFSET = 198,
    OPCODE_MOVE_SPECIAL = 199,
    OPCODE_MOVE_TURN = 200,
    OPCODE_OBJECT = 201,
    OPCODE_PAUSE_EFFECT = 202,
    OPCODE_PERFORMER = 203,
    OPCODE_PROCESS_SPECIAL = 204,
    OPCODE_PURSUE_TURN_LIVES = 205,
    OPCODE_PURSUE_TURN_LIVES2 = 206,
    OPCODE_RESET_ATTRIBUTE = 207,
    OPCODE_RESET_FUNCTION_ATTRIBUTE = 208,
    OPCODE_RESET_HIT_ATTRIBUTE = 209,
    OPCODE_RESET_OUTPUT_ATTRIBUTE = 210,
    OPCODE_RESET_REPLY_ATTRIBUTE = 211,
    OPCODE_RESUME_EFFECT = 212,
    OPCODE_RETURN = 213,
    OPCODE_SAVE_POSITION = 214,
    OPCODE_SCREEN_FADE_CHANGE = 215,
    OPCODE_SCREEN_FADE_CHANGE_ALL = 216,
    OPCODE_SCREEN_FADE_IN = 217,
    OPCODE_SCREEN_FADE_IN_ALL = 218,
    OPCODE_SCREEN_FADE_OUT = 219,
    OPCODE_SCREEN_FADE_OUT_ALL = 220,
    OPCODE_SCREEN_FLUSH_CHANGE = 221,
    OPCODE_SCREEN_FLUSH_IN = 222,
    OPCODE_SCREEN_FLUSH_OUT = 223,
    OPCODE_SCREEN_WHITE_CHANGE = 224,
    OPCODE_SCREEN_WHITE_CHANGE_ALL = 225,
    OPCODE_SCREEN_WHITE_IN = 226,
    OPCODE_SCREEN_WHITE_IN_ALL = 227,
    OPCODE_SCREEN_WHITE_OUT = 228,
    OPCODE_SCREEN_WHITE_OUT_ALL = 229,
    OPCODE_SCREEN2_FADE_CHANGE = 230,
    OPCODE_SCREEN2_FADE_CHANGE_ALL = 231,
    OPCODE_SCREEN2_FADE_IN = 232,
    OPCODE_SCREEN2_FADE_IN_ALL = 233,
    OPCODE_SCREEN2_FADE_OUT = 234,
    OPCODE_SCREEN2_FADE_OUT_ALL = 235,
    OPCODE_SCREEN2_FLUSH_CHANGE = 236,
    OPCODE_SCREEN2_FLUSH_IN = 237,
    OPCODE_SCREEN2_FLUSH_OUT = 238,
    OPCODE_SCREEN2_WHITE_CHANGE = 239,
    OPCODE_SCREEN2_WHITE_CHANGE_ALL = 240,
    OPCODE_SCREEN2_WHITE_IN = 241,
    OPCODE_SCREEN2_WHITE_IN_ALL = 242,
    OPCODE_SCREEN2_WHITE_OUT = 243,
    OPCODE_SCREEN2_WHITE_OUT_ALL = 244,
    OPCODE_SE_CHANGE_PAN = 245,
    OPCODE_SE_CHANGE_VOLUME = 246,
    OPCODE_SE_FADE_OUT = 247,
    OPCODE_SE_PLAY = 248,
    OPCODE_SE_PLAY_FULL = 249,
    OPCODE_SE_PLAY_PAN = 250,
    OPCODE_SE_PLAY_VOLUME = 251,
    OPCODE_SE_STOP = 252,
    OPCODE_SET_ANIMATION = 253,
    OPCODE_SET_ATTRIBUTE = 254,
    OPCODE_SET_BLINK = 255,
    OPCODE_SET_DIRECTION = 256,
    OPCODE_SET_DIRECTION_LIVES = 257,
    OPCODE_SET_EFFECT = 258,
    OPCODE_SET_FUNCTION_ATTRIBUTE = 259,
    OPCODE_SET_HEIGHT = 260,
    OPCODE_SET_HIT_ATTRIBUTE = 261,
    OPCODE_SET_MOVE_RANGE = 262,
    OPCODE_SET_OUTPUT_ATTRIBUTE = 263,
    OPCODE_SET_POSITION = 264,
    OPCODE_SET_POSITION_INITIAL = 265,
    OPCODE_SET_POSITION_LIVES = 266,
    OPCODE_SET_POSITION_MARK = 267,
    OPCODE_SET_POSITION_OFFSET = 268,
    OPCODE_SET_POSITION_OFFSET_RANDOM = 269,
    OPCODE_SET_REPLY_ATTRIBUTE = 270,
    OPCODE_SETUP_OUTPUT_ATTRIBUTE_AND_ANIMATION = 271,
    OPCODE_SLIDE2_POSITION = 272,
    OPCODE_SLIDE2_POSITION_LIVES = 273,
    OPCODE_SLIDE2_POSITION_MARK_VARIADIC = 274,
    OPCODE_SLIDE2_POSITION_MARK = 275,
    OPCODE_SLIDE2_POSITION_OFFSET_VARIADIC = 276,
    OPCODE_SLIDE2_POSITION_OFFSET = 277,
    OPCODE_SLIDE2_POSITION_OFFSET_RANDOM = 278,
    OPCODE_SLIDE3_POSITION = 279,
    OPCODE_SLIDE3_POSITION_LIVES = 280,
    OPCODE_SLIDE3_POSITION_MARK_VARIADIC = 281,
    OPCODE_SLIDE3_POSITION_MARK = 282,
    OPCODE_SLIDE3_POSITION_OFFSET_VARIADIC = 283,
    OPCODE_SLIDE3_POSITION_OFFSET = 284,
    OPCODE_SLIDE3_POSITION_OFFSET_RANDOM = 285,
    OPCODE_SLIDE_HEIGHT = 286,
    OPCODE_SLIDE_POSITION = 287,
    OPCODE_SLIDE_POSITION_LIVES = 288,
    OPCODE_SLIDE_POSITION_LIVES_TIME = 289,
    OPCODE_SLIDE_POSITION_MARK_VARIADIC = 290,
    OPCODE_SLIDE_POSITION_MARK = 291,
    OPCODE_SLIDE_POSITION_MARK_TIME = 292,
    OPCODE_SLIDE_POSITION_OFFSET_VARIADIC = 293,
    OPCODE_SLIDE_POSITION_OFFSET = 294,
    OPCODE_SOUND_FADE_OUT = 295,
    OPCODE_SOUND_STOP = 296,
    OPCODE_STOP_ANIMATION = 297,
    OPCODE_SUPERVISION_ACTING = 298,
    OPCODE_SUPERVISION_ACTING_INVISIBLE = 299,
    OPCODE_SUPERVISION_EXECUTE_ACTING = 300,
    OPCODE_SUPERVISION_EXECUTE_ACTING_SUB = 301,
    OPCODE_SUPERVISION_EXECUTE_COMMON = 302,
    OPCODE_SUPERVISION_EXECUTE_ENTER = 303,
    OPCODE_SUPERVISION_EXECUTE_STATION = 304,
    OPCODE_SUPERVISION_EXECUTE_STATION_COMMON = 305,
    OPCODE_SUPERVISION_EXECUTE_STATION_COMMON_SUB = 306,
    OPCODE_SUPERVISION_EXECUTE_STATION_SUB = 307,
    OPCODE_SUPERVISION_EXECUTE_EXPORT = 308,
    OPCODE_SUPERVISION_EXECUTE_EXPORT_SUB = 309,
    OPCODE_SUPERVISION_LOAD_STATION = 310,
    OPCODE_SUPERVISION_REMOVE = 311,
    OPCODE_SUPERVISION_REMOVE_ACTING = 312,
    OPCODE_SUPERVISION_REMOVE_COMMON = 313,
    OPCODE_SUPERVISION_SPECIAL_ACTING = 314,
    OPCODE_SUPERVISION_STATION = 315,
    OPCODE_SUPERVISION_STATION_COMMON = 316,
    OPCODE_SUPERVISION_SUSPEND = 317,
    OPCODE_SUPERVISION2_SPECIAL_ACTING = 318,
    OPCODE_SWITCH = 319,
    OPCODE_SWITCH_DIRECTION = 320,
    OPCODE_SWITCH_DIRECTION_LIVES = 321,
    OPCODE_SWITCH_DIRECTION_LIVES2 = 322,
    OPCODE_SWITCH_DIRECTION_MARK = 323,
    OPCODE_SWITCH_DUNGEON_MODE = 324,
    OPCODE_SWITCH_LIVES = 325,
    OPCODE_SWITCH_RANDOM = 326,
    OPCODE_SWITCH_SCENARIO = 327,
    OPCODE_SWITCH_SCENARIO_LEVEL = 328,
    OPCODE_SWITCH_SECTOR = 329,
    OPCODE_SWITCH_VALUE = 330,
    OPCODE_SWITCH_VARIABLE = 331,
    OPCODE_TURN2_DIRECTION = 332,
    OPCODE_TURN2_DIRECTION_LIVES = 333,
    OPCODE_TURN2_DIRECTION_LIVES2 = 334,
    OPCODE_TURN2_DIRECTION_MARK = 335,
    OPCODE_TURN2_DIRECTION_TURN = 336,
    OPCODE_TURN3 = 337,
    OPCODE_TURN_DIRECTION = 338,
    OPCODE_TURN_DIRECTION_LIVES = 339,
    OPCODE_TURN_DIRECTION_LIVES2 = 340,
    OPCODE_TURN_DIRECTION_MARK = 341,
    OPCODE_UNLOCK = 342,
    OPCODE_WAIT = 343,
    OPCODE_WAIT_ANIMATION = 344,
    OPCODE_WAIT_BACK_EFFECT = 345,
    OPCODE_WAIT_BACK2_EFFECT = 346,
    OPCODE_WAIT_BGM = 347,
    OPCODE_WAIT_BGM2 = 348,
    OPCODE_WAIT_BGM_SIGNAL = 349,
    OPCODE_WAIT_EFFECT = 350,
    OPCODE_WAIT_END_ANIMATION = 351,
    OPCODE_WAIT_EXECUTE_LIVES = 352,
    OPCODE_WAIT_EXECUTE_OBJECT = 353,
    OPCODE_WAIT_EXECUTE_PERFORMER = 354,
    OPCODE_WAIT_FADE_IN = 355,
    OPCODE_WAIT_LOCK_LIVES = 356,
    OPCODE_WAIT_LOCK_OBJECT = 357,
    OPCODE_WAIT_LOCK_PERFORMER = 358,
    OPCODE_WAIT_LOCK_SUPERVISION = 359,
    OPCODE_WAIT_ME = 360,
    OPCODE_WAIT_MOVE_CAMERA = 361,
    OPCODE_WAIT_MOVE_CAMERA2 = 362,
    OPCODE_WAIT_RANDOM = 363,
    OPCODE_WAIT_SCREEN_FADE = 364,
    OPCODE_WAIT_SCREEN_FADE_ALL = 365,
    OPCODE_WAIT_SCREEN2_FADE = 366,
    OPCODE_WAIT_SE = 367,
    OPCODE_WAIT_SPECIAL_ACTING = 368,
    OPCODE_WAIT_SUB_SCREEN = 369,
    OPCODE_WAIT_SUB_SPECIAL_ACTING = 370,
    OPCODE_WORLDMAP_BLINK_MARK = 371,
    OPCODE_WORLDMAP_CHANGE_LEVEL = 372,
    OPCODE_WORLDMAP_DELETE_ARROW = 373,
    OPCODE_WORLDMAP_MOVE_CAMERA = 374,
    OPCODE_WORLDMAP_OFF_MESSAGE = 375,
    OPCODE_WORLDMAP_SET_ARROW = 376,
    OPCODE_WORLDMAP_SET_CAMERA = 377,
    OPCODE_WORLDMAP_SET_LEVEL = 378,
    OPCODE_WORLDMAP_SET_MARK = 379,
    OPCODE_WORLDMAP_SET_MESSAGE = 380,
    OPCODE_WORLDMAP_SET_MESSAGE_PLACE = 381,
    OPCODE_WORLDMAP_SET_MODE = 382,
};

// Special processes invoked through script calls to OPCODE_PROCESS_SPECIAL.
// Descriptions are (mostly) taken from pmd2scriptdata.xml from skytemple_files.
// Also see the corresponding functions that implement some of the special processes.
enum special_process_id {
    SPECIAL_PROC_NONE = 0,
    SPECIAL_PROC_RETURN_DUNGEON = 1, // Named after what it logs
    SPECIAL_PROC_INIT_MAIN_TEAM_AFTER_QUIZ = 2,
    // Special 3 and 4 branch off into functions with a LOT of switch cases.
    // Both seem to reset the active roster to the main characters most of the time...?
    // Both seem to be team management functions for their special episodes...?
    SPECIAL_PROC_0x3 = 3,
    SPECIAL_PROC_0x4 = 4,
    SPECIAL_PROC_NEXT_DAY = 5,
    // Appears to check if the dungeon had a request done, but also checks if the
    // dungeon's mode is DMODE_REQUEST or DMODE_OPEN_AND_REQUEST?
    SPECIAL_PROC_0x6 = 6,
    SPECIAL_PROC_DUNGEON_HAD_REQUEST_DONE = 7, // Returns true if the player finished a mission
    // Special 8 and 9 are unused but are checks related to active missions in the pending
    // dungeon. Special 8 appears to check for any active missions while Special 9 also checks
    // for any active missions and (if applicable) if the required item is in the treasure bag.
    SPECIAL_PROC_0x8 = 8,
    SPECIAL_PROC_0x9 = 9,
    SPECIAL_PROC_GET_SOS_MAIL_COUNT = 10,
    SPECIAL_PROC_GET_SOS_MISSION_RETRIES_LEFT = 11,
    // The following are more or less guesses based on P01P04A/enter11.ssb and some other maps
    SPECIAL_PROC_INIT_RANDOM_NPC_JOBS = 12,
    SPECIAL_PROC_GET_RANDOM_NPC_JOB_TYPE = 13,
    SPECIAL_PROC_GET_RANDOM_NPC_JOB_SUBTYPE = 14,
    SPECIAL_PROC_GET_RANDOM_NPC_JOB_STILL_AVAILABLE = 15,
    SPECIAL_PROC_ACCEPT_RANDOM_NPC_JOB = 16,
    // 17 just returns 0, probably a removed debug function. Might be related to some kind of
    // debug camera control. 18 seems to execute the debug camera.
    SPECIAL_PROC_INIT_SPECIAL_EPISODE_PARTNERS = 17,
    SPECIAL_PROC_INIT_EXTRA_SPECIAL_EPISODE_PARTNERS = 18,
    SPECIAL_PROC_KEY_WAIT_PRESS = 19, // Equivalent to SPECIAL_PROC_KEY_WAIT_CHECK
    SPECIAL_PROC_KEY_WAIT_INIT = 20,
    SPECIAL_PROC_KEY_WAIT_CHECK = 21,
    // 22 has something to do with processing inputs during the title screen sequence, but
    // I'm not 100% sure which one does what. 24 too, and seems to indicate that the sequence is
    // only skipable if a save file exits?
    SPECIAL_PROC_0x16 = 22,
    SPECIAL_PROC_RANDOMIZE_DEMO_ACTORS = 23,
    SPECIAL_PROC_0x18 = 24, // Equivalent to SPECIAL_PROC_JUMP_TO_TITLE_SCREEN
    SPECIAL_PROC_JUMP_TO_TITLE_SCREEN = 25,
    SPECIAL_PROC_0x1A = 26, // Possibly jumps to the demo version of the title screen?
    SPECIAL_PROC_RETURN_TO_TITLE_SCREEN = 27,
    SPECIAL_PROC_IS_RECRUITABLE_IN_TEAM = 28,
    SPECIAL_PROC_GET_RECRUITABLE_CURRENT_LEVEL = 29,
    SPECIAL_PROC_RECRUITABLE_CAN_ADD = 30,
    SPECIAL_PROC_ADD_RECRUITABLE_TO_TEAM = 31,
    SPECIAL_PROC_COUNT_ITEM_TYPE_IN_BAG = 32,
    SPECIAL_PROC_COUNT_TABLE_ITEM_TYPE_IN_BAG = 33, // Reads from item table slot 0 instead
    SPECIAL_PROC_COUNT_ITEM_TYPE_IN_STORAGE = 34,
    SPECIAL_PROC_COUNT_TABLE_ITEM_TYPE_IN_STORAGE = 35, // Unused
    SPECIAL_PROC_ADD_ITEM_TO_BAG = 36,
    SPECIAL_PROC_ADD_TABLE_ITEM_TO_BAG = 37,
    SPECIAL_PROC_ADD_ITEM_TO_STORAGE = 38, // Unused
    SPECIAL_PROC_ADD_TABLE_ITEM_TO_STORAGE = 39,
    SPECIAL_PROC_REMOVE_ITEMS_TYPE_IN_BAG = 40,
    SPECIAL_PROC_REMOVE_TABLE_ITEMS_TYPE_IN_BAG = 41,
    SPECIAL_PROC_REMOVE_ITEM_TYPE_IN_STORAGE = 42,
    SPECIAL_PROC_REMOVE_TABLE_ITEM_TYPE_IN_STORAGE = 43,
    SPECIAL_PROC_PREPARE_MENU_ACCEPT_TEAM_MEMBER = 44,
    SPECIAL_PROC_GET_HERO_KIND = 45,
    SPECIAL_PROC_GET_PARTNER_KIND = 46,
    SPECIAL_PROC_IS_TEAM_SETUP_HERO_AND_PARTNER_ONLY = 47,
    SPECIAL_PROC_IS_TEAM_SETUP_PARTNER_AND_HERO_ONLY = 48,
    SPECIAL_PROC_IS_TEAM_SETUP_CONTAINS_PARTNER_AND_HERO = 49,
    SPECIAL_PROC_IS_TEAM_SETUP_SOLO = 50,
    SPECIAL_PROC_SET_TEAM_SETUP_HERO_AND_PARTNER_ONLY = 51,
    SPECIAL_PROC_SET_TEAM_SETUP_HERO_ONLY = 52,
    // 53 is used when ending the demo game. It just returns 0, probably a removed debug function.
    SPECIAL_PROC_0x35 = 53,
    // That's what it's called on the debug map.
    // It sets a single value, but I don't know the effect.
    SPECIAL_PROC_STATUS_UPDATE = 54,
    // 55 is sometimes used at the end of dungeons before the exploration results.
    // Don't know what it does.
    SPECIAL_PROC_0x37 = 55,
    SPECIAL_PROC_IS_BAG_FULL = 56,
    // 57 and 58 are unused but sure do something! Don't know what though.
    // 57 might be "IsStorageFull"?
    SPECIAL_PROC_0x39 = 57,
    SPECIAL_PROC_INCREMENT_DUNGEONS_CLEARED = 58,
    SPECIAL_PROC_INCREMENT_BIG_TREASURE_WINS = 59,
    SPECIAL_PROC_SEND_SKY_GIFT_TO_GUILDMASTER = 60,
    SPECIAL_PROC_SET_ACTOR_TALK_MAIN = 61,
    SPECIAL_PROC_SET_ACTOR_TALK_SUB = 62,
};

// Common routines (often abbreviated as "coroutine" or "coro" by the community) available to the
// script engine.
enum common_routine_id {
    ROUTINE_END_TALK = 0,
    ROUTINE_WAIT_START_FUNC = 1,
    ROUTINE_NORMAL_WAIT_END_TALK = 2,
    ROUTINE_LIVES_REPLY_NORMAL = 3,
    ROUTINE_LIVES_REPLY = 4,
    ROUTINE_OBJECT_REPLY_NORMAL = 5,
    ROUTINE_LIVES_ACTING_NORMAL = 6,
    ROUTINE_ENTER_WAIT_FUNC = 7,
    ROUTINE_TALK_UNIT = 8,
    ROUTINE_TALK_ADVENTURE = 9,
    ROUTINE_TALK_ATTENDANT = 10,
    ROUTINE_TALK_PARTNER_MESSAGE = 11,
    ROUTINE_LIVES_MOVE_NORMAL = 12,
    ROUTINE_LIVES_MOVE_CHANGE = 13,
    ROUTINE_LIVES_MOVE_PARTY = 14,
    ROUTINE_OBJECT_MOVE_NORMAL = 15,
    ROUTINE_OBJECT_MOVE_CHANGE = 16,
    ROUTINE_PERFORMER_MOVE_NORMAL = 17,
    ROUTINE_PERFORMER_MOVE_CHANGE = 18,
    ROUTINE_MOVE_INIT = 19,
    ROUTINE_MOVE_PAUSE = 20,
    ROUTINE_MOVE_STAY = 21,
    ROUTINE_MOVE_SLEEP = 22,
    ROUTINE_MOVE_RANDOM = 23,
    ROUTINE_MOVE_TYPE1 = 24,
    ROUTINE_MOVE_TYPE2 = 25,
    ROUTINE_WAKEUP_FUNC = 26,
    ROUTINE_WAKEUP_FUNC_SERIES = 27,
    ROUTINE_LOOK_AROUND_FUNC = 28,
    ROUTINE_LOOK_AROUND_FUNC_SERIES = 29,
    ROUTINE_LOOK_AROUND_DOWN_FUNC = 30,
    ROUTINE_LOOK_AROUND_DOWN_FUNC_SERIES = 31,
    ROUTINE_LOOK_AROUND_RIGHT_FUNC = 32,
    ROUTINE_LOOK_AROUND_RIGHT_FUNC_SERIES = 33,
    ROUTINE_LOOK_AROUND_LEFT_FUNC = 34,
    ROUTINE_LOOK_AROUND_LEFT_FUNC_SERIES = 35,
    ROUTINE_JUMP_HAPPY_FUNC = 36,
    ROUTINE_JUMP_HAPPY_FUNC_SERIES = 37,
    ROUTINE_JUMP_SURPRISE_FUNC = 38,
    ROUTINE_JUMP_SURPRISE_FUNC_SERIES = 39,
    ROUTINE_JUMP_ANGRY_FUNC = 40,
    ROUTINE_JUMP_ANGRY_FUNC_SERIES = 41,
    ROUTINE_LOOK_AROUND_FAST_FUNC = 42,
    ROUTINE_LOOK_AROUND_FAST_FUNC_SERIES = 43,
    ROUTINE_HEAD_SHAKE_FUNC = 44,
    ROUTINE_HEAD_SHAKE_FUNC_SERIES = 45,
    ROUTINE_EXPLANATION_FUNC = 46,
    ROUTINE_EXPLANATION_FUNC_SERIES = 47,
    ROUTINE_INIT_SLEEP_FUNC = 48,
    ROUTINE_INIT_SLEEP_FUNC_SERIES = 49,
    ROUTINE_INIT_BASE_FUNC = 50,
    ROUTINE_INIT_BASE_FUNC_SERIES = 51,
    ROUTINE_HANYOU_SAVE_FUNC = 52,
    ROUTINE_NORMAL_MESSAGE = 53,
    ROUTINE_NORMAL_EVENT = 54,
    ROUTINE_NORMAL_CAMERA = 55,
    ROUTINE_EVENT_FORMATION = 56,
    ROUTINE_EVENT_EVOLUTION = 57,
    ROUTINE_EFFECT_MOVE_DIVE = 58,
    ROUTINE_EFFECT_MOVE_WAVE = 59,
    ROUTINE_EVENT_DIVIDE = 60,
    ROUTINE_EVENT_DIVIDE_NEXT = 61,
    ROUTINE_EVENT_DIVIDE_INIT_FUNC = 62,
    ROUTINE_EVENT_DIVIDE_NEXT_DAY_FUNC = 63,
    ROUTINE_EVENT_DIVIDE_NEXT_DAY2_FUNC = 64,
    ROUTINE_EVENT_DIVIDE_FIRST = 65,
    ROUTINE_EVENT_DIVIDE_SECOND = 66,
    ROUTINE_EVENT_DIVIDE_AFTER = 67,
    ROUTINE_EVENT_DIVIDE_RESUME = 68,
    ROUTINE_EVENT_DIVIDE_SPECIAL_EPISODE = 69,
    ROUTINE_DEBUG_SCRIPT = 70,
    ROUTINE_DEBUG_SCENE = 71,
    ROUTINE_DEMO_CANCEL = 72,
    ROUTINE_DEBUG_FLAG_SETTING_BIPPA = 73,
    ROUTINE_DEBUG_FLAG_SETTING_PUPURIN = 74,
    ROUTINE_DEBUG_FLAG_SETTING_FUTURE = 75,
    ROUTINE_DEBUG_FLAG_SETTING_CHARMS = 76,
    ROUTINE_DEBUG_FLAG_SETTING_KIMAWARI = 77,
    ROUTINE_DEMO_01 = 78,
    ROUTINE_DEMO_02 = 79,
    ROUTINE_DEMO_03 = 80,
    ROUTINE_DEMO_04 = 81,
    ROUTINE_DEMO_05 = 82,
    ROUTINE_DEMO_06 = 83,
    ROUTINE_EVENT_M00A_01 = 84,
    ROUTINE_EVENT_HA_YU_KI = 85,
    ROUTINE_EVENT_HA_YU_KI_DO = 86,
    ROUTINE_EVENT_HA_YU_SHU = 87,
    ROUTINE_EVENT_HA_YU_SHU_DO = 88,
    ROUTINE_EVENT_HA_CHO = 89,
    ROUTINE_EVENT_HA_CHO_DO = 90,
    ROUTINE_EVENT_HA_KI = 91,
    ROUTINE_EVENT_HA_YU_KIX = 92,
    ROUTINE_EVENT_HA_YU_KI_DOX = 93,
    ROUTINE_EVENT_HA_YU_SHUX = 94,
    ROUTINE_EVENT_HA_YU_SHU_DOX = 95,
    ROUTINE_EVENT_HA_SAME = 96,
    ROUTINE_HA_MIHARIBAN = 97,
    ROUTINE_EVENT_HA_SAMEX = 98,
    ROUTINE_EVENT_M00_01 = 99,
    ROUTINE_EVENT_M00_02 = 100,
    ROUTINE_EVENT_M00_03 = 101,
    ROUTINE_EVENT_M00_04 = 102,
    ROUTINE_EVENT_M00_05 = 103,
    ROUTINE_EVENT_M00_06 = 104,
    ROUTINE_EVENT_M00_07 = 105,
    ROUTINE_EVENT_M00_08 = 106,
    ROUTINE_EVENT_M00_09 = 107,
    ROUTINE_EVENT_M00_12 = 108,
    ROUTINE_EVENT_M00_13 = 109,
    ROUTINE_EVENT_M00_14 = 110,
    ROUTINE_EVENT_M01_01_02 = 111,
    ROUTINE_EVENT_M01_03 = 112,
    ROUTINE_EVENT_M01_04 = 113,
    ROUTINE_EVENT_M01_05 = 114,
    ROUTINE_EVENT_M01_06 = 115,
    ROUTINE_EVENT_M01_07_08 = 116,
    ROUTINE_EVENT_M02_01_02 = 117,
    ROUTINE_EVENT_M02_03_04 = 118,
    ROUTINE_EVENT_M02_05_07A = 119,
    ROUTINE_EVENT_M02_07B = 120,
    ROUTINE_EVENT_M02_09_10 = 121,
    ROUTINE_EVENT_M03_01B = 122,
    ROUTINE_EVENT_M03_02 = 123,
    ROUTINE_EVENT_M03_03 = 124,
    ROUTINE_EVENT_M03_04 = 125,
    ROUTINE_EVENT_M03_05_06 = 126,
    ROUTINE_EVENT_M03_07A = 127,
    ROUTINE_EVENT_M03_07B = 128,
    ROUTINE_EVENT_M03_08 = 129,
    ROUTINE_EVENT_M03_10_13 = 130,
    ROUTINE_EVENT_M04_01 = 131,
    ROUTINE_EVENT_M04_02_03 = 132,
    ROUTINE_EVENT_M05_01_02 = 133,
    ROUTINE_EVENT_M05_03 = 134,
    ROUTINE_EVENT_M05_04 = 135,
    ROUTINE_EVENT_M05_05 = 136,
    ROUTINE_EVENT_M05_06_09 = 137,
    ROUTINE_EVENT_M06_01 = 138,
    ROUTINE_EVENT_M06_02 = 139,
    ROUTINE_EVENT_M06_03 = 140,
    ROUTINE_EVENT_M06_04 = 141,
    ROUTINE_EVENT_M06_05 = 142,
    ROUTINE_EVENT_M07_01_02 = 143,
    ROUTINE_EVENT_M07_03 = 144,
    ROUTINE_EVENT_M07_04 = 145,
    ROUTINE_EVENT_M07_05 = 146,
    ROUTINE_EVENT_M07_06 = 147,
    ROUTINE_EVENT_M07_07_12 = 148,
    ROUTINE_EVENT_M07_13 = 149,
    ROUTINE_EVENT_M07_14 = 150,
    ROUTINE_EVENT_M08_01_02 = 151,
    ROUTINE_EVENT_M08_03 = 152,
    ROUTINE_EVENT_M08_04 = 153,
    ROUTINE_EVENT_M08_05_06 = 154,
    ROUTINE_EVENT_M08_07 = 155,
    ROUTINE_EVENT_M08_08 = 156,
    ROUTINE_EVENT_M08_09 = 157,
    ROUTINE_EVENT_M08_10 = 158,
    ROUTINE_EVENT_M08_11 = 159,
    ROUTINE_EVENT_M08_12 = 160,
    ROUTINE_EVENT_M08_13 = 161,
    ROUTINE_EVENT_M09_01 = 162,
    ROUTINE_EVENT_M09_02 = 163,
    ROUTINE_EVENT_M09_03 = 164,
    ROUTINE_EVENT_M09_04 = 165,
    ROUTINE_EVENT_M09_05 = 166,
    ROUTINE_EVENT_M09_06_07 = 167,
    ROUTINE_EVENT_M10_01 = 168,
    ROUTINE_EVENT_M10_02 = 169,
    ROUTINE_EVENT_M10_03 = 170,
    ROUTINE_EVENT_M10_04 = 171,
    ROUTINE_EVENT_M10_05 = 172,
    ROUTINE_EVENT_M10_06 = 173,
    ROUTINE_EVENT_M10_07 = 174,
    ROUTINE_EVENT_M10_08 = 175,
    ROUTINE_EVENT_M10_09 = 176,
    ROUTINE_EVENT_M10_10 = 177,
    ROUTINE_EVENT_M10_11_12 = 178,
    ROUTINE_EVENT_M11_01 = 179,
    ROUTINE_EVENT_M11_02 = 180,
    ROUTINE_EVENT_M11_03 = 181,
    ROUTINE_EVENT_M11_04 = 182,
    ROUTINE_EVENT_M11_05 = 183,
    ROUTINE_EVENT_M11_06_07 = 184,
    ROUTINE_EVENT_M12_01 = 185,
    ROUTINE_EVENT_M12_02 = 186,
    ROUTINE_EVENT_M12_03A = 187,
    ROUTINE_EVENT_M12_03B = 188,
    ROUTINE_EVENT_M12_04 = 189,
    ROUTINE_EVENT_M12_05 = 190,
    ROUTINE_EVENT_M12_06 = 191,
    ROUTINE_EVENT_M12_07 = 192,
    ROUTINE_EVENT_M12_08 = 193,
    ROUTINE_EVENT_M12_09 = 194,
    ROUTINE_EVENT_M12_10_12 = 195,
    ROUTINE_EVENT_M13_01 = 196,
    ROUTINE_EVENT_M13_02 = 197,
    ROUTINE_EVENT_M13_03 = 198,
    ROUTINE_EVENT_M13_04 = 199,
    ROUTINE_EVENT_M13_05A = 200,
    ROUTINE_EVENT_M13_05B = 201,
    ROUTINE_EVENT_M13_06 = 202,
    ROUTINE_EVENT_M13_07 = 203,
    ROUTINE_EVENT_M13_08 = 204,
    ROUTINE_EVENT_M13_09 = 205,
    ROUTINE_EVENT_M14_01 = 206,
    ROUTINE_EVENT_M14_02A = 207,
    ROUTINE_EVENT_M14_02B = 208,
    ROUTINE_EVENT_M14_03 = 209,
    ROUTINE_EVENT_M14_04 = 210,
    ROUTINE_EVENT_M14_05 = 211,
    ROUTINE_EVENT_M14_06 = 212,
    ROUTINE_EVENT_M14_07 = 213,
    ROUTINE_EVENT_M14_08_09 = 214,
    ROUTINE_EVENT_M15_01 = 215,
    ROUTINE_EVENT_M15_02 = 216,
    ROUTINE_EVENT_M15_03 = 217,
    ROUTINE_EVENT_M15_04 = 218,
    ROUTINE_EVENT_M15_05 = 219,
    ROUTINE_EVENT_M15_06 = 220,
    ROUTINE_EVENT_M15_07 = 221,
    ROUTINE_EVENT_M15_08 = 222,
    ROUTINE_EVENT_M15_09 = 223,
    ROUTINE_EVENT_M15_10_12 = 224,
    ROUTINE_EVENT_M16_01 = 225,
    ROUTINE_EVENT_M16_02 = 226,
    ROUTINE_EVENT_M16_03 = 227,
    ROUTINE_EVENT_M16_04 = 228,
    ROUTINE_EVENT_M16_05 = 229,
    ROUTINE_EVENT_M16_06_07 = 230,
    ROUTINE_EVENT_M17_01 = 231,
    ROUTINE_EVENT_M17_02 = 232,
    ROUTINE_EVENT_M17_03_05 = 233,
    ROUTINE_EVENT_M17_06 = 234,
    ROUTINE_EVENT_M17_07 = 235,
    ROUTINE_EVENT_M18_01 = 236,
    ROUTINE_EVENT_M18_02 = 237,
    ROUTINE_EVENT_M18_03 = 238,
    ROUTINE_EVENT_M18_04_05 = 239,
    ROUTINE_EVENT_M18_06 = 240,
    ROUTINE_EVENT_M18_07 = 241,
    ROUTINE_EVENT_M18_08 = 242,
    ROUTINE_EVENT_M18_09 = 243,
    ROUTINE_EVENT_M18_10 = 244,
    ROUTINE_EVENT_M18_11 = 245,
    ROUTINE_EVENT_M18_12 = 246,
    ROUTINE_EVENT_M18_13_14 = 247,
    ROUTINE_EVENT_M19_01_02 = 248,
    ROUTINE_EVENT_M19_03 = 249,
    ROUTINE_EVENT_M19_04 = 250,
    ROUTINE_EVENT_M19_05 = 251,
    ROUTINE_EVENT_M19_06_07 = 252,
    ROUTINE_EVENT_M19_08 = 253,
    ROUTINE_EVENT_M19_09 = 254,
    ROUTINE_EVENT_M19_10 = 255,
    ROUTINE_EVENT_M20_01_04 = 256,
    ROUTINE_EVENT_M20_05 = 257,
    ROUTINE_EVENT_M20_06 = 258,
    ROUTINE_EVENT_M20_07_09 = 259,
    ROUTINE_EVENT_M21_01_03 = 260,
    ROUTINE_EVENT_M21_04 = 261,
    ROUTINE_EVENT_M21_05 = 262,
    ROUTINE_EVENT_M21_06 = 263,
    ROUTINE_EVENT_M22_01_02 = 264,
    ROUTINE_EVENT_M22_03 = 265,
    ROUTINE_EVENT_M22_04 = 266,
    ROUTINE_EVENT_M22_05 = 267,
    ROUTINE_EVENT_M22_06 = 268,
    ROUTINE_EVENT_M22_07 = 269,
    ROUTINE_EVENT_M22_08 = 270,
    ROUTINE_EVENT_M22_09_10 = 271,
    ROUTINE_EVENT_M22_11 = 272,
    ROUTINE_EVENT_M23_01_02 = 273,
    ROUTINE_EVENT_M23_03 = 274,
    ROUTINE_EVENT_M23_04A = 275,
    ROUTINE_EVENT_M23_04B = 276,
    ROUTINE_EVENT_M23_05 = 277,
    ROUTINE_EVENT_M23_06 = 278,
    ROUTINE_EVENT_M23_07 = 279,
    ROUTINE_EVENT_M23_08 = 280,
    ROUTINE_EVENT_M23_09_10 = 281,
    ROUTINE_EVENT_M24_01_03 = 282,
    ROUTINE_EVENT_M24_04 = 283,
    ROUTINE_EVENT_M24_05 = 284,
    ROUTINE_EVENT_M24_06 = 285,
    ROUTINE_EVENT_M24_07 = 286,
    ROUTINE_EVENT_M24_08 = 287,
    ROUTINE_EVENT_M24_09 = 288,
    ROUTINE_EVENT_M24_10 = 289,
    ROUTINE_EVENT_M24_11 = 290,
    ROUTINE_EVENT_M25_01_05 = 291,
    ROUTINE_EVENT_M25_06 = 292,
    ROUTINE_EVENT_M25_07 = 293,
    ROUTINE_EVENT_M25_08 = 294,
    ROUTINE_EVENT_M25_09 = 295,
    ROUTINE_EVENT_M25_10 = 296,
    ROUTINE_EVENT_M25_11 = 297,
    ROUTINE_EVENT_M25_12 = 298,
    ROUTINE_EVENT_M26_01_08 = 299,
    ROUTINE_EVENT_S01_01 = 300,
    ROUTINE_EVENT_S01_02 = 301,
    ROUTINE_EVENT_S01_03 = 302,
    ROUTINE_EVENT_S01_04 = 303,
    ROUTINE_EVENT_S01_05 = 304,
    ROUTINE_EVENT_S01_06 = 305,
    ROUTINE_EVENT_S01_07 = 306,
    ROUTINE_EVENT_S01_08 = 307,
    ROUTINE_EVENT_S01_09_10 = 308,
    ROUTINE_EVENT_S01_12 = 309,
    ROUTINE_EVENT_S02_01 = 310,
    ROUTINE_EVENT_S02_02 = 311,
    ROUTINE_EVENT_S02_03 = 312,
    ROUTINE_EVENT_S02_04 = 313,
    ROUTINE_EVENT_S02_05 = 314,
    ROUTINE_EVENT_S02_06 = 315,
    ROUTINE_EVENT_S02_07 = 316,
    ROUTINE_EVENT_S02_08 = 317,
    ROUTINE_EVENT_S02_09 = 318,
    ROUTINE_EVENT_S02_10 = 319,
    ROUTINE_EVENT_S03_01 = 320,
    ROUTINE_EVENT_S03_02 = 321,
    ROUTINE_EVENT_S03_03 = 322,
    ROUTINE_EVENT_S03_04 = 323,
    ROUTINE_EVENT_S03_05 = 324,
    ROUTINE_EVENT_S03_06 = 325,
    ROUTINE_EVENT_S03_07 = 326,
    ROUTINE_EVENT_S03_08 = 327,
    ROUTINE_EVENT_S03_09 = 328,
    ROUTINE_EVENT_S03_10 = 329,
    ROUTINE_EVENT_S03_11 = 330,
    ROUTINE_EVENT_S03_12 = 331,
    ROUTINE_EVENT_S03_13 = 332,
    ROUTINE_EVENT_S03_14 = 333,
    ROUTINE_EVENT_S03_15 = 334,
    ROUTINE_EVENT_S03_16 = 335,
    ROUTINE_EVENT_S03_17_20 = 336,
    ROUTINE_EVENT_S03_18 = 337,
    ROUTINE_EVENT_S03_19 = 338,
    ROUTINE_EVENT_S04_01 = 339,
    ROUTINE_EVENT_S04_02 = 340,
    ROUTINE_EVENT_S04_03 = 341,
    ROUTINE_EVENT_S04_04 = 342,
    ROUTINE_EVENT_S04_05 = 343,
    ROUTINE_EVENT_S04_06_07 = 344,
    ROUTINE_EVENT_S04_08 = 345,
    ROUTINE_EVENT_S04_09 = 346,
    ROUTINE_EVENT_S04_10 = 347,
    ROUTINE_EVENT_S04_11 = 348,
    ROUTINE_EVENT_S04_12 = 349,
    ROUTINE_EVENT_S04_13 = 350,
    ROUTINE_EVENT_S04_14 = 351,
    ROUTINE_EVENT_S04_15 = 352,
    ROUTINE_EVENT_S04_16 = 353,
    ROUTINE_EVENT_S04_17 = 354,
    ROUTINE_EVENT_S04_18 = 355,
    ROUTINE_EVENT_S04_19 = 356,
    ROUTINE_EVENT_S04_20 = 357,
    ROUTINE_EVENT_S04_21 = 358,
    ROUTINE_EVENT_S04_22 = 359,
    ROUTINE_EVENT_S04_23 = 360,
    ROUTINE_EVENT_S04_24 = 361,
    ROUTINE_EVENT_S11_01 = 362,
    ROUTINE_EVENT_S12_01 = 363,
    ROUTINE_EVENT_S12_02 = 364,
    ROUTINE_EVENT_S12_03 = 365,
    ROUTINE_EVENT_S12_04 = 366,
    ROUTINE_EVENT_S12_05 = 367,
    ROUTINE_EVENT_S12_06 = 368,
    ROUTINE_EVENT_S12_07 = 369,
    ROUTINE_EVENT_S12_08 = 370,
    ROUTINE_EVENT_S12_09 = 371,
    ROUTINE_EVENT_S12_10 = 372,
    ROUTINE_EVENT_S12_11 = 373,
    ROUTINE_EVENT_S12_12 = 374,
    ROUTINE_EVENT_S12_13 = 375,
    ROUTINE_EVENT_S12_14 = 376,
    ROUTINE_EVENT_S12_15 = 377,
    ROUTINE_EVENT_S12_16 = 378,
    ROUTINE_EVENT_S12_17 = 379,
    ROUTINE_EVENT_S12_18 = 380,
    ROUTINE_EVENT_S12_19 = 381,
    ROUTINE_EVENT_S12_20 = 382,
    ROUTINE_EVENT_S12_21 = 383,
    ROUTINE_EVENT_S20_01 = 384,
    ROUTINE_EVENT_S20_02 = 385,
    ROUTINE_EVENT_S20_03 = 386,
    ROUTINE_EVENT_S20_04_05 = 387,
    ROUTINE_EVENT_S20_06 = 388,
    ROUTINE_EVENT_S20_07_08 = 389,
    ROUTINE_EVENT_S20_09 = 390,
    ROUTINE_EVENT_S20_10 = 391,
    ROUTINE_EVENT_S20_11 = 392,
    ROUTINE_EVENT_S21_01 = 393,
    ROUTINE_EVENT_S21_02 = 394,
    ROUTINE_EVENT_S21_03 = 395,
    ROUTINE_EVENT_S21_04_06 = 396,
    ROUTINE_EVENT_S21_07 = 397,
    ROUTINE_EVENT_S21_08 = 398,
    ROUTINE_EVENT_S21_09 = 399,
    ROUTINE_EVENT_S21_10 = 400,
    ROUTINE_EVENT_S21_11 = 401,
    ROUTINE_EVENT_S21_12 = 402,
    ROUTINE_EVENT_S21_13 = 403,
    ROUTINE_EVENT_S22_01_04 = 404,
    ROUTINE_EVENT_S22_05 = 405,
    ROUTINE_EVENT_S22_06 = 406,
    ROUTINE_EVENT_S22_07 = 407,
    ROUTINE_EVENT_S22_08 = 408,
    ROUTINE_EVENT_S22_09 = 409,
    ROUTINE_EVENT_S22_10 = 410,
    ROUTINE_EVENT_S22_11 = 411,
    ROUTINE_EVENT_S22_12 = 412,
    ROUTINE_EVENT_S22_13 = 413,
    ROUTINE_EVENT_S22_14_15 = 414,
    ROUTINE_EVENT_S23_01 = 415,
    ROUTINE_EVENT_S30_01 = 416,
    ROUTINE_EVENT_S30_02 = 417,
    ROUTINE_EVENT_S30_03 = 418,
    ROUTINE_EVENT_S30_04 = 419,
    ROUTINE_EVENT_S30_05 = 420,
    ROUTINE_EVENT_S30_06 = 421,
    ROUTINE_EVENT_S30_07 = 422,
    ROUTINE_EVENT_S31_01 = 423,
    ROUTINE_EVENT_S31_02 = 424,
    ROUTINE_EVENT_S31_03 = 425,
    ROUTINE_EVENT_S31_04 = 426,
    ROUTINE_EVENT_S31_05 = 427,
    ROUTINE_EVENT_S31_06 = 428,
    ROUTINE_EVENT_S31_07 = 429,
    ROUTINE_EVENT_S31_08 = 430,
    ROUTINE_EVENT_S31_09 = 431,
    ROUTINE_EVENT_S31_10 = 432,
    ROUTINE_EVENT_S31_11 = 433,
    ROUTINE_EVENT_S31_12 = 434,
    ROUTINE_EVENT_S31_13 = 435,
    ROUTINE_EVENT_S31_14 = 436,
    ROUTINE_EVENT_S31_15 = 437,
    ROUTINE_EVENT_S31_16 = 438,
    ROUTINE_EVENT_S31_17 = 439,
    ROUTINE_EVENT_S31_18 = 440,
    ROUTINE_EVENT_S31_19 = 441,
    ROUTINE_EVENT_S31_20 = 442,
    ROUTINE_EVENT_S31_21 = 443,
    ROUTINE_EVENT_S31_22 = 444,
    ROUTINE_EVENT_S31_23 = 445,
    ROUTINE_EVENT_S31_24 = 446,
    ROUTINE_EVENT_S31_25 = 447,
    ROUTINE_EVENT_S31_26 = 448,
    ROUTINE_EVENT_S31_27 = 449,
    ROUTINE_EVENT_S31_28 = 450,
    ROUTINE_EVENT_S31_29 = 451,
    ROUTINE_EVENT_S31_30 = 452,
    ROUTINE_EVENT_S31_31 = 453,
    ROUTINE_EVENT_S31_32 = 454,
    ROUTINE_EVENT_S31_33 = 455,
    ROUTINE_EVENT_S31_34 = 456,
    ROUTINE_EVENT_S31_35 = 457,
    ROUTINE_EVENT_S31_36 = 458,
    ROUTINE_EVENT_S31_37 = 459,
    ROUTINE_EVENT_S31_38 = 460,
    ROUTINE_EVENT_S31_39 = 461,
    ROUTINE_EVENT_S31_40 = 462,
    ROUTINE_EVENT_S32_01 = 463,
    ROUTINE_EVENT_S32_02 = 464,
    ROUTINE_EVENT_S32_03 = 465,
    ROUTINE_EVENT_S32_04 = 466,
    ROUTINE_EVENT_N00_01 = 467,
    ROUTINE_EVENT_N01_01 = 468,
    ROUTINE_EVENT_N01_02 = 469,
    ROUTINE_EVENT_N01_03 = 470,
    ROUTINE_EVENT_N01_04 = 471,
    ROUTINE_EVENT_N01_05_07 = 472,
    ROUTINE_EVENT_N01_08 = 473,
    ROUTINE_EVENT_N01_09 = 474,
    ROUTINE_EVENT_N01_10YARARETA = 475,
    ROUTINE_EVENT_N01_10CHOUREI = 476,
    ROUTINE_EVENT_N01_14 = 477,
    ROUTINE_EVENT_N01_15 = 478,
    ROUTINE_EVENT_N01_16 = 479,
    ROUTINE_EVENT_N01_17 = 480,
    ROUTINE_EVENT_N01_18 = 481,
    ROUTINE_EVENT_N01_19 = 482,
    ROUTINE_EVENT_N01_19_2 = 483,
    ROUTINE_EVENT_N01_20 = 484,
    ROUTINE_EVENT_N01_21 = 485,
    ROUTINE_EVENT_N01_22 = 486,
    ROUTINE_EVENT_N01_23 = 487,
    ROUTINE_EVENT_N01_24 = 488,
    ROUTINE_EVENT_N01_25 = 489,
    ROUTINE_EVENT_N01_27 = 490,
    ROUTINE_EVENT_N01_28 = 491,
    ROUTINE_EVENT_N01_30 = 492,
    ROUTINE_EVENT_N01_31 = 493,
    ROUTINE_EVENT_N02_01 = 494,
    ROUTINE_EVENT_N02_02 = 495,
    ROUTINE_EVENT_N02_03 = 496,
    ROUTINE_EVENT_N02_04 = 497,
    ROUTINE_EVENT_N02_05 = 498,
    ROUTINE_EVENT_N02_06_07 = 499,
    ROUTINE_EVENT_N02_08 = 500,
    ROUTINE_EVENT_N02_09_10 = 501,
    ROUTINE_EVENT_N03_01 = 502,
    ROUTINE_EVENT_N03_02 = 503,
    ROUTINE_EVENT_N03_03 = 504,
    ROUTINE_EVENT_N03_04 = 505,
    ROUTINE_EVENT_N03_05 = 506,
    ROUTINE_EVENT_N03_06_07 = 507,
    ROUTINE_EVENT_N03_08 = 508,
    ROUTINE_EVENT_N03_09 = 509,
    ROUTINE_EVENT_N03_10 = 510,
    ROUTINE_EVENT_N03_11 = 511,
    ROUTINE_EVENT_N03_12 = 512,
    ROUTINE_EVENT_N03_13 = 513,
    ROUTINE_EVENT_N03_14 = 514,
    ROUTINE_EVENT_N03_15_16 = 515,
    ROUTINE_EVENT_N03_17 = 516,
    ROUTINE_EVENT_N03_18 = 517,
    ROUTINE_EVENT_N03_19 = 518,
    ROUTINE_EVENT_N04_01 = 519,
    ROUTINE_EVENT_N04_02 = 520,
    ROUTINE_EVENT_N04_03 = 521,
    ROUTINE_EVENT_N04_04 = 522,
    ROUTINE_EVENT_N04_05 = 523,
    ROUTINE_EVENT_N04_06 = 524,
    ROUTINE_EVENT_N04_07 = 525,
    ROUTINE_EVENT_N04_08 = 526,
    ROUTINE_EVENT_N04_09 = 527,
    ROUTINE_EVENT_N04_10 = 528,
    ROUTINE_EVENT_N04_11 = 529,
    ROUTINE_EVENT_N04_12 = 530,
    ROUTINE_EVENT_N04_13 = 531,
    ROUTINE_EVENT_N04_14 = 532,
    ROUTINE_EVENT_N04_15 = 533,
    ROUTINE_EVENT_N04_16 = 534,
    ROUTINE_EVENT_N04_17 = 535,
    ROUTINE_EVENT_N04_18 = 536,
    ROUTINE_EVENT_N04_19 = 537,
    ROUTINE_EVENT_N04_20 = 538,
    ROUTINE_EVENT_N04_21 = 539,
    ROUTINE_EVENT_N04_22 = 540,
    ROUTINE_EVENT_N04_23 = 541,
    ROUTINE_EVENT_N04_24 = 542,
    ROUTINE_EVENT_N04_25 = 543,
    ROUTINE_EVENT_N04_26 = 544,
    ROUTINE_EVENT_N06_01_02 = 545,
    ROUTINE_EVENT_N06_03 = 546,
    ROUTINE_EVENT_N06_04 = 547,
    ROUTINE_EVENT_N06_05_07 = 548,
    ROUTINE_EVENT_N06_08 = 549,
    ROUTINE_EVENT_N06_09 = 550,
    ROUTINE_EVENT_N06_10 = 551,
    ROUTINE_EVENT_N06_11 = 552,
    ROUTINE_EVENT_N06_12 = 553,
    ROUTINE_EVENT_N06_13 = 554,
    ROUTINE_EVENT_N06_14 = 555,
    ROUTINE_EVENT_N06_15 = 556,
    ROUTINE_EVENT_N06_16 = 557,
    ROUTINE_EVENT_N06_17 = 558,
    ROUTINE_EVENT_N06_18_19 = 559,
    ROUTINE_EVENT_N06_20 = 560,
    ROUTINE_EVENT_N06_21 = 561,
    ROUTINE_EVENT_N06_22 = 562,
    ROUTINE_EVENT_N06_23 = 563,
    ROUTINE_EVENT_N06_24 = 564,
    ROUTINE_EVENT_N06_25 = 565,
    ROUTINE_EVENT_N06_26 = 566,
    ROUTINE_EVENT_N06_27 = 567,
    ROUTINE_EVENT_N06_28 = 568,
    ROUTINE_EVENT_N06_29 = 569,
    ROUTINE_EVENT_N06_30 = 570,
    ROUTINE_EVENT_N06_31 = 571,
    ROUTINE_EVENT_N06_32 = 572,
    ROUTINE_EVENT_N06_33 = 573,
    ROUTINE_EVENT_N06_34 = 574,
    ROUTINE_EVENT_N06_35 = 575,
    ROUTINE_EVENT_N06_36 = 576,
    ROUTINE_EVENT_N06_37 = 577,
    ROUTINE_EVENT_N06_38 = 578,
    ROUTINE_EVENT_N06_39 = 579,
    ROUTINE_EVENT_N08_01_02 = 580,
    ROUTINE_EVENT_N08_03 = 581,
    ROUTINE_EVENT_N08_04 = 582,
    ROUTINE_EVENT_N08_05 = 583,
    ROUTINE_EVENT_N08_06 = 584,
    ROUTINE_EVENT_N08_07 = 585,
    ROUTINE_EVENT_N08_08 = 586,
    ROUTINE_EVENT_N08_09 = 587,
    ROUTINE_EVENT_N08_10 = 588,
    ROUTINE_EVENT_N08_11 = 589,
    ROUTINE_EVENT_N08_12 = 590,
    ROUTINE_EVENT_N08_13 = 591,
    ROUTINE_EVENT_N08_14 = 592,
    ROUTINE_EVENT_N08_15 = 593,
    ROUTINE_EVENT_N08_16 = 594,
    ROUTINE_EVENT_N08_17 = 595,
    ROUTINE_EVENT_N08_18 = 596,
    ROUTINE_EVENT_N08_19 = 597,
    ROUTINE_EVENT_N08_20 = 598,
    ROUTINE_EVENT_N08_21 = 599,
    ROUTINE_EVENT_N08_22 = 600,
    ROUTINE_EVENT_N08_23 = 601,
    ROUTINE_EVENT_N08_24 = 602,
    ROUTINE_EVENT_N08_25 = 603,
    ROUTINE_EVENT_N08_26 = 604,
    ROUTINE_EVENT_N08_27 = 605,
    ROUTINE_EVENT_N08_28 = 606,
    ROUTINE_EVENT_N08_29 = 607,
    ROUTINE_EVENT_N08_30 = 608,
    ROUTINE_EVENT_N09_01 = 609,
    ROUTINE_EVENT_N09_02 = 610,
    ROUTINE_EVENT_N09_03 = 611,
    ROUTINE_EVENT_N09_04 = 612,
    ROUTINE_EVENT_N09_05 = 613,
    ROUTINE_EVENT_N09_06 = 614,
    ROUTINE_EVENT_N09_07 = 615,
    ROUTINE_EVENT_N09_08 = 616,
    ROUTINE_EVENT_N09_09 = 617,
    ROUTINE_EVENT_N09_10 = 618,
    ROUTINE_EVENT_N09_11 = 619,
    ROUTINE_EVENT_N09_12 = 620,
    ROUTINE_EVENT_N09_13 = 621,
    ROUTINE_EVENT_N09_14 = 622,
    ROUTINE_EVENT_N09_15 = 623,
    ROUTINE_EVENT_N09_16 = 624,
    ROUTINE_EVENT_N09_17 = 625,
    ROUTINE_EVENT_N09_18 = 626,
    ROUTINE_EVENT_N09_19 = 627,
    ROUTINE_EVENT_N09_20 = 628,
    ROUTINE_EVENT_N09_21 = 629,
    ROUTINE_EVENT_N09_22 = 630,
    ROUTINE_EVENT_TG01 = 631,
    ROUTINE_EVENT_TG02 = 632,
    ROUTINE_EVENT_TG03 = 633,
    ROUTINE_EVENT_TG04 = 634,
    ROUTINE_EVENT_TG05 = 635,
    ROUTINE_EVENT_TG06 = 636,
    ROUTINE_EVENT_TG07 = 637,
    ROUTINE_EVENT_TG08 = 638,
    ROUTINE_EVENT_TG09 = 639,
    ROUTINE_EVENT_TG10 = 640,
    ROUTINE_EVENT_TG11 = 641,
    ROUTINE_EVENT_TG12 = 642,
    ROUTINE_EVENT_TG13 = 643,
    ROUTINE_EVENT_TG14 = 644,
    ROUTINE_EVENT_TG15 = 645,
    ROUTINE_EVENT_TG16 = 646,
    ROUTINE_EVENT_TG17 = 647,
    ROUTINE_EVENT_TG18 = 648,
    ROUTINE_EVENT_TG19 = 649,
    ROUTINE_EVENT_TG20 = 650,
    ROUTINE_EVENT_TG21 = 651,
    ROUTINE_EVENT_TG22 = 652,
    ROUTINE_EVENT_TG23 = 653,
    ROUTINE_EVENT_TG24 = 654,
    ROUTINE_EVENT_MEND_01 = 655,
    ROUTINE_EVENT_MEND_02 = 656,
    ROUTINE_COMMON_ENTER = 657,
    ROUTINE_DEBUG_ENTER = 658,
    ROUTINE_GETOUT_NORMAL = 659,
    ROUTINE_GOTO_SCENARIO_DUNGEON = 660,
    ROUTINE_GOTO_REQUEST_DUNGEON = 661,
    ROUTINE_GOTO_TRAINING_DUNGEON = 662,
    ROUTINE_GOTO_RESCUE_DUNGEON = 663,
    ROUTINE_GOTO_FREE_DUNGEON = 664,
    ROUTINE_GOTO_DEBUG_DUNGEON = 665,
    ROUTINE_ENTER_SCENARIO_DUNGEON = 666,
    ROUTINE_ENTER_REQUEST_DUNGEON = 667,
    ROUTINE_ENTER_TRAINING_DUNGEON = 668,
    ROUTINE_ENTER_RESCUE_DUNGEON = 669,
    ROUTINE_ENTER_FREE_DUNGEON = 670,
    ROUTINE_ENTER_DEBUG_DUNGEON = 671,
    ROUTINE_GETOUT_SCENARIO_DUNGEON = 672,
    ROUTINE_GETOUT_REQUEST_DUNGEON = 673,
    ROUTINE_GETOUT_REQUEST_RETURN = 674,
    ROUTINE_GETOUT_REQUEST_CONQUEST = 675,
    ROUTINE_GETOUT_TRAINING_DUNGEON = 676,
    ROUTINE_GETOUT_DEBUG_DUNGEON = 677,
    ROUTINE_MOVE_WORLD_MAP = 678,
    ROUTINE_EVENT_CONTROL = 679,
    ROUTINE_EVENT_WAKEUP = 680,
    ROUTINE_EVENT_STATION = 681,
    ROUTINE_STATION_CONTROL = 682,
    ROUTINE_ENTER_CONTROL = 683,
    ROUTINE_SETUP_DEBUG_CAMERA = 684,
    ROUTINE_MOVE_DEBUG_CAMERA = 685,
    ROUTINE_MESSAGE_CLOSE_WAIT_FUNC = 686,
    ROUTINE_EVENT_END_MAPIN = 687,
    ROUTINE_EVENT_END_FREE = 688,
    ROUTINE_EVENT_END_MAPIN_AE = 689,
    ROUTINE_EVENT_END_FREE_AE = 690,
    ROUTINE_BOSS_WIPE_FUNC = 691,
    ROUTINE_FADE_OUT_ALL_BEFORE = 692,
    ROUTINE_FADE_OUT_ALL_AFTER = 693,
    ROUTINE_SUBSCREEN_INIT = 694,
    ROUTINE_RESCUE_SET = 695,
    ROUTINE_RESCUE_DEBUG = 696,
    ROUTINE_SORANOITADAKI_FLAGSET = 697,
    ROUTINE_TITLE_TEST = 698,
    ROUTINE_BUBBLE_TEST = 699,
    ROUTINE_MAP_TEST = 700,
};

// This is usually stored as a 16-bit integer
#pragma pack(push, 2)
ENUM_16_BIT(common_routine_id);
#pragma pack(pop)

// Script entity IDs. While a few script entities differ between versions, the IDs are the same.
enum script_entity_id {
    ENTITY_PLAYER = 0,
    ENTITY_PLAYER_HERO = 1,
    ENTITY_PLAYER_PARTNER = 2,
    ENTITY_PLAYER_PARTNER2 = 3,
    ENTITY_PLAYER_APPOINT = 4,
    ENTITY_PLAYER_BIPPA = 5,    // Bidoof
    ENTITY_PLAYER_PUPURIN = 6,  // Igglybuff
    ENTITY_PLAYER_KIMAWARI = 7, // Sunflora
    ENTITY_PLAYER_FUTURE = 8,   // Grovyle
    ENTITY_PLAYER_CHARMS = 9,   // Lopunny
    ENTITY_ATTENDANT1 = 10,
    ENTITY_ATTENDANT2 = 11,
    ENTITY_ATTENDANT_HERO = 12,
    ENTITY_ATTENDANT_PARTNER = 13,
    ENTITY_ATTENDANT_PARTNER2 = 14,
    ENTITY_ATTENDANT_APPOINT = 15,
    ENTITY_ATTENDANT_BIPPA = 16,    // Bidoof
    ENTITY_ATTENDANT_PUPURIN = 17,  // Igglybuff
    ENTITY_ATTENDANT_KIMAWARI = 18, // Sunflora
    ENTITY_ATTENDANT_FUTURE = 19,   // Grovyle
    ENTITY_ATTENDANT_CHARMS = 20,   // Lopunny
    ENTITY_ATTENDANT_MANAFI = 21,   // Manaphy
    ENTITY_UNIT_NPC1 = 22,
    ENTITY_UNIT_NPC2 = 23,
    ENTITY_UNIT_NPC3 = 24,
    ENTITY_UNIT_NPC4 = 25,
    ENTITY_ADVENTURE_NPC1 = 26,
    ENTITY_ADVENTURE_NPC2 = 27,
    ENTITY_ADVENTURE_NPC3 = 28,
    ENTITY_ADVENTURE_NPC4 = 29,
    ENTITY_FRIEND_NPC1 = 30,
    ENTITY_FRIEND_NPC2 = 31,
    ENTITY_FRIEND_NPC3 = 32,
    ENTITY_FRIEND_NPC4 = 33,
    ENTITY_FRIEND_NPC5 = 34,
    ENTITY_FRIEND_NPC6 = 35,
    ENTITY_FRIEND_NPC7 = 36,
    ENTITY_FRIEND_NPC8 = 37,
    ENTITY_FRIEND_NPC9 = 38,
    ENTITY_FRIEND_NPC10 = 39,
    ENTITY_FRIEND_NPC11 = 40,
    ENTITY_FRIEND_NPC12 = 41,
    ENTITY_FRIEND_NPC13 = 42,
    ENTITY_FRIEND_NPC14 = 43,
    ENTITY_FRIEND_NPC15 = 44,
    ENTITY_FRIEND_NPC16 = 45,
    ENTITY_TALK_MAIN = 46,
    ENTITY_TALK_SUB = 47,
    ENTITY_EVENT_NPC_MAIN = 48,
    ENTITY_EVENT_NPC_SUB = 49,
    ENTITY_RANDOM_REQUEST_NPC01 = 50,
    ENTITY_RANDOM_REQUEST_NPC02 = 51,
    ENTITY_RANDOM_REQUEST_NPC03 = 52,
    ENTITY_EVENT_NPC01 = 53,
    ENTITY_EVENT_NPC02 = 54,
    ENTITY_EVENT_NPC03 = 55,
    ENTITY_EVENT_NPC04 = 56,
    ENTITY_NPC_NEW_FRIEND = 57,
    ENTITY_NPC_DEMO_HERO = 58,
    ENTITY_NPC_DEMO_PARTNER = 59,
    ENTITY_NPC_HERO = 60,
    ENTITY_NPC_PARTNER = 61,
    ENTITY_NPC_PARTNER2 = 62,
    ENTITY_NPC_APPOINT = 63,
    ENTITY_NPC_HERO_REAL = 64,
    ENTITY_NPC_PARTNER_REAL = 65,
    ENTITY_NPC_HERO_FIRST = 66,
    ENTITY_NPC_PARTNER_FIRST = 67,
    ENTITY_NPC_DEBUG = 68,
    ENTITY_NPC_TEST001 = 69,
    ENTITY_NPC_TEST002 = 70,
    ENTITY_NPC_TEST003 = 71,
    ENTITY_NPC_TEST004 = 72,
    ENTITY_NPC_TEST005 = 73,
    ENTITY_NPC_TEST006 = 74,
    ENTITY_NPC_TEST007 = 75,
    ENTITY_NPC_TEST008 = 76,
    ENTITY_NPC_TEST009 = 77,
    ENTITY_NPC_TEST010 = 78,
    ENTITY_NPC_PUKURIN = 79,      // Wigglytuff
    ENTITY_NPC_ZUBATTO = 80,      // Zubat
    ENTITY_NPC_DIGUDA = 81,       // Diglett
    ENTITY_NPC_DAGUTORIO = 82,    // Dugtrio
    ENTITY_NPC_MANKII = 83,       // Mankey
    ENTITY_NPC_OKORIZARU = 84,    // Primeape
    ENTITY_NPC_KOIRU = 85,        // Magnemite
    ENTITY_NPC_SURIIPU = 86,      // Drowzee
    ENTITY_NPC_KURABU = 87,       // Krabby
    ENTITY_NPC_GARAGARA = 88,     // Marowak
    ENTITY_NPC_DOGAASU = 89,      // Koffing
    ENTITY_NPC_RAKKII = 90,       // Chansey
    ENTITY_NPC_GARUURA = 91,      // Kangaskhan
    ENTITY_NPC_GYARADOSU = 92,    // Gyarados
    ENTITY_NPC_RAPURASU = 93,     // Lapras
    ENTITY_NPC_OMUSUTAA = 94,     // Omastar
    ENTITY_NPC_OMUSUTAA2 = 95,    // Omastar 2
    ENTITY_NPC_KABUTOPUSU = 96,   // Kabutops
    ENTITY_NPC_NEITYIO = 97,      // Xatu
    ENTITY_NPC_DENRYUU = 98,      // Ampharos
    ENTITY_NPC_MARIRU = 99,       // Marill
    ENTITY_NPC_USOKKII = 100,     // Sudowoodo
    ENTITY_NPC_KIMAWARI = 101,    // Sunflora
    ENTITY_NPC_HIMEGUMA = 102,    // Teddiursa
    ENTITY_NPC_RINGUMA = 103,     // Ursaring
    ENTITY_NPC_SEREBII = 104,     // Celebi
    ENTITY_NPC_JUPUTORU = 105,    // Grovyle
    ENTITY_NPC_PERIPPAA1 = 106,   // Pelipper 1
    ENTITY_NPC_PERIPPAA2 = 107,   // Pelipper 2
    ENTITY_NPC_YARUKIMONO = 108,  // Vigoroth
    ENTITY_NPC_DOGOOMU = 109,     // Loudred
    ENTITY_NPC_RURIRI = 110,      // Azurill
    ENTITY_NPC_YAMIRAMI = 111,    // Sableye
    ENTITY_NPC_CHAAREMU = 112,    // Medicham
    ENTITY_NPC_KOOTASU = 113,     // Torkoal
    ENTITY_NPC_BANEBUU = 114,     // Spoink
    ENTITY_NPC_HEIGANI = 115,     // Corphish
    ENTITY_NPC_KAKUREON1 = 116,   // Kecleon 1
    ENTITY_NPC_KAKUREON2 = 117,   // Kecleon 2
    ENTITY_NPC_YOMAWARU = 118,    // Duskull
    ENTITY_NPC_CHIRIIN = 119,     // Chimecho
    ENTITY_NPC_REJIROKKU = 120,   // Regirock
    ENTITY_NPC_REJIAISU = 121,    // Regice
    ENTITY_NPC_REJISUCHIRU = 122, // Registeel
    ENTITY_NPC_GURAADON = 123,    // Groudon
    ENTITY_NPC_BIPPA = 124,       // Bidoof
    ENTITY_NPC_RUKUSHIO1 = 125,   // Luxio 1
    ENTITY_NPC_RUKUSHIO2 = 126,   // Luxio 2
    ENTITY_NPC_RENTORAA = 127,    // Luxray
    ENTITY_NPC_MIMIROPPU = 128,   // Lopunny
    ENTITY_NPC_SUKATANKU = 129,   // Skuntank
    ENTITY_NPC_MANENE = 130,      // Mime Jr.
    ENTITY_NPC_PERAPPU = 131,     // Chatot
    ENTITY_NPC_MIKARUGE = 132,    // Spiritomb
    ENTITY_NPC_GUREGGURU = 133,   // Croagunk
    ENTITY_NPC_JIBAKOIRU = 134,   // Magnezone
    ENTITY_NPC_EREKIBURU = 135,   // Electivire
    ENTITY_NPC_YONOWAARU = 136,   // Dusknoir
    ENTITY_NPC_YUKUSHII = 137,    // Uxie
    ENTITY_NPC_EMURITTO = 138,    // Mesprit
    ENTITY_NPC_AGUNOMU = 139,     // Azelf
    ENTITY_NPC_DIARUGA = 140,     // Dialga
    ENTITY_NPC_PARUKIA = 141,     // Palkia
    ENTITY_NPC_REJIGIGASU = 142,  // Regigigas
    ENTITY_NPC_GIRATYINA = 143,   // Giratina
    ENTITY_NPC_KURESERIA = 144,   // Cresselia
    ENTITY_NPC_FIONE1 = 145,      // Phione 1
    ENTITY_NPC_FIONE2 = 146,      // Phione 2
    ENTITY_NPC_FIONE3 = 147,      // Phione 3
    ENTITY_NPC_FIONE4 = 148,      // Phione 4
    ENTITY_NPC_FIONE5 = 149,      // Phione 5
    ENTITY_NPC_FIONE6 = 150,      // Phione 6
    ENTITY_NPC_FIONE7 = 151,      // Phione 7
    ENTITY_NPC_FIONE8 = 152,      // Phione 8
    ENTITY_NPC_MANAFI = 153,      // Manaphy
    ENTITY_NPC_DAAKURAI = 154,    // Darkrai
    ENTITY_OBJECT001 = 155,
    ENTITY_OBJECT002 = 156,
    ENTITY_NPC_KOIRU2 = 157,            // Magnemite 2
    ENTITY_NPC_POPPO = 158,             // Pidgey
    ENTITY_NPC_TANEBOO = 159,           // Seedot
    ENTITY_NPC_OOSUBAME = 160,          // Swellow
    ENTITY_NPC_KEMUSSO = 161,           // Wurmple
    ENTITY_NPC_BURAKKII = 162,          // Umbreon
    ENTITY_NPC_KAGEBOUZU = 163,         // Shuppet
    ENTITY_NPC_YAMIKARASU = 164,        // Murkrow
    ENTITY_NPC_POCHIENA_A = 165,        // Poochyena A
    ENTITY_NPC_POCHIENA_B = 166,        // Poochyena B
    ENTITY_NPC_POCHIENA_C = 167,        // Poochyena C
    ENTITY_NPC_ZANGUUSU = 168,          // Zangoose
    ENTITY_NPC_SUTORAIKU = 169,         // Scyther
    ENTITY_NPC_SANDOPAN = 170,          // Sandslash
    ENTITY_NPC_PONIITA = 171,           // Ponyta
    ENTITY_NPC_BAKUUDA = 172,           // Camerupt
    ENTITY_NPC_MADATSUBOMI = 173,       // Bellsprout
    ENTITY_NPC_TOGEPII = 174,           // Togepi
    ENTITY_NPC_NYOROTONO = 175,         // Politoed
    ENTITY_NPC_REDIBA = 176,            // Ledyba
    ENTITY_NPC_YADON = 177,             // Slowpoke
    ENTITY_NPC_NAMAKERO = 178,          // Slakoth
    ENTITY_NPC_ERUREIDO = 179,          // Gallade
    ENTITY_NPC_ROZUREIDO = 180,         // Roserade
    ENTITY_NPC_DOSAIDON = 181,          // Rhyperior
    ENTITY_NPC_MANYUURA = 182,          // Weavile
    ENTITY_NPC_AABOKKU = 183,           // Arbok
    ENTITY_NPC_DORAPION = 184,          // Drapion
    ENTITY_NPC_BASHAAMO = 185,          // Blaziken
    ENTITY_NPC_ENPERUTO = 186,          // Empoleon
    ENTITY_NPC_HERAKUROSU = 187,        // Heracross
    ENTITY_NPC_DERUBIRU_A = 188,        // Houndour A
    ENTITY_NPC_DERUBIRU_B = 189,        // Houndour B
    ENTITY_NPC_DERUBIRU_C = 190,        // Houndour C
    ENTITY_NPC_SAANAITO = 191,          // Gardevoir
    ENTITY_NPC_TODOZERUGA = 192,        // Walrein
    ENTITY_NPC_HASSAMU = 193,           // Scizor
    ENTITY_NPC_BARIYAADO = 194,         // Mr. Mime
    ENTITY_NPC_FURIIZAA = 195,          // Articuno
    ENTITY_NPC_YUKIMENOKO = 196,        // Froslass
    ENTITY_NPC_RINGUMA2 = 197,          // Ursaring 2
    ENTITY_NPC_YAMIRAMI2 = 198,         // Sableye 2
    ENTITY_NPC_YAMIRAMI3 = 199,         // Sableye 3
    ENTITY_NPC_YAMIRAMI4 = 200,         // Sableye 4
    ENTITY_NPC_YAMIRAMI5 = 201,         // Sableye 5
    ENTITY_NPC_YAMIRAMI6 = 202,         // Sableye 6
    ENTITY_NPC_RUKUSHIO3 = 203,         // Luxio 3
    ENTITY_NPC_RUKUSHIO4 = 204,         // Luxio 4
    ENTITY_NPC_RUKUSHIO5 = 205,         // Luxio 5
    ENTITY_NPC_RUKUSHIO6 = 206,         // Luxio 6
    ENTITY_NPC_RUKUSHIO7 = 207,         // Luxio 7
    ENTITY_NPC_RUKUSHIO8 = 208,         // Luxio 8
    ENTITY_NPC_HIIDORAN = 209,          // Heatran
    ENTITY_NPC_RUGIA = 210,             // Lugia
    ENTITY_NPC_MYUU = 211,              // Mew
    ENTITY_NPC_HOUOU = 212,             // Ho-Oh
    ENTITY_NPC_S_CHIRIIN = 213,         // Chimecho
    ENTITY_NPC_S_PUKURIN = 214,         // Wigglytuff
    ENTITY_NPC_S_YUKUSHII = 215,        // Uxie
    ENTITY_NPC_S_AGUNOMU = 216,         // Azelf
    ENTITY_NPC_S_EMURITTO = 217,        // Mesprit
    ENTITY_NPC_S_KIMAWARI = 218,        // Sunflora
    ENTITY_NPC_S_DIGUDA = 219,          // Diglett
    ENTITY_NPC_S_DAGUTORIO = 220,       // Dugtrio
    ENTITY_NPC_S_HEIGANI = 221,         // Corphish
    ENTITY_NPC_S_DOGOOMU = 222,         // Loudred
    ENTITY_NPC_S_BIPPA = 223,           // Bidoof
    ENTITY_NPC_S_PERAPPU = 224,         // Chatot
    ENTITY_NPC_S_JUPUTORU = 225,        // Grovyle
    ENTITY_NPC_S_YONOWAARU = 226,       // Dusknoir
    ENTITY_NPC_S_YAMIRAMI1 = 227,       // Sableye 1
    ENTITY_NPC_S_YAMIRAMI2 = 228,       // Sableye 2
    ENTITY_NPC_DARK_PUKURIN = 229,      // Wigglytuff
    ENTITY_OBJ_IMAGE_REJIGIGASU = 230,  // Regigigas
    ENTITY_NPC_DOOTAKUN1 = 231,         // Bronzong 1
    ENTITY_NPC_DOOTAKUN2 = 232,         // Bronzong 2
    ENTITY_NPC_DOOTAKUN3 = 233,         // Bronzong 3
    ENTITY_NPC_DOOTAKUN4 = 234,         // Bronzong 4
    ENTITY_NPC_DOOTAKUN5 = 235,         // Bronzong 5
    ENTITY_NPC_DOOTAKUN6 = 236,         // Bronzong 6
    ENTITY_OBJ_IMAGE_DOOTAKUN = 237,    // Bronzong
    ENTITY_OBJ_IMAGE_DOOTAKUN2 = 238,   // Bronzong 2
    ENTITY_OBJ_IMAGE_DOOTAKUN3 = 239,   // Bronzong 3
    ENTITY_OBJ_IMAGE_DOOTAKUN4 = 240,   // Bronzong 4
    ENTITY_OBJ_IMAGE_DOOTAKUN5 = 241,   // Bronzong 5
    ENTITY_OBJ_IMAGE_DOOTAKUN6 = 242,   // Bronzong 6
    ENTITY_NPC_SAWAMURAA1 = 243,        // Hitmonlee 1
    ENTITY_NPC_SAWAMURAA2 = 244,        // Hitmonlee 2
    ENTITY_NPC_SAWAMURAA3 = 245,        // Hitmonlee 3
    ENTITY_NPC_SAWAMURAA4 = 246,        // Hitmonlee 4
    ENTITY_NPC_SAWAMURAA5 = 247,        // Hitmonlee 5
    ENTITY_NPC_SAWAMURAA6 = 248,        // Hitmonlee 6
    ENTITY_OBJ_IMAGE_SAWAMURAA = 249,   // Hitmonlee
    ENTITY_OBJ_IMAGE_SAWAMURAA2 = 250,  // Hitmonlee 2
    ENTITY_OBJ_IMAGE_SAWAMURAA3 = 251,  // Hitmonlee 3
    ENTITY_OBJ_IMAGE_SAWAMURAA4 = 252,  // Hitmonlee 4
    ENTITY_OBJ_IMAGE_SAWAMURAA5 = 253,  // Hitmonlee 5
    ENTITY_OBJ_IMAGE_SAWAMURAA6 = 254,  // Hitmonlee 6
    ENTITY_NPC_DARK_DIARUGA = 255,      // Primal Dialga
    ENTITY_NPC_SHADOW_DAAKURAI = 256,   // Darkrai
    ENTITY_NPC_KURESERIA2 = 257,        // Cresselia 2
    ENTITY_NPC_BOSUGODORA = 258,        // Aggron
    ENTITY_NPC_MAGUKARUGO = 259,        // Magcargo
    ENTITY_NPC_BUUBAAN = 260,           // Magmortar
    ENTITY_NPC_MUUMAAJI = 261,          // Mismagius
    ENTITY_NPC_BARUBIITO1 = 262,        // Volbeat 1
    ENTITY_NPC_BARUBIITO2 = 263,        // Volbeat 2
    ENTITY_NPC_BARUBIITO3 = 264,        // Volbeat 3
    ENTITY_NPC_BARUBIITO4 = 265,        // Volbeat 4
    ENTITY_NPC_IRUMIIZE1 = 266,         // Illumise 1
    ENTITY_NPC_IRUMIIZE2 = 267,         // Illumise 2
    ENTITY_NPC_IRUMIIZE3 = 268,         // Illumise 3
    ENTITY_NPC_IRUMIIZE4 = 269,         // Illumise 4
    ENTITY_NPC_GABAITO = 270,           // Gabite
    ENTITY_NPC_REKKUUZA = 271,          // Rayquaza
    ENTITY_NPC_KAIOOGA = 272,           // Kyogre
    ENTITY_NPC_JIRAACHI = 273,          // Jirachi
    ENTITY_NPC_BIIDARU = 274,           // Bibarel
    ENTITY_NPC_BIPPA_BRO = 275,         // Bidoof brother
    ENTITY_NPC_BIPPA_SIS = 276,         // Bidoof sister
    ENTITY_NPC_GURAIGAA = 277,          // Gligar
    ENTITY_NPC_TATSUBEI = 278,          // Bagon
    ENTITY_NPC_YUKIKABURI = 279,        // Snover
    ENTITY_NPC_MANMUU = 280,            // Mamoswine
    ENTITY_NPC_ONIGOORI = 281,          // Glalie
    ENTITY_NPC_ONIGOORI2 = 282,         // Glalie 2
    ENTITY_NPC_ONIGOORI3 = 283,         // Glalie 3
    ENTITY_NPC_ONIGOORI4 = 284,         // Glalie 4
    ENTITY_NPC_PORIGON = 285,           // Porygon
    ENTITY_NPC_PORIGON2 = 286,          // Porygon2
    ENTITY_NPC_YUKIWARASHI = 287,       // Snorunt
    ENTITY_NPC_YUKIWARASHI2 = 288,      // Snorunt 2
    ENTITY_NPC_YUKIWARASHI3 = 289,      // Snorunt 3
    ENTITY_NPC_YUKIWARASHI4 = 290,      // Snorunt 4
    ENTITY_NPC_JUPETTA = 291,           // Banette
    ENTITY_NPC_SUKORUPI = 292,          // Skorupi
    ENTITY_NPC_AAMARUDO = 293,          // Armaldo
    ENTITY_NPC_PUKURIN_PAPA = 294,      // Wigglytuff Papa
    ENTITY_NPC_PUKURIN_MAMA = 295,      // Wigglytuff Mama
    ENTITY_NPC_UPAA = 296,              // Wooper
    ENTITY_NPC_MUCHUURU = 297,          // Smoochum
    ENTITY_NPC_SUBOMII = 298,           // Budew
    ENTITY_NPC_NIDOKINGU = 299,         // Nidoking
    ENTITY_NPC_SOONANO = 300,           // Wynaut
    ENTITY_NPC_SOONANSU = 301,          // Wobbuffet
    ENTITY_NPC_PATCHIIRU = 302,         // Spinda
    ENTITY_NPC_OKUTAN = 303,            // Octillery
    ENTITY_NPC_TSUBOTSUBO = 304,        // Shuckle
    ENTITY_NPC_RUNPAPPA = 305,          // Ludicolo
    ENTITY_NPC_RUNPAPPA2 = 306,         // Ludicolo 2
    ENTITY_NPC_RUNPAPPA3 = 307,         // Ludicolo 3
    ENTITY_NPC_RUNPAPPA4 = 308,         // Ludicolo 4
    ENTITY_NPC_RUNPAPPA5 = 309,         // Ludicolo 5
    ENTITY_NPC_NASSHII = 310,           // Exeggutor
    ENTITY_NPC_SHEIMI = 311,            // Shaymin
    ENTITY_NPC_SHEIMI1 = 312,           // Shaymin 1
    ENTITY_NPC_SHEIMI2 = 313,           // Shaymin 2
    ENTITY_NPC_SHEIMI3 = 314,           // Shaymin 3
    ENTITY_NPC_SHEIMI4 = 315,           // Shaymin 4
    ENTITY_NPC_SHEIMI5 = 316,           // Shaymin 5
    ENTITY_NPC_GOORIKII = 317,          // Machoke
    ENTITY_NPC_KINOGASSA = 318,         // Breloom
    ENTITY_NPC_KUCHIITO = 319,          // Mawile
    ENTITY_NPC_NYUURA = 320,            // Sneasel
    ENTITY_NPC_KOIKINGU = 321,          // Magikarp
    ENTITY_NPC_HINBASU = 322,           // Feebas
    ENTITY_NPC_FUWARAIDO = 323,         // Drifblim
    ENTITY_NPC_TAMATAMA = 324,          // Exeggcute
    ENTITY_NPC_JIGUZAGUMA = 325,        // Zigzagoon
    ENTITY_NPC_MASUKIPPA = 326,         // Carnivine
    ENTITY_NPC_MASUKIPPA2 = 327,        // Carnivine 2
    ENTITY_NPC_MASUKIPPA3 = 328,        // Carnivine 3
    ENTITY_NPC_MASUKIPPA4 = 329,        // Carnivine 4
    ENTITY_NPC_MASUKIPPA5 = 330,        // Carnivine 5
    ENTITY_NPC_MASUKIPPA6 = 331,        // Carnivine 6
    ENTITY_NPC_BETOBETAA = 332,         // Grimer
    ENTITY_NPC_BETOBETAA2 = 333,        // Grimer 2
    ENTITY_NPC_BETOBETAA3 = 334,        // Grimer 3
    ENTITY_NPC_BETOBETAA4 = 335,        // Grimer 4
    ENTITY_NPC_BETOBETAA5 = 336,        // Grimer 5
    ENTITY_NPC_BETOBETON = 337,         // Muk
    ENTITY_NPC_BETOBETON2 = 338,        // Muk 2
    ENTITY_NPC_BETOBETON3 = 339,        // Muk 3
    ENTITY_NPC_DERIBAADO = 340,         // Delibird
    ENTITY_NPC_MERIIPU = 341,           // Mareep
    ENTITY_NPC_WATAKKO = 342,           // Jumpluff
    ENTITY_NPC_DONMERU = 343,           // Numel
    ENTITY_NPC_PACHIRISU = 344,         // Pachirisu
    ENTITY_NPC_KIREIHANA = 345,         // Bellossom
    ENTITY_NPC_KIREIHANA2 = 346,        // Bellossom 2
    ENTITY_NPC_KIREIHANA3 = 347,        // Bellossom 3
    ENTITY_NPC_KIREIHANA4 = 348,        // Bellossom 4
    ENTITY_NPC_KODORA = 349,            // Lairon 1
    ENTITY_NPC_KODORA2 = 350,           // Lairon 2
    ENTITY_NPC_KODORA3 = 351,           // Lairon 3
    ENTITY_NPC_KODORA4 = 352,           // Lairon 4
    ENTITY_NPC_KODORA5 = 353,           // Lairon 5
    ENTITY_NPC_KODORA6 = 354,           // Lairon 6
    ENTITY_NPC_METAMON = 355,           // Ditto
    ENTITY_NPC_GOROON = 356,            // Graveler
    ENTITY_NPC_OTACHI = 357,            // Sentret
    ENTITY_NPC_HERUGAA = 358,           // Houndoom
    ENTITY_NPC_NOKUTASU = 359,          // Cacturne
    ENTITY_NPC_NOKOTCHI = 360,          // Dunsparse
    ENTITY_NPC_GOOSUTO = 361,           // Haunter
    ENTITY_NPC_GOOSUTO2 = 362,          // Haunter 2
    ENTITY_NPC_GOOSUTO3 = 363,          // Haunter 3
    ENTITY_NPC_MAGUMAGGU = 364,         // Slugma
    ENTITY_NPC_MAGUMAGGU2 = 365,        // Slugma 2
    ENTITY_NPC_SHEIMI_FC = 366,         // Shaymin Sky Forme
    ENTITY_NPC_RAIBORUTO = 367,         // Manectric
    ENTITY_NPC_RAKURAI = 368,           // Electrike
    ENTITY_NPC_RAKURAI2 = 369,          // Electrike 2
    ENTITY_NPC_RAKURAI3 = 370,          // Electrike 3
    ENTITY_NPC_RAKURAI4 = 371,          // Electrike 4
    ENTITY_NPC_RAKURAI5 = 372,          // Electrike 5
    ENTITY_NPC_RAKURAI6 = 373,          // Electrike 6
    ENTITY_NPC_RAKURAI7 = 374,          // Electrike 7
    ENTITY_NPC_RAKURAI8 = 375,          // Electrike 8
    ENTITY_NPC_YONOWAARU_N8 = 376,      // Dusknoir
    ENTITY_NPC_YONOWAARU_N9 = 377,      // Dusknoir
    ENTITY_NPC_METAMON_OTACHI = 378,    // Ditto Sentret
    ENTITY_NPC_METAMON_KIREIHANA = 379, // Ditto Bellossom
    ENTITY_NPC_BIIKUIN = 380,           // Vespiquen
    ENTITY_NPC_DAINOOZU = 381,          // Probopass
    ENTITY_NPC_BAKUONGU = 382,          // Exploud
    ENTITY_NPC_NENDOORU = 383,          // Claydol
    ENTITY_NPC_MEGAYANMA = 384,         // Yanmega
    ENTITY_NPC_PINPUKU = 385,           // Happiny
};

// These are definitely used for VAR_EXECUTE_SPECIAL_EPISODE_TYPE.
// Haven't confirmed whether they also apply to VAR_SPECIAL_EPISODE_TYPE...but probably.
enum special_episode_type {
    // Also used in "normal" mode when a special episode isn't running.
    // TODO: Adex says -1 is used outside of special episodes...need to investigate.
    EPISODE_BIDOOFS_WISH = 0,
    EPISODE_IGGLYBUFF_THE_PRODIGY = 1,
    EPISODE_TODAYS_OH_MY_GOSH = 2,
    EPISODE_HERE_COMES_TEAM_CHARM = 3,
    EPISODE_IN_THE_FUTURE_OF_DARKNESS = 4,
};

// This is usually stored as an 8-bit integer
#pragma pack(push, 1)
ENUM_8_BIT(special_episode_type);
#pragma pack(pop)

// Used as return values for GenerateMission
enum mission_generation_result {
    MISSION_GENERATION_SUCCESS = 0,
    MISSION_GENERATION_FAILURE = 1,
    MISSION_GENERATION_GLOBAL_FAILURE = 2,
};

// For the HERO_TALK_KIND and PARTNER_TALK_KIND script variables,
// which control dialogue variations.
// See the comments on VAR_HERO_TALK_KIND and VAR_PARTNER_TALK_KIND for more info.
enum talk_kind {
    TALK_NONE = 0,
    TALK_PARTNER_MALE_SPECIAL = 1,   // Squirtle, Totodile, Chimchar, Meowth, Munchlax
    TALK_PARTNER_DEFAULT = 2,        // All partners not in the "special" categories
    TALK_PARTNER_FEMALE_SPECIAL = 3, // Torchic, Chikorita, Eevee, Vulpix, Skitty
    TALK_HERO_MALE = 4,
    TALK_HERO_FEMALE = 5,
};

enum rank {
    RANK_NORMAL = 0,
    RANK_BRONZE = 1,
    RANK_SILVER = 2,
    RANK_GOLD = 3,
    RANK_DIAMOND = 4,
    RANK_SUPER = 5,
    RANK_ULTRA = 6,
    RANK_HYPER = 7,
    RANK_MASTER = 8,
    RANK_MASTER_1_STAR = 9,
    RANK_MASTER_2_STARS = 10,
    RANK_MASTER_3_STARS = 11,
    RANK_GUILDMASTER = 12,
};

enum animation_speed {
    ANIMATION_SPEED_NORMAL = 0,
    ANIMATION_SPEED_SLOW = 1,
    ANIMATION_SPEED_FAST = 2,
    ANIMATION_SPEED_FREEZE = 3,
};

// These are super long, so split them out into a separate file
#include "version_dep_enums.h"

#endif
