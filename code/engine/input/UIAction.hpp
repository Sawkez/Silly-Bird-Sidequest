#pragma once

#include <SDL3/SDL.h>

#include "lvgl/lvgl.h"

struct UIAction {
	SDL_Scancode key;
	Uint8 button;
	lv_key_t out;
};