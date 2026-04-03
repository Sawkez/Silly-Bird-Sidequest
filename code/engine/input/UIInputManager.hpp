#pragma once

#include <SDL.h>

#include "engine/input/UIAction.hpp"
#include "engine/input/UIActionEvent.hpp"
#include "lvgl/lvgl.h"

enum UIActionID { UI_ACTION_LEFT, UI_ACTION_RIGHT, UI_ACTION_UP, UI_ACTION_DOWN, UI_ACTION_ENTER, _UI_ACTION_COUNT };

class UIInputManager {
   private:
	UIActionEvent _event;
	bool _hasEvent;

#if !__PSP__
	lv_indev_t* _mouseInput;
#endif
	lv_indev_t* _keypadInput;

	lv_group_t* _mainGroup;

	UIAction _actions[_UI_ACTION_COUNT] = {
		{SDL_SCANCODE_A, SDL_CONTROLLER_BUTTON_DPAD_LEFT, LV_KEY_LEFT},	   // left
		{SDL_SCANCODE_D, SDL_CONTROLLER_BUTTON_DPAD_RIGHT, LV_KEY_RIGHT},  // right
		{SDL_SCANCODE_W, SDL_CONTROLLER_BUTTON_DPAD_UP, LV_KEY_UP},		   // up
		{SDL_SCANCODE_S, SDL_CONTROLLER_BUTTON_DPAD_DOWN, LV_KEY_DOWN},	   // down
		{SDL_SCANCODE_SPACE, SDL_CONTROLLER_BUTTON_A, LV_KEY_ENTER}		   // enter
	};

   public:
#if !__PSP__
	static void TouchReadCallback(lv_indev_t* mouseInput, lv_indev_data_t* data) {
		Uint32 buttons = SDL_GetMouseState(&(data->point.x), &(data->point.y));
		data->state = (buttons & SDL_BUTTON_LEFT) > 0 ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
		data->continue_reading = false;
	}
#endif

	static void KeypadReadCallback(lv_indev_t* keypadInput, lv_indev_data_t* data) {
		auto* instance = (UIInputManager*)lv_indev_get_user_data(keypadInput);

		const UIActionEvent& event = instance->GetEvent();

		data->key = (uint32_t)event.key;
		data->state = event.pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
	}

	UIInputManager()
		:
#if !__PSP__
		  _mouseInput(lv_indev_create()),
#endif
		  _keypadInput(lv_indev_create()),
		  _mainGroup(lv_group_create()) {
#if !__PSP__
		lv_indev_set_type(_mouseInput, LV_INDEV_TYPE_POINTER);
		lv_indev_set_read_cb(_mouseInput, TouchReadCallback);
		// lv_indev_set_group(_mouseInput, _mainGroup);
#endif

		lv_indev_set_type(_keypadInput, LV_INDEV_TYPE_KEYPAD);
		lv_indev_set_read_cb(_keypadInput, KeypadReadCallback);
		lv_indev_set_user_data(_keypadInput, this);
		lv_indev_set_group(_keypadInput, _mainGroup);
		lv_indev_set_mode(_keypadInput, LV_INDEV_MODE_EVENT);

		lv_group_set_default(_mainGroup);
	}

	bool HandleEvent(const SDL_Event& event) {
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

#if __PSP__
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

	const UIActionEvent& GetEvent() { return _event; }

	lv_group_t* GetMainGroup() const { return _mainGroup; }
};