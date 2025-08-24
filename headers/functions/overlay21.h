#ifndef HEADERS_FUNCTIONS_OVERLAY21_H_
#define HEADERS_FUNCTIONS_OVERLAY21_H_

void SwapShopDialogueManager(int current_switch_case);
struct item_id GetFirstExclusivePrerequisite(struct item_id);
int SwapShopEntryPoint(void);
void SwapShopDestructor(void);
void SwapShopMainManager(void);
void CloseTextboxAndSimpleMenu(void);
void SwapShopPrintCurrentGold(int window_id);
#endif
