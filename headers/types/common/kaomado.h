// Represents the state of a portrait that wants to be displayed
struct kaomado {
    uint16_t species_id;    // The species id, or the set index inside kaomado.kao
    int8_t emote_idx;       // Index of the emote in the species set of portraits
    uint8_t layout_idx;     // Represents the current layout to display the portrait
    uint32_t offset_x;      // Tile offset (x / 8) in the X axis to draw the portrait
    uint32_t offset_y;      // Tile offset (y / 8) in the Y axis to draw the portrait
    bool try_flip;          // Whether to try to get a flipped portrait from kaomado.kao
    bool has_flip;          // Whether the specified emote has a flipped variant
    bool hw_flip;           // Whether the portrait should be flipped using the hardware
    bool allow_default;     // If true, try using emote index 0 if the desired emote can't be found
};
ASSERT_SIZE(struct kaomado, 16);

struct portrait_offset {
    int32_t offset_x;   // X offset in tiles from the original offset, derived from the layout
    int32_t offset_y;   // Y offset in tiles from the original offset, derived from the layout
};
ASSERT_SIZE(struct portrait_offset, 8);

struct kaomado_buffer {
    struct rgb_24 palette[16]; // Buffer to load the palette of the portrait
    uint8_t at4px_buffer[800]; // Buffer to load the portrait image data
}
ASSERT_SIZE(struct kaomado_buffer, 0x350);

// Stores data of a specialized Kaomado canvas for rendering the portrait into itself
struct kaomado_canvas {
    uint8_t canvas_handle;
    uint8_t pad0x1;
    uint8_t pad0x2;
    uint8_t pad0x3;
    enum kaomado_canvas_state state : 32;
    struct kaomado render_state; // This is the Kaomado that actually gets rendered
    struct kaomado buffer_state; // Any changes are first stored here before being copied to the render_state on update time
    bool updated;
    bool hide;
    bool framed;
    uint8_t pad0x2b;
    struct kaomado_buffer buffer;
}
ASSERT_SIZE(struct kaomado_canvas, 0x380);