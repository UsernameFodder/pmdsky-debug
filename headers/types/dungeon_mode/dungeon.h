#ifndef HEADERS_TYPES_DUNGEON_MODE_DUNGEON_H_
#define HEADERS_TYPES_DUNGEON_MODE_DUNGEON_H_

// Dungeon state
struct dungeon {
    undefined field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    // 0x3: If true and there's an active mission on the floor, the message
    // "You've reached a destination floor! But where is the target pokémon..." will be displayed.
    bool target_monster_not_found_flag;
    undefined field_0x4;
    bool stepped_on_stairs; // 0x5: True if the leader just stepped on the stairs.
    // 0x6: If equal to 1 or 2, the floor will be advanced at the end of the turn,
    // unless the leader fainted. 2 is used for the leader falling through a pitfall
    // trap.
    uint8_t end_floor_flag;
    // 0x7: If set in tandem with either end_floor_flag or end_floor_no_death_check_flag,
    // a quicksave will occur instead of a floor advance.
    bool quicksave_flag;
    // 0x8: The floor will be advanced at the end of the turn. Set when quicksaving.
    bool end_floor_no_death_check_flag;
    undefined field_0x9;
    undefined field_0xa;
    undefined field_0xb;
    undefined field_0xc;
    undefined field_0xd;
    // 0xE: If true, artificial weather abilities will be activated on this turn.
    // Set to false by TryActivateArtificialWeatherAbilities
    bool activate_artificial_weather_flag;
    // 0xF: True if an enemy has defeated an ally on this turn and should evolve as a result
    // May also have a niche usage if an enemy gets enough experience to level up through
    // something like the Joy Ribbon?
    bool should_enemy_evolve;
    undefined field_0x10;
    // 0x11: True if the leader isn't doing anything right now. False if it's currently performing
    // an action (such as walking or attacking)
    bool no_action_in_progress;
    bool iq_disabled; // 0x12: IQ skills won't work in the dungeon
    undefined field_0x13;
    // 0x14: If > 0, you get kicked out of the dungeon, usually because an important team member
    // (such as your client) fainted.
    enum forced_loss_reason forced_loss_reason;
    undefined field_0x18;
    undefined field_0x19;
    undefined field_0x1a;
    undefined field_0x1b;
    // 0x1C: Increased once per frame until 0x64. Resets to 0 when the leader acts.
    undefined field_0x1c;
    undefined field_0x1d;
    undefined field_0x1e;
    // 0x1F: Turn counter, Speed Boost triggers every 250 turns, then the counter is reset.
    uint8_t speed_boost_counter;
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
    undefined field_0xa4;
    undefined field_0xa5;
    undefined field_0xa6;
    undefined field_0xa7;
    undefined field_0xa8;
    undefined field_0xa9;
    undefined field_0xaa;
    undefined field_0xab;
    undefined field_0xac;
    undefined field_0xad;
    undefined field_0xae;
    undefined field_0xaf;
    undefined field_0xb0;
    undefined field_0xb1;
    undefined field_0xb2;
    undefined field_0xb3;
    undefined field_0xb4;
    undefined field_0xb5;
    undefined field_0xb6;
    undefined field_0xb7;
    undefined field_0xb8;
    undefined field_0xb9;
    undefined field_0xba;
    undefined field_0xbb;
    undefined field_0xbc;
    undefined field_0xbd;
    undefined field_0xbe;
    undefined field_0xbf;
    undefined field_0xc0;
    undefined field_0xc1;
    undefined field_0xc2;
    undefined field_0xc3;
    // 0xC4: Pointer to the entity that is taking its turn right now
    struct entity* current_active_entity;
    // 0xC8: Pointer to the monster that will become the leader of the team after changing leaders
    struct entity* new_leader;
    // 0xCC: Entity struct of the item that was just thrown or dropped
    struct entity thrown_item;
    // 0x184: Info about the most recent damage calculation. Reset with each call to CalcDamage
    struct damage_calc_diag last_damage_calc;
    // 0x1D8: Somehow related to executing a monster's actions (including leader).
    undefined2 field_0x1d8;
    // 0x1DA: Somehow related to executing the leader's actions. Also maybe when leader opens
    // some menus?
    undefined2 field_0x1da;
    undefined field_0x1dc;
    undefined field_0x1dd;
    undefined field_0x1de;
    undefined field_0x1df;
    // 0x1E0: Color table. Used to apply a tint to the colors shown on screen.
    // Changes depending on the current weather.
    struct rgb color_table[256];
    // 0x5E0: Data for Kecleon shopkeeper spawns. It's weird that there's room for more than one;
    // it seems like only the first entry should ever be filled
    struct spawned_shopkeeper_data shopkeeper_spawns[8];
    // 0x610: Number of valid shopkeeper spawns
    int shopkeeper_spawn_count;
    undefined field_0x614;
    undefined field_0x615;
    undefined field_0x616;
    undefined field_0x617;
    undefined field_0x618;
    undefined field_0x619;
    undefined field_0x61a;
    undefined field_0x61b;
    undefined field_0x61c;
    undefined field_0x61d;
    undefined field_0x61e;
    undefined field_0x61f;
    undefined field_0x620;
    undefined field_0x621;
    undefined field_0x622;
    undefined field_0x623;
    undefined field_0x624;
    undefined field_0x625;
    undefined field_0x626;
    undefined field_0x627;
    undefined field_0x628;
    undefined field_0x629;
    undefined field_0x62a;
    undefined field_0x62b;
    undefined field_0x62c;
    undefined field_0x62d;
    undefined field_0x62e;
    undefined field_0x62f;
    undefined field_0x630;
    undefined field_0x631;
    undefined field_0x632;
    undefined field_0x633;
    undefined field_0x634;
    undefined field_0x635;
    undefined field_0x636;
    undefined field_0x637;
    undefined field_0x638;
    undefined field_0x639;
    undefined field_0x63a;
    undefined field_0x63b;
    undefined field_0x63c;
    undefined field_0x63d;
    undefined field_0x63e;
    undefined field_0x63f;
    undefined field_0x640;
    undefined field_0x641;
    undefined field_0x642;
    undefined field_0x643;
    undefined field_0x644;
    undefined field_0x645;
    undefined field_0x646;
    undefined field_0x647;
    undefined field_0x648;
    undefined field_0x649;
    undefined field_0x64a;
    undefined field_0x64b;
    undefined field_0x64c;
    undefined field_0x64d;
    undefined field_0x64e;
    undefined field_0x64f;
    undefined field_0x650;
    undefined field_0x651;
    undefined field_0x652;
    undefined field_0x653;
    undefined field_0x654;
    undefined field_0x655;
    undefined field_0x656;
    undefined field_0x657;
    undefined field_0x658;
    undefined field_0x659;
    undefined field_0x65a;
    undefined field_0x65b;
    undefined field_0x65c;
    undefined field_0x65d;
    undefined field_0x65e;
    undefined field_0x65f;
    undefined field_0x660;
    undefined field_0x661;
    undefined field_0x662;
    undefined field_0x663;
    undefined field_0x664;
    undefined field_0x665;
    undefined field_0x666;
    undefined field_0x667;
    undefined field_0x668;
    undefined field_0x669;
    undefined field_0x66a;
    undefined field_0x66b;
    undefined field_0x66c;
    undefined field_0x66d;
    undefined field_0x66e;
    undefined field_0x66f;
    undefined field_0x670;
    undefined field_0x671;
    undefined field_0x672;
    undefined field_0x673;
    undefined field_0x674;
    undefined field_0x675;
    undefined field_0x676;
    undefined field_0x677;
    undefined field_0x678;
    undefined field_0x679;
    undefined field_0x67a;
    undefined field_0x67b;
    undefined field_0x67c;
    undefined field_0x67d;
    undefined field_0x67e;
    undefined field_0x67f;
    undefined field_0x680;
    undefined field_0x681;
    undefined field_0x682;
    undefined field_0x683;
    undefined field_0x684;
    undefined field_0x685;
    undefined field_0x686;
    undefined field_0x687;
    undefined field_0x688;
    undefined field_0x689;
    undefined field_0x68a;
    undefined field_0x68b;
    undefined field_0x68c;
    undefined field_0x68d;
    undefined field_0x68e;
    undefined field_0x68f;
    undefined field_0x690;
    undefined field_0x691;
    undefined field_0x692;
    undefined field_0x693;
    undefined field_0x694;
    undefined field_0x695;
    undefined field_0x696;
    undefined field_0x697;
    undefined field_0x698;
    undefined field_0x699;
    undefined field_0x69a;
    undefined field_0x69b;
    undefined field_0x69c;
    undefined field_0x69d;
    undefined field_0x69e;
    undefined field_0x69f;
    undefined field_0x6a0;
    undefined field_0x6a1;
    undefined field_0x6a2;
    undefined field_0x6a3;
    undefined field_0x6a4;
    undefined field_0x6a5;
    undefined field_0x6a6;
    undefined field_0x6a7;
    undefined field_0x6a8;
    undefined field_0x6a9;
    undefined field_0x6aa;
    undefined field_0x6ab;
    undefined field_0x6ac;
    undefined field_0x6ad;
    undefined field_0x6ae;
    undefined field_0x6af;
    undefined field_0x6b0;
    undefined field_0x6b1;
    undefined field_0x6b2;
    undefined field_0x6b3;
    undefined field_0x6b4;
    undefined field_0x6b5;
    undefined field_0x6b6;
    undefined field_0x6b7;
    undefined field_0x6b8;
    undefined field_0x6b9;
    undefined field_0x6ba;
    undefined field_0x6bb;
    undefined field_0x6bc;
    undefined field_0x6bd;
    undefined field_0x6be;
    undefined field_0x6bf;
    undefined field_0x6c0;
    undefined field_0x6c1;
    undefined field_0x6c2;
    undefined field_0x6c3;
    undefined field_0x6c4;
    undefined field_0x6c5;
    undefined field_0x6c6;
    undefined field_0x6c7;
    undefined field_0x6c8;
    undefined field_0x6c9;
    undefined field_0x6ca;
    undefined field_0x6cb;
    undefined field_0x6cc;
    undefined field_0x6cd;
    undefined field_0x6ce;
    undefined field_0x6cf;
    undefined field_0x6d0;
    undefined field_0x6d1;
    undefined field_0x6d2;
    undefined field_0x6d3;
    undefined field_0x6d4;
    undefined field_0x6d5;
    undefined field_0x6d6;
    undefined field_0x6d7;
    undefined field_0x6d8;
    undefined field_0x6d9;
    undefined field_0x6da;
    undefined field_0x6db;
    undefined field_0x6dc;
    undefined field_0x6dd;
    undefined field_0x6de;
    undefined field_0x6df;
    undefined field_0x6e0;
    undefined field_0x6e1;
    undefined field_0x6e2;
    undefined field_0x6e3;
    undefined field_0x6e4;
    undefined field_0x6e5;
    undefined field_0x6e6;
    undefined field_0x6e7;
    undefined field_0x6e8;
    undefined field_0x6e9;
    undefined field_0x6ea;
    undefined field_0x6eb;
    undefined field_0x6ec;
    undefined field_0x6ed;
    undefined field_0x6ee;
    undefined field_0x6ef;
    undefined field_0x6f0;
    undefined field_0x6f1;
    undefined field_0x6f2;
    undefined field_0x6f3;
    undefined field_0x6f4;
    undefined field_0x6f5;
    undefined field_0x6f6;
    undefined field_0x6f7;
    undefined field_0x6f8;
    undefined field_0x6f9;
    undefined field_0x6fa;
    undefined field_0x6fb;
    undefined field_0x6fc;
    undefined field_0x6fd;
    undefined field_0x6fe;
    undefined field_0x6ff;
    undefined field_0x700;
    undefined field_0x701;
    undefined field_0x702;
    undefined field_0x703;
    undefined field_0x704;
    undefined field_0x705;
    undefined field_0x706;
    undefined field_0x707;
    undefined field_0x708;
    undefined field_0x709;
    undefined field_0x70a;
    undefined field_0x70b;
    undefined field_0x70c;
    undefined field_0x70d;
    undefined field_0x70e;
    undefined field_0x70f;
    undefined field_0x710;
    undefined field_0x711;
    undefined field_0x712;
    undefined field_0x713;
    undefined field_0x714;
    undefined field_0x715;
    undefined field_0x716;
    undefined field_0x717;
    undefined field_0x718;
    undefined field_0x719;
    undefined field_0x71a;
    undefined field_0x71b;
    undefined field_0x71c;
    undefined field_0x71d;
    undefined field_0x71e;
    undefined field_0x71f;
    undefined field_0x720;
    undefined field_0x721;
    undefined field_0x722;
    undefined field_0x723;
    undefined field_0x724;
    undefined field_0x725;
    undefined field_0x726;
    undefined field_0x727;
    undefined field_0x728;
    undefined field_0x729;
    undefined field_0x72a;
    undefined field_0x72b;
    undefined field_0x72c;
    undefined field_0x72d;
    undefined field_0x72e;
    undefined field_0x72f;
    undefined field_0x730;
    undefined field_0x731;
    undefined field_0x732;
    undefined field_0x733;
    undefined field_0x734;
    undefined field_0x735;
    undefined field_0x736;
    undefined field_0x737;
    undefined field_0x738;
    undefined field_0x739;
    undefined field_0x73a;
    undefined field_0x73b;
    undefined field_0x73c;
    undefined field_0x73d;
    undefined field_0x73e;
    undefined field_0x73f;
    undefined field_0x740;
    undefined field_0x741;
    undefined field_0x742;
    undefined field_0x743;
    undefined field_0x744;
    undefined field_0x745;
    undefined field_0x746;
    undefined field_0x747;
    struct dungeon_id_8 id;             // 0x748
    uint8_t floor;                      // 0x749: Current floor number
    struct dungeon_group_id_8 group_id; // 0x74A: Same for different segments of a dungeon
    undefined field_0x74b;
    undefined field_0x74c;
    undefined field_0x74d;
    undefined field_0x74e;
    undefined field_0x74f;
    undefined field_0x750;
    uint8_t rescue_floor; // 0x751: Floor number where the rescue point is, if applicable
    undefined field_0x752;
    undefined field_0x753;
    undefined field_0x754;
    undefined field_0x755;
    undefined field_0x756;
    undefined field_0x757;
    // 0x758: Whether the current floor should continue or end and why
    struct floor_loop_status_8 floor_loop_status;
    bool recruiting_enabled; // 0x759: Recruitment is allowed in this dungeon
    undefined field_0x75a;
    undefined field_0x75b;
    bool nonstory_flag;      // 0x75C: Allows leader changing and continuing without the partner
    bool send_home_disabled; // 0x75D: Sending teammates home is not allowed in this dungeon
    // 0x75E: Disables sending home/leader changing, lose if partner faints. Set for dungeons
    // between DUNGEON_HIDDEN_LAND and DUNGEON_TEMPORAL_PINNACLE.
    bool hidden_land_flag;
    // 0x75F: If true, the message log won't be shown and the yellow beam animation won't
    // appear over team members after the leader faints
    bool skip_faint_animation_flag;
    // 0x760: Info about the next mission destination floor, if applicable
    struct mission_destination_info mission_destination;
    undefined field_0x77c;
    undefined field_0x77d;
    undefined field_0x77e;
    undefined field_0x77f;
    // 0x780: Controls when a monster at a certain speed stage is able to act.
    // Increased by 1-4 each turn, depending on the team leader's speed level:
    // At normal speed, this will tick up by 4 each turn (can act when x % 4 == 3)
    // At +1 speed, ticks up by 2 each turn (can act when x % 2 == 1)
    // At +2 speed, ticks up by 1 or 2 each turn (can act when x % 4 != 0)
    // At +3 speed, ticks up by 1 each turn (an act every tick)
    // The counter is increased modulo 24, goes from 0x00 to 0x17 then starts over
    uint16_t fractional_turn;
    uint16_t enemy_spawn_counter; // 0x782: Counts from 0-35, spawns happen at 0
    int16_t wind_turns;           // 0x784: Countdown to the wind blowing you out of the dungeon
    uint16_t enemy_density;       // 0x786: If 0, prevents the enemy_spawn_counter for increasing
    // 0x788: The genid of the next monster to be spawned. Ticks up with each spawn.
    uint16_t next_spawn_genid;
    undefined field_0x78a;
    // 0x78B: True if the leader is running. Causes the leader's action for the next turn
    // to be set to action::ACTION_WALK until it hits an obstacle.
    bool leader_running;
    // 0x78C: Likely related to the actions of the leader because it is set to 0 in
    // SetLeaderAction and this value gets bitwise or'd with 0x78B and then saved back to here.
    undefined field_0x78c;
    // 0x78D: This flag is set by the move 0x189 ("HP Gauge") which is the
    // effect of the Identify Orb. If true, monsters not in the team that are
    // holding an item will be marked by a blue exclamation mark icon.
    bool identify_orb_flag;
    bool pass_turn;        // 0x78E: True if the leader has chosen to pass its turn
    bool drought_orb_flag; // 0x78F: True if a drought orb has been used.
    bool thief_alert;      // 0x790: If you've stolen from Kecleon (actual dungeon state)
    // 0x791: If you've stolen from Kecleon (triggers music and other events?)
    bool thief_alert_event;
    // 0x792: Can only be 0,1,2,3,4. Controls when to play certain special music tracks 0x7C
    // (1), 0x7D (2), 0xA8 (3), 0xA9 (4). Used by the Gone Pebbble to play track 0xA8,
    // challenge letter missions to play track 0x7C and dungeon failure to play 0xA9?
    uint8_t unk_music_flag;
    bool monster_house_triggered; // 0x793: You Entered a Monster House (actual dungeon state)
    // 0x794: You entered a Monster House (triggers music and other events?)
    bool monster_house_triggered_event;
    undefined field_0x795;
    undefined field_0x796;
    undefined field_0x797;
    struct dungeon_objective_8 dungeon_objective; // 0x798: Objective of the current dungeon
    undefined field_0x799;
    undefined field_0x79a;
    int8_t rescue_attempts_left; // 0x79B: Number of times you can be rescued in this dungeon
    uint32_t prng_seed;          // 0x79C: The dungeon PRNG seed, if set
    undefined field_0x7a0;
    undefined field_0x7a1;
    undefined field_0x7a2;
    undefined field_0x7a3;
    undefined field_0x7a4;
    undefined field_0x7a5;
    undefined field_0x7a6;
    undefined field_0x7a7;
    // 0x7A8: Holds some data for a monster id to loads its sprite. If this value is non-zero,
    // it gets loaded after loading the dungeon floor monster spawn entries.
    struct monster_id_16 some_monster_sprite_to_load;
    undefined field_0x7aa;
    undefined field_0x7ab;
    // 0x7AC: Second number in the default LCG sequence, used for computing the actual dungeon PRNG
    // seed
    uint32_t prng_preseed;
    undefined field_0x7b0;
    undefined field_0x7b1;
    undefined field_0x7b2;
    undefined field_0x7b3;
    undefined field_0x7b4;
    undefined field_0x7b5;
    undefined field_0x7b6;
    undefined field_0x7b7;
    undefined field_0x7b8;
    undefined field_0x7b9;
    undefined field_0x7ba;
    undefined field_0x7bb;
    undefined field_0x7bc;
    undefined field_0x7bd;
    undefined field_0x7be;
    undefined field_0x7bf;
    undefined field_0x7c0;
    undefined field_0x7c1;
    undefined field_0x7c2;
    undefined field_0x7c3;
    undefined field_0x7c4;
    undefined field_0x7c5;
    undefined field_0x7c6;
    undefined field_0x7c7;
    undefined field_0x7c8;
    // 0x7C9: You entered a Kecleon Shop (triggers music and maybe more?)
    bool standing_in_kecleon_shop;
    undefined field_0x7ca;
    undefined field_0x7cb;
    bool boost_max_money_amount; // 0x7CC: Boost the floor's maximum Poké limit by 8x
    uint8_t _padding_0x7cd[3];
    undefined field_0x7d0;
    undefined field_0x7d1;
    undefined field_0x7d2;
    undefined field_0x7d3;
    undefined field_0x7d4;
    undefined field_0x7d5;
    undefined field_0x7d6;
    undefined field_0x7d7;
    undefined field_0x7d8;
    undefined field_0x7d9;
    undefined field_0x7da;
    undefined field_0x7db;
    undefined field_0x7dc;
    undefined field_0x7dd;
    undefined field_0x7de;
    undefined field_0x7df;
    undefined field_0x7e0;
    undefined field_0x7e1;
    undefined field_0x7e2;
    undefined field_0x7e3;
    undefined field_0x7e4;
    undefined field_0x7e5;
    undefined field_0x7e6;
    undefined field_0x7e7;
    undefined field_0x7e8;
    undefined field_0x7e9;
    undefined field_0x7ea;
    undefined field_0x7eb;
    undefined field_0x7ec;
    undefined field_0x7ed;
    undefined field_0x7ee;
    undefined field_0x7ef;
    undefined field_0x7f0;
    undefined field_0x7f1;
    undefined field_0x7f2;
    undefined field_0x7f3;
    struct monster monsters[20]; // 0x7F4: Info for all the monsters currently in the dungeon
    // 0x34F4: Array that contains the spawn stats for enemies, which are only calculated
    // once at the start of the floor.
    // Since the enemy spawn list of a floor can only have a maximum of 16 entries,
    // the remaining 48 are unused.
    struct enemy_spawn_stats enemy_spawn_stats[64];
    // 0x3974: Monsters that can be spawned on the floor.
    // Read when spawning enemies, at initialization the one at 0x2C964 is used.
    // The number of valid entries is stored in monster_spawn_table_length.
    // The last 2 entries of the table are always Kecleon (0x17F) and the dummy (0x229).
    struct monster_spawn_entry spawn_entries[16];
    undefined field_0x39f4;
    undefined field_0x39f5;
    undefined field_0x39f6;
    undefined field_0x39f7;
    undefined field_0x39f8;
    undefined field_0x39f9;
    undefined field_0x39fa;
    undefined field_0x39fb;
    undefined field_0x39fc;
    undefined field_0x39fd;
    undefined field_0x39fe;
    undefined field_0x39ff;
    undefined field_0x3a00;
    undefined field_0x3a01;
    undefined field_0x3a02;
    undefined field_0x3a03;
    undefined field_0x3a04;
    undefined field_0x3a05;
    undefined field_0x3a06;
    undefined field_0x3a07;
    undefined field_0x3a08;
    undefined field_0x3a09;
    undefined field_0x3a0a;
    undefined field_0x3a0b;
    undefined field_0x3a0c;
    undefined field_0x3a0d;
    undefined field_0x3a0e;
    undefined field_0x3a0f;
    undefined field_0x3a10;
    undefined field_0x3a11;
    undefined field_0x3a12;
    undefined field_0x3a13;
    undefined field_0x3a14;
    undefined field_0x3a15;
    undefined field_0x3a16;
    undefined field_0x3a17;
    undefined field_0x3a18;
    undefined field_0x3a19;
    undefined field_0x3a1a;
    undefined field_0x3a1b;
    undefined field_0x3a1c;
    undefined field_0x3a1d;
    undefined field_0x3a1e;
    undefined field_0x3a1f;
    undefined field_0x3a20;
    undefined field_0x3a21;
    undefined field_0x3a22;
    undefined field_0x3a23;
    undefined field_0x3a24;
    undefined field_0x3a25;
    undefined field_0x3a26;
    undefined field_0x3a27;
    undefined field_0x3a28;
    undefined field_0x3a29;
    undefined field_0x3a2a;
    undefined field_0x3a2b;
    undefined field_0x3a2c;
    undefined field_0x3a2d;
    undefined field_0x3a2e;
    undefined field_0x3a2f;
    undefined field_0x3a30;
    undefined field_0x3a31;
    undefined field_0x3a32;
    undefined field_0x3a33;
    undefined field_0x3a34;
    undefined field_0x3a35;
    undefined field_0x3a36;
    undefined field_0x3a37;
    undefined field_0x3a38;
    undefined field_0x3a39;
    undefined field_0x3a3a;
    undefined field_0x3a3b;
    undefined field_0x3a3c;
    undefined field_0x3a3d;
    undefined field_0x3a3e;
    undefined field_0x3a3f;
    undefined field_0x3a40;
    undefined field_0x3a41;
    undefined field_0x3a42;
    undefined field_0x3a43;
    undefined field_0x3a44;
    undefined field_0x3a45;
    undefined field_0x3a46;
    undefined field_0x3a47;
    undefined field_0x3a48;
    undefined field_0x3a49;
    undefined field_0x3a4a;
    undefined field_0x3a4b;
    undefined field_0x3a4c;
    undefined field_0x3a4d;
    undefined field_0x3a4e;
    undefined field_0x3a4f;
    undefined field_0x3a50;
    undefined field_0x3a51;
    undefined field_0x3a52;
    undefined field_0x3a53;
    undefined field_0x3a54;
    undefined field_0x3a55;
    undefined field_0x3a56;
    undefined field_0x3a57;
    undefined field_0x3a58;
    undefined field_0x3a59;
    undefined field_0x3a5a;
    undefined field_0x3a5b;
    undefined field_0x3a5c;
    undefined field_0x3a5d;
    undefined field_0x3a5e;
    undefined field_0x3a5f;
    undefined field_0x3a60;
    undefined field_0x3a61;
    undefined field_0x3a62;
    undefined field_0x3a63;
    undefined field_0x3a64;
    undefined field_0x3a65;
    undefined field_0x3a66;
    undefined field_0x3a67;
    undefined field_0x3a68;
    undefined field_0x3a69;
    undefined field_0x3a6a;
    undefined field_0x3a6b;
    undefined field_0x3a6c;
    undefined field_0x3a6d;
    undefined field_0x3a6e;
    undefined field_0x3a6f;
    undefined field_0x3a70;
    undefined field_0x3a71;
    undefined field_0x3a72;
    undefined field_0x3a73;
    undefined field_0x3a74;
    undefined field_0x3a75;
    undefined field_0x3a76;
    undefined field_0x3a77;
    undefined field_0x3a78;
    undefined field_0x3a79;
    undefined field_0x3a7a;
    undefined field_0x3a7b;
    undefined field_0x3a7c;
    undefined field_0x3a7d;
    undefined field_0x3a7e;
    undefined field_0x3a7f;
    undefined field_0x3a80;
    undefined field_0x3a81;
    undefined field_0x3a82;
    undefined field_0x3a83;
    undefined field_0x3a84;
    undefined field_0x3a85;
    undefined field_0x3a86;
    undefined field_0x3a87;
    undefined field_0x3a88;
    undefined field_0x3a89;
    undefined field_0x3a8a;
    undefined field_0x3a8b;
    undefined field_0x3a8c;
    undefined field_0x3a8d;
    undefined field_0x3a8e;
    undefined field_0x3a8f;
    undefined field_0x3a90;
    undefined field_0x3a91;
    undefined field_0x3a92;
    undefined field_0x3a93;
    undefined field_0x3a94;
    undefined field_0x3a95;
    undefined field_0x3a96;
    undefined field_0x3a97;
    undefined field_0x3a98;
    undefined field_0x3a99;
    undefined field_0x3a9a;
    undefined field_0x3a9b;
    undefined field_0x3a9c;
    undefined field_0x3a9d;
    undefined field_0x3a9e;
    undefined field_0x3a9f;
    undefined field_0x3aa0;
    undefined field_0x3aa1;
    undefined field_0x3aa2;
    undefined field_0x3aa3;
    undefined field_0x3aa4;
    undefined field_0x3aa5;
    undefined field_0x3aa6;
    undefined field_0x3aa7;
    undefined field_0x3aa8;
    undefined field_0x3aa9;
    undefined field_0x3aaa;
    undefined field_0x3aab;
    undefined field_0x3aac;
    undefined field_0x3aad;
    undefined field_0x3aae;
    undefined field_0x3aaf;
    undefined field_0x3ab0;
    undefined field_0x3ab1;
    undefined field_0x3ab2;
    undefined field_0x3ab3;
    undefined field_0x3ab4;
    undefined field_0x3ab5;
    undefined field_0x3ab6;
    undefined field_0x3ab7;
    undefined field_0x3ab8;
    undefined field_0x3ab9;
    undefined field_0x3aba;
    undefined field_0x3abb;
    undefined field_0x3abc;
    undefined field_0x3abd;
    undefined field_0x3abe;
    undefined field_0x3abf;
    undefined field_0x3ac0;
    undefined field_0x3ac1;
    undefined field_0x3ac2;
    undefined field_0x3ac3;
    undefined field_0x3ac4;
    undefined field_0x3ac5;
    undefined field_0x3ac6;
    undefined field_0x3ac7;
    undefined field_0x3ac8;
    undefined field_0x3ac9;
    undefined field_0x3aca;
    undefined field_0x3acb;
    undefined field_0x3acc;
    undefined field_0x3acd;
    undefined field_0x3ace;
    undefined field_0x3acf;
    undefined field_0x3ad0;
    undefined field_0x3ad1;
    undefined field_0x3ad2;
    undefined field_0x3ad3;
    undefined field_0x3ad4;
    undefined field_0x3ad5;
    undefined field_0x3ad6;
    undefined field_0x3ad7;
    undefined field_0x3ad8;
    undefined field_0x3ad9;
    undefined field_0x3ada;
    undefined field_0x3adb;
    undefined field_0x3adc;
    undefined field_0x3add;
    undefined field_0x3ade;
    undefined field_0x3adf;
    undefined field_0x3ae0;
    undefined field_0x3ae1;
    undefined field_0x3ae2;
    undefined field_0x3ae3;
    undefined field_0x3ae4;
    undefined field_0x3ae5;
    undefined field_0x3ae6;
    undefined field_0x3ae7;
    undefined field_0x3ae8;
    undefined field_0x3ae9;
    undefined field_0x3aea;
    undefined field_0x3aeb;
    undefined field_0x3aec;
    undefined field_0x3aed;
    undefined field_0x3aee;
    undefined field_0x3aef;
    undefined field_0x3af0;
    undefined field_0x3af1;
    undefined field_0x3af2;
    undefined field_0x3af3;
    undefined field_0x3af4;
    undefined field_0x3af5;
    undefined field_0x3af6;
    undefined field_0x3af7;
    undefined field_0x3af8;
    undefined field_0x3af9;
    undefined field_0x3afa;
    undefined field_0x3afb;
    undefined field_0x3afc;
    undefined field_0x3afd;
    undefined field_0x3afe;
    undefined field_0x3aff;
    undefined field_0x3b00;
    undefined field_0x3b01;
    undefined field_0x3b02;
    undefined field_0x3b03;
    undefined field_0x3b04;
    undefined field_0x3b05;
    undefined field_0x3b06;
    undefined field_0x3b07;
    undefined field_0x3b08;
    undefined field_0x3b09;
    undefined field_0x3b0a;
    undefined field_0x3b0b;
    undefined field_0x3b0c;
    undefined field_0x3b0d;
    undefined field_0x3b0e;
    undefined field_0x3b0f;
    undefined field_0x3b10;
    undefined field_0x3b11;
    undefined field_0x3b12;
    undefined field_0x3b13;
    undefined field_0x3b14;
    undefined field_0x3b15;
    undefined field_0x3b16;
    undefined field_0x3b17;
    undefined field_0x3b18;
    undefined field_0x3b19;
    undefined field_0x3b1a;
    undefined field_0x3b1b;
    undefined field_0x3b1c;
    undefined field_0x3b1d;
    undefined field_0x3b1e;
    undefined field_0x3b1f;
    undefined field_0x3b20;
    undefined field_0x3b21;
    undefined field_0x3b22;
    undefined field_0x3b23;
    undefined field_0x3b24;
    undefined field_0x3b25;
    undefined field_0x3b26;
    undefined field_0x3b27;
    undefined field_0x3b28;
    undefined field_0x3b29;
    undefined field_0x3b2a;
    undefined field_0x3b2b;
    undefined field_0x3b2c;
    undefined field_0x3b2d;
    undefined field_0x3b2e;
    undefined field_0x3b2f;
    undefined field_0x3b30;
    undefined field_0x3b31;
    undefined field_0x3b32;
    undefined field_0x3b33;
    undefined field_0x3b34;
    undefined field_0x3b35;
    undefined field_0x3b36;
    undefined field_0x3b37;
    undefined field_0x3b38;
    undefined field_0x3b39;
    undefined field_0x3b3a;
    undefined field_0x3b3b;
    undefined field_0x3b3c;
    undefined field_0x3b3d;
    undefined field_0x3b3e;
    undefined field_0x3b3f;
    undefined field_0x3b40;
    undefined field_0x3b41;
    undefined field_0x3b42;
    undefined field_0x3b43;
    undefined field_0x3b44;
    undefined field_0x3b45;
    undefined field_0x3b46;
    undefined field_0x3b47;
    undefined field_0x3b48;
    undefined field_0x3b49;
    undefined field_0x3b4a;
    undefined field_0x3b4b;
    undefined field_0x3b4c;
    undefined field_0x3b4d;
    undefined field_0x3b4e;
    undefined field_0x3b4f;
    undefined field_0x3b50;
    undefined field_0x3b51;
    undefined field_0x3b52;
    undefined field_0x3b53;
    undefined field_0x3b54;
    undefined field_0x3b55;
    undefined field_0x3b56;
    undefined field_0x3b57;
    undefined field_0x3b58;
    undefined field_0x3b59;
    undefined field_0x3b5a;
    undefined field_0x3b5b;
    undefined field_0x3b5c;
    undefined field_0x3b5d;
    undefined field_0x3b5e;
    undefined field_0x3b5f;
    undefined field_0x3b60;
    undefined field_0x3b61;
    undefined field_0x3b62;
    undefined field_0x3b63;
    undefined field_0x3b64;
    undefined field_0x3b65;
    undefined field_0x3b66;
    undefined field_0x3b67;
    undefined field_0x3b68;
    undefined field_0x3b69;
    undefined field_0x3b6a;
    undefined field_0x3b6b;
    undefined field_0x3b6c;
    undefined field_0x3b6d;
    undefined field_0x3b6e;
    undefined field_0x3b6f;
    undefined field_0x3b70;
    undefined field_0x3b71;
    undefined field_0x3b72;
    undefined field_0x3b73;
    // 0x3B74: Unknown array, likely one entry per monster species
    uint8_t unknown_array_0x3B74[600];
    // 0x3DCC: Uncertain what this array holds, but this value is compared to statuses:0x7 in
    // several places and accessd in a loop with the upper bound being 20.
    undefined4 field_0x3dcc[20];
    undefined4 field_0x3e1c;
    // 0x3E20: Number of valid monster spawn entries (see spawn_entries).
    int monster_spawn_entries_length;
    undefined field_0x3e24;
    undefined field_0x3e25;
    undefined field_0x3e26;
    undefined field_0x3e27;
    undefined field_0x3e28;
    undefined field_0x3e29;
    undefined field_0x3e2a;
    undefined field_0x3e2b;
    undefined field_0x3e2c;
    undefined field_0x3e2d;
    undefined field_0x3e2e;
    undefined field_0x3e2f;
    undefined field_0x3e30;
    undefined field_0x3e31;
    undefined field_0x3e32;
    undefined field_0x3e33;
    bool plus_is_active[2];  // 0x3E34: A monster on the {enemy, team} side has the ability Plus
    bool minus_is_active[2]; // 0x3E36: A monster on the {enemy, team} side has the ability Minus
    // 0x3E38: If true, a monster on the floor is a decoy.
    bool decoy_is_active;
    // 0x3E39: If true, a monster with id 0x97 (Mew) cannot be spawned on the floor.
    bool mew_cannot_spawn;
    undefined field_0x3e3a;
    undefined field_0x3e3b;
    // 0x3E3C: Gets set to true in ChangeShayminForme. Seems to also control which sprite to
    // load for a Shaymin on the team?
    bool shaymin_sky_form_loaded;
    undefined field_0x3e3d;
    undefined field_0x3e3e;
    undefined field_0x3e3f;
    struct item items[64]; // 0x3E40: Info for all the items on the ground
    uint16_t n_items;      // 0x3FC0: Number of active items in the above items array
    undefined field_0x3fc2;
    undefined field_0x3fc3;
    struct trap traps[64];                   // 0x3FC4: Info for all the traps on the floor
    struct dungeon_generation_info gen_info; // 0x40C4: Stuff involved with dungeon generation
    undefined field_0xccfc;
    undefined field_0xccfd;
    undefined field_0xccfe;
    undefined field_0xccff;
    undefined field_0xcd00;
    undefined field_0xcd01;
    undefined field_0xcd02;
    undefined field_0xcd03;
    undefined field_0xcd04;
    undefined field_0xcd05;
    undefined field_0xcd06;
    undefined field_0xcd07;
    undefined field_0xcd08;
    undefined field_0xcd09;
    undefined field_0xcd0a;
    undefined field_0xcd0b;
    undefined field_0xcd0c;
    undefined field_0xcd0d;
    undefined field_0xcd0e;
    undefined field_0xcd0f;
    undefined field_0xcd10;
    undefined field_0xcd11;
    undefined field_0xcd12;
    undefined field_0xcd13;
    // Min x of the generated Kecleon shop, if it exists. This reflects the original generation, and
    // is not updated if some shop tiles are deleted by later steps in floor generation
    int kecleon_shop_min_x; // 0xCD14: inclusive
    int kecleon_shop_min_y; // 0xCD18: inclusive
    int kecleon_shop_max_x; // 0xCD1C: inclusive
    int kecleon_shop_max_y; // 0xCD20: inclusive
    undefined field_0xcd24;
    undefined field_0xcd25;
    undefined field_0xcd26;
    undefined field_0xcd27;
    undefined field_0xcd28;
    undefined field_0xcd29;
    undefined field_0xcd2a;
    undefined field_0xcd2b;
    undefined field_0xcd2c;
    undefined field_0xcd2d;
    undefined field_0xcd2e;
    undefined field_0xcd2f;
    undefined field_0xcd30;
    undefined field_0xcd31;
    undefined field_0xcd32;
    undefined field_0xcd33;
    undefined field_0xcd34;
    undefined field_0xcd35;
    undefined field_0xcd36;
    undefined field_0xcd37;
    struct weather_id_8 weather; // 0xCD38: current weather
    // 0xCD39: Default weather on the floor that will be reverted to if the current weather is
    // artificial and ends
    struct weather_id_8 natural_weather;
    // 0xCD3A: Turns left for each weather type in enum weather_id (except WEATHER_RANDOM). If
    // multiple of these are nonzero, the one with the highest number of turns left is chosen.
    // Ties are broken in enum order
    uint16_t weather_turns[8];
    // 0xCD4A: Turns left for artificial permaweather from weather-setting abilities like Drought,
    // Sand Stream, Drizzle, and Snow Warning; one counter for each weather type in enum weather_id
    // (except WEATHER_RANDOM). Any nonzero value triggers that weather condition (it's usually set
    // to 1 or 0). If the weather's source is removed, this value becomes the normal number of turns
    // left for that weather condition. Priority in the event of multiple nonzero counters is the
    // same as with weather_turns.
    uint16_t artificial_permaweather_turns[8];
    // 0xCD5A: For damaging weather conditions like sandstorm. Counts down from 9-0, damage on 9
    uint8_t weather_damage_counter;
    uint8_t mud_sport_turns;   // 0xCD5B: Number of turns left for the Mud Sport condition
    uint8_t water_sport_turns; // 0xCD5C: Number of turns left for the Water Sport condition
    bool nullify_weather;      // 0xCD5D: Cloud Nine/Air Lock is in effect
    undefined field_0xcd5e;
    undefined field_0xcd5f;
    // 0xCD60: Seems to be tile data for tiles within fixed rooms
    struct tile fixed_room_tiles[8][8];
    undefined field_0xd260;
    undefined field_0xd261;
    undefined field_0xd262;
    undefined field_0xd263;
    undefined field_0xd264;
    undefined field_0xd265;
    undefined field_0xd266;
    undefined field_0xd267;
    undefined field_0xd268;
    undefined field_0xd269;
    undefined field_0xd26a;
    undefined field_0xd26b;
    undefined field_0xd26c;
    undefined field_0xd26d;
    undefined field_0xd26e;
    undefined field_0xd26f;
    undefined field_0xd270;
    undefined field_0xd271;
    undefined field_0xd272;
    undefined field_0xd273;
    undefined field_0xd274;
    undefined field_0xd275;
    undefined field_0xd276;
    undefined field_0xd277;
    undefined field_0xd278;
    undefined field_0xd279;
    undefined field_0xd27a;
    undefined field_0xd27b;
    undefined field_0xd27c;
    undefined field_0xd27d;
    undefined field_0xd27e;
    undefined field_0xd27f;
    undefined field_0xd280;
    undefined field_0xd281;
    undefined field_0xd282;
    undefined field_0xd283;
    undefined field_0xd284;
    undefined field_0xd285;
    undefined field_0xd286;
    undefined field_0xd287;
    undefined field_0xd288;
    undefined field_0xd289;
    undefined field_0xd28a;
    undefined field_0xd28b;
    undefined field_0xd28c;
    undefined field_0xd28d;
    undefined field_0xd28e;
    undefined field_0xd28f;
    undefined field_0xd290;
    undefined field_0xd291;
    undefined field_0xd292;
    undefined field_0xd293;
    undefined field_0xd294;
    undefined field_0xd295;
    undefined field_0xd296;
    undefined field_0xd297;
    undefined field_0xd298;
    undefined field_0xd299;
    undefined field_0xd29a;
    undefined field_0xd29b;
    undefined field_0xd29c;
    undefined field_0xd29d;
    undefined field_0xd29e;
    undefined field_0xd29f;
    undefined field_0xd2a0;
    undefined field_0xd2a1;
    undefined field_0xd2a2;
    undefined field_0xd2a3;
    undefined field_0xd2a4;
    undefined field_0xd2a5;
    undefined field_0xd2a6;
    undefined field_0xd2a7;
    undefined field_0xd2a8;
    undefined field_0xd2a9;
    undefined field_0xd2aa;
    undefined field_0xd2ab;
    undefined field_0xd2ac;
    undefined field_0xd2ad;
    undefined field_0xd2ae;
    undefined field_0xd2af;
    undefined field_0xd2b0;
    undefined field_0xd2b1;
    undefined field_0xd2b2;
    undefined field_0xd2b3;
    undefined field_0xd2b4;
    undefined field_0xd2b5;
    undefined field_0xd2b6;
    undefined field_0xd2b7;
    undefined field_0xd2b8;
    undefined field_0xd2b9;
    undefined field_0xd2ba;
    undefined field_0xd2bb;
    undefined field_0xd2bc;
    undefined field_0xd2bd;
    undefined field_0xd2be;
    undefined field_0xd2bf;
    undefined field_0xd2c0;
    undefined field_0xd2c1;
    undefined field_0xd2c2;
    undefined field_0xd2c3;
    undefined field_0xd2c4;
    undefined field_0xd2c5;
    undefined field_0xd2c6;
    undefined field_0xd2c7;
    undefined field_0xd2c8;
    undefined field_0xd2c9;
    undefined field_0xd2ca;
    undefined field_0xd2cb;
    undefined field_0xd2cc;
    undefined field_0xd2cd;
    undefined field_0xd2ce;
    undefined field_0xd2cf;
    undefined field_0xd2d0;
    undefined field_0xd2d1;
    undefined field_0xd2d2;
    undefined field_0xd2d3;
    undefined field_0xd2d4;
    undefined field_0xd2d5;
    undefined field_0xd2d6;
    undefined field_0xd2d7;
    undefined field_0xd2d8;
    undefined field_0xd2d9;
    undefined field_0xd2da;
    undefined field_0xd2db;
    undefined field_0xd2dc;
    undefined field_0xd2dd;
    undefined field_0xd2de;
    undefined field_0xd2df;
    undefined field_0xd2e0;
    undefined field_0xd2e1;
    undefined field_0xd2e2;
    undefined field_0xd2e3;
    // 0xD2E4: This is an array of pointers that point to the actual tile structs within the
    // dungeon struct
    struct tile* tile_ptrs[32][56];
    uint8_t n_rooms;        // 0xEEE4: Number of rooms on the current floor
    undefined field_0xeee5; // Uninitialized, likely padding
    undefined field_0xeee6; // Uninitialized, likely padding
    undefined field_0xeee7; // Uninitialized, likely padding
    // 0xEEE8: Array of structs that contain data about the rooms on the current floor
    struct room_data room_data[32];
    // 0xF268: Array that counts the amount of elements on each row of natural_junction_list
    uint16_t natural_junction_list_counts[32];
    // 0xF2A8: Matrix that stores the positions of up to 32 natural junctions
    // for each room on the floor. Each row represents a room.
    struct position natural_junction_list[32][32];
    // 0x102A8: Buffer to store some file after being decompressed.
    // The file that gets loaded depends on the tileset ID of the dungeon.
    uint8_t unknown_file_buffer_0x102A8[7200];
    // 0x11EC8: Matrix related to tile graphics.
    // Skipping the code that initializes it at the start of the dungeon causes all tiles
    // to be shown as black squares.
    uint16_t unknown_tile_matrix[9][34];
    // 0x1212C: Unknown matrix, probably related to unknown_tile_matrix
    // since they get initialized together.
    uint16_t unknown_matrix_0x1212C[9][3];
    // 0x12162: Buffer to store some AT4PX file after being decompressed
    uint8_t unknown_file_buffer_0x12162[2352];
    // 0x12A92: Unknown array, probably related to unknown_tile_matrix
    // since they get initialized together.
    uint16_t unknown_array_0x12A92[9];
    // 0x12AA4: Gets set after loading the data for a fixed room when generating a
    // floor. Gets set back to null when done generating a floor.
    undefined* unk_fixed_room_pointer;
    // 0x12AA8: This flag is set by the move 0x191 ("Trapper") which is the effect
    // of the Trapper Orb. If true, the game will try to spawn a trap.
    bool trapper_orb_flag;
    undefined field_0x12aa9; // Likely padding
    // 0x12AAA: Location to spawn the trap by trapper_orb_flag.
    struct position trapper_orb_trap_pos;
    // 0x12AAE: Trap ID to spawn the trap by trapper_orb_flag. Notably, the game
    // will always set this to 0x19, but could work with other values.
    struct trap_id_8 traper_orb_trap_id;
    // 0x12AAF: Alignment to spawn the trap by trapper_orb_flag.
    uint8_t trapper_orb_trap_team;
    undefined field_0x12ab0;
    undefined field_0x12ab1;
    undefined field_0x12ab2;
    undefined field_0x12ab3;
    undefined field_0x12ab4;
    undefined field_0x12ab5;
    undefined field_0x12ab6;
    undefined field_0x12ab7;
    undefined field_0x12ab8;
    undefined field_0x12ab9;
    undefined field_0x12aba;
    undefined field_0x12abb;
    undefined field_0x12abc;
    undefined field_0x12abd;
    undefined field_0x12abe;
    undefined field_0x12abf;
    undefined field_0x12ac0;
    undefined field_0x12ac1;
    undefined field_0x12ac2;
    undefined field_0x12ac3;
    undefined field_0x12ac4;
    undefined field_0x12ac5;
    undefined field_0x12ac6;
    undefined field_0x12ac7;
    undefined field_0x12ac8;
    undefined field_0x12ac9;
    undefined field_0x12aca;
    undefined field_0x12acb;
    undefined field_0x12acc;
    undefined field_0x12acd;
    undefined field_0x12ace;
    undefined field_0x12acf;
    undefined field_0x12ad0;
    undefined field_0x12ad1;
    undefined field_0x12ad2;
    undefined field_0x12ad3;
    undefined field_0x12ad4;
    undefined field_0x12ad5;
    undefined field_0x12ad6;
    undefined field_0x12ad7;
    undefined field_0x12ad8;
    undefined field_0x12ad9;
    undefined field_0x12ada;
    undefined field_0x12adb;
    undefined field_0x12adc;
    undefined field_0x12add;
    undefined field_0x12ade;
    undefined field_0x12adf;
    undefined field_0x12ae0;
    undefined field_0x12ae1;
    undefined field_0x12ae2;
    undefined field_0x12ae3;
    undefined field_0x12ae4;
    undefined field_0x12ae5;
    undefined field_0x12ae6;
    undefined field_0x12ae7;
    undefined field_0x12ae8;
    undefined field_0x12ae9;
    undefined field_0x12aea;
    undefined field_0x12aeb;
    undefined field_0x12aec;
    undefined field_0x12aed;
    undefined field_0x12aee;
    undefined field_0x12aef;
    undefined field_0x12af0;
    undefined field_0x12af1;
    undefined field_0x12af2;
    undefined field_0x12af3;
    undefined field_0x12af4;
    undefined field_0x12af5;
    undefined field_0x12af6;
    undefined field_0x12af7;
    undefined field_0x12af8;
    undefined field_0x12af9;
    // 0x12AFA: Number of normal item spawns. Does not include monster held items and additional
    // items in walls or Monster Houses
    uint16_t n_normal_item_spawns;
    bool gravity; // 0x12AFC: Gravity is in effect
    undefined field_0x12afd;
    undefined field_0x12afe;
    undefined field_0x12aff;
    undefined field_0x12b00;
    undefined field_0x12b01;
    undefined field_0x12b02;
    undefined field_0x12b03;
    undefined field_0x12b04;
    undefined field_0x12b05;
    undefined field_0x12b06;
    undefined field_0x12b07;
    undefined field_0x12b08;
    undefined field_0x12b09;
    undefined field_0x12b0a;
    undefined field_0x12b0b;
    undefined field_0x12b0c;
    undefined field_0x12b0d;
    undefined field_0x12b0e;
    undefined field_0x12b0f;
    undefined field_0x12b10;
    undefined field_0x12b11;
    undefined field_0x12b12;
    undefined field_0x12b13;
    undefined field_0x12b14;
    undefined field_0x12b15;
    undefined field_0x12b16;
    undefined field_0x12b17;
    undefined field_0x12b18;
    undefined field_0x12b19;
    undefined field_0x12b1a;
    undefined field_0x12b1b;
    undefined field_0x12b1c;
    undefined field_0x12b1d;
    undefined field_0x12b1e;
    undefined field_0x12b1f;
    undefined field_0x12b20;
    undefined field_0x12b21;
    undefined field_0x12b22;
    undefined field_0x12b23;
    // 0x12B24: Whether or not the kecleon shop spawn chance be boosted for the floor
    bool boost_kecleon_shop_spawn_chance;
    // 0x12B25: Whether or not the hidden stairs spawn chance be boosted for the floor
    // (by 20%; see HIDDEN_STAIRS_SPAWN_CHANCE_MULTIPLIER)
    bool boost_hidden_stairs_spawn_chance;
    undefined field_0x12b26;
    undefined field_0x12b27;
    struct entity_table entity_table; // 0x12B28: Table of all entities in the dungeon
    undefined field_0x198e4;
    undefined field_0x198e5;
    undefined field_0x198e6;
    undefined field_0x198e7;
    undefined field_0x198e8;
    undefined field_0x198e9;
    undefined field_0x198ea;
    undefined field_0x198eb;
    undefined field_0x198ec;
    undefined field_0x198ed;
    undefined field_0x198ee;
    undefined field_0x198ef;
    undefined field_0x198f0;
    undefined field_0x198f1;
    undefined field_0x198f2;
    undefined field_0x198f3;
    undefined field_0x198f4;
    undefined field_0x198f5;
    undefined field_0x198f6;
    undefined field_0x198f7;
    undefined field_0x198f8;
    undefined field_0x198f9;
    undefined field_0x198fa;
    undefined field_0x198fb;
    undefined field_0x198fc;
    undefined field_0x198fd;
    undefined field_0x198fe;
    undefined field_0x198ff;
    undefined field_0x19900;
    undefined field_0x19901;
    undefined field_0x19902;
    undefined field_0x19903;
    // 0x19904: Unknown entity pointer to a monster.
    struct entity* unknown_monster;
    // 0x19908: Pointer to the entity to be spawned by the effect of Illuminate
    struct entity* illuminate_spawn_entity;
    // 0x1990C: Seems to store the value of field 0x7 from the
    // statuses struct of the monster that 0x19904 points to.
    undefined field_0x1990c;
    undefined field_0x1990d;
    undefined field_0x1990e;
    undefined field_0x1990f;
    // 0x19910: Spawn genid of the monster to be spawned by the effect of Illuminate
    uint16_t illuminate_spawn_genid;
    undefined field_0x19912;
    undefined field_0x19913;
    // 0x19914: List that matches monster IDs with their sprite index on this floor
    struct monster_id_16 sprite_indexes[1155];
    undefined field_0x1a21a;
    undefined field_0x1a21b;
    // 0x1A21C: Data about the map, the camera and the touchscreen numbers
    struct display_data display_data;
    struct minimap_display_data minimap_display_data; // 0x1A264: Data used to display the minimap
    undefined field_0x286b0;
    undefined field_0x286b1;
    struct floor_properties floor_properties; // 0x286B2: Properties about the current floor
    undefined field_0x286d2;
    undefined field_0x286d3;
    undefined field_0x286d4;
    undefined field_0x286d5;
    undefined field_0x286d6;
    undefined field_0x286d7;
    undefined field_0x286d8;
    undefined field_0x286d9;
    undefined field_0x286da;
    undefined field_0x286db;
    undefined field_0x286dc;
    undefined field_0x286dd;
    undefined field_0x286de;
    undefined field_0x286df;
    undefined field_0x286e0;
    undefined field_0x286e1;
    undefined field_0x286e2;
    undefined field_0x286e3;
    undefined field_0x286e4;
    undefined field_0x286e5;
    undefined field_0x286e6;
    undefined field_0x286e7;
    undefined field_0x286e8;
    undefined field_0x286e9;
    undefined field_0x286ea;
    undefined field_0x286eb;
    undefined field_0x286ec;
    undefined field_0x286ed;
    undefined field_0x286ee;
    undefined field_0x286ef;
    undefined field_0x286f0;
    undefined field_0x286f1;
    // 0x286F2: Spawn weights for regular items. It's the unrolled form of this floor's regular
    // item spawn list.
    // It has enough space to hold 1416 entries (1400 items + 16 categories), but only the
    // first 0x16C slots are used since spawn lists can't encode item IDs larger than that.
    uint16_t regular_item_weights[1416];
    // 0x29202: Spawn weights for Kecleon shop items. Same format as regular_item_weights.
    uint16_t kecleon_item_weights[1416];
    // 0x29D12: Spawn weights for monster house items. Same format as regular_item_weights.
    uint16_t monster_house_item_weights[1416];
    // 0x2A822: Spawn weights for buried items. Same format as regular_item_weights.
    uint16_t buried_item_weights[1416];
    // 0x2B332: Spawn weights for bazaar grab bag items. Same format as regular_item_weights.
    uint16_t grab_bag_item_weights[1416];
    // 0x2BE42: Spawn weights for secret room items in treasure boxes.
    // Same format as regular_item_weights? For some reason the weights for the secret rooms
    // are of a different length than the other item weight lists before it. The trap weights
    // appear to overlap what would be entries 1400-1415.
    uint16_t secret_room_item_weights[1400];
    // 0x2C932: Spawn weights for traps.
    uint16_t trap_weights[25];
    // 0x2C964: List of spawn entries on this floor
    // This is used during initialization, enemies are spawned using the copy at 0x3974
    struct monster_spawn_entry spawn_entries_master[16];
    undefined2 field_0x2c9e4;
    // 0x2C9E6: Highest level among all the enemies that spawn on this floor
    uint16_t highest_enemy_level;
    // 0x2C9E8: ID of an item guaranteed to spawn on the floor, if applicable
    // (e.g., certain mission types)
    struct item_id_16 guaranteed_item_id;
    // 0x2C9EA: List of the indices in the complete monster spawn table for this floor that were
    // chosen to spawn on it. It gets rerandomized at the start of each new floor.
    uint16_t spawn_table_entries_chosen[16];
    undefined field_0x2ca0a;
    undefined field_0x2ca0b;
    // 0x2CA0C: Holds the name for the entity that caused the faint. The exact size is a guess.
    // Likely larger because of entities like the Explosion Trap.
    char fainted_monster_cause_entity_name[10];
    undefined field_0x2ca16;
    undefined field_0x2ca17;
    undefined field_0x2ca18;
    undefined field_0x2ca19;
    undefined field_0x2ca1a;
    undefined field_0x2ca1b;
    undefined field_0x2ca1c;
    undefined field_0x2ca1d;
    undefined field_0x2ca1e;
    undefined field_0x2ca1f;
    undefined field_0x2ca20;
    undefined field_0x2ca21;
    undefined field_0x2ca22;
    undefined field_0x2ca23;
    undefined field_0x2ca24;
    undefined field_0x2ca25;
    undefined field_0x2ca26;
    undefined field_0x2ca27;
    undefined field_0x2ca28;
    undefined field_0x2ca29;
    // 0x2CA2A: Holds the name for the monster that fainted. The exact size is a guess.
    char fainted_monster_name[10];
    undefined field_0x2ca34;
    undefined field_0x2ca35;
    undefined field_0x2ca36;
    undefined field_0x2ca37;
    undefined field_0x2ca38;
    undefined field_0x2ca39;
    undefined field_0x2ca3a;
    undefined field_0x2ca3b;
    undefined field_0x2ca3c;
    undefined field_0x2ca3d;
    undefined field_0x2ca3e;
    undefined field_0x2ca3f;
    undefined field_0x2ca40;
    undefined field_0x2ca41;
    undefined field_0x2ca42;
    undefined field_0x2ca43;
    undefined field_0x2ca44;
    undefined field_0x2ca45;
    undefined field_0x2ca46;
    undefined field_0x2ca47;
    // 0x2CA0C: Possible the game may always set it to the null terminator and never actually
    // copies something inside. Uncertain of exact size
    char field_0x2ca48[1];
    undefined field_0x2ca49;
    undefined field_0x2ca4a;
    undefined field_0x2ca4b;
    undefined field_0x2ca4c;
    undefined field_0x2ca4d;
    undefined field_0x2ca4e;
    undefined field_0x2ca4f;
    undefined field_0x2ca50;
    undefined field_0x2ca51;
    undefined field_0x2ca52;
    undefined field_0x2ca53;
    undefined field_0x2ca54;
    undefined field_0x2ca55;
    undefined field_0x2ca56;
    undefined field_0x2ca57;
    undefined field_0x2ca58;
    undefined field_0x2ca59;
    undefined field_0x2ca5a;
    undefined field_0x2ca5b;
    undefined field_0x2ca5c;
    undefined field_0x2ca5d;
    undefined field_0x2ca5e;
    undefined field_0x2ca5f;
    undefined field_0x2ca60;
    undefined field_0x2ca61;
    undefined field_0x2ca62;
    undefined field_0x2ca63;
    undefined field_0x2ca64;
    undefined field_0x2ca65;
    undefined field_0x2ca66;
    undefined field_0x2ca67;
    struct dungeon_id_8 fainted_id; // 0x2CA68: Copied from dungeon::id, upon fainting.
    uint8_t fainted_floor;          // 0x2CA69: Copied from dungeon::floor, upon fainting.
    // 0x2CA6A: Copy of the fainted monster's held item.
    struct item fainted_monster_held_item;
    int fainted_monster_exp; // 0x2CA70: Copy of fainted monster's experience.
    // 0x2CA74: Copy of the fainted monster's ACTUAL max hp (monster::max_hp_stat +
    // monster::max_hp_boost)
    int16_t fainted_monster_max_hp;
    // 0x2CA76: Copy of the fainted monster's offensive stats.
    uint8_t fainted_monster_offensive_stats[2];
    // 0x2CA78: Copy of the fainted monster's defenive stats.
    uint8_t fainted_monster_defensive_stats[2];
    uint8_t fainted_monster_level; // 0x2CA7A: Copy of fainted monster's level.
    undefined field_0x2ca7b;
    undefined field_0x2ca7c;
    undefined field_0x2ca7d;
    undefined field_0x2ca7e;
    undefined field_0x2ca7f;
    undefined field_0x2ca80;
    undefined field_0x2ca81;
    undefined field_0x2ca82;
    undefined field_0x2ca83;
    undefined field_0x2ca84;
    undefined field_0x2ca85;
    undefined field_0x2ca86;
    undefined field_0x2ca87;
    undefined field_0x2ca88;
    undefined field_0x2ca89;
    undefined field_0x2ca8a;
    undefined field_0x2ca8b;
    undefined field_0x2ca8c;
    undefined field_0x2ca8d;
    undefined field_0x2ca8e;
    undefined field_0x2ca8f;
    undefined field_0x2ca90;
    undefined field_0x2ca91;
    undefined field_0x2ca92;
    undefined field_0x2ca93;
    undefined field_0x2ca94;
    undefined field_0x2ca95;
    undefined field_0x2ca96;
    undefined field_0x2ca97;
    undefined field_0x2ca98;
    undefined field_0x2ca99;
    undefined field_0x2ca9a;
    undefined field_0x2ca9b;
    undefined field_0x2ca9c;
    undefined field_0x2ca9d;
    undefined field_0x2ca9e;
    undefined field_0x2ca9f;
    undefined field_0x2caa0;
    undefined field_0x2caa1;
    undefined field_0x2caa2;
    undefined field_0x2caa3;
    undefined field_0x2caa4;
    undefined field_0x2caa5;
    undefined field_0x2caa6;
    undefined field_0x2caa7;
    undefined field_0x2caa8;
    undefined field_0x2caa9;
    undefined field_0x2caaa;
    undefined field_0x2caab;
    undefined field_0x2caac;
    undefined field_0x2caad;
    undefined field_0x2caae;
    undefined field_0x2caaf;
    undefined field_0x2cab0;
    undefined field_0x2cab1;
    undefined field_0x2cab2;
    undefined field_0x2cab3;
    undefined field_0x2cab4;
    undefined field_0x2cab5;
    undefined field_0x2cab6;
    undefined field_0x2cab7;
    undefined field_0x2cab8;
    undefined field_0x2cab9;
    undefined field_0x2caba;
    undefined field_0x2cabb;
    undefined field_0x2cabc;
    undefined field_0x2cabd;
    undefined field_0x2cabe;
    undefined field_0x2cabf;
    undefined field_0x2cac0;
    undefined field_0x2cac1;
    undefined field_0x2cac2;
    undefined field_0x2cac3;
    undefined field_0x2cac4;
    undefined field_0x2cac5;
    undefined field_0x2cac6;
    undefined field_0x2cac7;
    undefined field_0x2cac8;
    undefined field_0x2cac9;
    undefined field_0x2caca;
    undefined field_0x2cacb;
    undefined field_0x2cacc;
    undefined field_0x2cacd;
    undefined field_0x2cace;
    undefined field_0x2cacf;
    undefined field_0x2cad0;
    undefined field_0x2cad1;
    undefined field_0x2cad2;
    undefined field_0x2cad3;
    undefined field_0x2cad4;
    undefined field_0x2cad5;
    undefined field_0x2cad6;
    undefined field_0x2cad7;
    undefined field_0x2cad8;
    undefined field_0x2cad9;
    undefined field_0x2cada;
    undefined field_0x2cadb;
    undefined field_0x2cadc;
    undefined field_0x2cadd;
    undefined field_0x2cade;
    undefined field_0x2cadf;
    undefined field_0x2cae0;
    undefined field_0x2cae1;
    undefined field_0x2cae2;
    undefined field_0x2cae3;
    undefined field_0x2cae4;
    undefined field_0x2cae5;
    undefined field_0x2cae6;
    undefined field_0x2cae7;
    undefined field_0x2cae8;
    undefined field_0x2cae9;
    undefined field_0x2caea;
    undefined field_0x2caeb;
    undefined field_0x2caec;
    undefined field_0x2caed;
    undefined field_0x2caee;
    undefined field_0x2caef;
    undefined field_0x2caf0;
    undefined field_0x2caf1;
    undefined field_0x2caf2;
    undefined field_0x2caf3;
    // 0x2CAF4: One more than the maximum number of floors in the current dungeon
    uint8_t n_floors_plus_one;
    undefined field_0x2caf5;
    undefined field_0x2caf6;
    undefined field_0x2caf7;
    undefined field_0x2caf8;
    undefined field_0x2caf9;
    undefined field_0x2cafa;
    undefined field_0x2cafb;
    // Related to playing dungeon music?
    undefined field_0x2cafc;
    undefined field_0x2cafd;
    undefined field_0x2cafe;
    undefined field_0x2caff;
    undefined field_0x2cb00;
    undefined field_0x2cb01;
    // Related to playing dungeon music?
    undefined2 field_0x2cb02;
    // 0x2CB04: Related to playing dungeon music?
    undefined2 field_0x2cb04;
    // 0x2CB06: Current Dunegon Music ID
    struct music_id_16 dungeon_music_playing_id;
    undefined field_0x2cb08;
    undefined field_0x2cb09;
    undefined field_0x2cb0a;
    undefined field_0x2cb0b;
    undefined field_0x2cb0c;
    undefined field_0x2cb0d;
    undefined field_0x2cb0e;
    undefined field_0x2cb0f;
    undefined field_0x2cb10;
    undefined field_0x2cb11;
    undefined field_0x2cb12;
    undefined field_0x2cb13;
};
ASSERT_SIZE(struct dungeon, 183060);

#endif
