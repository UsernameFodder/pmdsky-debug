#ifndef HEADERS_FUNCTIONS_ARM9_H_
#define HEADERS_FUNCTIONS_ARM9_H_

bool ShouldMonsterRunAwayVariationOutlawCheck(struct entity* monster, undefined param_2);
void AiMovement(struct entity* monster, undefined param_2);
void CalculateAiTargetPos(struct entity* monster);
void ChooseAiMove(struct entity* monster);
void InitMemAllocTable(void);
void SetMemAllocatorParams(get_alloc_arena_fn_t get_alloc_arena,
                           get_free_arena_fn_t get_free_arena);
struct mem_arena* GetAllocArenaDefault(struct mem_arena* arena, uint32_t flags);
struct mem_arena* GetFreeArenaDefault(struct mem_arena* arena, uint32_t flags);
void InitMemArena(struct mem_arena* arena, struct iovec* mem, struct mem_block* blocks,
                  uint32_t max_blocks);
uint32_t MemAllocFlagsToBlockType(uint32_t alloc_flags);
int FindAvailableMemBlock(struct mem_arena* arena, uint32_t alloc_flags, uint32_t len);
struct mem_block* SplitMemBlock(struct mem_arena* arena, uint32_t idx, uint32_t alloc_flags,
                                uint32_t len, uint32_t user_flags);
void* MemAlloc(uint32_t len, uint32_t flags);
void MemFree(void* ptr);
struct mem_arena* MemArenaAlloc(struct mem_arena* parent_arena, uint32_t len, uint32_t max_blocks,
                                uint32_t flags);
struct mem_arena* CreateMemArena(struct iovec* mem, uint32_t max_blocks);
void* MemLocateSet(struct mem_arena* arena, uint32_t len, uint32_t flags);
void MemLocateUnset(struct mem_arena* arena, void* ptr);
int RoundUpDiv256(int x);
int MultiplyByFixedPoint(int x, int mult_fp);
uint32_t UMultiplyByFixedPoint(uint32_t x, uint32_t mult_fp);
uint16_t GetRngSeed(void);
void SetRngSeed(uint16_t seed);
uint16_t Rand16Bit(void);
uint32_t RandInt(uint32_t n);
int RandRange(int x, int y);
uint32_t Rand32Bit(void);
uint32_t RandIntSafe(uint32_t n);
int RandRangeSafe(int x, int y);
void WaitForever(void);
void InitMemAllocTableVeneer(void);
void MemZero(void* ptr, uint32_t len);
void MemcpySimple(void* dest, void* src, uint32_t n);
void TaskProcBoot(void);
bool EnableAllInterrupts(void);
float GetTime(void);
bool DisableAllInterrupts(void);
bool SoundResume(void);
void CardPullOutWithStatus(int status);
void CardPullOut(void);
void CardBackupError(void);
void HaltProcessDisp(int status);
bool OverlayIsLoaded(enum overlay_group_id group_id);
void LoadOverlay(enum overlay_group_id group_id);
void UnloadOverlay(enum overlay_group_id group_id);
float EuclideanNorm(int* vec2);
void ClampComponentAbs(int* vec2, int max);
void KeyWaitInit(void);
void DataTransferInit(void);
void DataTransferStop(void);
void FileInitVeneer(struct file_stream* file);
void FileOpen(struct file_stream* file, const char* filepath);
uint32_t FileGetSize(struct file_stream* file);
uint32_t FileRead(struct file_stream* file, void* buf, uint32_t size);
void FileSeek(struct file_stream* file, int offset, int whence);
void FileClose(struct file_stream* file);
void LoadFileFromRom(struct iovec* iov, const char* filepath, uint32_t flags);
uint32_t GetDebugFlag1(uint32_t flag_id);
void SetDebugFlag1(uint32_t flag_id, uint32_t val);
int AppendProgPos(char* str, struct prog_pos_info* prog_pos, const char* msg);
void DebugPrintTrace(const char* msg, struct prog_pos_info* prog_pos);
void DebugPrint0(const char* fmt, ...);
uint32_t GetDebugFlag2(uint32_t flag_id);
void SetDebugFlag2(uint32_t flag_id, uint32_t val);
void DebugPrint(uint8_t level, const char* fmt, ...);
void FatalError(struct prog_pos_info prog_pos, const char* fmt, ...);
bool IsAuraBow(enum item_id item_id);
int SprintfStatic(char* str, const char* format, ...);
void SetMoneyCarried(int amount);
bool IsBagFull(void);
int CountItemTypeInBag(enum item_id);
bool AddItemToBag(struct owned_item* item);
bool ScriptSpecialProcess0x39(void);
int CountItemTypeInStorage(struct owned_item* item);
bool RemoveItemsTypeInStorage(struct owned_item* item);
bool AddItemToStorage(struct owned_item* item);
void SetMoneyStored(int amount);
int GetExclusiveItemOffset(enum item_id item_id);
void ApplyExclusiveItemStatBoosts(enum item_id item_id, uint8_t* atk_boost, uint8_t* sp_atk_boost,
                                  uint8_t* def_boost, uint8_t* sp_def_boost);
