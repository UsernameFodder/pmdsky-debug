#ifndef HEADERS_FUNCTIONS_ARM7_H_
#define HEADERS_FUNCTIONS_ARM7_H_

void EntryArm7(void);
void MainArm7(void);
int ClearIrqFlag(void);
int EnableIrqFlag(void);
int SetIrqFlag(int new_value);
int EnableIrqFiqFlags(void);
int SetIrqFiqFlags(int new_value);
int GetProcessorMode(void);
unsigned long long __divsi3(int dividend, int divisor);
unsigned long long __udivsi3(uint32_t dividend, uint32_t divisor);
unsigned long long __udivsi3_no_zero_check(uint32_t dividend, uint32_t divisor);

#endif
