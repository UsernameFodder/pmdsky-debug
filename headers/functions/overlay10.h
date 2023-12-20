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
int GetEffectAnimationField0x19(int anim_id);
bool AnimationHasMoreFrames(int param_1);
struct effect_animation* GetEffectAnimation(int anim_id);
struct move_animation* GetMoveAnimation(enum move_id move_id);
struct special_monster_move_animation* GetSpecialMonsterMoveAnimation(int ent_id);
int16_t GetTrapAnimation(enum trap_id trap_id);
int16_t GetItemAnimation1(enum item_id item_id);
int16_t GetItemAnimation2(enum item_id item_id);
int GetMoveAnimationSpeed(enum move_id move_id);
undefined4 DrawTeamStats(undefined4 param_1, int param_2, undefined4 param_3, uint32_t param_4);
void UpdateTeamStats(void);
int FreeTeamStats(void);
int FreeMapAndTeam(void);
void ProcessTeamStatsLvHp(int idx);
void ProcessTeamStatsNameGender(int idx);
bool IsBackgroundTileset(int tileset_id);
void InitTilesetBuffer(void* buffer, int tileset_id, uint32_t malloc_flags);
int MainGame(int end_cond);

#endif