void SetExclusiveItemEffect(uint32_t* effect_flags, enum exclusive_item_effect_id effect_id);
bool ExclusiveItemEffectFlagTest(uint32_t* effect_flags, enum exclusive_item_effect_id effect_id);
void ApplyGummiBoostsGroundMode(undefined2* param_1, undefined2* param_2, undefined* param_3,
                                undefined* param_4, undefined2 param_5, undefined param_6,
                                void* buffer);
struct move_target_and_range GetMoveTargetAndRange(struct move* move, bool is_ai);
enum type_id GetMoveType(struct move* move);
uint8_t GetMoveAiWeight(struct move* move);
int GetMoveBasePower(struct move* move);
uint8_t GetMoveAccuracyOrAiChance(struct move* move, int which);
int GetMaxPp(struct move* move);
int GetMoveCritChance(struct move* move);
bool IsMoveRangeString19(struct move* move);
bool IsRecoilMove(enum move_id move_id);
bool IsPunchMove(enum move_id move_id);
enum move_category GetMoveCategory(enum move_id move_id);
void LoadWteFromRom(struct wte_handle* handle, const char* path, uint32_t flags);
void LoadWteFromFileDirectory(struct wte_handle* handle, uint16_t pack_file_id, uint16_t file_index,
                              uint32_t malloc_flags);
void UnloadWte(struct wte_handle* handle);
void HandleSir0Translation(uint8_t** dst, uint8_t* src);
void HandleSir0TranslationVeneer(uint8_t** dst, uint8_t* src);
int GetLanguageType(void);
int GetLanguage(void);
int PreprocessString(char* output, int output_size, const char* format,
                     struct preprocessor_flags flags, struct preprocessor_args* args);
void StrcpySimple(char* dest, const char* src);
void StrncpySimple(char* dest, const char* src, uint32_t n);
char* StringFromMessageId(int message_id);
void SetScreenWindowsColor(int palette_idx, bool upper_screen);
void SetBothScreensWindowsColor(int palette_idx);
bool GetNotifyNote(void);
void SetNotifyNote(bool flag);
void InitMainTeamAfterQuiz(void);
void ScriptSpecialProcess0x3(void);
void ScriptSpecialProcess0x4(void);
int NoteSaveBase(int param_1);
int NoteLoadBase(void);
int GetGameMode(void);
void InitScriptVariableValues(void);
void InitEventFlagScriptVars(void);
void ZinitScriptVariable(void* local_var_vals, enum script_var_id id);
void LoadScriptVariableRaw(struct script_var_desc* var, void* local_var_vals,
                           enum script_var_id id);
