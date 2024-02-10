// Types related to the window system. A "window" is a rectangular UI element containing content
// on top of an optional backdrop, surrounded by an optional frame. For example, a window can
// contain text, scrolling character dialogue, portraits, menu text, etc.

#ifndef HEADERS_TYPES_COMMON_WINDOW_H_
#define HEADERS_TYPES_COMMON_WINDOW_H_

#include "enums.h"

// These flags are shared with the function to display text inside windows
// So they might need a rename once more information is found
struct preprocessor_flags {
    bool timer_1 : 1; // Enabled when creating the play time string
    uint16_t flags_1 : 9;
    bool timer_2 : 1; // Enabled when creating the play time string
    uint16_t flags_11 : 2;
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
    // When they're talking through a window. It requires its respective flag to be on
    uint32_t speaker_id;
};
ASSERT_SIZE(struct preprocessor_args, 80);

struct window;
typedef void (*update_window_fn_t)(struct window* window);

struct window_params {
    update_window_fn_t update; // 0x0: A function to update the window
    // 0x4: Window left (or right in tandem with the x_offset_end window flag) edge x coordinate,
    // divided by 8.
    uint8_t x_offset;
    // 0x5: Window top (or bottom in tandem with the y_offset_end window flag) edge y coordinate,
    // divided by 8.
    uint8_t y_offset;
    uint8_t width;          // 0x6: Window width, divided by 8.
    uint8_t height;         // 0x7: Window height, divided by 8.
    struct screen_8 screen; // 0x8: Screen where the window resides
    // 0x9: The box type can affect the window frame and/or the backdrop.
    struct box_type_8 box_type;
    undefined field_0xa;
    undefined field_0xb;
    // 0xC: heap-allocated contents for the window, varying by type.
    // See callers of NewWindowScreenCheck for the different types
    void* contents;
};
ASSERT_SIZE(struct window_params, 16);

// Corner coordinates of the rectangle a window occupies.
struct window_rectangle {
    int y_min; // 0x0: Window top y coordinate
    int y_max; // 0x4: Window bottom y coordinate
    int x_min; // 0x8: Window left x coordinate
    int x_max; // 0xC: Window right x coordinate
};
ASSERT_SIZE(struct window_rectangle, 16);

// These flags are used to modify windows by adding (or taking away) elements.
struct window_flags {
    bool a_accept : 1;        // 0 (0x1)
    bool b_cancel : 1;        // 1 (0x2): Also adds screen button.
    bool accept_button : 1;   // 2 (0x4)
    bool up_down_buttons : 1; // 3 (0x8)
    bool se_on : 1;           // 4 (0x10)
    bool set_choice : 1;      // 5 (0x20)
    uint32_t unknown6 : 3;    // 6-8 (0x40-0x100)
    bool disable_options : 1; // 9 (0x200): If true, see window_extra_info::option_states
    bool custom_height : 1;   // 10 (0x400)
    bool menu_title : 1;      // 11 (0x800)
    bool menu_lower_bar : 1;  // 12 (0x1000)
    bool list_button : 1;     // 13 (0x2000)
    bool search_button : 1;   // 14 (0x4000)
    bool unknown_15 : 1; // 15 (0x8000): Set to true when initalizing an advanced and simple menu?
    bool first_last_page_buttons : 1; // 16 (0x10000)
    bool up_down : 1;                 // 17 (0x20000)
    uint32_t unknown18 : 2;           // 18-19 (0x40000-0x80000)
    bool y_offset_end : 1;  // 20 (0x100000): If set, window_params::y_offset sets the bottom edge
    bool x_offset_end : 1;  // 21 (0x200000): If set, window_params::x_offset sets the right edge
    bool partial_menu : 1;  // 22 (0x400000): If set, menu will not close upon selecting an option
    bool no_cursor : 1;     // 23 (0x800000)
    bool no_up_down : 1;    // 24 (0x1000000)
    bool no_left_right : 1; // 25 (0x2000000)
    bool invisible_cursor : 1; // 26 (0x4000000)
    bool only_list : 1;        // 27 (0x8000000)
    bool no_accept_button : 1; // 28 (0x10000000)
    uint32_t unknown29 : 3;    // 29-31 (0x20000000-0x80000000)
};
ASSERT_SIZE(struct window_flags, 4);

