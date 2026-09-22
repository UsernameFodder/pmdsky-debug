#ifndef HEADERS_FUNCTIONS_OVERLAY10_H_
#define HEADERS_FUNCTIONS_OVERLAY10_H_

int CreateInventoryMenu(struct window_params* params, struct window_flags flags,
                        struct window_extra_info* window_extra_info, undefined* param_4,
                        undefined4 param_5, int n_items, int n_items_per_page, uint8_t param_8);
void SetInventoryMenuState0(int window_id);
void SetInventoryMenuState6(int window_id);
void CloseInventoryMenu(int window_id);
bool IsInventoryMenuActive(int window_id);
bool CheckInventoryMenuField0x1A0(int window_id);
uint8_t PopInventoryMenuField0x1A3(int window_id);
void UpdateInventoryMenu(struct window* window);
bool IsInventoryMenuState3(int window_id);
void CreateInventoryMenuOuter(int n_items, int param_2, int param_3, uint8_t* param_4,
                              undefined* param_5, undefined* param_6, uint32_t param_7);
void LoadPaletteBase(uint8_t sprite_index, bool is_4bpp, uint8_t start_4bpp);
void TerminateAllEffects(void);
void TerminateEffect(int unique_id, bool not_special_wan_file);
void TerminateEffectWrapper(int unique_id);
enum wan_offset_type GetEffectAnimationWanOffset(int anim_id);
void UnloadEffect(struct live_effect* effect, bool not_special_wan_file);
void PlayEffect(struct live_effect* effect, enum screen screen);
int InitEffect(undefined4 param_1, undefined4* param_2, enum screen screen);
int GetLiveEffectIdx(int unique_id);
enum wan_offset_type GetLiveEffectWanOffset(int unique_id);
bool DisplayEffect(struct live_effect* effect, struct vec2_16* camera_pos);
bool ScreenEffectActive(enum screen screen);
bool BlockingAnimationInProgress(int unique_id);
bool IsBlizzardTailwindOrPowderSnow(enum move_id move_id);
bool IsWanOrWat(enum effect_file_type effect_file_type);
bool IsScreenOrWba(enum effect_file_type effect_file_type);
struct effect_animation* GetEffectAnimation(int anim_id);
struct move_animation* GetMoveAnimation(enum move_id move_id);
struct special_monster_move_animation* GetSpecialMonsterMoveAnimation(int ent_id);
int16_t GetTrapAnimation(enum trap_id trap_id);
int16_t GetItemAnimation1(enum item_id item_id);
int16_t GetItemAnimation2(enum item_id item_id);
int GetMoveAnimationSpeed(enum move_id move_id);
int LoadEffectWanTableEntry(enum effect_file_type effect_file_type, uint32_t file_index,
                            uint8_t live_effect_idx);
undefined4 DrawTeamStats(undefined4 param_1, int param_2, undefined4 param_3, uint32_t param_4);
void UpdateTeamStats(void);
int FreeTeamStats(void);
int FreeMapAndTeam(void);
void ProcessTeamStatsLvHp(int window_id);
void ProcessTeamStatsNameGender(int window_id);
bool IsBackgroundTileset(int tileset_id);
void InitTilesetBuffer(void* buffer, int tileset_id, uint32_t malloc_flags);
int MainGame(int end_cond);

#endif