int LoadScriptVariableValue(void* local_var_vals, enum script_var_id id);
int LoadScriptVariableValueAtIndex(void* local_var_vals, enum script_var_id id, int idx);
void SaveScriptVariableValue(void* local_var_vals, enum script_var_id id, int val);
void SaveScriptVariableValueAtIndex(void* local_var_vals, enum script_var_id id, int idx, int val);
int LoadScriptVariableValueSum(void* local_var_vals, enum script_var_id id);
void LoadScriptVariableValueBytes(enum script_var_id id, void* dest, uint32_t n);
void SaveScriptVariableValueBytes(enum script_var_id id, void* src, uint32_t n);
bool ScriptVariablesEqual(void* local_var_vals, enum script_var_id id1, enum script_var_id id2);
void EventFlagBackup(void);
int DumpScriptVariableValues(void* dest);
bool RestoreScriptVariableValues(void* src);
void InitScenarioScriptVars(void);
void SetScenarioScriptVar(enum script_var_id id, uint8_t val0, uint8_t val1);
int GetSpecialEpisodeType(void);
void ScenarioFlagBackup(void);
void InitWorldMapScriptVars(void);
void InitDungeonListScriptVars(void);
void SetDungeonTipShown(int tip_id);
bool GetDungeonTipShown(int tip_id);
bool MonsterSpawnsEnabled(void);
void SetAdventureLogStructLocation(void);
void SetAdventureLogDungeonFloor(struct dungeon_floor_pair dungeon_floor);
struct dungeon_floor_pair GetAdventureLogDungeonFloor(void);
void ClearAdventureLogStruct(void);
void SetAdventureLogCompleted(uint32_t entry_id);
bool IsAdventureLogNotEmpty(void);
bool GetAdventureLogCompleted(uint32_t entry_id);
void IncrementNbDungeonsCleared(void);
uint32_t GetNbDungeonsCleared(void);
void IncrementNbFriendRescues(void);
uint32_t GetNbFriendRescues(void);
void IncrementNbEvolutions(void);
uint32_t GetNbEvolutions(void);
void IncrementNbSteals(void);
void IncrementNbEggsHatched(void);
uint32_t GetNbEggsHatched(void);
uint32_t GetNbPokemonJoined(void);
uint32_t GetNbMovesLearned(void);
void SetVictoriesOnOneFloor(uint32_t nb_victories);
uint32_t GetVictoriesOnOneFloor(void);
void SetPokemonJoined(enum monster_id monster_id);
void SetPokemonBattled(enum monster_id monster_id);
uint32_t GetNbPokemonBattled(void);
void IncrementNbBigTreasureWins(void);
void SetNbBigTreasureWins(uint32_t nb_big_treasure_wins);
uint32_t GetNbBigTreasureWins(void);
void SetNbRecycled(uint32_t nb_recycled);
uint32_t GetNbRecycled(void);
void IncrementNbSkyGiftsSent(void);
void SetNbSkyGiftsSent(uint32_t nb_sky_gifts_sent);
uint32_t GetNbSkyGiftsSent(void);
void ComputeSpecialCounters(void);
void RecruitSpecialPokemonLog(enum monster_id monster_id);
void IncrementNbFainted(void);
uint32_t GetNbFainted(void);
void SetItemAcquired(enum item_id item_id);
uint32_t GetNbItemAcquired(void);
void SetChallengeLetterCleared(uint32_t challenge_id);
uint32_t GetSentryDutyGamePoints(int32_t rank);
int32_t SetSentryDutyGamePoints(uint32_t points);
uint32_t SubFixedPoint(uint32_t val_fp, uint32_t dec_fp);
uint32_t BinToDecFixedPoint(uint32_t* q16);
int CeilFixedPoint(uint32_t val_fp);
bool DungeonGoesUp(enum dungeon_id dungeon_id);
int8_t GetMaxRescueAttempts(enum dungeon_id dungeon_id);
bool JoinedAtRangeCheck(struct dungeon_id_8 joined_at);
bool ShouldCauseGameOverOnFaint(struct dungeon_id_8 joined_at);
uint8_t GetMonsterGender(enum monster_id monster_id);
uint8_t GetSpriteSize(enum monster_id monster_id);
uint8_t GetSpriteFileSize(enum monster_id monster_id);
bool GetCanMoveFlag(enum monster_id monster_id);
enum monster_id GetMonsterPreEvolution(enum monster_id monster_id);
int GetEvolutions(enum monster_id monster_id, enum monster_id* output_list,
                  bool skip_sprite_size_check, bool skip_shedinja_check);
