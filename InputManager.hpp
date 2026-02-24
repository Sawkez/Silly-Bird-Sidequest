#pragma once

#include <SDL.h>
#include <iostream>

#include "Action.hpp"
#include "Math.hpp"
#include "Vector2.hpp"

enum ActionID {
	ACTION_JUMP,
	ACTION_DIVE,
	ACTION_LEFT,
	ACTION_RIGHT,
	ACTION_UP,
	ACTION_DOWN,
	ACTION_INTERACT,
	ACTION_PAN_CAMERA,
	ACTION_COUNT
};

class InputManager {
	static const int LEFT_TRIGGER_BUTTON = SDL_CONTROLLER_BUTTON_MAX + 1;
	static const int RIGHT_TRIGGER_BUTTON = SDL_CONTROLLER_BUTTON_MAX + 1;

	static const Sint16 TRIGGER_DOWN_VALUE = SDL_MAX_SINT16 / 2;

	static constexpr float JOY_DEADZONE = 0.5 * 0.5;
	static const int JOY_REMAP_COUNT = 7;

	struct JoyRemap {
		float start;
		float end;
		Vector2 result;

		bool Has(float angle) const { return angle >= start && angle <= end; }
	};

	const JoyRemap JOY_REMAPS[JOY_REMAP_COUNT] = {
		JoyRemap{Math::Radians(-181.0), Math::Radians(-120.0), Vector2::LEFT},
		JoyRemap{Math::Radians(-120.0), Math::Radians(-60.0), Vector2::UP},
		JoyRemap{Math::Radians(-60.0), Math::Radians(25.0), Vector2::RIGHT},
		JoyRemap{Math::Radians(25.0), Math::Radians(75.0), Vector2::RIGHT + Vector2::DOWN},
		JoyRemap{Math::Radians(75.0), Math::Radians(105.0), Vector2::DOWN},
		JoyRemap{Math::Radians(105.0), Math::Radians(155.0), Vector2::LEFT + Vector2::DOWN},
		JoyRemap{Math::Radians(155.0), Math::Radians(181.0), Vector2::LEFT}};

  private:
	Vector2 _dir = Vector2(0.0, 0.0);
	SDL_JoystickID _lastUsedJoystick;

	Action _actions[ACTION_COUNT]{
		Action(SDL_SCANCODE_SPACE, SDL_SCANCODE_UNKNOWN, SDL_CONTROLLER_BUTTON_A,
			   SDL_CONTROLLER_BUTTON_INVALID), // jump
		Action(SDL_SCANCODE_LSHIFT, SDL_SCANCODE_UNKNOWN, SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
			   LEFT_TRIGGER_BUTTON), // dive
		Action(SDL_SCANCODE_A, SDL_SCANCODE_UNKNOWN, SDL_CONTROLLER_BUTTON_DPAD_LEFT,
			   SDL_CONTROLLER_BUTTON_INVALID), // left
		Action(SDL_SCANCODE_D, SDL_SCANCODE_UNKNOWN, SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
			   SDL_CONTROLLER_BUTTON_INVALID), // right
		Action(SDL_SCANCODE_W, SDL_SCANCODE_UNKNOWN, SDL_CONTROLLER_BUTTON_DPAD_UP,
			   SDL_CONTROLLER_BUTTON_INVALID), // up
		Action(SDL_SCANCODE_S, SDL_SCANCODE_UNKNOWN, SDL_CONTROLLER_BUTTON_DPAD_DOWN,
			   SDL_CONTROLLER_BUTTON_INVALID), // down
		Action(SDL_SCANCODE_E, SDL_SCANCODE_UNKNOWN, SDL_CONTROLLER_BUTTON_X,
			   SDL_CONTROLLER_BUTTON_INVALID), // interact
		Action(SDL_SCANCODE_Q, SDL_SCANCODE_UNKNOWN, SDL_CONTROLLER_BUTTON_B,
			   SDL_CONTROLLER_BUTTON_INVALID) // pan camera
	};


  public:
	InputManager() : _dir(Vector2(0.0, 0.0)) {}

	bool HandleEvent(SDL_Event event) {
		switch (event.type) {
		case SDL_CONTROLLERDEVICEADDED:
		case SDL_CONTROLLERDEVICEREMOVED:
			HandleEvent(event.cdevice);
			return true;

#if !__PSP__

		case SDL_KEYDOWN:
		case SDL_KEYUP:
			HandleEvent(event.key);
			return true;

#endif

		case SDL_CONTROLLERBUTTONDOWN:
		case SDL_CONTROLLERBUTTONUP:
			HandleEvent(event.cbutton);
			return true;

		case SDL_CONTROLLERAXISMOTION:
			return HandleEvent(event.caxis);
		}

		return false;
	}

	void HandleEvent(SDL_KeyboardEvent event) {
		int actionId = -1;

		for (int i = 0; i < ACTION_COUNT; i++) {
			if (_actions[i].HasKey(event.keysym.scancode)) {
				actionId = i;
				break;
			}
		}

		if (actionId < 0)
			return;

		_actions[actionId].SetDown(event.state == SDL_PRESSED);
	}

	void HandleEvent(SDL_ControllerButtonEvent event) {
		_lastUsedJoystick = event.which;
		int actionId = -1;

		ButtonEvent(event.button, event.state == SDL_PRESSED);
	}

	bool ButtonEvent(int button, bool pressed) { // separated so we can handle triggers as buttons
		for (int i = 0; i < ACTION_COUNT; i++) {
			if (_actions[i].HasButton(button)) {
				_actions[i].SetDown(pressed);
				return true;
			}
		}

		return false;
	}

	bool HandleEvent(SDL_ControllerAxisEvent event) {
		switch (event.axis) {
		case SDL_CONTROLLER_AXIS_LEFTX:
		case SDL_CONTROLLER_AXIS_LEFTY:
#if !__PSP__
		case SDL_CONTROLLER_AXIS_RIGHTX:
		case SDL_CONTROLLER_AXIS_RIGHTY:
#endif
			_lastUsedJoystick = event.which;
			return true;

#if !__PSP__
		case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
			return ButtonEvent(LEFT_TRIGGER_BUTTON, event.value > TRIGGER_DOWN_VALUE);

		case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
			return ButtonEvent(RIGHT_TRIGGER_BUTTON, event.value > TRIGGER_DOWN_VALUE);
#endif
		}

		return false;
	}

	void HandleEvent(SDL_ControllerDeviceEvent event) { SDL_GameControllerOpen(event.which); }

	void UpdateDir() {
		SDL_GameController* controller = SDL_GameControllerFromInstanceID(_lastUsedJoystick);
		_dir.x = float(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX)) / SDL_MAX_SINT16;
		_dir.y = float(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY)) / SDL_MAX_SINT16;

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
			_dir.x = float(IsDown(ACTION_RIGHT)) - float(IsDown(ACTION_LEFT));
			_dir.y = float(IsDown(ACTION_DOWN)) - float(IsDown(ACTION_UP));
		}

		// TODO set dir actions to true when joy inputting

		return;
	}

	void UpdateTapStates() {
		for (int i = 0; i < ACTION_COUNT; i++) {
			_actions[i].UpdateTapState();
		}
	}

	bool IsDown(int id) const { return _actions[id].IsDown(); }

	bool IsTapped(int id) const { return _actions[id].IsTapped(); }

	Vector2 GetDir() const { return _dir; }
};