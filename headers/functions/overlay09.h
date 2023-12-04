#ifndef HEADERS_FUNCTIONS_OVERLAY09_H_
#define HEADERS_FUNCTIONS_OVERLAY09_H_

int CreateJukeboxTrackMenu(struct window_params* params, struct window_flags flags,
                           struct window_extra_info* extra_info,
                           struct simple_menu_id_item* menu_items, int n_items);
void CloseJukeboxTrackMenu(int window_id);
bool IsJukeboxTrackMenuActive(int window_id);
void UpdateJukeboxTrackMenu(struct window* window);
int CreatePlaybackControlsMenu(struct window_params* params, struct window_flags flags,
                               struct window_extra_info* extra_info, undefined4 param_4,
                               struct simple_menu_id_item* menu_items1,
                               struct simple_menu_id_item* menu_items2);
int ClosePlaybackControlsMenu(int window_id);
bool IsPlaybackControlsMenuActive(int window_id);
void UpdatePlaybackControlsMenu(struct window* window);
int CreateInputLockBox(struct window_params* params, struct window_flags flags,
                       struct window_extra_info* extra_info, undefined4 param_4,
                       uint16_t string_id);
void CloseInputLockBox(int window_id);
bool IsInputLockBoxActive(int window_id);
void UpdateInputLockBox(struct window* window);

#endif
