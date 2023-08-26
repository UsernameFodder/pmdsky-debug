#ifndef HEADERS_FUNCTIONS_OVERLAY31_H_
#define HEADERS_FUNCTIONS_OVERLAY31_H_

void EntryOverlay31(void);
void DungeonMenuSwitch(int idx);
void MovesMenu(struct action_data* action);
bool HandleMovesMenu(struct entity** entity, undefined param_2, undefined param_3,
                     int monster_index);
undefined TeamMenu(struct entity* leader);
void RestMenu(void);
int RecruitmentSearchMenuLoop(void);
int HelpMenuLoop(void);

#endif
