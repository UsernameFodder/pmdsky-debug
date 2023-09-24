// Types related to the window system. A "window" is a rectangular UI element containing content
// on top of an optional backdrop, surrounded by an optional frame. For example, a window can
// contain text, scrolling character dialogue, portraits, menu text, etc.

#ifndef HEADERS_TYPES_WINDOW_H_
#define HEADERS_TYPES_WINDOW_H_

#include "enums.h"

// These flags are shared with the function to display text inside message boxes
// So they might need a rename once more information is found
struct preprocessor_flags {
    uint16_t flags_0 : 13;
    bool show_speaker : 1;
    uint16_t flags_14 : 2;
};
ASSERT_SIZE(struct preprocessor_flags, 2);

// Represents arguments that might be passed to the PreprocessString function
struct preprocessor_args {
    uint32_t flag_vals[4];  // 0x0: These are usually IDs with additional flags attached to them
    uint32_t id_vals[5];    // 0x10
    int32_t number_vals[5]; // 0x24
    char* strings[5];       // 0x38
    // 0x4C: An optional argument that is used to insert the name of a Pokémon
    // When they're talking through a message box. It requires it's respective flag to be on
    uint32_t speaker_id;
};
ASSERT_SIZE(struct preprocessor_args, 80);

struct window;
typedef void (*update_window_fn_t)(struct window* window);

struct window_params {
    update_window_fn_t update; // 0x0: A function to update the window
    uint8_t x_offset;          // 0x4: Window top left corner x coordinate, divided by 8.
    uint8_t y_offset;          // 0x5: Window top left corner y coordinate, divided by 8.
    uint8_t width;             // 0x6: Window width, divided by 8.
    uint8_t height;            // 0x7: Window height, divided by 8.
    struct screen_8 screen;    // 0x8: Screen where the window resides
    // 0x9: The box type can affect the window frame and/or the backdrop. For example, 0xFA makes
    // both the frame and backdrop invisible
    uint8_t box_type;
    undefined field_0xa;
    undefined field_0xb;
    // 0xC: heap-allocated contents for the window, varying by type.
    // See callers of NewWindowScreenCheck for the different types
    void* contents;
};
ASSERT_SIZE(struct window_params, 16);

// Placeholder name; not sure what this struct is
struct window_trailer {
    int field_0x0;
    uint8_t field_0x4;
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
    undefined field_0x13;
    undefined field_0x14;
    undefined field_0x15;
    undefined field_0x16;
    undefined field_0x17;
    uint8_t field_0x18;
    uint8_t field_0x19;
    // Not actually sure where this struct ends; these might just be part of the parent window
    undefined field_0x1a;
    undefined field_0x1b;
    undefined field_0x1c;
    undefined field_0x1d;
    undefined field_0x1e;
    undefined field_0x1f;
    undefined field_0x20;
    undefined field_0x21;
    undefined field_0x22;
    undefined field_0x23;
};
ASSERT_SIZE(struct window_trailer, 36);

// Generic structure for a window containing content
struct window {
    struct window_params params; // 0x0
    int8_t field_0x10;
    uint8_t field_0x11;
    uint16_t field_0x12;
    // Some heap-allocated struct pointer with size (hdr.width * hdr.height * 0x40)
    undefined* field_0x14;
    int field_0x18;
    int field_0x1c; // hdr.width * hdr.height * 0x40
    uint32_t field_0x20;
    undefined field_0x24;
    undefined field_0x25;
    undefined field_0x26;
    undefined field_0x27;
    int field_0x28;
    undefined field_0x2c;
    undefined field_0x2d;
    undefined field_0x2e;
    undefined field_0x2f;
    uint16_t field_0x30;
    undefined field_0x32;
    undefined field_0x33;
    struct render_3d_element_64 backdrop; // 0x34: type RENDER64_RECTANGLE
    struct render_3d_element_64 frame;    // 0x74: type RENDER64_WINDOW_FRAME
    uint8_t field_0xb4;
    undefined field_0xb5;
    int8_t valid; // 0xB6: usually seems to be 0-3
    uint8_t field_0xb7;
    int32_t field_0xb8;
    struct window_trailer trailer; // 0xBC
};
ASSERT_SIZE(struct window, 224);

