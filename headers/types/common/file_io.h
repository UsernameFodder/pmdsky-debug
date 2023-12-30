// Structures about reading files from the filesystem, as well as parsing them.

#ifndef HEADERS_TYPES_COMMON_FILE_IO_H_
#define HEADERS_TYPES_COMMON_FILE_IO_H_

#include "enums.h"

// This is essentially the standard (32-bit) Unix I/O vector struct.
// It's used for file I/O and represents a buffer of data with a pointer and a length.
struct iovec {
    void* iov_base;
    uint32_t iov_len;
};
ASSERT_SIZE(struct iovec, 8);

struct rect16_xywh {
    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
};
ASSERT_SIZE(struct rect16_xywh, 8);

// A structure that represents a file stream for file I/O.
struct file_stream {
    undefined4 field_0x0;
    undefined4 field_0x4;
    undefined4 field_0x8;
    undefined4 field_0xc;
    undefined4 field_0x10;
    undefined4 field_0x14;
    undefined4 field_0x18;
    undefined4 field_0x1c;
    undefined4 field_0x20;
    void* start_address;
    void* end_address;
    void* current_address;
    undefined4 field_0x30;
    undefined4 field_0x34;
    undefined4 field_0x38;
    undefined4 field_0x3c;
    undefined4 field_0x40;
    undefined4 field_0x44;
};
ASSERT_SIZE(struct file_stream, 72);

// Contain information about a single file contained inside a Pack archive
struct pack_file_table_of_content {
    uint32_t offset; // The offset of the file relative to the start of the Pack file
    uint32_t length; // The size of the file
};
ASSERT_SIZE(struct pack_file_table_of_content, 8);

/*  Information for reading a .bin Pack file

    A pack file is an archive that contains multiple other files. They aren't named.

    File format documentation at
   https://projectpokemon.org/docs/mystery-dungeon-nds/pmd2-pack-file-format-r42 */
struct pack_file_opened {
    struct pack_file_table_of_content* table_of_content;
    undefined4 zero; // Loaded from file, but is always zero here. Seemingly unused.
    uint32_t nb_entry_table_of_content; // loaded from file
    struct file_stream opened_file;
};
ASSERT_SIZE(struct pack_file_opened, 84);

// Data to return result of the AllocAndLoadFileInPack function
struct pack_alloc_and_load_result {
    void* data;
    uint32_t length;
};
ASSERT_SIZE(struct pack_alloc_and_load_result, 8);

/*  Handle to a memory-allocated WTE file.

    The WTE file format is a simple file format found both in file directories
    and in the ROM filesystem. This format specializes in storing texture data,
    and is closely linked to the 3D engine, as well as the 3D resource manager!

    While EoS is not a 3D game, the game still utilizes the 3D hardware to draw
    graphics onto the screen. Examples of its usage are the Dungeon Mode GUI
    (DUNGEON/dungeon.bin+0x3F4) and the fog (DUNGEON/dungeon.bin+0x401) */
struct wte_handle {
    void* content; // Pointer to the heap-allocated WTE data. Only stored for freeing the data
    struct wte_header* header;
};
ASSERT_SIZE(struct wte_handle, 8);

// These arguments are almost directly passed to the TEXIMAGE_PARAM register, just rearranged
// For more information see:
// https://problemkaputt.de/gbatek.htm#ds3dtextureattributes
#pragma pack(push, 2)
struct wte_texture_params {
    uint8_t texture_smult : 3;
    uint8_t texture_tmult : 3;
    uint8_t unused6 : 2;
    enum texture_format format : 3;
    bool repeat_x : 1;
    bool repeat_y : 1;
    uint8_t unusedD : 3;
};
ASSERT_SIZE(struct wte_texture_params, 2);
#pragma pack(pop)

struct wte_header {
    char signature[4];                // 0x0: Signature bytes (must be "\x57\x54\x45\x00")
    void* texture;                    // 0x4
    uint32_t texture_size;            // 0x8
    struct wte_texture_params params; // 0xC
    uint16_t _padding_0xe;
    /*  These bounds are NOT used by the game, but they prove useful to extract the texture out
        of the file. The offsets are redundant and should be zero

        The width specified here should always be the same as the one specified in the texture
        params, but the height may be lower. The reason is that just like the width, the height
        needs to be a power of 2 in the range of 8..1024. The actual texture can have a lower
        height, but not a lower width, as the width is required to properly read the image */
    struct rect16_xywh texture_bounds; // 0x10
    struct rgba* palette;              // 0x18
    uint16_t color_amt;                // 0x1C: How many colors are stored in the palette
    uint16_t _padding_0x1e;
};
ASSERT_SIZE(struct wte_header, 32);

struct bg_list_entry_sub_struct {
    char name[10]; // Null-terminated string
};
ASSERT_SIZE(struct bg_list_entry_sub_struct, 10);

// Represents an entry in the bg_list.dat file, after being loaded by LoadBgListEntry
struct bg_list_entry {
    struct bg_list_entry_sub_struct bpl;
    struct bg_list_entry_sub_struct bpc;
    struct bg_list_entry_sub_struct bma;
    struct bg_list_entry_sub_struct others_bpa[8];
};
ASSERT_SIZE(struct bg_list_entry, 110);

#endif
