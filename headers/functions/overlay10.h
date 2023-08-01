#ifndef HEADERS_FUNCTIONS_OVERLAY10_H_
#define HEADERS_FUNCTIONS_OVERLAY10_H_

int GetEffectAnimationField0x19(int anim_id);
bool AnimationHasMoreFrames(int param_1);
struct effect_animation* GetEffectAnimation(int anim_id);
struct move_animation* GetMoveAnimation(enum move_id move_id);
struct special_monster_move_animation* GetSpecialMonsterMoveAnimation(int ent_id);
int16_t GetTrapAnimation(enum trap_id trap_id);
int16_t GetItemAnimation1(enum item_id item_id);
int16_t GetItemAnimation2(enum item_id item_id);
int GetMoveAnimationSpeed(enum move_id move_id);
bool IsBackgroundTileset(int tileset_id);
int MainGame(int end_cond);

#endif
