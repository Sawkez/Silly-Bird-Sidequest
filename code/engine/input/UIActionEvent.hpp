#pragma once

#include "engine/input/UIAction.hpp"
#include "lvgl/lvgl.h"

struct UIActionEvent {
	lv_key_t key;
	bool pressed;

	UIActionEvent(lv_key_t actionKey, bool actionPressed) : key(actionKey), pressed(actionPressed) {}
};