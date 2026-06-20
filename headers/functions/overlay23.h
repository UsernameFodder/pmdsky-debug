#ifndef HEADERS_FUNCTIONS_OVERLAY23_H_
#define HEADERS_FUNCTIONS_OVERLAY23_H_

void KangaskhanStorageSubcaseManager1(int32_t subcase_id);
void KangaskhanStorageSubcaseManager2(void);
undefined4 KangaskhanStorageEntryPoint(void);
void KangaskhanStorageDestructor(void);
undefined4 KangaskhanStorageFrameUpdate(void);
void KangaskhanStorageCloseSimpleMenu(void);
void KangaskhanStorageShowDialogueAndPortraitIfNotRock(int window_id,
                                                       struct preprocessor_flags flags,
                                                       int string_id);
void KangaskhanStorageWithdrawSelectedItems(void);
void KangaskhanStorageDepositSelectedItems(void);
void KangaskhanStorageWithdrawSingleItem(void);
void KangaskhanStorageDepositSingleItem(void);

#endif
