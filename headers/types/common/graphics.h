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
    uint32_t anim_frame_flag_sum; // ORed from the current WAN animation frame flag
    undefined2 field18_0x30;
    undefined2 field19_0x32;
    undefined field20_0x34;
    undefined field21_0x35;
    undefined2 field22_0x36;
    undefined2 field23_0x38;
    uint16_t anim_frame_frame_id;
    undefined2 field25_0x3c;
    undefined field26_0x3e;
    undefined field27_0x3f;
    uint8_t palette_pos_high; // Used to determine where to store the palette’s colors. Multiplied
                              // by 0x100
    uint8_t palette_pos_low;  // Used to determine where to store the palette’s colors
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

#endif