// This struct is used when a menu needs extra information like the title
// and custom height. It also seems to be used for other non-menu window types,
// so it's probably generic?
struct window_extra_info {
    // 0x0: The choice to have selected initally when window_flags::set_choice is true.
    uint32_t set_choice_id;
    // 0x4: Custom height value for a single option
    uint32_t custom_height;
    // 0x8: Menu title string id
    uint16_t title_string_id;
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
    // 0x60: A list of states per menu option. Enabled is 0x0 and Disabled is 0x3.
    uint8_t* option_states;
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
ASSERT_SIZE(struct window_extra_info, 152);

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
    // 0x10: it seems like some windows, such as the scroll box, can have an associated sub-window
    // within the main window
    int8_t subwindow_id;
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

// Appears to be used to track state related to interactive windows?
// Probably used for various things?
struct window_input_ctx {
    undefined fields_0x0[244];
    uint8_t field_0xf4;
    undefined field_0xf5;
    undefined field_0xf6;
    undefined field_0xf7;
};
ASSERT_SIZE(struct window_input_ctx, 248);

struct unk_scroll_box_struct {
    undefined fields_0x0[1036];
};
ASSERT_SIZE(struct unk_scroll_box_struct, 1036);

struct unk_scroll_box_coords {
    undefined field_0x0;
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    undefined field_0x4;
    undefined field_0x5;
    undefined field_0x6;
    undefined field_0x7;
    // These are added to x_min and y_min from a window_rectangle
    int dx1; // 0x8
    int dy1; // 0xC
    int dx2; // 0x10
    int dy2; // 0x14
};
ASSERT_SIZE(struct unk_scroll_box_coords, 24);

// Also see CreateScrollBoxSingle/CreateScrollBoxMulti
// Up/down paged text boxes with formatting?
// Observed examples:
// - item info description box
// - move info description box
// - IQ skill info description box
struct scroll_box {
    update_window_fn_t update;           // 0x0
    struct window_input_ctx input_ctx;   // 0x4
    struct window_flags flags;           // 0xFC
    struct window_extra_info extra_info; // 0x100
    int n_strings; // 0x198: number of strings in each of string_ids1 and string_ids2
    int page;      // 0x19C: current page
    // 0x1A0: probably string IDs since they're coupled to preprocessor_args1
    uint16_t string_ids1[4];
    struct preprocessor_args preprocessor_args1[4]; // 0x1A8
    // 0x2E8: probably string IDs since they're coupled to preprocessor_args2
    uint16_t string_ids2[4];
    struct preprocessor_args preprocessor_args2[4]; // 0x2F0
    int unk_struct_arr_len;
    // 0x434: length is a guess, but the size fits perfectly, and unk_struct_arr_len is checked for
    // < 3 implying it can be at least 3. 4 Also matches the array fields above
    struct unk_scroll_box_struct unk_struct_arr[4];
    int state; // 0x1464: appears to be a state value
    uint8_t field_0x1468;
    undefined field_0x1469;
    undefined field_0x146a;
    undefined field_0x146b;
    struct preprocessor_args preprocessor_args3; // 0x146C
    int field_0x14bc;
    undefined4 field_0x14c0;
    char* strings1[8];  // 0x14C4: length is a guess, but copied into strings2 so 8 makes sense
    char* strings2[8];  // 0x14E4: length is a guess, but copied from strings1 so 8 makes sense
    int unk_coords_len; // 0x1504
    // 0x1508: array of some coordinate-related structs. Length is a guess, but it's coupled to
    // field_0x1568 so 4 makes sense
    struct unk_scroll_box_coords unk_coords[4];
    uint8_t field_0x1568[4];
    char string_buffer1[1024]; // 0x156C
    char string_buffer2[1024]; // 0x196C
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
    update_window_fn_t update;     // 0x0
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

typedef void (*text_box_callback_fn_t)(int window_id);
typedef void (*text_box_callback_with_arg_fn_t)(int window_id, void* arg);

// Also see CreateTextBox/CreateTextBoxWithArg
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
    // One of the callback functions seems to get invoked at the end of every update, if at least
    // one is non-null? callback_with_arg takes precedence over callback if non-null
    text_box_callback_fn_t callback;                   // 0x4
    text_box_callback_with_arg_fn_t callback_with_arg; // 0x8
    update_window_fn_t update;                         // 0xC
    int state;                                         // 0x10: appears to be a state value
    void* callback_arg; // 0x14: second argument for callback_with_arg
};
ASSERT_SIZE(struct text_box, 24);

// Also see CreateAreaNameBox
// Text box containing the current area name (from the "[area:0]" tag)
// Observed examples:
// - Area name box in the top-level ground mode menu
// - Area name box in the world map transition screen before a dungeon
struct area_name_box {
    update_window_fn_t update;           // 0x0
    struct window_flags flags;           // 0x4
    struct window_extra_info extra_info; // 0x8
    int state;                           // 0xA0: appears to be a state value
    uint32_t id;                         // 0xA4: ID for preprocessor_args
    // 0xA8: Some number computed from the contents of string_buffer, likely the pixel width of the
    // rendered text in string_buffer based on how it's used
    int text_width;
    char string_buffer[256]; // 0xAC: string buffer for PreprocessString
    // 0x1AC: seems to be the amount of vertical padding necessary to vertically center the rendered
    // text? Computed as (window_params::height * 8 - 12) / 2
    int y_padding;
    // 0x1B0: seems to be the amount of horizontal padding necessary to horizontally center the
    // rendered text? Computed as (window_params::width * 8 - text_width) / 2
    int x_padding;
};
ASSERT_SIZE(struct area_name_box, 436);

// Also see CreateControlsChart
// Used for the "Controls" top screen in ground mode, the main menu, and the Sky Jukebox
struct controls_chart {
    update_window_fn_t update;           // 0x0
    struct window_flags flags;           // 0x4: seems unused?
    struct window_extra_info extra_info; // 0x8
    int state;                           // 0xA0: appears to be a state value
    uint16_t string_id;                  // 0xA4
    undefined field_0xa6;
    undefined field_0xa7;
};
ASSERT_SIZE(struct controls_chart, 168);

struct unk_alert_box_struct {
    undefined fields_0x0[328];
};
ASSERT_SIZE(struct unk_alert_box_struct, 328);

// Also see CreateAlertBox
// Used for popup log messages in dungeon mode that disappear after some time
struct alert_box {
    int8_t window_id; // 0x0
    undefined field_0x1;
    undefined field_0x2;
    undefined field_0x3;
    int state;                                  // 0x4: appears to be a state value
    struct dialogue_display_ctx display_ctx;    // 0x8
    struct unk_alert_box_struct field_0x124[4]; // length is a guess, but the size fits nicely
    int16_t field_0x644;
    int16_t idx1; // 0x646: some index into field_0x124
    int16_t idx2; // 0x648: some index into field_0x124
    int16_t field_0x64a;
    int16_t field_0x64c;
    undefined field_0x64e;
    undefined field_0x64f;
};
ASSERT_SIZE(struct alert_box, 1616);

typedef void (*unk_advanced_text_box_fn_t)(undefined* param_1, undefined* param_2);
typedef void (*unk_advanced_text_box_window_fn_t)(int window_id, int param_2);

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
    update_window_fn_t update;         // 0x0
    struct window_input_ctx input_ctx; // 0x4
    int16_t field_0xfc;
    int16_t field_0xfe;
    int16_t field_0x100;
    int16_t field_0x102;
    struct window_flags flags;           // 0x104
    struct window_extra_info extra_info; // 0x108
    int8_t window_id;                    // 0x1A0
    undefined field_0x1a1;
    undefined field_0x1a2;
    undefined field_0x1a3;
    // One of the callback functions seems to get invoked at the end of every update, if at least
    // one is non-null? callback_with_arg takes precedence over callback if non-null
    text_box_callback_fn_t callback;                   // 0x1A4
    text_box_callback_with_arg_fn_t callback_with_arg; // 0x1A8
    void* callback_arg; // 0x1AC: second argument for callback_with_arg
    uint32_t flags2;    // 0x1B0: bitflags of some sort
    unk_advanced_text_box_fn_t field_0x1b4;
    unk_advanced_text_box_window_fn_t field_0x1b8;
    int state; // 0x1BC: appears to be a state value
    uint8_t field_0x1c0;
    uint8_t field_0x1c1;
    uint8_t field_0x1c2;
    bool partial_menu; // 0x1C3: seems coupled to flags::partial_menu
    uint8_t field_0x1c4;
    undefined field_0x1c5;
    undefined field_0x1c6;
    undefined field_0x1c7;
};
ASSERT_SIZE(struct advanced_text_box, 456);