struct window_list {
    struct window windows[20];
};
ASSERT_SIZE(struct window_list, 4480);

// Also see CreateScrollBox1/CreateScrollBox2
// Up/down paged text boxes with formatting?
// Observed examples:
// - item info description box
// - move info description box
// - IQ skill info description box
struct scroll_box {
    undefined fields_0x0[7532];
};
ASSERT_SIZE(struct scroll_box, 7532);

struct dialogue_display_unk_struct {
    uint8_t field_0x0;
    undefined field_0x1;
    undefined field_0x2;
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
    undefined field_0x13;
    undefined field_0x14;
    undefined field_0x15;
    undefined field_0x16;
    undefined field_0x17;
};
ASSERT_SIZE(struct dialogue_display_unk_struct, 24);

typedef void (*unk_dialogue_display_fn_t)(undefined* param_1, uint8_t param_2);

struct dialogue_display_state {
    // possibly another function pointer, for one example, observed to point to UpdateDialogueBox
    int field_0x0;
    int field_0x4;
    undefined fields_0x8[40];
    unk_dialogue_display_fn_t field_0x30; // for one example, observed to point to [NA]0x2026020
    undefined fields_0x34[32];
    struct dialogue_display_ctx* parent_ctx; // seems to point to the parent context
    undefined field_0x58;
    undefined field_0x59;
    undefined field_0x5a;
    undefined field_0x5b;
    int field_0x5c;
    struct preprocessor_flags flags; // 0x60
    bool field_0x62;
    undefined field_0x63;
    int text_speed; // 0x64: 1 normally, or 4 if DEBUG_FLAG_TEXT_SPEEDUP is set
    int field_0x68;
    int field_0x6c;
    undefined field_0x70;
    undefined field_0x71;
    undefined field_0x72;
    undefined field_0x73;
    undefined field_0x74;
    undefined field_0x75;
    undefined field_0x76;
    undefined field_0x77;
    uint32_t text_scroll_anim_frame;     // 0x78: width uncertain
    uint32_t page_transition_anim_frame; // 0x7C: width uncertain
    int text_scrolling_done;             // 0x80: for the current page
    char* string;                        // 0x84: points to &dialogue_box::string_data[0]
    // 0x88: points to &dialogue_box::string_data[i] for the next character to be displayed
    char* cursor;
    uint16_t field_0x8c;
    undefined field_0x8e;
    undefined field_0x8f;
    int field_0x90;
    int field_0x94;
    uint8_t field_0x98;
    uint8_t field_0x99[4];
    uint8_t field_0x9d;
    undefined field_0x9e;
    undefined field_0x9f;
    int field_0xa0;
    int field_0xa4;
    uint8_t field_0xa8;
    undefined field_0xa9;
    undefined field_0xaa;
    undefined field_0xab;
    struct dialogue_display_unk_struct field_0xac[4];
    // size is uncertain, not sure when this struct ends
    int field_0x10c;
    undefined field_0x110;
    undefined field_0x111;
    undefined field_0x112;
    undefined field_0x113;
    undefined field_0x114;
    undefined field_0x115;
    undefined field_0x116;
    undefined field_0x117;
};
ASSERT_SIZE(struct dialogue_display_state, 280);

struct dialogue_display_ctx {
    undefined field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    struct dialogue_display_state display;
};
ASSERT_SIZE(struct dialogue_display_ctx, 284);

// Also see CreateDialogueBox
// Scrolling message boxes, like those used for dialogue
struct dialogue_box {
    undefined field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    enum dialogue_box_state state; // 0x4
    bool active;                   // 0x8
    uint8_t field_0x9;
    uint8_t field_0xa;
    undefined field_0xb;
    struct dialogue_display_ctx display_ctx; // 0xC
    char string_data[1024];                  // 0x128
};
ASSERT_SIZE(struct dialogue_box, 1320);

