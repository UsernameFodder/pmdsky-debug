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

// Those FS struct come from the pokediamond decomp. See also how they are used here:
// https://github.com/pret/pokediamond/blob/ed142f69171f575e9359e113cd18f315cb0fcdd3/arm9/lib/NitroSDK/src/FS_command_default.c

enum FSCommandType {
    FS_COMMAND_ASYNC_BEGIN = 0,
    FS_COMMAND_READFILE = FS_COMMAND_ASYNC_BEGIN,
    FS_COMMAND_WRITEFILE,
    FS_COMMAND_ASYNC_END,

    FS_COMMAND_SYNC_BEGIN = FS_COMMAND_ASYNC_END,
    FS_COMMAND_SEEKDIR = FS_COMMAND_SYNC_BEGIN,
    FS_COMMAND_READDIR,
    FS_COMMAND_FINDPATH,
    FS_COMMAND_GETPATH,
    FS_COMMAND_OPENFILEFAST,
    FS_COMMAND_OPENFILEDIRECT,
    FS_COMMAND_CLOSEFILE,
    FS_COMMAND_SYNC_END,

    FS_COMMAND_STATUS_BEGIN = FS_COMMAND_SYNC_END,
    FS_COMMAND_ACTIVATE = FS_COMMAND_STATUS_BEGIN,
    FS_COMMAND_IDLE,
    FS_COMMAND_SUSPEND,
    FS_COMMAND_RESUME,
    FS_COMMAND_STATUS_END,

    FS_COMMAND_INVALID
};

struct FSFileLink {
    struct FSFile* prev;
    struct FSFile* next;
};

struct OSThreadQueue {
    void* head;
    void* tail;
};

#define FS_FILE_STATUS_BUSY 0x00000001
#define FS_FILE_STATUS_CANCEL 0x00000002
#define FS_FILE_STATUS_SYNC 0x00000004
#define FS_FILE_STATUS_ASYNC 0x00000008
#define FS_FILE_STATUS_IS_FILE 0x00000010
#define FS_FILE_STATUS_IS_DIR 0x00000020
#define FS_FILE_STATUS_OPERATING 0x00000040

enum FSResult {
    FS_RESULT_SUCCESS = 0,
    FS_RESULT_FAILURE,
    FS_RESULT_BUSY,
    FS_RESULT_CANCELED,
    FS_RESULT_CANCELLED = FS_RESULT_CANCELED, // SDK alias
    FS_RESULT_UNSUPPORTED,
    FS_RESULT_ERROR,
    FS_RESULT_PROC_ASYNC,
    FS_RESULT_PROC_DEFAULT,
    FS_RESULT_PROC_UNKNOWN
};

#define FS_FILE_NAME_MAX 127

typedef enum FSSeekFileMode { FS_SEEK_SET = 0, FS_SEEK_CUR, FS_SEEK_END } FSSeekFileMode;

struct FSFile;

#define FS_DMA_NOT_USE ((u32)~0)

struct FSDirPos {
    struct FSArchive* arc;
    uint16_t own_id;
    uint16_t index;
    uint32_t pos;
};
ASSERT_SIZE(struct FSDirPos, 12);

struct FSFileID {
    struct FSArchive* arc;
    uint32_t file_id;
};
ASSERT_SIZE(struct FSFileID, 8);

struct FSDirEntry {
    union {
        struct FSFileID file_id;
        struct FSDirPos dir_id;
    };
    uint32_t is_directory;
    uint32_t name_len;
    char name[128];
};
ASSERT_SIZE(struct FSDirEntry, 148);

struct FSSeekDirInfo {
    struct FSDirPos pos;
};
ASSERT_SIZE(struct FSSeekDirInfo, 12);

struct FSReadDirInfo {
    struct FSDirEntry* p_entry;
    uint32_t skip_string; // 4-byte long boolean
};
ASSERT_SIZE(struct FSReadDirInfo, 8);

struct FSFindPathInfo {
    struct FSDirPos pos;
    const char* path;
    uint32_t find_directory; // 4-byte long boolean
    union {
        struct FSFileID* file;
        struct FSDirPos* dir;
    } result;
};
ASSERT_SIZE(struct FSFindPathInfo, 24);

struct FSGetPathInfo {
    uint8_t* buf;
    uint32_t buf_len;
    uint16_t total_len;
    uint16_t dir_id;
};
ASSERT_SIZE(struct FSGetPathInfo, 12);

struct FSOpenFileFastInfo {
    struct FSFileID id;
};
ASSERT_SIZE(struct FSOpenFileFastInfo, 8);

struct FSOpenFileDirectInfo {
    uint32_t top;
    uint32_t bottom;
    uint32_t index;
};
ASSERT_SIZE(struct FSOpenFileDirectInfo, 12);

struct FSCloseFileInfo {
    uint32_t reserved;
};
ASSERT_SIZE(struct FSCloseFileInfo, 4);

struct FSReadFileInfo {
    void* dst;
    uint32_t len_org;
    uint32_t len;
};
ASSERT_SIZE(struct FSReadFileInfo, 12);

struct FSWriteFileInfo {
    const void* src;
    uint32_t len_org;
    uint32_t len;
};
ASSERT_SIZE(struct FSWriteFileInfo, 12);

// A structure that is used to store info about a file or directory, it’s state, and input/output to
// file-reading function.
struct FSFile {
    struct FSFileLink link;        // 0x00
    struct FSArchive* arc;         // 0x08
    uint32_t stat;                 // 0xC
    enum FSCommandType command;    // 0x10
    enum FSResult error;           // 0x14
    struct OSThreadQueue queue[1]; // 0x18
    union {
        struct {
            uint32_t own_id; // 0x20
            uint32_t top;    // 0x24
            uint32_t bottom; // 0x28
            uint32_t pos;    // 0x2C
        } file;
        struct {
            struct FSDirPos pos; // 0x20
            uint32_t parent;     // 0x2C
        } dir;
    } prop; // 0x20

    union {
        struct FSReadFileInfo readfile;   // 0x30
        struct FSWriteFileInfo writefile; // 0x30

        struct FSSeekDirInfo seekdir;               // 0x30
        struct FSReadDirInfo readdir;               // 0x30
        struct FSFindPathInfo findpath;             // 0x30
        struct FSGetPathInfo getpath;               // 0x30
        struct FSOpenFileFastInfo openfilefast;     // 0x30
        struct FSOpenFileDirectInfo openfiledirect; // 0x30
        struct FSCloseFileInfo closefile;           // 0x30
    } arg;                                          // 0x30
};
ASSERT_SIZE(struct FSFile, 72);

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
    struct FSFile opened_file;
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