enum monster_id GetMonsterIdFromSpawnEntry(struct monster_spawn_entry*);
uint8_t GetMonsterGenderVeneer(enum monster_id monster_id);
uint8_t GetMonsterLevelFromSpawnEntry(struct monster_spawn_entry* entry);
bool IsUnown(enum monster_id monster_id);
bool IsShaymin(enum monster_id monster_id);
bool IsCastform(enum monster_id monster_id);
bool IsCherrim(enum monster_id monster_id);
bool IsDeoxys(enum monster_id monster_id);
bool IsMonsterOnTeam(enum monster_id monster_id, int param_2);
struct team_member* GetTeamMemberData(uint8_t index);
void SetTeamSetupHeroAndPartnerOnly(void);
void SetTeamSetupHeroOnly(void);
int GetPartyMembers(uint16_t* party_members);
bool IqSkillFlagTest(uint32_t* iq_skill_flags, enum iq_skill_id iq_id);
struct ground_monster* GetExplorerMazeMonster(uint8_t entry_number);
int GetSosMailCount(int param_1, bool param_2);
int DungeonRequestsDone(uint8_t param_1, bool param_2);
int DungeonRequestsDoneWrapper(uint8_t param_1);
bool AnyDungeonRequestsDone(uint8_t param_1);
void ScriptSpecialProcess0x3D(void);
void ScriptSpecialProcess0x3E(void);
void ScriptSpecialProcess0x17(void);
void ItemAtTableIdx(int idx, struct owned_item* item);
void WaitForInterrupt(void);
void FileInit(struct file_stream* file);
int Abs(int x);
int Mbtowc(wchar_t* pwc, const char* s, uint32_t n);
bool TryAssignByte(uint8_t* ptr, uint8_t val);
bool TryAssignByteWrapper(uint8_t* ptr, uint8_t val);
uint32_t Wcstombs(char* dest, wchar_t* src, uint32_t n);
void Memcpy(void* dest, void* src, uint32_t n);
void Memmove(void* dest, void* src, uint32_t n);
void* Memset(void* s, int c, uint32_t n);
void* Memchr(void* s, char c, uint32_t n);
int Memcmp(void* s1, void* s2, uint32_t n);
void MemsetInternal(void* s, int c, uint32_t n);
int VsprintfInternalSlice(slice_append_fn_t append, struct slice* str, const char* format,
                          va_list ap);
bool TryAppendToSlice(struct slice* slice, void* data, uint32_t data_len);
int VsprintfInternal(char* str, uint32_t maxlen, const char* format, va_list ap);
int Vsprintf(char* str, const char* format, va_list ap);
int Snprintf(char* str, uint32_t n, const char* format, ...);
int Sprintf(char* str, const char* format, ...);
uint32_t Strlen(const char* s);
void Strcpy(char* dest, const char* src);
void Strncpy(char* dest, const char* src, uint32_t n);
void Strcat(char* dest, const char* src);
void Strncat(char* dest, const char* src, uint32_t n);
int Strcmp(const char* s1, const char* s2);
int Strncmp(const char* s1, const char* s2, uint32_t n);
char* Strchr(const char* string, char c);
uint32_t Strcspn(const char* string, const char* stopset);
char* Strstr(const char* haystack, const char* needle);
uint32_t Wcslen(wchar_t* ws);
float AddFloat(float a, float b);
float DivideFloat(float dividend, float divisor);
double FloatToDouble(float f);
int FloatToInt(float f);
float IntToFloat(int i);
float UIntToFloat(uint32_t u);
float MultiplyFloat(float a, float b);
float Sqrtf(float x);
float SubtractFloat(float a, float b);
int DivideInt(int dividend, int divisor);
uint32_t DivideUInt(uint32_t dividend, uint32_t divisor);
uint32_t DivideUIntNoZeroCheck(uint32_t dividend, uint32_t divisor);

#endif