// Represents the state of a portrait to be displayed inside a window
struct portrait_params {
    struct monster_id_16 monster_id; // 0x0: The species id, or the set index inside kaomado.kao
    // 0x2: Index of the emote in the species set of portraits
    struct portrait_emotion_8 portrait_emotion;
    uint8_t layout_idx; // 0x3: Represents the current layout to display the portrait
    uint32_t offset_x;  // 0x4: Tile offset (x / 8) in the X axis to draw the portrait
    uint32_t offset_y;  // 0x8: Tile offset (y / 8) in the Y axis to draw the portrait
    bool try_flip;      // 0xC: Whether to try to get a flipped portrait from kaomado.kao
    bool has_flip;      // 0xD: Whether the specified emote has a flipped variant
    bool hw_flip;       // 0xE: Whether the portrait should be flipped using the hardware
    bool allow_default; // 0xF: If true, try using emote index 0 if the desired emote can't be found
};
ASSERT_SIZE(struct portrait_params, 16);

// Identifies a default position for a portrait, as well as whether it'll be flipped
struct portrait_layout {
    int16_t offset_x;
    int16_t offset_y;
    bool try_flip;
    uint8_t _padding;
};
ASSERT_SIZE(struct portrait_layout, 6);

// Holds portrait image data loaded from kaomado.kao.
// See https://projectpokemon.org/home/docs/mystery-dungeon-nds/kaomadokao-file-format-r54/
struct kaomado_buffer {
    struct rgb palette[16];    // Buffer to load the palette of the portrait
    uint8_t at4px_buffer[800]; // Buffer to load the portrait image data
};
ASSERT_SIZE(struct kaomado_buffer, 848);

// Also see CreatePortraitBox
// Stores data and state for a specialized Kaomado canvas for rendering portraits
struct portrait_box {
    uint8_t window_id; // 0x0
    uint8_t _padding_0x1;
    uint8_t _padding_0x2;
    uint8_t _padding_0x3;
    enum portrait_box_state state; // 0x4
    // The buffer_state is the one that receives and stores any commits,
    // but render_state is only set to the value of buffer_state during
    // the Kaomado canvas update function
    struct portrait_params render_state; // 0x8
    struct portrait_params buffer_state; // 0x18
    bool updated;                        // 0x28
    bool hide;                           // 0x29
    bool framed;                         // 0x2A
    uint8_t _padding_0x2b;
    uint32_t palette_idx;         // 0x2C
    struct kaomado_buffer buffer; // 0x2D
};
ASSERT_SIZE(struct portrait_box, 896);

// Also see CreateTextBoxInternal
// Static text boxes
// Observed examples:
// - "Team stats" top screen in ground mode
// - "Job" top screen in ground mode
// - "Map and team" top screen in dungeon mode
// - Team info box on the top-level menu in dungeon mode
// - Password display and password keyboard input display in the main menu
struct text_box {
    int8_t window_id; // 0x0
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    undefined4 field_0x4;
    undefined4 field_0x8;
    undefined field_0xc;
    undefined field_0xd;
    undefined field_0xe;
    undefined field_0xf;
    undefined field_0x10;
    undefined field_0x11;
    undefined field_0x12;
    undefined field_0x13;
    undefined4 field_0x14;
};
ASSERT_SIZE(struct text_box, 24);

// Also see CreateDynamicTextBox
// Text box with dynamically populated content?
// This is the same size as struct simple_menu...maybe they're the same struct?
// But it seems like they probably wouldn't share functionality, so they're probably different.
// Observed examples:
// - Area name box in the top-level ground mode menu
struct dynamic_text_box {
    undefined fields_0x0[436];
};
ASSERT_SIZE(struct dynamic_text_box, 436);

// Also see CreateControlsChart
// Used for the "Controls" top screen in ground mode, the main menu, and the Sky Jukebox
struct controls_chart {
    undefined fields_0x0[168];
};
ASSERT_SIZE(struct controls_chart, 168);

// Also see CreateAlertBox
// Used for popup log messages in dungeon mode that disappear after some time
struct alert_box {
    int8_t window_id; // 0x0
    undefined fields_0x0[1615];
};
ASSERT_SIZE(struct alert_box, 1616);

