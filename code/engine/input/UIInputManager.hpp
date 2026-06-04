#pragma once

#include <SDL3/SDL.h>

#include "engine/PlatformDefines.hpp"
#include "engine/devconsole/DevConsole.hpp"
#include "engine/input/UIAction.hpp"
#include "engine/input/UIActionEvent.hpp"
#include "lvgl/lvgl.h"

enum UIActionID { UI_ACTION_LEFT, UI_ACTION_RIGHT, UI_ACTION_UP, UI_ACTION_DOWN, UI_ACTION_ENTER, UI_ACTION_ESC, _UI_ACTION_COUNT };

class UIInputManager {
   private:
	static inline const float SCROLL_MOUSE_SENSITIVITY = 50;
	static inline const float SCROLL_JOY_SENSITIVITY = 50;
	static inline const float SCROLL_JOY_DEADZONE_SQUARED = 0.3 * 0.3;

	UIInputManager() = delete;
	static inline auto _event = UIActionEvent();
	static inline float _xScroll = 0;
	static inline float _yScroll = 0;
	static inline SDL_JoystickID _lastUsedJoystick = 0;

#ifdef PLATFORM_HAS_MOUSE
	static inline lv_indev_t* _mouseInput = NULL;
#endif
	static inline lv_indev_t* _keypadInput = NULL;

	static inline lv_group_t* _mainGroup = NULL;

	static inline UIAction _actions[_UI_ACTION_COUNT] = {{SDL_SCANCODE_A, SDL_GAMEPAD_BUTTON_DPAD_LEFT, LV_KEY_LEFT},	 // left
														 {SDL_SCANCODE_D, SDL_GAMEPAD_BUTTON_DPAD_RIGHT, LV_KEY_RIGHT},	 // right
														 {SDL_SCANCODE_W, SDL_GAMEPAD_BUTTON_DPAD_UP, LV_KEY_UP},		 // up
														 {SDL_SCANCODE_S, SDL_GAMEPAD_BUTTON_DPAD_DOWN, LV_KEY_DOWN},	 // down
														 {SDL_SCANCODE_SPACE, SDL_GAMEPAD_BUTTON_SOUTH, LV_KEY_ENTER},	 // enter
														 {SDL_SCANCODE_ESCAPE, SDL_GAMEPAD_BUTTON_EAST, LV_KEY_ESC}};	 // esc

   public:
#ifdef PLATFORM_HAS_MOUSE
	static void TouchReadCallback(lv_indev_t* mouseInput, lv_indev_data_t* data) {
		float x, y;
		Uint32 buttons = SDL_GetMouseState(&x, &y);
		data->point.x = int(x);
		data->point.y = int(y);

		data->state = (buttons & SDL_BUTTON_LEFT) > 0 ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
	}

	static void HandleMouseScroll(const SDL_MouseWheelEvent& event) {
		_xScroll += event.x;
		_yScroll += event.y;
	}
#endif

	static void KeypadReadCallback(lv_indev_t* keypadInput, lv_indev_data_t* data) {
		data->key = (uint32_t)_event.key;
		data->state = _event.pressed ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
	}

	static void Init() {
		_mainGroup = lv_group_create();
		lv_group_set_default(_mainGroup);

#ifdef PLATFORM_HAS_MOUSE
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
		lv_key_t out = (lv_key_t)-2;

		switch (event.type) {
#ifdef PLATFORM_HAS_KEYBOARD
			case SDL_EVENT_KEY_DOWN:
			case SDL_EVENT_KEY_UP:
				for (int i = 0; i < _UI_ACTION_COUNT; i++) {
					if (_actions[i].key == event.key.scancode) {
						out = _actions[i].out;
						break;
					}
				}

				if (event.key.repeat && out == LV_KEY_ESC) return true;
				break;
#endif

#ifdef PLATFORM_HAS_MOUSE
			case SDL_EVENT_MOUSE_WHEEL:
				HandleMouseScroll(event.wheel);
			case SDL_EVENT_MOUSE_BUTTON_DOWN:
			case SDL_EVENT_MOUSE_MOTION:
			case SDL_EVENT_MOUSE_BUTTON_UP:
				lv_indev_read(_mouseInput);
				return true;
#endif
			case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
			case SDL_EVENT_GAMEPAD_BUTTON_UP:
				for (int i = 0; i < _UI_ACTION_COUNT; i++) {
					if (_actions[i].button == event.gbutton.button) {
						out = _actions[i].out;
						break;
					}
				}
				break;

			case SDL_EVENT_GAMEPAD_AXIS_MOTION:
				_lastUsedJoystick = event.gaxis.which;
				return true;

			default:
				return false;
		}

		if (out < 0) {
			return true;
		}
		bool pressed = event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN;

		if (!pressed && out != _event.key) {
			return true;
		}

		if (_event.key != out && _event.pressed) {
			_event.pressed = false;
			lv_indev_read(_keypadInput);
		}

		_event = UIActionEvent(out, event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_GAMEPAD_BUTTON_DOWN);
		lv_indev_read(_keypadInput);
		return true;
	}

	static void Reset() {
		_event = UIActionEvent();
		lv_indev_read(_keypadInput);
	}

	static const UIActionEvent& GetEvent() { return _event; }

	static lv_group_t* GetMainGroup() { return _mainGroup; }

	static void UpdateScroll(float delta) {
		lv_obj_t* target;

		SDL_Gamepad* controller = SDL_GetGamepadFromID(_lastUsedJoystick);

#ifdef PLATFORM_HAS_RIGHT_JOY
		float xJoy = float(SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_RIGHTX)) / SDL_MAX_SINT16;
		float yJoy = float(SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_RIGHTY)) / SDL_MAX_SINT16;
#else
		float xJoy = float(SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_LEFTX)) / SDL_MAX_SINT16;
		float yJoy = float(SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_LEFTY)) / SDL_MAX_SINT16;
#endif

		if (xJoy * xJoy + yJoy * yJoy > SCROLL_JOY_DEADZONE_SQUARED) {
			target = lv_group_get_focused(_mainGroup);
			_xScroll = xJoy;
			_yScroll = yJoy;
		}

		else if (_xScroll == 0.0 && _yScroll == 0.0)
			return;

		else {
			lv_point_t mousePoint;
			lv_indev_get_point(_mouseInput, &mousePoint);

			target = lv_indev_search_obj(lv_screen_active(), &mousePoint);
		}

		while (target && !lv_obj_has_flag(target, LV_OBJ_FLAG_SCROLLABLE)) {
			target = lv_obj_get_parent(target);
		}

		if (target != nullptr) {
			if (lv_obj_get_scroll_dir(target) == LV_DIR_HOR) {
				_xScroll += _yScroll;
			}

			lv_obj_scroll_by_bounded(target, _xScroll * lv_dpx(SCROLL_MOUSE_SENSITIVITY), _yScroll * lv_dpx(SCROLL_MOUSE_SENSITIVITY), LV_ANIM_OFF);
		}
		_xScroll = _yScroll = 0.0f;
	}
};