#ifndef HEADERS_TYPES_COMMON_KAOMADO_H_
#define HEADERS_TYPES_COMMON_KAOMADO_H_

// Represents the state of a portrait that wants to be displayed
struct kaomado {
    uint16_t species_id; // The species id, or the set index inside kaomado.kao
    int8_t emote_idx;    // Index of the emote in the species set of portraits
    uint8_t layout_idx;  // Represents the current layout to display the portrait
    uint32_t offset_x;   // Tile offset (x / 8) in the X axis to draw the portrait
    uint32_t offset_y;   // Tile offset (y / 8) in the Y axis to draw the portrait
    bool try_flip;       // Whether to try to get a flipped portrait from kaomado.kao
    bool has_flip;       // Whether the specified emote has a flipped variant
    bool hw_flip;        // Whether the portrait should be flipped using the hardware
    bool allow_default;  // If true, try using emote index 0 if the desired emote can't be found
};
ASSERT_SIZE(struct kaomado, 16);

struct portrait_offset {
    int32_t offset_x; // X offset in tiles from the original offset, derived from the layout
    int32_t offset_y; // Y offset in tiles from the original offset, derived from the layout
};
ASSERT_SIZE(struct portrait_offset, 8);

struct kaomado_buffer {
    struct rgb palette[16];    // Buffer to load the palette of the portrait
    uint8_t at4px_buffer[800]; // Buffer to load the portrait image data
};
ASSERT_SIZE(struct kaomado_buffer, 0x350);

// Stores data of a specialized Kaomado canvas for rendering the portrait into itself
struct kaomado_canvas {
    uint8_t canvas_handle;
    uint8_t pad0x1;
    uint8_t pad0x2;
    uint8_t pad0x3;
    enum kaomado_canvas_state state;
    // The buffer_state is the one that receives and stores any commits,
    // but render_state is only set to the value of buffer_state during
    // the Kaomado canvas update function
    struct kaomado render_state;
    struct kaomado buffer_state;
    bool updated;
    bool hide;
    bool framed;
    uint32_t palette_idx;
    uint8_t pad0x2b;
    struct kaomado_buffer buffer;
};
ASSERT_SIZE(struct kaomado_canvas, 0x380);

// Identifies a default position for a Kaomado portrait, as well as whether it'll be flipped
struct kaomado_layout {
    int16_t offset_x;
    int16_t offset_y;
    bool try_flip;
    uint8_t pad0x5;
};
ASSERT_SIZE(struct kaomado_layout, 6);

#endif
