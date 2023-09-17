#ifndef HEADERS_TYPES_COMMON_GRAPHICS_H_
#define HEADERS_TYPES_COMMON_GRAPHICS_H_

#include "enums.h"
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

// It seems like there may be other flags packed alongside the render type
#pragma pack(push, 2)
struct render_type_and_flags {
    enum render_type type : 3;
    uint16_t flags : 13;
};
ASSERT_SIZE(struct render_type_and_flags, 2);
#pragma pack(pop)

// Common header for all render_3d_element union variants
struct render_3d_element_hdr {
    // 0x0: Corresponds to the render function in RENDER_3D_FUNCTIONS
    struct render_type_and_flags type;
    // 0x2: Used to set the z coordinate for the rendered element, where z = -(z_index + 1)
    uint16_t z_index;
};
ASSERT_SIZE(struct render_3d_element_hdr, 4);

// Data for RENDER_RECTANGLE
struct render_3d_rectangle {
    struct render_3d_element_hdr hdr; // 0x0
    // 0x4: translation applied to the polygon, after scaling and rotation.
    // Components are specified as normal integers, NOT fixed-point numbers.
    struct vec2_16 translation;
    struct vec2_16 size; // 0x8: width and height, as fixed-point numbers with 6 fraction bits
    // 0xC: negative position of the top-left corner relative to the translated origin, i.e. (origin
    // - corner) fixed-point number with 6 fraction bits
    struct vec2_16 negative_corner_pos;
    struct vec2_16 _unused_v3; // 0x10: 4th vector parameter, not needed for rectangles
    // 0x14: scale factor for the polygon, as fixed-point numbers with 6 fraction bits.
    // Scaling is applied before rotation and translation.
    // The game often uses a scale factor of 64.0 (equivalent to setting scale == 0x1000 here).
    struct uvec2_16 scale;
    // 0x18: clockwise rotation applied to the polygon, after scaling and before translation.
    // angle = rotation * (2π radians)/2^16 (so the uint16 value range covers one full revolution).
    uint16_t rotation;
    // 0x1A: vertex colors for the upper-left, upper-right, lower-left, and lower-right vertices,
    // respectively. If !multicolored, colors[0] is used for the whole polygon
    struct rgb5 colors[4];
    // 0x22: polygon ID for certain 3D effects, see
    // https://problemkaputt.de/gbatek.htm#ds3dpolygonattributes
    uint8_t polygon_id;
    uint8_t alpha;     // 0x23: ranges from 0-31
    bool multicolored; // 0x24: whether or not more than one vertex color is specified
    uint8_t _padding;  // 0x25
};
ASSERT_SIZE(struct render_3d_rectangle, 38);

// Data for RENDER_QUADRILATERAL
struct render_3d_quadrilateral {
    struct render_3d_element_hdr hdr; // 0x0
    // 0x4: vertices defining the quadrilateral, as fixed-point numbers with 6 fraction bits.
    // Quadrilaterals are drawn in "triangle strips" mode
    // (mode 2, see https://problemkaputt.de/gbatek.htm#ds3dpolygondefinitionsbyvertices),
    // so an example of vertex ordering for a square would be:
    //   v0 v1
    //   v2 v3
    struct vec2_16 vertices[4];
    struct uvec2_16 _unused_scale; // 0x14
    uint16_t _unused_rotation;     // 0x18
    // 0x1A: vertex colors for the upper-left, upper-right, lower-left, and lower-right vertices,
    // respectively. if !multicolored, colors[0] is used for the whole polygon
    struct rgb5 colors[4];
    uint8_t _unused_polygon_id; // 0x22
    uint8_t alpha;              // 0x23: ranges from 0-31
    bool multicolored;          // 0x24: whether or not more than one vertex color is specified
    uint8_t _padding;           // 0x25
};
ASSERT_SIZE(struct render_3d_quadrilateral, 38);

// Collection of texture parameters to send with a TEXIMAGE_PARAM command
// (see https://problemkaputt.de/gbatek.htm#ds3dtextureattributes)
struct render_3d_texture_params {
    uint16_t texture_s_size : 3;
    uint16_t texture_t_size : 3;
    uint16_t teximage_param_flags_6 : 2;
    uint16_t texture_format : 3;
    uint16_t repeat_s : 1;
    uint16_t repeat_t : 1;
    uint16_t teximage_param_flags_d : 3;
};
ASSERT_SIZE(struct render_3d_texture_params, 2);