// Also see CreateAdvancedTextBoxInternal
// Pretty marked-up displays with segmented layouts?
// Observed examples:
// - "Text log" top screen
// - Item display when giving an item to a team member
// - Team member summary box
// - Move selection menu
// - Message log window in dungeon mode
// - Mission objectives window in dungeon mode
// - Recruitment search window in dungeon mode
// - Friend code display and numeric input window in the main menu
struct advanced_text_box {
    undefined fields_0x0[416];
    int8_t window_id; // 0x1A0
    undefined field_0x1a1;
    undefined field_0x1a2;
    undefined field_0x1a3;
    undefined4 field_0x1a4;
    undefined4 field_0x1a8;
    undefined4 field_0x1ac;
    undefined fields_0x1b0[24];
};
ASSERT_SIZE(struct advanced_text_box, 456);

// Also see CreateInputLockBox
// Used for the Sky Jukebox's "Locked" text window when the screen is locked
struct input_lock_box {
    undefined fields_0x0[688];
};
ASSERT_SIZE(struct input_lock_box, 688);

// Also see CreateTeamInfoBox
// Specialized team info box?
// Observed examples:
// - Team info box (with team rank and money) for the top-level ground mode menu
struct team_info_box {
    undefined fields_0x0[888];
};
ASSERT_SIZE(struct team_info_box, 888);

// These flags are used to modify menus by adding (or taking away) elements.
struct menu_flags {
    bool a_accept : 1;
    bool b_cancel : 1; // Also adds screen button.
    bool accept_button : 1;
    bool up_down_buttons : 1;
    bool se_on : 1;
    bool set_choice : 1;
    uint32_t unknown6 : 3;
    bool unknown9 : 1; // Breaks the first choice?
    bool custom_height : 1;
    bool menu_title : 1;
    bool menu_lower_bar : 1;
    bool list_button : 1;
    bool search_button : 1;
    bool unknown_15 : 1; // Set to true when initalizing an advanced and simple menu?
    bool first_last_page_buttons : 1;
    bool up_down : 1;
    uint32_t unknown18 : 2;
    bool y_pos_end : 1;
    bool x_pos_end : 1;
    bool partial_menu : 1;
    bool no_cursor : 1;
    bool no_up_down : 1;
    bool no_left_right : 1;
    bool invisible_cursor : 1;
    bool only_list : 1;
    bool no_accept_button : 1;
    uint32_t unknown29 : 3;
};
ASSERT_SIZE(struct menu_flags, 4);

// Used in an array for all the options for a simple menu.
struct simple_menu_option {
    // 0x0: Message ID
    uint16_t option_msg_id;
    undefined field_0x2;
    undefined field_0x3;
    // 0x4: The result value for selecting this option?
    uint32_t option_result_value;
};
ASSERT_SIZE(struct simple_menu_option, 8);

// This struct is used when a menu needs additional information like the title
// and custom height.
struct menu_additional_info {
    // 0x0: The choice to have selected initally when menu_flags::set_choice is true.
    uint32_t set_choice_id;
    // 0x4: Custom height value?
    uint32_t custom_height;
    // 0x8: Menu title string id
    uint16_t title_message_id;
    undefined field_0xA;
    undefined field_0xB;
    // 0xC: Height of the title?
    uint32_t title_height;
    // 0x10: Preprocess arguments for the title.
    struct preprocessor_args* title_args;
    undefined field_0x14;
    undefined field_0x15;
    undefined field_0x16;
    undefined field_0x17;
    undefined field_0x18;
    undefined field_0x19;
    undefined field_0x1A;
    undefined field_0x1B;
    undefined field_0x1C;
    undefined field_0x1D;
    undefined field_0x1E;
    undefined field_0x1F;
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
    undefined field_0x2A;
    undefined field_0x2B;
    undefined field_0x2C;
    undefined field_0x2D;
    undefined field_0x2E;
    undefined field_0x2F;
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
    undefined field_0x3A;
    undefined field_0x3B;
    undefined field_0x3C;
    undefined field_0x3D;
    undefined field_0x3E;
    undefined field_0x3F;
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
    undefined field_0x4A;
    undefined field_0x4B;
    undefined field_0x4C;
    undefined field_0x4D;
    undefined field_0x4E;
    undefined field_0x4F;
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
    undefined field_0x5A;
    undefined field_0x5B;
    undefined field_0x5C;
    undefined field_0x5D;
    undefined field_0x5E;
    undefined field_0x5F;
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
    undefined field_0x6A;
    undefined field_0x6B;
    undefined field_0x6C;
    undefined field_0x6D;
    undefined field_0x6E;
    undefined field_0x6F;
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
    undefined field_0x7A;
    undefined field_0x7B;
    undefined field_0x7C;
    undefined field_0x7D;
    undefined field_0x7E;
    undefined field_0x7F;
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
    undefined field_0x8A;
    undefined field_0x8B;
    undefined field_0x8C;
    undefined field_0x8D;
    undefined field_0x8E;
    undefined field_0x8F;
    undefined field_0x90;
    undefined field_0x91;
    undefined field_0x92;
    undefined field_0x93;
    undefined field_0x94;
    undefined field_0x95;
    undefined field_0x96;
    undefined field_0x97;
};
ASSERT_SIZE(struct menu_additional_info, 152);

