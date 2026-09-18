#ifndef HEADERS_FUNCTIONS_OVERLAY24_H_
#define HEADERS_FUNCTIONS_OVERLAY24_H_

int ChanseyDaycareEntrypoint(void);
void ChanseyDaycareDestructor(void);
int ChanseyDaycareFrameUpdate(void);
void ChanseyDaycareSubcaseManager(int state);
void MaybeSetUpMonsterFromEgg(undefined *param_1, enum monster_id species, char* name_buffer, undefined *param_4, int param_5, undefined param_6);
ground_monster* MaybeAddGroundMonsterFromEggToTeam(void);

#endif