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
void LoadBgListEntry(struct bg_list_entry* entry, uint32_t bg_id);
void StatusUpdate(void);

#endif
