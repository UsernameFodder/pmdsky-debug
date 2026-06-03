#ifndef HEADERS_FUNCTIONS_OVERLAY20_H_
#define HEADERS_FUNCTIONS_OVERLAY20_H_



undefined4 RecycleShopEntryPointFirst(void);
undefined4 RecycleShopEntryPointResume(void);
void RecycleShopDestructor(void);
undefined4 RecycleShopFrameUpdate(void);
undefined4 RecycleShopCommonEntryPoint(bool resuming_menu);
void PrintWynautTextbox(int strind_id);
void PrintWynautSimpleMenu(struct recycle_shop_main *param_1,undefined4 string_id,struct simple_menu_id_item *param_3);
void RecycleShopTryHidePortraitBox(struct recycle_shop_main *param_1,bool param_2);
void PrintWynautNormalPortrait(struct recycle_shop_main *param_1);
void RecycleShopClosePortraitBox(struct recycle_shop_main *param_1);
void RecycleShopCloseAllWindows(void);
undefined4 RecycleShopHandleTopScreenWindow(void);
void RecycleShopTopScreenDrawItemStats(int param_1);
void RecycleShopTopScreenDrawItemName(int window_id);
void RecycleShopTopScreenDrawInfoWindow(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4);
void DisplaySelectionTutorialWindow(int window_id,undefined4 param_2,undefined4 param_3,undefined4 param_4);

#endif