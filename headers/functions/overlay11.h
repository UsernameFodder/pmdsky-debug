#ifndef HEADERS_FUNCTIONS_OVERLAY11_H_
#define HEADERS_FUNCTIONS_OVERLAY11_H_

void UnlockScriptingLock(int lock_id);
void FuncThatCallsRunNextOpcode(undefined* script_engine_state);
void RunNextOpcode(undefined* script_engine_state);
void HandleUnlocks(void);
void LoadFileFromRomVeneer(struct iovec* iov, const char* filepath, uint32_t flags);
void SsbLoad2(void);
int16_t ScriptParamToInt(uint16_t parameter);
int16_t ScriptParamToFixedPoint16(uint16_t parameter);
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
bool IsScreenFadeInProgress(void);
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
void SetAttributeBitfieldLiveActor(int16_t live_actor_id, uint32_t attribute_bitfield);
void ResetAttributeBitfieldLiveActor(int16_t live_actor_id, uint32_t attribute_bitfield);
int GetCollidingActorId(int16_t live_actor_id, uint32_t attribute_bitfield, struct vec2* coord_min,
                        struct vec2* coord_max);
void ChangeActorAnimation(struct live_actor* actor, uint16_t setanimation_param);
void SetPositionLiveActor(struct live_actor* actor, struct vec2* pos);
int16_t GetIdLiveActor(struct live_actor* actor);
void GetCollisionBoxLiveActor(struct live_actor* actor, struct uvec2* collision_box);
void SetPositionInitialLiveActor(struct live_actor* actor, struct vec2* offset);
void SetMovementRangeLiveActor(struct live_actor* actor, struct vec2* limit_min_pos,
                               struct vec2* limit_max_pos);
void GetCollisionBoxCenterLiveActor(struct live_actor* actor, struct uvec2* collision_box_center);
void SetPositionLiveActorVeneer(struct live_actor* actor, struct vec2* pos);
void GetHeightLiveActor(struct live_actor* actor, int* height, int* second_height);
void SetHeightLiveActor(struct live_actor* actor, int height);
void GetDirectionLiveActor(struct live_actor* actor, enum direction_id* target);
void SetDirectionLiveActor(struct live_actor* actor, enum direction_id direction);
void SetAnimationLiveActor(struct live_actor* actor, uint16_t setanimation_param);
void SetEffectLiveActor(struct live_actor* actor, bool flag, int16_t effect_id,
                        int position_marker);
int GetAnimationStatusLiveActor(struct live_actor* actor);
int GetEffectStatusLiveActor(struct live_actor* actor);
void GetAttributeBitfieldLiveActor(struct live_actor* actor, uint32_t* attribute_bitfield);
void SetAttributeBitfieldLiveActorWrapper(struct live_actor* actor, uint32_t attribute_bitfield);
void ResetAttributeBitfieldLiveActorWrapper(struct live_actor* actor, uint32_t attribute_bitfield);
void SetBlinkLiveActor(struct live_actor* actor, uint16_t interval_blink_in,
                       uint16_t interval_blink_out);
void SetPositionOffsetLiveActor(struct live_actor* actor, struct vec2* offset);
void InitPartnerFollowData(void);
void SetAttributeBitfieldLiveObject(int16_t live_object_id, uint32_t attribute_bitfield);
void ResetAttributeBitfieldLiveObject(int16_t live_object_id, uint32_t attribute_bitfield);
int GetCollidingObjectId(uint32_t attribute_bitfield, struct vec2* coord_min,
                         struct vec2* coord_max);
void SetPositionLiveObject(struct live_object* object, struct vec2* pos);
int16_t GetIdLiveObject(struct live_object* object);
void GetCollisionBoxLiveObject(struct live_object* object, struct uvec2* collision_box);
void SetPositionInitialLiveObject(struct live_object* object, struct vec2* offset);
void SetMovementRangeLiveObject(struct live_object* object, struct vec2* limit_min_pos,
                                struct vec2* limit_max_pos);
void GetCollisionBoxCenterLiveObject(struct live_object* object,
                                     struct uvec2* collision_box_center);
void SetPositionLiveObjectVeneer(struct live_object* object, struct vec2* pos);
void GetHeightLiveObject(struct live_object* object, int* height, int* second_height);
void SetHeightLiveObject(struct live_object* object, int height);
void GetDirectionLiveObject(struct live_object* object, enum direction_id* target);
void SetDirectionLiveObject(struct live_object* object, enum direction_id direction);
void SetAnimationLiveObject(struct live_object* object, uint16_t setanimation_param);
void SetEffectLiveObject(struct live_object* object, bool flag, int16_t effect_id);
int GetAnimationStatusLiveObject(struct live_object* object);
int GetEffectStatusLiveObject(struct live_object* object);
void GetAttributeBitfieldLiveObject(struct live_object* object, uint32_t* attribute_bitfield);
void SetAttributeBitfieldLiveObjectWrapper(struct live_object* object, uint32_t attribute_bitfield);
void ResetAttributeBitfieldLiveObjectWrapper(struct live_object* object,
                                             uint32_t attribute_bitfield);
void SetBlinkLiveObject(struct live_object* object, uint16_t interval_blink_in,
                        uint16_t interval_blink_out);
void SetPositionOffsetLiveObject(struct live_object* object, struct vec2* offset);
int CreateTeamInfoBox(void);
void CloseTeamInfoBox(int window_id);
bool IsTeamInfoBoxActive(int window_id);
void UpdateTeamInfoBox(struct window* window);
int CreateTopGroundMenu(void);
void CloseTopGroundMenu(void);
int UpdateTopGroundMenu(void);
bool IsBagNotEmpty(void);
void GetExclusiveItemRequirements(undefined param_1, undefined param_2);
void HandleControlsTopScreenGround(void);
bool GetDungeonMapPos(struct uvec2* out_pos, enum dungeon_id dungeon);
void WorldMapSetMode(uint32_t world_map_mode);
void WorldMapSetCamera(uint32_t map_marker_id);
void StatusUpdate(void);
void HandleTeamStatsGround(void);

#endif
