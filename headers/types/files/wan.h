#ifndef HEADERS_TYPES_FILES_WAN_H_
#define HEADERS_TYPES_FILES_WAN_H_

// For now, there is only the top-level header for WAN files.

// Directly loaded from memory as they are stored. Names comes from the pmd_wan project.
#pragma pack(push, 1)
struct wan_header {
    undefined* anim_header;
    undefined* image_header;
    undefined2 sprite_type;
};
#pragma pack(pop)
ASSERT_SIZE(struct wan_header, 10);

#endif
