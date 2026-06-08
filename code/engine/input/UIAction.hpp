#pragma once

#include <SDL3/SDL.h>

#include "lvgl/lvgl.h"

struct UIAction {
	SDL_Scancode key;
	Sint16 button;
	lv_key_t out;
};