// Data for RENDER_TILING
struct render_3d_tiling {
    struct render_3d_element_hdr hdr; // 0x0
    // 0x4: texture VRAM offset (NOT divided by 8), see GeomSetTexImageParam
    int32_t texture_vram_offset;
    // 0x8: texture palette base address (NOT divided by anything), see the PLTT_BASE command:
    // https://problemkaputt.de/gbatek.htm#ds3dtextureattributes
    int32_t palette_base_addr;
    // 0xC: position of the top-left corner of the texture in the source tilemap, in an integer
    // number of texels
    struct vec2_16 texture_top_left;
    // 0x10: width and height of the texture in the source tilemap, in an integer number of texels
    struct vec2_16 texture_size;
    struct render_3d_texture_params texture_params; // 0x14
    // 0x16: vertices defining the quadrilateral on which to render the tiling, as
    // fixed-point numbers with 6 fraction bits.
    // Tiling polygons are drawn in "separate quadrilaterals" mode
    // (mode 1, see https://problemkaputt.de/gbatek.htm#ds3dpolygondefinitionsbyvertices),
    // but the vertex order here purposely swaps the last two entries so that the API is more
    // like triangle strips, so an example of vertex ordering for a square would be:
    //   v0 v1
    //   v2 v3
    struct vec2_16 vertices[4];
    // 0x26: vertex colors for the upper-left, upper-right, lower-left, and lower-right vertices,
    // respectively. If !multicolored, colors[0] is used for the whole polygon
    struct rgb5 colors[4];
    uint16_t _unused_rotation;
    // 0x30: polygon ID for certain 3D effects, see
    // https://problemkaputt.de/gbatek.htm#ds3dpolygonattributes
    uint8_t polygon_id;
    uint8_t alpha;     // 0x31: ranges from 0-31
    bool multicolored; // 0x32: whether or not more than one vertex color is specified
    uint8_t _padding;
};
ASSERT_SIZE(struct render_3d_tiling, 52);

// Data for RENDER_TEXTURE
struct render_3d_texture {
    struct render_3d_element_hdr hdr;
    // 0x4: texture VRAM offset (NOT divided by 8), see GeomSetTexImageParam
    int32_t texture_vram_offset;
    // 0x8: texture palette base address (NOT divided by anything), see the PLTT_BASE command:
    // https://problemkaputt.de/gbatek.htm#ds3dtextureattributes
    int32_t palette_base_addr;
    // 0xC: position of the top-left corner of the texture in the source tilemap, in an integer
    // number of texels
    struct vec2_16 texture_top_left;
    // 0x10: width and height of the texture in the source tilemap, in an integer number of texels
    struct vec2_16 texture_size;
    struct render_3d_texture_params texture_params; // 0x14
    // 0x16: translation applied to the polygon, after scaling and rotation.
    // Components are specified as normal integers, NOT fixed-point numbers.
    struct vec2_16 translation;
    // 0x1A: negative position of the top-left corner of the underlying polygon relative to the
    // translated origin, i.e. (origin - corner) fixed-point number with 6 fraction bits
    struct vec2_16 negative_corner_pos;
    // 0x1E: scale factor for the polygon, as fixed-point numbers with 6 fraction bits.
    // Scaling is applied before rotation and translation.
    // The game often uses a scale factor of 64.0 (equivalent to setting scale == 0x1000 here).
    struct uvec2_16 scale;
    struct rgb5 color; // 0x22: polygon color
    // 0x24: clockwise rotation applied to the polygon, after scaling and before translation.
    // angle = rotation * (2π radians)/2^16 (so the uint16 value range covers one full revolution).
    uint16_t rotation;
    // 0x26: polygon ID for certain 3D effects, see
    // https://problemkaputt.de/gbatek.htm#ds3dpolygonattributes
    uint8_t polygon_id;
    uint8_t alpha; // 0x27: ranges from 0-31
};
ASSERT_SIZE(struct render_3d_texture, 40);

