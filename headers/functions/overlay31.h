#ifndef HEADERS_FUNCTIONS_OVERLAY31_H_
#define HEADERS_FUNCTIONS_OVERLAY31_H_

void InitDungeonMenu(void);
void DrawDungeonMenuStatusWindow(int param_1);
void DungeonMenuSwitch(int idx);
int HandleDungeonMenu(void);
void FreeDungeonMenu(void);
void InitStairsMenu(struct entity* entity);
void StairsSubheadingCallback(int window_id);
int HandleStairsMenu(void);
void CloseMainStairsMenu(void);
void FreeStairsMenu(void);
bool EntityIsValidOverlay31(struct entity* entity);
void MovesMenu(struct action_data* action);
bool HandleMovesMenuWrapper0(undefined* param_1, undefined param_2, undefined param_3,
                             int monster_index);
bool HandleMovesMenuWrapper1(undefined* param_1, undefined param_2, undefined param_3,
                             int monster_index);
bool HandleMovesMenu(undefined* param_1, undefined param_2, undefined param_3, int monster_index);
undefined TeamMenu(struct entity* leader);
void RestMenu(void);
int RecruitmentSearchMenuLoop(void);
void DrawDungeonHintContents(int window_id);
int HelpMenuLoop(void);

#endif
