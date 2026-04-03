#pragma once

#include "engine/input/UIAction.hpp"
#include "lvgl/lvgl.h"

struct UIActionEvent {
	lv_key_t key;
	bool pressed;

	UIActionEvent() : UIActionEvent(LV_KEY_HOME, false) {};
	UIActionEvent(lv_key_t actionKey, bool actionPressed) : key(actionKey), pressed(actionPressed) {}

	bool operator!=(const UIActionEvent& other) { return key != other.key || pressed != other.pressed; }
};