// Also see CreateInputLockBox
// Used for the Sky Jukebox's "Locked" text window when the screen is locked
struct input_lock_box {
    update_window_fn_t update; // 0x0
    undefined4 field_0x4;
    struct window_input_ctx input_ctx; // 0x8
    undefined4 field_0x100;
    struct window_extra_info extra_info; // 0x104
    int state;                           // 0x19C: appears to be a state value
    uint16_t string_id;                  // 0x1A0
    undefined field_0x1a2;
    undefined field_0x1a3;
    // 0x1A4: Some number computed from the contents of string_buffer, likely the pixel width of the
    // rendered text in string_buffer based on how it's used
    int text_width;
    char string_buffer[256]; // 0x1A8: string buffer for PreprocessStringFromId
    // 0x2A8: seems to be the amount of vertical padding necessary to vertically center the rendered
    // text? Computed as (window_params::height * 8 - 12) / 2
    int y_padding;
    // 0x2AC: seems to be the amount of horizontal padding necessary to horizontally center the
    // rendered text? Computed as (window_params::width * 8 - text_width) / 2
    int x_padding;
};
ASSERT_SIZE(struct input_lock_box, 688);

// Also see CreateTeamInfoBox
// Specialized team info box?
// Observed examples:
// - Team info box (with team rank and money) for the top-level ground mode menu
struct team_info_box {
    update_window_fn_t update; // 0x0
    int8_t field_0x4;
    undefined field_0x5;
    undefined field_0x6;
    undefined field_0x7;
    struct portrait_params portrait; // 0x8
    int field_0x18;
    undefined fields_0x1c[852];
    int state; // 0x370: appears to be a state value
    int field_0x374;
};
ASSERT_SIZE(struct team_info_box, 888);

