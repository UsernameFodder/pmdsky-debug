#ifndef HEADERS_FUNCTIONS_OVERLAY15_H_
#define HEADERS_FUNCTIONS_OVERLAY15_H_

void InitDuskullBankStruct(void);
undefined4 DuskullBankSubcaseManager1(void);
void DuskullBankSubcaseManager2(enum bank_subcase subcase);
void BankUpdateDigitInputMenuDisplay(bool is_withdrawing);
void DuskullBankTextboxGoldStatusCallback(int window_id);
void DuskullBankDigitInputDisplayCallback(void);
undefined4 DuskullBankEntryPoint(void);
void DuskullBankDestructor(void);
undefined4 DuskullBankFrameUpdate(void);

#endif
