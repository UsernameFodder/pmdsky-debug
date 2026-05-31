#ifndef HEADERS_FUNCTIONS_OVERLAY19_H_
#define HEADERS_FUNCTIONS_OVERLAY19_H_

struct bar_item* GetBarItem(enum item_id item_id);
void SetMiracleDrinkStatCountRange(int *param_1,int *param_2);
void SetBadDrinkStatCountRange(int *param_1,int *param_2);
unsigned short * GetBarItemResultWeights(enum item_id param_1);
unsigned short * BarGetDrinkEventWeights(void);
enum monster_id BarGetRandomValidEggRecruit(void);
enum monster_id BarGetValidEggGiverFromGourmetTable(void);
enum monster_id BarGetRandomValidRecruitFromGourmetTable(void);
int BarGetUnlockableDungeonPtr(struct bar_dungeon_unlock **param_1);
int BarPopulateUnlockableDungeons(struct dungeon_id_8 *param_1);
void BarTryClosePortraitBox(void);
undefined4 BarCaseManagerSub(void);
undefined4 BarMaybeSetBarScrollBoxWindowId(undefined4 param_1,undefined4 param_2,undefined4 param_3);
void BarPopulateDrinkableBagItems(void);
void BarZeroInitPreprocessorFlagsAndReturnCase(void);
bool BarZeroInitBufferAndPreprocessorArgsZero(void);
undefined4 BarTrySetEventMainActor(void);
bool BarZeroInitBufferAndPreprocessorArgsOne(void);
undefined4 BarPrintHpIncreaseString(int32_t param_1,int32_t param_2);
undefined4 BarPrintOtherStatIncreaseString(int param_1,int param_2,int param_3);
undefined4 BarPrintIqAndSkills(int param_1);
void BarPrintLevelMaxedOutString(int param_1);
undefined4 BarDrawAndUpdateGainedIqSkills(char *param_1,char *param_2,int param_3,int param_4,int param_5,int param_6,int param_7);
undefined4 BarUpdateDrinkEventNewStats(struct bar_stats_data *param_1,int param_2);
void BarPrintReactionsAndStatIncreases(struct bar_stats_data *param_1);
void BarHandleDrinkEventSubcase(void);
void BarPostDrinkCaseForCustomer(void);
undefined4 BarMaybeIsClosedUnkStringIdMenu(void);
void BarCreateUnkStringIdMenu(undefined4 param_1,undefined4 param_2,struct simple_menu_id_item *param_3,undefined4 param_4,char param_5,uint32_t param_6);
int GetUnkStringIdMenuResult(void);
void BarTryShowPortrait(void);
void BarTryHidePortrait(void);
undefined4 BarInventoryMenuCallback(undefined4 param_1,int param_2,undefined4 param_3);
void BarTryCloseInventoryMenu(void);
void BarTryCloseTeamMovesMenu(void);
undefined4 SpindaBarEntry(void);
undefined4 SpindaBarResume(void);
void SpindaBarDestruct(void);
undefined4 SpindaBarMenuUpdate(void);
#endif
