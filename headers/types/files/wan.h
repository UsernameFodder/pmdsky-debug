#ifndef HEADERS_TYPES_FILES_WAN_H_
#define HEADERS_TYPES_FILES_WAN_H_

#include "../common/util.h"

enum wan_sprite_type {
    WAN_SPRITE_PROPS_UI = 0, // for object and UI objects
    WAN_SPRITE_CHARA = 1,    // for monster sprites
    WAN_SPRITE_UNK_2 = 2,
    WAN_SPRITE_UNK_3 = 3,
};

// This is usually stored as a 16-bit integer
#pragma pack(push, 2)
ENUM_16_BIT(wan_sprite_type);
#pragma pack(pop)

struct wan_animation_header {
    void* frames;
    struct wan_offset* frame_offsets;
    struct wan_animation_group* animations;
    uint16_t nb_animation_groups;
    uint16_t allocation_for_max_frame;
    undefined field5_0x10;
    undefined field6_0x11;
    undefined field7_0x12;
    undefined field8_0x13;
    // appears to always be 0, but the game seems to still decode it
    // the image header is missing for some reason
    uint16_t is_256_color_alt;
    undefined field9_0x16;
    undefined field10_0x17;
};
ASSERT_SIZE(struct wan_animation_header, 24);

struct wan_palettes {
    struct rgba* palette_bytes;
    uint16_t unk1;
    uint16_t nb_color;
    uint16_t unk2;
    uint8_t unk3;
    uint8_t unk4;
    uint32_t unk5; // Normally always 0
};
ASSERT_SIZE(struct wan_palettes, 16);

// An animation group is a set of animations, either a single one or 8 for the eight possible
// orientations of a monster. An animation is itself made of frames.
struct wan_animation_group {
    // first level is animations, second is frame pointer
    struct wan_animation_frame** pnt;
    uint16_t len;
    // The frame offset relative to the first frame of the animation that will be used instead of
    // the first frame when looping the animation. (untested)
    uint16_t loop_start;
};
ASSERT_SIZE(struct wan_animation_group, 8);

struct wan_animation_frame {
    uint8_t duration;
    uint8_t flag;
    uint16_t frame_id;
    struct vec2_16 offset;
    struct vec2_16 shadow_offset;
};
ASSERT_SIZE(struct wan_animation_frame, 12);

struct wan_image_header {
    void** fragments_bytes_store;
    struct wan_palettes* palettes;
    uint16_t unk1;
    bool is_256_color;
    undefined1 padding1;
    uint16_t unk2;
    uint16_t nb_fragments;
};
ASSERT_SIZE(struct wan_image_header, 16);

struct wan_offset {
    struct uvec2_16 head;
    struct uvec2_16 hand_left;
    struct uvec2_16 hand_right;
    struct uvec2_16 center;
};
ASSERT_SIZE(struct wan_offset, 16);

// A sprite file, that are directly loaded from memory as they are stored (after decompression if
// needed). Names comes from the pmd_wan project.
#pragma pack(push, 2)
struct wan_header {
    struct wan_animation_header* anim_header;
    struct wan_image_header* image_header;
    struct wan_sprite_type_16 sprite_type;
};
#pragma pack(pop)
ASSERT_SIZE(struct wan_header, 10);

#endif
