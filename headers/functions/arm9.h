#ifndef HEADERS_FUNCTIONS_ARM9_H_
#define HEADERS_FUNCTIONS_ARM9_H_

void* MemAlloc(uint32_t len, uint32_t flags);
void MemFree(void* ptr);
int RoundUpDiv256(int x);
int MultiplyByFixedPoint(int x, int mult_fp);
uint32_t UMultiplyByFixedPoint(uint32_t x, uint32_t mult_fp);
void MemZero(void* ptr, uint32_t len);
void TaskProcBoot(void);
bool SoundResume(void);
void CardPullOutWithStatus(int status);
void CardPullOut(void);
void CardBackupError(void);
void HaltProcessDisp(int status);
bool OverlayIsLoaded(enum overlay_group_id group_id);
void LoadOverlay(enum overlay_group_id group_id);
void UnloadOverlay(enum overlay_group_id group_id);
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
void DebugPrint0(char* fmt, ...);
uint32_t GetDebugFlag2(uint32_t flag_id);
void SetDebugFlag2(uint32_t flag_id, uint32_t val);
void DebugPrint(uint8_t level, char* fmt, ...);
bool IsAuraBow(enum item_id item_id);
void Sprintf(char* str, char* format, ...);
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
void Strcpy(char* dest, char* src);
void Strncpy(char* dest, char* src, uint32_t num);
char* StringFromMessageId(int message_id);
uint32_t SubFixedPoint(uint32_t val_fp, uint32_t dec_fp);
uint32_t BinToDecFixedPoint(uint32_t* q16);
int CeilFixedPoint(uint32_t val_fp);
bool IsUnown(enum monster_id monster_id);
bool IsShaymin(enum monster_id monster_id);
bool IsCastform(enum monster_id monster_id);
bool IsCherrim(enum monster_id monster_id);
bool IsDeoxys(enum monster_id monster_id);
bool IqSkillFlagTest(uint32_t* iq_skill_flags, enum iq_skill_id iq_id);
void FileInit(struct file_stream* file);
int Abs(int x);
int DivideInt(int dividend, int divisor);
uint32_t DivideUInt(uint32_t dividend, uint32_t divisor);
uint32_t DivideUIntNoZeroCheck(uint32_t dividend, uint32_t divisor);

#endif
