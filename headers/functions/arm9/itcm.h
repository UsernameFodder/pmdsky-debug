#ifndef HEADERS_FUNCTIONS_ARM9_ITCM_H_
#define HEADERS_FUNCTIONS_ARM9_ITCM_H_

struct render_3d_element* AllocateRender3dElement(void);
void Render3dStack(void);
void GetKeyN2MSwitch(int key, int sw);
enum monster_id GetKeyN2M(int key);
enum monster_id GetKeyN2MBaseForm(int key);
void GetKeyM2NSwitch(enum monster_id monster_id, int sw);
int GetKeyM2N(enum monster_id monster_id);
int GetKeyM2NBaseForm(enum monster_id monster_id);
bool ShouldMonsterRunAwayVariationOutlawCheck(struct entity* monster, undefined param_2);
void AiMovement(struct entity* monster, undefined param_2);
void CalculateAiTargetPos(struct entity* monster);
void ChooseAiMove(struct entity* monster);
bool LightningRodStormDrainCheck(struct entity* attacker, struct entity* defender,
                                 struct move* move, bool storm_drain);

#endif
