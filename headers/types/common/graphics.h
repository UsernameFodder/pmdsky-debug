#ifndef HEADERS_TYPES_COMMON_GRAPHICS_H_
#define HEADERS_TYPES_COMMON_GRAPHICS_H_

// Structure related to rendering the graphics not specific to any mode

// Contain data relating to animation
// mentionned under the name of "AnimeCtrl" in a debug message
struct animation_control {
    uint16_t some_bitfield;
    undefined2 field1_0x2;
    undefined2 field2_0x4;
    undefined2 field3_0x6;
    undefined2 field4_0x8;
    undefined2 field5_0xa;
    undefined2 field6_0xc;
    undefined field7_0xe;
    undefined field8_0xf;
    undefined2
        field9_0x10[6]; // appears to be a a structure used for unknown purpose with 6 2-bytes value
    undefined2 field10_0x1c;
    undefined2 field11_0x1e;
    undefined2 field12_0x20;
    undefined2 field13_0x22;
    undefined2 field14_0x24;
    undefined2 field15_0x26;
    undefined4 field16_0x28;
    undefined4 field17_0x2c;
    undefined2 field18_0x30;
    undefined2 field19_0x32;
    undefined field20_0x34;
    undefined field21_0x35;
    undefined2 field22_0x36;
    undefined2 field23_0x38;
    undefined2 field24_0x3a;
    undefined2 field25_0x3c;
    undefined field26_0x3e;
    undefined field27_0x3f;
    undefined1 field28_0x40;
    undefined1 field29_0x41;
    undefined field30_0x42;
    undefined field31_0x43;
    undefined field32_0x44;
    undefined field33_0x45;
    undefined field34_0x46;
    undefined field35_0x47;
    struct WanAnimationFrame** field36_0x48;
    struct WanAnimationFrame** field37_0x4c;
    struct WanOffset* wan_offsets;
    undefined* wan_frames;
    void** wan_fragments_byte_store;
    struct WanPalettes* wan_palettes;
    uint16_t wan_is_256_color;
    uint16_t id_of_something;
    undefined field44_0x64;
    undefined field45_0x65;
    undefined field46_0x66;
    undefined field47_0x67;
    undefined* sprite_override; /* seems to point to a SIRO file containing another sprite, that is
                                   sometimes used instead of the one specified in loaded_sprite_id
                                   (maybe it is loaded_sprite_id but looked up?) */
    undefined4 field49_0x6c;
    uint16_t loaded_sprite_id; /* id in the WAN_TABLE static list */
    undefined2 field51_0x72;
    undefined field52_0x74;
    undefined field53_0x75;
    uint16_t animation_group_id;
    uint16_t animation_id;
    undefined1 palette_bank;
    undefined field57_0x7b;
};
ASSERT_SIZE(struct animation_control, 124);

#endif