#ifndef HEADERS_FUNCTIONS_OVERLAY13_H_
#define HEADERS_FUNCTIONS_OVERLAY13_H_

void PersonalityTestEntryPoint(void);
void PersonalityTestDestructor(void);
void PersonalityTestFrameUpdate(void);
void PersonalityTestMainManager(void);
int GetPersonality(void);
char* GetPartnerOptionString(char* buf, int idx);
void WaitForNextStep(enum personality_test_state next_state);
void DrawPersonalityTestDebug(int window_id);

#endif
