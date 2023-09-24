#ifndef HEADERS_FUNCTIONS_OVERLAY11_H_
#define HEADERS_FUNCTIONS_OVERLAY11_H_

void UnlockScriptingLock(int lock_id);
void FuncThatCallsRunNextOpcode(undefined* script_engine_state);
void RunNextOpcode(undefined* script_engine_state);
void HandleUnlocks(void);
void LoadFileFromRomVeneer(struct iovec* iov, const char* filepath, uint32_t flags);
void SsbLoad2(void);
uint16_t ProcessScriptParam(uint16_t parameter);
void StationLoadHanger(void);
void ScriptStationLoadTalk(void);
void SsbLoad1(void);
int ScriptSpecialProcessCall(undefined4* param_1, enum special_process_id id, int arg1, int arg2);
bool GetCoroutineInfo(struct coroutine_info* coroutine_info, enum common_routine_id coroutine_id);
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
void LoadBackgroundAttributes(struct bg_list_entry* entry, int bg_id);
void LoadMapType10(void* buf, int map_id, undefined* dungeon_info_str, undefined4 additional_info);
void LoadMapType11(void* buf, int map_id, undefined* dungeon_info_str, undefined4 additional_info);
void GetSpecialLayoutBackground(int bg_id, undefined* dungeon_info_str, undefined4 additional_info,
                                bool copy_fixed_room_layout);
void SetAnimDataFields(struct animation* anim, uint16_t param_2);
void SetAnimDataFieldsWrapper(struct animation* anim, uint32_t param_2);
void InitAnimDataFromOtherAnimData(struct animation* dst, struct animation* src);
void SetAnimDataFields2(struct animation* anim, uint32_t flags, uint32_t param_3);
struct animation_data GetIdleAnimationType(enum monster_id, undefined* param_2);
void LoadObjectAnimData(struct animation* anim, int16_t object_id, uint32_t flags);
void InitAnimDataFromOtherAnimDataVeneer(struct animation* dst, struct animation* src);
void AnimRelatedFunction(struct animation* anim, undefined4 param_2, undefined4 param_3);
void AllocAndInitPartnerFollowDataAndLiveActorList(void);
void InitPartnerFollowDataAndLiveActorList(void);
void DeleteLiveActor(int16_t actor_id);
void ChangeActorAnimation(undefined param_1, uint16_t setanimation_param);
void InitPartnerFollowData(void);
void GetDirectionLiveActor(struct live_actor* actor, struct direction_id_8* target);
void SetDirectionLiveActor(struct live_actor* actor, struct direction_id_8 direction);
int CreateTeamInfoBox(void);
void UpdateTeamInfoBox(struct window* window);
void GetExclusiveItemRequirements(undefined param_1, undefined param_2);
bool GetDungeonMapPos(struct uvec2* out_pos, enum dungeon_id dungeon);
void WorldMapSetMode(uint32_t world_map_mode);
void WorldMapSetCamera(uint32_t map_marker_id);
void StatusUpdate(void);

#endif
