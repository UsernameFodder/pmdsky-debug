#ifndef HEADERS_FUNCTIONS_OVERLAY10_H_
#define HEADERS_FUNCTIONS_OVERLAY10_H_

int CreateInventoryMenu(struct window_params* params, uint32_t param_2, undefined* param_3,
                        undefined* param_4, undefined4 param_5, int param_6, int param_7,
                        uint8_t param_8);
void UpdateInventoryMenu(struct window* window);
int GetEffectAnimationField0x19(int anim_id);
bool AnimationHasMoreFrames(int param_1);
struct effect_animation* GetEffectAnimation(int anim_id);
struct move_animation* GetMoveAnimation(enum move_id move_id);
struct special_monster_move_animation* GetSpecialMonsterMoveAnimation(int ent_id);
int16_t GetTrapAnimation(enum trap_id trap_id);
int16_t GetItemAnimation1(enum item_id item_id);
int16_t GetItemAnimation2(enum item_id item_id);
int GetMoveAnimationSpeed(enum move_id move_id);
void ProcessTeamStatsLvHp(int idx);
void ProcessTeamStatsNameGender(int idx);
bool IsBackgroundTileset(int tileset_id);
void InitTilesetBuffer(void* buffer, int tileset_id, uint32_t malloc_flags);
int MainGame(int end_cond);

#endif
