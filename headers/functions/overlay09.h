#ifndef HEADERS_FUNCTIONS_OVERLAY09_H_
#define HEADERS_FUNCTIONS_OVERLAY09_H_

int CreateJukeboxTrackMenu(struct window_params* params, uint32_t param_2, undefined* param_3,
                           undefined* param_4, int param_5);
void UpdateJukeboxTrackMenu(struct window* window);
int CreatePlaybackControlsMenu(struct window_params* params, uint32_t param_2, undefined* param_3,
                               undefined4 param_4, undefined* param_5, undefined* param_6);
void UpdatePlaybackControlsMenu(struct window* window);
int CreateInputLockBox(struct window_params* params, uint32_t param_2, undefined* param_3,
                       undefined4 param_4, uint16_t param_5);
void UpdateInputLockBox(struct window* window);

#endif
