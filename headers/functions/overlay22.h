#ifndef HEADERS_FUNCTIONS_OVERLAY22_H_
#define HEADERS_FUNCTIONS_OVERLAY22_H_

void GreenKecleonShopUpdateItemNamesAndCollectionMenu(void);
int GreenKecleonShopInitCollectionMenu(void);
void GreenKecleonShopDoNothing(void);
undefined4 GreenKecleonShopGetCollectionMenuStatus(void);
uint16_t GreenKecleonShopGetSelectedItemSlot(void);
void GreenKecleonShopInitItemNameData(void);
void GreenKecleonShopFreeItemNameData(void);
void GreenKecleonShopFillItemNameData(void);
int GreenKecleonShopCountSelectedItems(void);
int GreenKecleonShopGetFirstSelectedItemIndex(void);
int GreenKecleonShopSumSelectedItemPrices(void);
void GreenKecleonShopPurchaseSingleItem(uint32_t param_1);
void GreenKecleonShopPurchaseSelectedItems(void);
char* GreenKecleonGetItemNameStringByIndex(undefined4 param_1,int param_2,uint32_t *param_3);
undefined4 GreenKecleonShopUnkCollectionMenuCallback(uint32_t param_1);
void GreenKecleonShopVoidFnCollectionMenuCallback(uint32_t param_1);
void PurpleKecleonShopUpdateItemNamesAndCollectionMenu(void);
int PurpleKecleonShopInitCollectionMenu(void);
void PurpleKecleonShopDoNothing(void);
undefined4 PurpleKecleonShopGetCollectionMenuStatus(void);
void PurpleKecleonShopInitItemNameData(void);
void PurpleKecleonShopFreeItemNameData(void);
void PurpleKecleonShopFillItemNameData(void);
int PurpleKecleonShopCountSelectedItems(void);
int PurpleKecleonShopGetFirstSelectedItemIndex(void);
int PurpleKecleonShopSumSelectedItemPrices(void);
void PurpleKecleonShopPurchaseSingleItem(uint32_t param_1);
void PurpleKecleonShopPurchaseSelectedItems(void);
char* PurpleKecleonGetItemNameStringByIndex(undefined4 param_1,int param_2,uint32_t *param_3);
undefined4 PurpleKecleonShopUnkCollectionMenuCallback(uint32_t param_1);
void PurpleKecleonShopCollectionMenuCallback(uint32_t param_1);
void KecleonShopSubcaseManager1(undefined4 param_1);
void KecleonShopSubcaseManager2(void);
void RemoveInvalidKecleonShopItems(void);
void KecleonShopSumBagItemSellPrices(void);
void KecleonShopUpdatePortraitEmotion(bool is_angry);
undefined4 KecleonShopEntryPoint(void);
void KecleonShopDestructor(void);
undefined4 KecleonShopFrameUpdate(void);
void KecleonShopCloseSimpleMenu(void);

#endif
