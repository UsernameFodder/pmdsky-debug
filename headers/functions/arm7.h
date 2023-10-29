#ifndef HEADERS_FUNCTIONS_ARM7_H_
#define HEADERS_FUNCTIONS_ARM7_H_

void EntryArm7(void);
int ClearIrqFlag(void);
int EnableIrqFlag(void);
int SetIrqFlag(int new_value);
int EnableIrqFiqFlags(void);
int SetIrqFiqFlags(int new_value);
int GetProcessorMode(void);

#endif
