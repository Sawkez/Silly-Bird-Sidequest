#pragma once

#include <SDL.h>

#include "engine/input/UIAction.hpp"
#include "engine/input/UIActionEvent.hpp"
#include "lvgl/lvgl.h"

enum UIActionID { UI_ACTION_LEFT, UI_ACTION_RIGHT, UI_ACTION_UP, UI_ACTION_DOWN, UI_ACTION_ENTER, UI_ACTION_ESC, _UI_ACTION_COUNT };

class UIInputManager {
   private:
	UIInputManager() = delete;
	static inline auto _event = UIActionEvent();

#if !__PSP__
	static inline lv_indev_t* _mouseInput = NULL;
#endif
	static inline lv_indev_t* _keypadInput = NULL;

	static inline lv_group_t* _mainGroup = NULL;

	static inline UIAction _actions[_UI_ACTION_COUNT] = {{SDL_SCANCODE_A, SDL_CONTROLLER_BUTTON_DPAD_LEFT, LV_KEY_LEFT},	// left
														 {SDL_SCANCODE_D, SDL_CONTROLLER_BUTTON_DPAD_RIGHT, LV_KEY_RIGHT},	// right
														 {SDL_SCANCODE_W, SDL_CONTROLLER_BUTTON_DPAD_UP, LV_KEY_UP},		// up
														 {SDL_SCANCODE_S, SDL_CONTROLLER_BUTTON_DPAD_DOWN, LV_KEY_DOWN},	// down
														 {SDL_SCANCODE_SPACE, SDL_CONTROLLER_BUTTON_A, LV_KEY_ENTER},		// enter
														 {SDL_SCANCODE_ESCAPE, SDL_CONTROLLER_BUTTON_B, LV_KEY_ESC}};		// esc

   public:
#if !__PSP__
	static void TouchReadCallback(lv_indev_t* mouseInput, lv_indev_data_t* data) {
		Uint32 buttons = SDL_GetMouseState(&(data->point.x), &(data->point.y));
		data->state = (buttons & SDL_BUTTON_LEFT) > 0 ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
	}
#endif

	static void KeypadReadCallback(lv_indev_t* keypadInput, lv_indev_data_t* data) {
		data->key = (uint32_t)_event.key;
		data->state = _event.pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
	}

	static void Init() {
		_mainGroup = lv_group_create();
		lv_group_set_default(_mainGroup);

#if !__PSP__
		_mouseInput = lv_indev_create();
		lv_indev_set_type(_mouseInput, LV_INDEV_TYPE_POINTER);
		lv_indev_set_mode(_mouseInput, LV_INDEV_MODE_EVENT);
		lv_indev_set_read_cb(_mouseInput, TouchReadCallback);
#endif
		_keypadInput = lv_indev_create();
		lv_indev_set_type(_keypadInput, LV_INDEV_TYPE_KEYPAD);
		lv_indev_set_read_cb(_keypadInput, KeypadReadCallback);
		lv_indev_set_group(_keypadInput, _mainGroup);
		lv_indev_set_mode(_keypadInput, LV_INDEV_MODE_EVENT);
	}

	static bool HandleEvent(const SDL_Event& event) {
		lv_key_t out = (lv_key_t)-1;

		switch (event.type) {
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				for (int i = 0; i < _UI_ACTION_COUNT; i++) {
					if (_actions[i].key == event.key.keysym.scancode) {
						out = _actions[i].out;
						break;
					}
				}
				break;

			case SDL_CONTROLLERBUTTONDOWN:
			case SDL_CONTROLLERBUTTONUP:
				for (int i = 0; i < _UI_ACTION_COUNT; i++) {
					if (_actions[i].button == event.cbutton.button) {
						out = _actions[i].out;
						break;
					}
				}
				break;

#if !__PSP__
			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				lv_indev_read(_mouseInput);
				return true;
#endif
		}

		if (out < 0) {
			return false;
		}

		_event = UIActionEvent(out, event.type == SDL_KEYDOWN || event.type == SDL_CONTROLLERBUTTONDOWN);
		lv_indev_read(_keypadInput);
		return true;
	}

	static const UIActionEvent& GetEvent() { return _event; }

	static lv_group_t* GetMainGroup() { return _mainGroup; }
};