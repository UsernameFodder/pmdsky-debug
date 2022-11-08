#ifndef HEADERS_FUNCTIONS_OVERLAY10_H_
#define HEADERS_FUNCTIONS_OVERLAY10_H_

struct effect_animation* GetEffectAnimation(int anim_id);
struct move_animation* GetMoveAnimation(enum move_id move_id);
struct special_monster_move_animation* GetSpecialMonsterMoveAnimation(int ent_id);
int16_t GetTrapAnimation(enum trap_id trap_id);
int16_t GetItemAnimation1(enum item_id item_id);
int16_t GetItemAnimation2(enum item_id item_id);
int GetMoveAnimationSpeed(enum move_id move_id);
int CheckEndDungeon(int end_cond);

#endif
