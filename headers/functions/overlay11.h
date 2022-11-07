#ifndef HEADERS_FUNCTIONS_OVERLAY11_H_
#define HEADERS_FUNCTIONS_OVERLAY11_H_

int ScriptSpecialProcessCall(undefined4* param_1, enum special_process_id id, int arg1, int arg2);
enum monster_id GetSpecialRecruitmentSpecies(int idx);
void PrepareMenuAcceptTeamMember(int idx);
void InitRandomNpcJobs(int job_type, undefined2 param_2);
int GetRandomNpcJobType(void);
int GetRandomNpcJobSubtype(void);
bool GetRandomNpcJobStillAvailable(void);
bool AcceptRandomNpcJob(void);
int GroundMainLoop(int mode);
struct mem_arena* GetAllocArenaGround(struct mem_arena* arena, uint32_t flags);
struct mem_arena* GetFreeArenaGround(struct mem_arena* arena, uint32_t flags);
void GroundMainReturnDungeon(void);
void GroundMainNextDay(void);
bool JumpToTitleScreen(int arg);
bool ReturnToTitleScreen(uint32_t fade_duration);
void ScriptSpecialProcess0x16(bool param_1);
void LoadBackgroundAttributes(char* bg_attr_str, int bg_id);
void LoadMapType10(void* buf, int map_id, undefined* dungeon_info_str, undefined4 additional_info);
void LoadMapType11(void* buf, int map_id, undefined* dungeon_info_str, undefined4 additional_info);
void GetSpecialLayoutBackground(int bg_id, undefined* dungeon_info_str, undefined4 additional_info,
                                bool copy_fixed_room_layout);
void StatusUpdate(void);

#endif
