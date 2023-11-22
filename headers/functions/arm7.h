#ifndef HEADERS_FUNCTIONS_ARM7_H_
#define HEADERS_FUNCTIONS_ARM7_H_

void _start_arm7(void);
void do_autoload_arm7(void);
void StartAutoloadDoneCallbackArm7(void);
void NitroSpMain(void);
void HardwareInterrupt(void);
void ReturnFromInterrupt(void);
void AudioInterrupt(int function_index);
bool ClearImeFlag(void);
int ClearIeFlag(int flag_id);
long long GetCurrentPlaybackTime(void);

#endif
