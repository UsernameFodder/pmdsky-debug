#ifndef HEADERS_FUNCTIONS_OVERLAY31_H_
#define HEADERS_FUNCTIONS_OVERLAY31_H_

void EntryOverlay31(void);
void DrawDungeonMenuStatusWindow(int param_1);
void DungeonMenuSwitch(int idx);
void MovesMenu(struct action_data* action);
bool HandleMovesMenuWrapper0(undefined* param_1, undefined param_2, undefined param_3,
                             int monster_index);
bool HandleMovesMenuWrapper1(undefined* param_1, undefined param_2, undefined param_3,
                             int monster_index);
bool HandleMovesMenu(undefined* param_1, undefined param_2, undefined param_3, int monster_index);
undefined TeamMenu(struct entity* leader);
void RestMenu(void);
int RecruitmentSearchMenuLoop(void);
int HelpMenuLoop(void);

#endif