struct simple_menu_id_item {
    uint16_t string_id; // 0x0: string ID
    uint16_t _padding;
    int result_value; // 0x4: result value for selecting this item?
};
ASSERT_SIZE(struct simple_menu_id_item, 8);

// Used in an array for all the items for a simple menu.
struct simple_menu_item {
    char string[256]; // 0x0: menu item text
    int result_value; // 0x100: result value for selecting this item?
};
ASSERT_SIZE(struct simple_menu_item, 260);

struct simple_menu_ptr_item {
    char* string;     // 0x0: menu item text
    int result_value; // 0x100: result value for selecting this item?
};
ASSERT_SIZE(struct simple_menu_ptr_item, 8);

// Also see CreateSimpleMenuInternal and CreateParentMenuInternal
// Menu with a simple list of options, that can possibly contain submenus? Note that the associated
// contents structures for CreateSimpleMenuInternal and CreateParentMenuInternal have the same size,
// and seem to share the same layout. Seeing as the two are similar in functionality, the structure
// is probably shared? This isn't fully certain, however.
//
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
struct simple_menu {
    update_window_fn_t update;           // 0x0
    struct window_input_ctx input_ctx;   // 0x4
    struct window_flags flags;           // 0xFC
    struct window_extra_info extra_info; // 0x100
    // 0x198: array of items, terminated by an entry with an empty string
    struct simple_menu_item* items;
    int state; // 0x19C: appears to be a state value
    uint8_t field_0x1a0;
    uint8_t field_0x1a1;
    undefined field_0x1a2;
    undefined field_0x1a3;
    int field_0x1a4;
    undefined field_0x1a8;
    undefined field_0x1a9;
    undefined field_0x1aa;
    undefined field_0x1ab;
    int field_0x1ac;
    int field_0x1b0;
};
ASSERT_SIZE(struct simple_menu, 436);

// Seems to retrieve the menu item text (member name) for a given team member. Returns a pointer
// into the input buffer. Usually points to TeamSelectionMenuGetItem
typedef char* (*team_selection_menu_get_item_fn_t)(char* buffer, int member_idx);

// Also see CreateTeamSelectionMenu
// Seems similar in functionality to a simple menu. The struct is even the same size. However,
// the layout is very slightly different.
//
// Observed examples of team selection menus:
// - Electivire Link Shop team member selection menu
// - Chimecho Assembly team member selection menu
// - Croagunk Swap Shop team member selection menu
// - Luminous Spring team member selection menu
struct team_selection_menu {
    update_window_fn_t update;                  // 0x0
    struct window_input_ctx input_ctx;          // 0x4
    struct window_flags flags;                  // 0xFC
    struct window_extra_info extra_info;        // 0x100
    team_selection_menu_get_item_fn_t get_item; // 0x198: gets the item text for a given team member
    int state;                                  // 0x19C: appears to be a state value
    uint8_t field_0x1a0;
    uint8_t field_0x1a1;
    undefined field_0x1a2;
    undefined field_0x1a3;
    int field_0x1a4;
    int field_0x1a8;
    int field_0x1ac;
    uint8_t field_0x1b0;
    undefined field_0x1b1;
    undefined field_0x1b2;
    undefined field_0x1b3;
};
ASSERT_SIZE(struct team_selection_menu, 436);

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
    update_window_fn_t update;         // 0x0
    struct window_input_ctx input_ctx; // 0x4
    int field_0xfc;
    struct window_extra_info extra_info; // 0x100
    undefined field_0x198;
    undefined field_0x199;
    undefined field_0x19a;
    undefined field_0x19b;
    int state; // 0x19C: appears to be a state value
    uint8_t field_0x1a0;
    uint8_t field_0x1a1;
    undefined field_0x1a2;
    undefined field_0x1a3;
    int field_0x1a4;
    undefined field_0x1a8;
    undefined field_0x1a9;
    undefined field_0x1aa;
    undefined field_0x1ab;
    undefined field_0x1ac;
    undefined field_0x1ad;
    undefined field_0x1ae;
    undefined field_0x1af;
};
ASSERT_SIZE(struct advanced_menu, 432);