// Also see CreateSimpleMenu, CreateParentMenu, CreateTeamSelectionMenu, CreatePlaybackControlsMenu
// Menu with a simple list of options, that can possibly contain submenus?
// Note that the associated contents structures for CreateSimpleMenu, CreateParentMenu,
// CreateTeamSelectionMenu, and CreatePlaybackControlsMenu all have the same size. Seeing as the
// three are similar in functionality, the structure is probably shared? This is a guess, however.
// Observed examples of simple menus:
// - Yes/No menus
// - Duskull Bank action menu
//
// Observed examples of parent menus:
// - selection box for the top-level ground mode menu with [Items, Team, Job List, Others, Exit]
// - item action sub-menu from treasure bag with [Give, Trash, Info, Exit]
// - team member selection menu when giving an item
// - team member selection menu when modifying team
// - team member action sub-menu when modifying team member with [Summary, Moves, Check IQ, Exit]
// - move selection menu
// - tactics selection menu
//
// Observed examples of team selection menus:
// - Electivire Link Shop team member selection menu
// - Chimecho Assembly team member selection menu
// - Croagunk Swap Shop team member selection menu
// - Luminous Spring team member selection menu
//
// Observed examples of playback controls menu:
// - Sky Jukebox track playback controls window
struct simple_menu {
    undefined fields_0x0[436];
};
ASSERT_SIZE(struct simple_menu, 436);

// Functions for advanced menus to get the strings for the current list of options.
// Returns the input buffer.
typedef char* (*advanced_menu_entry_fn_t)(char* buffer, int option_id);

// Also see CreateAdvancedMenu
// Menu with complex layout and functionality, like paging.
// Observed examples:
// - IQ skills menu
// - Dungeon select menu
// - Sentry duty high score window
// - Adventure Log window in the main menu
struct advanced_menu {
    undefined fields_0x0[432];
};
ASSERT_SIZE(struct advanced_menu, 432);

// Also see CreateCollectionMenu
// Like advanced menus, but specifically for mutating a collection of objects
// Observed examples:
// - Job selection menu
// - Job bulletin board menu
// - Kecleon Shop item selection menu
// - Kangaskhan Storage item selection menu
// - Croagunk Swap Shop item selection menu
struct collection_menu {
    undefined fields_0x0[460];
};
ASSERT_SIZE(struct collection_menu, 460);

// Also see CreateOptionsMenu
// Menu for game options?
// Observed examples:
// - Options menu
// - Window Options menu
struct options_menu {
    undefined fields_0x0[440];
};
ASSERT_SIZE(struct options_menu, 440);

// Also see CreateDebugMenu
// Probably the special debug menu, as the code references string IDs 15461/15462, which are from
// the debug menu
struct debug_menu {
    undefined fields_0x0[948];
};
ASSERT_SIZE(struct debug_menu, 948);

// Also see CreateJukeboxTrackMenu
// Sky Jukebox track selection menu.
struct jukebox_track_menu {
    undefined fields_0x0[428];
};
ASSERT_SIZE(struct jukebox_track_menu, 428);

// Also see CreateInventoryMenu
// Menu for managing items?
// Observed examples:
// - Treasure bag menu item selection window
// - Kecleon Shop item info/price window in dungeons
struct inventory_menu {
    undefined fields_0x0[420];
};
ASSERT_SIZE(struct inventory_menu, 420);

#endif
