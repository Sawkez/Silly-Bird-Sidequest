#pragma once

#include <SDL3/SDL.h>

#include <iostream>

#include "engine/Math.hpp"
#include "engine/Vector2.hpp"
#include "engine/input/Action.hpp"

enum ActionID {
	ACTION_JUMP,
	ACTION_DIVE,
	ACTION_LEFT,
	ACTION_RIGHT,
	ACTION_UP,
	ACTION_DOWN,
	ACTION_INTERACT,
	ACTION_PAN_CAMERA,
	ACTION_PAUSE,
	_ACTION_COUNT
};

class InputManager {
	static const Sint16 TRIGGER_DOWN_VALUE = SDL_MAX_SINT16 / 2;

	static constexpr float JOY_DEADZONE = 0.5 * 0.5;
	static constexpr float JOY_AXIS_DEADZONE = int(float(SDL_MAX_SINT16) * 0.5);
	static const int JOY_REMAP_COUNT = 7;

	struct JoyRemap {
		float start;
		float end;
		Vector2 result;

		bool Has(float angle) const { return angle >= start && angle <= end; }
	};

	const JoyRemap JOY_REMAPS[JOY_REMAP_COUNT] = {JoyRemap{Math::Radians(-181.0), Math::Radians(-120.0), Vector2::LEFT},
												  JoyRemap{Math::Radians(-120.0), Math::Radians(-60.0), Vector2::UP},
												  JoyRemap{Math::Radians(-60.0), Math::Radians(25.0), Vector2::RIGHT},
												  JoyRemap{Math::Radians(25.0), Math::Radians(75.0), Vector2::RIGHT + Vector2::DOWN},
												  JoyRemap{Math::Radians(75.0), Math::Radians(105.0), Vector2::DOWN},
												  JoyRemap{Math::Radians(105.0), Math::Radians(155.0), Vector2::LEFT + Vector2::DOWN},
												  JoyRemap{Math::Radians(155.0), Math::Radians(181.0), Vector2::LEFT}};

   private:
	Vector2 _dir = Vector2(0.0, 0.0);
	SDL_JoystickID _lastUsedJoystick;
	bool _dirJoystickPriority = false;
	bool _lastInputWasController = false;

	Action _actions[_ACTION_COUNT]{
		Action(SDL_SCANCODE_SPACE, SDL_SCANCODE_UNKNOWN, SDL_GAMEPAD_BUTTON_SOUTH, SDL_GAMEPAD_BUTTON_INVALID),			   // jump
		Action(SDL_SCANCODE_LSHIFT, SDL_SCANCODE_UNKNOWN, SDL_GAMEPAD_BUTTON_LEFT_SHOULDER, Action::LEFT_TRIGGER_BUTTON),  // dive
		Action(SDL_SCANCODE_A, SDL_SCANCODE_UNKNOWN, SDL_GAMEPAD_BUTTON_DPAD_LEFT, SDL_GAMEPAD_BUTTON_INVALID),			   // left
		Action(SDL_SCANCODE_D, SDL_SCANCODE_UNKNOWN, SDL_GAMEPAD_BUTTON_DPAD_RIGHT, SDL_GAMEPAD_BUTTON_INVALID),		   // right
		Action(SDL_SCANCODE_W, SDL_SCANCODE_UNKNOWN, SDL_GAMEPAD_BUTTON_DPAD_UP, SDL_GAMEPAD_BUTTON_INVALID),			   // up
		Action(SDL_SCANCODE_S, SDL_SCANCODE_UNKNOWN, SDL_GAMEPAD_BUTTON_DPAD_DOWN, SDL_GAMEPAD_BUTTON_INVALID),			   // down
		Action(SDL_SCANCODE_E, SDL_SCANCODE_UNKNOWN, SDL_GAMEPAD_BUTTON_WEST, SDL_GAMEPAD_BUTTON_INVALID),				   // interact
		Action(SDL_SCANCODE_Q, SDL_SCANCODE_UNKNOWN, SDL_GAMEPAD_BUTTON_EAST, SDL_GAMEPAD_BUTTON_INVALID),				   // pan camera
		Action(SDL_SCANCODE_ESCAPE, SDL_SCANCODE_UNKNOWN, SDL_GAMEPAD_BUTTON_START, SDL_GAMEPAD_BUTTON_INVALID)			   // pause
	};

	bool IsDirectionAction(int id) { return id >= ACTION_LEFT && id <= ACTION_DOWN; }

   public:
	InputManager() : _dir(Vector2(0.0, 0.0)) {}

	bool HandleEvent(const SDL_Event& event) {
		switch (event.type) {
			case SDL_EVENT_GAMEPAD_ADDED:
			case SDL_EVENT_GAMEPAD_REMOVED:
				HandleEvent(event.gdevice);
				return true;

#if !SDL_PLATFORM_PSP

			case SDL_EVENT_KEY_DOWN:
			case SDL_EVENT_KEY_UP:
				HandleEvent(event.key);
				return true;

#endif

			case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
			case SDL_EVENT_GAMEPAD_BUTTON_UP:
				HandleEvent(event.gbutton);
				return true;

			case SDL_EVENT_GAMEPAD_AXIS_MOTION:
				return HandleEvent(event.gaxis);
		}

		return false;
	}

