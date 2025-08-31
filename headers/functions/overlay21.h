#ifndef HEADERS_FUNCTIONS_OVERLAY21_H_
#define HEADERS_FUNCTIONS_OVERLAY21_H_

void SwapShopDialogueManager(enum swap_shared_case new_shared_case);
enum item_id GetFirstExclusivePrerequisite(enum item_id item_id);
int SwapShopEntryPoint(void);
void SwapShopDestructor(void);
int SwapShopMainManager(void);
void CloseTextboxAndSimpleMenu(void);
void SwapShopPrintCurrentGold(int window_id);

#endif
