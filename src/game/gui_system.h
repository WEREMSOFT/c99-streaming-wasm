#ifndef __GUI_SYSTEM_H__
#define __GUI_SYSTEM_H__

#include "cimgui_impl_raylib.h"
#include "game_options.h"
#include "game_board.h"
#include "recording_system.h"

void gui_init(int screen_width, int screen_height);
void gui_fini();
void gui_draw();
void ui_pre_frame_update();
#endif