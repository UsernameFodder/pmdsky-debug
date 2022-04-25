#ifndef HEADERS_FUNCTIONS_ARM9_H_
#define HEADERS_FUNCTIONS_ARM9_H_

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
void FileOpen(struct file_stream* file, char* filepath);
uint32_t FileGetSize(struct file_stream* file);
uint32_t FileRead(struct file_stream* file, void* buf, uint32_t size);
void FileSeek(struct file_stream* file, int offset, int whence);
void FileClose(struct file_stream* file);
void LoadFileFromRom(struct iovec* iov, char* filepath, uint32_t flags);
uint32_t GetDebugFlag1(uint32_t flag_id);
void SetDebugFlag1(uint32_t flag_id, uint32_t val);
int AppendProgPos(char* str, struct prog_pos_info* prog_pos, char* msg);
void DebugPrintTrace(char* msg, struct prog_pos_info* prog_pos);
void DebugPrint0(char* fmt, ...);
uint32_t GetDebugFlag2(uint32_t flag_id);
void SetDebugFlag2(uint32_t flag_id, uint32_t val);
void DebugPrint(uint8_t level, char* fmt, ...);
void FatalError(struct prog_pos_info prog_pos, char* fmt, ...);
bool IsAuraBow(enum item_id item_id);
int SprintfStatic(char* str, char* format, ...);
bool IsBagFull(void);
int CountItemTypeInBag(enum item_id);
bool AddItemToBag(struct owned_item* item);
bool ScriptSpecialProcess0x39(void);
int CountItemTypeInStorage(struct owned_item* item);
bool RemoveItemsTypeInStorage(struct owned_item* item);
bool AddItemToStorage(struct owned_item* item);
int GetExclusiveItemOffset(enum item_id item_id);
void ApplyExclusiveItemStatBoosts(enum item_id item_id, uint8_t* atk_boost, uint8_t* sp_atk_boost,
                                  uint8_t* def_boost, uint8_t* sp_def_boost);
void SetExclusiveItemEffect(uint32_t* effect_flags, enum exclusive_item_effect_id effect_id);
bool ExclusiveItemEffectFlagTest(uint32_t* effect_flags, enum exclusive_item_effect_id effect_id);
struct move_target_and_range GetMoveTargetAndRange(struct move* move, bool is_ai);
int GetMoveBasePower(struct move* move);
int GetMaxPp(struct move* move);
int GetMoveCritChance(struct move* move);
bool IsRecoilMove(enum move_id move_id);
bool IsPunchMove(enum move_id move_id);
enum move_category GetMoveCategory(enum move_id move_id);
void LoadWteFromRom(struct wte_handle* handle, char* path, uint32_t flags);
void LoadWteFromFileDirectory(struct wte_handle* handle, uint16_t pack_file_id, uint16_t file_index,
                              uint32_t malloc_flags);
void UnloadWte(struct wte_handle* handle);
void HandleSir0Translation(uint8_t** dst, uint8_t* src);
void HandleSir0TranslationVeneer(uint8_t** dst, uint8_t* src);
int GetLanguage(void);
int PreprocessString(char* output, int output_size, char* format, struct preprocessor_flags flags,
                     struct preprocessor_args* args);
void StrcpySimple(char* dest, char* src);
void StrncpySimple(char* dest, char* src, uint32_t n);
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
void ScriptSpecialProcess0x3A(void);
void ScriptSpecialProcess0x3B(void);
void SendSkyGiftToGuildmaster(void);
uint32_t SubFixedPoint(uint32_t val_fp, uint32_t dec_fp);
uint32_t BinToDecFixedPoint(uint32_t* q16);
int CeilFixedPoint(uint32_t val_fp);
bool DungeonGoesUp(enum dungeon_id dungeon_id);
bool IsUnown(enum monster_id monster_id);
bool IsShaymin(enum monster_id monster_id);
bool IsCastform(enum monster_id monster_id);
bool IsCherrim(enum monster_id monster_id);
bool IsDeoxys(enum monster_id monster_id);
bool IsMonsterOnTeam(enum monster_id monster_id, int param_2);
void SetTeamSetupHeroAndPartnerOnly(void);
void SetTeamSetupHeroOnly(void);
int GetPartyMembers(uint16_t* party_members);
bool IqSkillFlagTest(uint32_t* iq_skill_flags, enum iq_skill_id iq_id);
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
int Mbtowc(wchar_t* pwc, char* s, uint32_t n);
bool TryAssignByte(uint8_t* ptr, uint8_t val);
bool TryAssignByteWrapper(uint8_t* ptr, uint8_t val);
uint32_t Wcstombs(char* dest, wchar_t* src, uint32_t n);
void Memcpy(void* dest, void* src, uint32_t n);
void Memmove(void* dest, void* src, uint32_t n);
void* Memset(void* s, int c, uint32_t n);
void* Memchr(void* s, char c, uint32_t n);
int Memcmp(void* s1, void* s2, uint32_t n);
void MemsetInternal(void* s, int c, uint32_t n);
int VsprintfInternalSlice(slice_append_fn_t append, struct slice* str, char* format, va_list ap);
bool TryAppendToSlice(struct slice* slice, void* data, uint32_t data_len);
int VsprintfInternal(char* str, uint32_t maxlen, char* format, va_list ap);
int Vsprintf(char* str, char* format, va_list ap);
int Snprintf(char* str, uint32_t n, char* format, ...);
int Sprintf(char* str, char* format, ...);
uint32_t Strlen(char* s);
void Strcpy(char* dest, char* src);
void Strncpy(char* dest, char* src, uint32_t n);
void Strcat(char* dest, char* src);
void Strncat(char* dest, char* src, uint32_t n);
int Strcmp(char* s1, char* s2);
int Strncmp(char* s1, char* s2, uint32_t n);
char* Strchr(char* string, char c);
uint32_t Strcspn(char* string, char* stopset);
char* Strstr(char* haystack, char* needle);
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