// Represents a single element to render using the 3D engine.
// Queued in RENDER_3D and used by the 3D engine to render an object.
// Most of the time, the game works with the related render_3d_element_64 union, which
// is eventually converted to a render_3d_element when enqueuing for rendering.
//
// Other general notes:
// - under the usual scaling, 1 texel in texture coordinates is often equivalent to 1/64 in
//   vertex coordinates
// - x increases rightward, y increases downward
// - the (untranslated) origin is usually at the top-left corner
// - scaling typically appears to be the same for the x and y axes
// - at normal scaling, it seems like a distance of 1 pixel corresponds to a coordinate distance
//   of 4.0 * 2^-6 (fixed point, i.e. 0x4)?
union render_3d_element {
    struct render_3d_rectangle rectangle;
    struct render_3d_quadrilateral quadrilateral;
    struct render_3d_tiling tiling;
    struct render_3d_texture texture;
};
ASSERT_SIZE(union render_3d_element, 52);

// Function that will use the relevant 3d render API to render a render_3d_element from the
// RENDER_3D queue.
typedef void (*render_3d_element_fn_t)(union render_3d_element* element);

// 64-byte version of render_3d_element used in some places.
// Not all fields are used for all types.
struct render_3d_element_64 {
    // 0x0: four vectors defining the polygon to be drawn. By type:
    // RENDER64_TEXTURE, RENDER64_TEXTURE_0x7, RENDER64_RECTANGLE, RENDER64_RECTANGLE_MULTICOLOR:
    //   vec[0] -> translation
    //   vec[1] -> size (unused for texture types)
    //   vec[2] -> negative_corner_pos
    //   vec[3] -> unused
    // RENDER64_QUADRILATERAL, RENDER64_TILING:
    //   vec[i] -> vertices[i]
    struct vec2_16 vec[4];
    struct uvec2_16 scale;                          // 0x10: maps to render_3d_*::scale
    struct render_3d_texture_params texture_params; // 0x14: maps to render_3d_*::texture_params
    struct vec2_16 texture_top_left;                // 0x16: maps to render_3d_*::texture_top_left
    struct vec2_16 texture_size;                    // 0x1A: maps to render_3d_*::texture_size
    undefined field_0x1e;
    undefined field_0x1f;
    int32_t texture_vram_offset; // 0x20: maps to render_3d_*::texture_vram_offset
    int32_t palette_base_addr;   // 0x24: maps to render_3d_*::palette_base_addr
    uint16_t rotation;           // 0x28: maps to render_3d_*::rotation
    uint16_t z_index;            // 0x2A: maps to render_3d_element_hdr::z_index
    // 0x2C: maps to render_3d_*::color (subject to the rgba -> rgb5 conversion),
    // and alpha (color[0] only)
    struct rgba color[4];
    // 0x3C: Corresponds to the render64 function in RENDER_3D_FUNCTIONS_64.
    // NOT the same as render_3d_element_hdr's type
    struct render_type_64_8 type;
    uint8_t flags_0x3d_0 : 1;
    bool multicolored : 1; // maps to render_3d_*::multicolored
    uint8_t flags_0x3d_2 : 6;
    uint8_t polygon_id; // 0x3E: maps to render_3d_*::polygon_id
    undefined field_0x3f;
};
ASSERT_SIZE(struct render_3d_element_64, 64);

typedef void (*render_3d_element_64_fn_t)(struct render_3d_element_64* element64);

// A global, unique structure that stores element relating to the 3d engine, in particular the list
// of elements to render later in the frame.
struct render_3d_global {
    int16_t current_index; // 0x0
    int16_t max_index;     // 0x2: Seems to consistently be 128, size of render_queue
    // 0x4: palette_base_addr of the most recent render_3d_texture/render_3d_tiling
    int32_t palette_base_addr;
    // 0x8: texture_vram_offset of the most recent render_3d_texture/render_3d_tiling
    int32_t texture_vram_offset;
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
    union render_3d_element* render_queue; // 0x40: array that can contain up to max_index entries.
};
ASSERT_SIZE(struct render_3d_global, 68);

#endif
