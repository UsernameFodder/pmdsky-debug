#ifndef HEADERS_FUNCTIONS_OVERLAY21_H_
#define HEADERS_FUNCTIONS_OVERLAY21_H_

void SwapShopDialogueManager(int current_switch_case);
enum item_id GetFirstExclusivePrerequisite(enum item_id item_id);
int SwapShopEntryPoint(void);
void SwapShopDestructor(void);
void SwapShopMainManager(void);
void CloseTextboxAndSimpleMenu(void);
void SwapShopPrintCurrentGold(int window_id);

#endif
