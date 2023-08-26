#ifndef HEADERS_TYPES_COMMON_GRAPHICS_H_
#define HEADERS_TYPES_COMMON_GRAPHICS_H_

#include "../files/wan.h"

// Structures related to rendering graphics not specific to any mode

// Contains data relating to animation.
// Mentioned under the name of "AnimeCtrl" in a debug message.
struct animation_control {
    uint16_t some_bitfield;
    undefined2 field1_0x2;
    undefined2 field2_0x4;
    uint16_t anim_frame_duration;
    undefined2 field4_0x8;
    undefined2 field5_0xa;
    uint16_t field6_0xc; // (from struct entity) animation frame counter for the idle animation?
    undefined field7_0xe;
    undefined field8_0xf;
    // appears to be a structure used for unknown purpose with 6 2-byte value
    undefined2 field9_0x10[6];
    undefined2 field10_0x1c;
    undefined2 field11_0x1e;
    struct vec2_16 anim_frame_offset;
    struct vec2_16 anim_frame_shadow_offset;
    uint32_t anim_frame_flag;
    uint32_t anim_frame_flag_sum;     // ORed from the current WAN animation frame flag
    uint16_t animations_or_group_len; // number of animation group Chara sprites, other number of
                                      // animation in the first animation group
    undefined2 field19_0x32;
    undefined field20_0x34;
    undefined field21_0x35;
    undefined2 field22_0x36; // seems to be an index into the wan fragments table
    undefined2 field23_0x38;
    uint16_t anim_frame_frame_id;
    undefined2 field25_0x3c;
    undefined field26_0x3e;
    undefined field27_0x3f;
    // Used to determine where to store the palette's colors. Multiplied by 0x100
    uint8_t palette_pos_high;
    uint8_t palette_pos_low; // Used to determine where to store the palette's colors
    undefined field30_0x42;
    undefined field31_0x43;
    undefined field32_0x44;
    undefined field33_0x45;
    undefined field34_0x46;
    undefined field35_0x47;
    struct wan_animation_frame* first_animation_frame; // first frame in animation group
    struct wan_animation_frame* next_animation_frame;
    struct wan_offset* wan_offsets;
    undefined* wan_frames;
    void** wan_fragments_byte_store;
    struct wan_palettes* wan_palettes;
    uint16_t wan_is_256_color;
    uint16_t loop_start; // same meaning as in struct wan_animation_frame
    undefined field44_0x64;
    undefined field45_0x65;
    undefined field46_0x66;
    undefined field47_0x67;
    // seems to point to a SIRO file containing another sprite, that is sometimes used instead of
    // the one specified in loaded_sprite_id (maybe it is loaded_sprite_id but looked up?)
    undefined* sprite_override;
    undefined4 field49_0x6c;
    uint16_t loaded_sprite_id; // id in the WAN_TABLE static list
    struct monster_id_16 field51_0x72;
    undefined field52_0x74;
    undefined field53_0x75;
    uint16_t animation_group_id;
    uint16_t animation_id;
    undefined1 palette_bank;
    undefined field57_0x7b;
};
ASSERT_SIZE(struct animation_control, 124);

// Represent a single element to render using the 3D engine
// This structure is used in two different ways. The first one is for planning rendering, that will
// be passed as an argument to a function. Said function will then add them to RENDER_3D
// for rendering later in the frame (second type of use, with slightly different argument, but that
// often appear to be a copy of the struct with some value changed)
struct render_3d_element {
    // range from 0 to 3 (included). Function used in the rendering phase, from the
    // RENDER_3D_FUNCTIONS table
    undefined2 render_function_id;
    undefined2 field1_0x2; // appears to be a render priority level. Impact sorting.
    undefined4 field2_0x4;
    undefined4 field3_0x8;
    uint16_t x_tileset_start;
    uint16_t y_tileset_start;
    uint16_t x_tileset_width_offset;
    uint16_t y_tileset_height_offset;
    undefined2 field8_0x14;
    undefined2 x_start_1;
    undefined2 y_start_1;
    undefined2 x_end_1;
    undefined2 y_start_2;
    undefined2 x_start_2;
    undefined2 y_end_1;
    undefined2 x_end_2;
    undefined2 y_end_2;
    undefined2 field17_0x26[4];
    undefined2 field18_0x2e;
    undefined field19_0x30;
    char field20_0x31;
    bool field21_0x32;
    undefined field22_0x33;
};
ASSERT_SIZE(struct render_3d_element, 52);

// Function that will use the relevant 3d render API to render a render_3d_element from the
// RENDER_3D queue.
typedef void (*render_3d_element_concrete_fn_t)(struct render_3d_element* element);

// A global, unique structure that stores element relating to the 3d engine, in particular the list
// of elements to render later in the frame.
struct render_3d_global {
    uint16_t current_index;
    uint16_t max_index; // Seems to consistently be 128, size of render_stack
    undefined4 field2_0x4;
    undefined4 field3_0x8;
    undefined field4_0xc;
    undefined field5_0xd;
    undefined field6_0xe;
    undefined field7_0xf;
    undefined field8_0x10;
    undefined field9_0x11;
    undefined field10_0x12;
    undefined field11_0x13;
    undefined field12_0x14;
    undefined field13_0x15;
    undefined field14_0x16;
    undefined field15_0x17;
    undefined field16_0x18;
    undefined field17_0x19;
    undefined field18_0x1a;
    undefined field19_0x1b;
    undefined field20_0x1c;
    undefined field21_0x1d;
    undefined field22_0x1e;
    undefined field23_0x1f;
    undefined field24_0x20;
    undefined field25_0x21;
    undefined field26_0x22;
    undefined field27_0x23;
    undefined field28_0x24;
    undefined field29_0x25;
    undefined field30_0x26;
    undefined field31_0x27;
    undefined field32_0x28;
    undefined field33_0x29;
    undefined field34_0x2a;
    undefined field35_0x2b;
    undefined field36_0x2c;
    undefined field37_0x2d;
    undefined field38_0x2e;
    undefined field39_0x2f;
    undefined field40_0x30;
    undefined field41_0x31;
    undefined field42_0x32;
    undefined field43_0x33;
    undefined field44_0x34;
    undefined field45_0x35;
    undefined field46_0x36;
    undefined field47_0x37;
    undefined field48_0x38;
    undefined field49_0x39;
    undefined field50_0x3a;
    undefined field51_0x3b;
    undefined field52_0x3c;
    undefined field53_0x3d;
    undefined field54_0x3e;
    undefined field55_0x3f;
    struct render_3d_element* render_stack; // an array that can contain up to max_index entries.
};
ASSERT_SIZE(struct render_3d_global, 68);

#endif
