#ifndef HEADERS_FUNCTIONS_OVERLAY13_H_
#define HEADERS_FUNCTIONS_OVERLAY13_H_

void EntryOverlay13(void);
void ExitOverlay13(void);
int Overlay13SwitchFunctionNa238A1C8(void);
void Overlay13SwitchFunctionNa238A574(void);
int GetPersonality(void); // TODO: Change return type to enum
char* GetOptionStringFromID(char* output, int option_id);
void WaitForNextStep(int switch_case);

#endif