typedef void (*unk_collection_menu_fn_t)(undefined* param_1, int param_2, undefined* param_3);
typedef bool (*unk_collection_menu_check_fn_t)(void);
typedef void (*unk_collection_menu_void_fn_t)(void);

// Also see CreateCollectionMenu
// Like advanced menus, but specifically for mutating a collection of objects
// Observed examples:
// - Job selection menu
// - Job bulletin board menu
// - Kecleon Shop item selection menu
// - Kangaskhan Storage item selection menu
// - Croagunk Swap Shop item selection menu
struct collection_menu {
    update_window_fn_t update;           // 0x0
    struct window_input_ctx input_ctx;   // 0x4
    struct window_flags flags;           // 0xFC
    struct window_extra_info extra_info; // 0x100
    unk_collection_menu_fn_t field_0x198;
    unk_collection_menu_check_fn_t field_0x19c;
    undefined4 field_0x1a0;
    undefined4 field_0x1a4;
    unk_collection_menu_void_fn_t field_0x1a8;
    int state; // 0x1AC: appears to be a state value
    uint8_t field_0x1b0;
    uint8_t field_0x1b1;
    uint8_t field_0x1b2;
    undefined field_0x1b3;
    undefined4 field_0x1b4;
    undefined4 field_0x1b8;
    int field_0x1bc;
    int width; // 0x1c0: initially computed as (window_params::width * 8 - 1)
    uint8_t field_0x1c4;
    uint8_t field_0x1c5;
    uint8_t field_0x1c6;
    uint8_t field_0x1c7;
    uint8_t field_0x1c8;
    uint8_t field_0x1c9;
    undefined field_0x1ca;
    undefined field_0x1cb;
};
ASSERT_SIZE(struct collection_menu, 460);

struct options_menu_id_item {
    uint16_t string_id; // 0x0: string ID
    uint16_t _padding;
    int n_choices;     // 0x104: the number of choices to allow, 0 means all of them
    uint16_t* choices; // 0x100: null-terminated array of string IDs of choices for this item
};
ASSERT_SIZE(struct options_menu_id_item, 12);

struct options_menu_item {
    char string[256];  // 0x0: menu item text
    uint16_t* choices; // 0x100: array of string IDs of choices for this item
    int n_choices;     // 0x104: the number of choices
    int field_0x108;   // populated from param_6 in CreateOptionsMenu
};
ASSERT_SIZE(struct options_menu_item, 268);

// Also see CreateOptionsMenu
// Menu where each item can be paged through a selection of possible values.
// Observed examples:
// - Options menu
// - Window Options menu
// - Email Reception Settings menu
struct options_menu {
    update_window_fn_t update;           // 0x0
    struct window_input_ctx input_ctx;   // 0x4
    struct window_flags flags;           // 0xFC
    struct window_extra_info extra_info; // 0x100
    struct options_menu_item* items;     // 0x198: array of items, terminated by a zero entry
    undefined4* field_0x19c;
    int state; // 0x1A0: appears to be a state value
    uint8_t field_0x1a4;
    uint8_t field_0x1a5;
    undefined field_0x1a6;
    undefined field_0x1a7;
    int field_0x1a8;
    int field_0x1ac;
    int field_0x1b0;
    int field_0x1b4;
};
ASSERT_SIZE(struct options_menu, 440);

struct debug_menu_item {
    char string[256]; // 0x0: menu item text
    uint8_t field_0x100;
};
ASSERT_SIZE(struct debug_menu_item, 257);