	void HandleEvent(const SDL_KeyboardEvent& event) {
		int actionId = -1;

		for (int i = 0; i < _ACTION_COUNT; i++) {
			if (_actions[i].HasKey(event.scancode)) {
				actionId = i;
				break;
			}
		}

		if (actionId < 0) return;

		_lastInputWasController = false;

		if (event.down && IsDirectionAction(actionId)) _dirJoystickPriority = false;

		_actions[actionId].SetDown(event.down);
	}

	void HandleEvent(const SDL_GamepadButtonEvent& event) {
		_lastInputWasController = true;
		_lastUsedJoystick = event.which;

		ButtonEvent(event.button, event.down);
	}

	bool ButtonEvent(int button, bool pressed) {  // separated so we can handle triggers as buttons
		for (int i = 0; i < _ACTION_COUNT; i++) {
			if (_actions[i].HasButton(button)) {
				if (pressed && IsDirectionAction(i)) _dirJoystickPriority = false;
				_actions[i].SetDown(pressed);
				return true;
			}
		}

		return false;
	}

	bool HandleEvent(const SDL_GamepadAxisEvent& event) {
		_lastInputWasController = true;

		switch (event.axis) {
			case SDL_GAMEPAD_AXIS_LEFTX:
			case SDL_GAMEPAD_AXIS_LEFTY:
				if (abs(event.value) > JOY_AXIS_DEADZONE) {
					_dirJoystickPriority = true;
				}
#if !SDL_PLATFORM_PSP
			case SDL_GAMEPAD_AXIS_RIGHTX:
			case SDL_GAMEPAD_AXIS_RIGHTY:
#endif
				_lastUsedJoystick = event.which;
				return true;

#if !SDL_PLATFORM_PSP
			case SDL_GAMEPAD_AXIS_LEFT_TRIGGER:
				return ButtonEvent(Action::LEFT_TRIGGER_BUTTON, event.value > TRIGGER_DOWN_VALUE);

			case SDL_GAMEPAD_AXIS_RIGHT_TRIGGER:
				return ButtonEvent(Action::RIGHT_TRIGGER_BUTTON, event.value > TRIGGER_DOWN_VALUE);
#endif
		}

		return false;
	}

	void HandleEvent(const SDL_GamepadDeviceEvent& event) { SDL_OpenGamepad(event.which); }

	void UpdateDir() {
		if (_dirJoystickPriority) {
			SDL_Gamepad* controller = SDL_GetGamepadFromID(_lastUsedJoystick);
			_dir.x = float(SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_LEFTX)) / SDL_MAX_SINT16;
			_dir.y = float(SDL_GetGamepadAxis(controller, SDL_GAMEPAD_AXIS_LEFTY)) / SDL_MAX_SINT16;

			if (_dir.LengthSquared() > JOY_DEADZONE) {
				float angle = _dir.Angle();

				for (int i = 0; i < JOY_REMAP_COUNT; i++) {
					if (JOY_REMAPS[i].Has(angle)) {
						_dir = JOY_REMAPS[i].result;
						break;
					}
				}
			}

			else {
				_dir = Vector2::ZERO;
			}

			_actions[ACTION_LEFT].SetDown(_dir.x < 0.0);
			_actions[ACTION_RIGHT].SetDown(_dir.x > 0.0);
			_actions[ACTION_UP].SetDown(_dir.y < 0.0);
			_actions[ACTION_DOWN].SetDown(_dir.y > 0.0);
		}

		else {
			_dir.x = float(IsDown(ACTION_RIGHT)) - float(IsDown(ACTION_LEFT));
			_dir.y = float(IsDown(ACTION_DOWN)) - float(IsDown(ACTION_UP));
		}

		return;
	}

	void UpdateTapStates() {
		for (int i = 0; i < _ACTION_COUNT; i++) {
			_actions[i].UpdateTapState();
		}
	}

	void Reset() {
		for (int i = 0; i < _ACTION_COUNT; i++) {
			_actions[i].SetDown(false);
		}
	}

	void ResetToState() {
		if (_lastInputWasController)
			ResetToControllerState();
		else
			ResetToKeyboardState();
	}

	void ResetToKeyboardState() {
		const bool* state = SDL_GetKeyboardState(NULL);

		for (int i = 0; i < _ACTION_COUNT; i++) {
			_actions[i].ResetToKeyboardState(state);
		}
	}

	void ResetToControllerState() {
		SDL_Gamepad* controller = SDL_GetGamepadFromID(_lastUsedJoystick);
		for (int i = 0; i < _ACTION_COUNT; i++) {
			_actions[i].ResetToControllerState(controller);
		}
	}

	bool IsDown(int id) const { return _actions[id].IsDown(); }

	bool IsTapped(int id) const { return _actions[id].IsTapped(); }

	Vector2 GetDir() const { return _dir; }
};