// Also see CreateDebugMenu
// Probably the special debug menu, as the code references string IDs 15461/15462, which are from
// the debug menu
struct debug_menu {
    update_window_fn_t update;           // 0x0
    struct window_input_ctx input_ctx;   // 0x4
    struct window_flags flags;           // 0xFC
    struct window_extra_info extra_info; // 0x100
    // 0x198: array of items, terminated by an entry with an empty string
    struct debug_menu_item* items;
    undefined* field_0x19c;
    int state; // 0x1A0: appears to be a state value
    uint8_t field_0x1a4;
    uint8_t field_0x1a5;
    undefined field_0x1a6;
    undefined field_0x1a7;
    int field_0x1a8;
    char string_buffer1[256]; // 0x1AC: populated by GetStringFromFileVeneer
    char string_buffer2[256]; // 0x2AC: populated by GetStringFromFileVeneer
    // 0x3AC: some integer computed from string_buffer1, likely the pixel width of the rendered text
    // in string_buffer1 by analogy with the area_name_box fields
    int text_width1;
    // 0x3B0: some integer computed from string_buffer2, likely the pixel width of the rendered text
    // in string_buffer2 by analogy with the area_name_box fields
    int text_width2;
};
ASSERT_SIZE(struct debug_menu, 948);

// Also see CreateJukeboxTrackMenu
// Sky Jukebox track selection menu.
struct jukebox_track_menu {
    update_window_fn_t update; // 0x0
    undefined* field_0x4;
    struct window_input_ctx input_ctx;   // 0x8
    struct window_flags flags;           // 0x100
    struct window_extra_info extra_info; // 0x104
    // 0x19C: array of items, terminated by an entry with an empty string
    struct simple_menu_item* items;
    int state; // 0x1A0: appears to be a state value
    uint8_t field_0x1a4;
    uint8_t field_0x1a5;
    undefined field_0x1a6;
    undefined field_0x1a7;
    uint32_t field_0x1a8;
};
ASSERT_SIZE(struct jukebox_track_menu, 428);

// Also see CreatePlaybackControlsMenu
// Seems similar in functionality to a simple menu. The struct is even the same size. However,
// the layout is very slightly different.
// Observed examples of playback controls menu:
// - Sky Jukebox track playback controls window
struct playback_controls_menu {
    update_window_fn_t update; // 0x0
    undefined* field_0x4;
    struct window_input_ctx input_ctx;   // 0x8
    struct window_flags flags;           // 0x100
    struct window_extra_info extra_info; // 0x104
    // 0x19C: array of items, terminated by an entry with an empty string. Length matches items2
    struct simple_menu_item* items1;
    // 0x1A0: array of items, terminated by an entry with an empty string. Length matches items1
    struct simple_menu_item* items2;
    int state; // 0x1A4: appears to be a state value
    uint8_t field_0x1a8;
    uint8_t field_0x1a9;
    undefined field_0x1aa;
    undefined field_0x1ab;
    undefined4 field_0x1ac;
    uint8_t field_0x1b0;
    undefined field_0x1b1;
    undefined field_0x1b2;
    undefined field_0x1b3;
};
ASSERT_SIZE(struct playback_controls_menu, 436);

// Seems to be almost exactly the same as struct window_input_ctx, except shorter.
// The width is uncertain, but it's at least 240 and no more than 244. It's probably 240 because
// InitInventoryMenuInput is missing the assignment to the field at offset 244 that is present in
// InitWindowInput
struct inventory_menu_input_ctx {
    undefined fields_0x0[240];
};
ASSERT_SIZE(struct inventory_menu_input_ctx, 240);

// Also see CreateInventoryMenu
// Menu for managing items?
// Observed examples:
// - Treasure bag menu item selection window
// - Kecleon Shop item info/price window in dungeons
struct inventory_menu {
    update_window_fn_t update;                 // 0x0
    struct inventory_menu_input_ctx input_ctx; // 0x4
    undefined field_0xf4;
    undefined field_0xf5;
    undefined field_0xf6;
    undefined field_0xf7;
    struct window_flags flags;           // 0xF8
    struct window_extra_info extra_info; // 0xFC
    undefined* field_0x194;
    undefined4 field_0x198;
    int state; // 0x19C: appears to be a state value
    uint8_t field_0x1a0;
    uint8_t field_0x1a1;
    uint8_t field_0x1a2;
    uint8_t field_0x1a3;
};
ASSERT_SIZE(struct inventory_menu, 420);

#endif
