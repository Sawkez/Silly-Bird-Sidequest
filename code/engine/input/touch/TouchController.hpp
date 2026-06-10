#pragma once

#include <vector>

#include "engine/input/InputManager.hpp"
#include "engine/input/touch/TouchButton.hpp"

class TouchController {
   private:
	enum TouchButtonID {
		BUTTON_LEFT,
		BUTTON_UP,
		BUTTON_RIGHT,

		BUTTON_DOWNLEFT,
		BUTTON_DOWN,
		BUTTON_DOWNRIGHT,

		BUTTON_DIVEDASH,
		BUTTON_DASH,
		BUTTON_DASHUSE,

		BUTTON_DIVE,
		BUTTON_ALL,
		BUTTON_USE,

		BUTTON_PAUSE,
		BUTTON_CONSOLE,

		_BUTTON_COUNT
	};

	TouchButton _buttons[_BUTTON_COUNT];

   public:
	// clang-format off
	
	TouchController(InputManager& inputManager) : _buttons{
		TouchButton(SDL_FPoint{0.05, 0.55}, SDL_FPoint{0.1, 0.2}, inputManager, {ACTION_LEFT}),
		TouchButton(SDL_FPoint{0.15, 0.55}, SDL_FPoint{0.1, 0.2}, inputManager, {ACTION_UP}),
		TouchButton(SDL_FPoint{0.25, 0.55}, SDL_FPoint{0.1, 0.2}, inputManager, {ACTION_RIGHT}),

		TouchButton(SDL_FPoint{0.05, 0.75}, SDL_FPoint{0.1, 0.2}, inputManager, {ACTION_LEFT, ACTION_DOWN}),
		TouchButton(SDL_FPoint{0.15, 0.75}, SDL_FPoint{0.1, 0.2}, inputManager, {ACTION_DOWN}),
		TouchButton(SDL_FPoint{0.25, 0.75}, SDL_FPoint{0.1, 0.2}, inputManager, {ACTION_RIGHT, ACTION_DOWN}),

		TouchButton(SDL_FPoint{0.65, 0.55}, SDL_FPoint{0.1, 0.2}, inputManager, {ACTION_DIVE, ACTION_JUMP}),
		TouchButton(SDL_FPoint{0.75, 0.55}, SDL_FPoint{0.1, 0.2}, inputManager, {ACTION_JUMP}),
		TouchButton(SDL_FPoint{0.85, 0.55}, SDL_FPoint{0.1, 0.2}, inputManager, {ACTION_JUMP, ACTION_INTERACT}),
		
		TouchButton(SDL_FPoint{0.65, 0.75}, SDL_FPoint{0.1, 0.2}, inputManager, {ACTION_DIVE}),
		TouchButton(SDL_FPoint{0.75, 0.75}, SDL_FPoint{0.1, 0.2}, inputManager, {ACTION_DIVE, ACTION_JUMP, ACTION_INTERACT}),
		TouchButton(SDL_FPoint{0.85, 0.75}, SDL_FPoint{0.1, 0.2}, inputManager, {ACTION_INTERACT}),

		TouchButton(SDL_FPoint{0.05, 0.05}, SDL_FPoint{0.05, 0.1}, inputManager, {ACTION_PAUSE}),
		TouchButton(SDL_FPoint{0.10, 0.05}, SDL_FPoint{0.05, 0.1}, inputManager, {ACTION_CONSOLE}),
	} {}

	// clang-format on

	void Init(SDL_Renderer* renderer, SDL_Window* window) {
		for (int i = 0; i < _BUTTON_COUNT; i++) {
			int w, h;
			SDL_GetWindowSize(window, &w, &h);
			_buttons[i].UpdateDrawRect(w, h);
			_buttons[i].LoadIcons(renderer, i);
		}
	}

	void Draw(SDL_Renderer* renderer) {
		for (int i = 0; i < _BUTTON_COUNT; i++) {
			_buttons[i].Draw(renderer);
		}
	}

	bool FindButton(float x, float y, TouchButton*& outButton) {
		for (int i = 0; i < _BUTTON_COUNT; i++) {
			if (_buttons[i].HasPoint(x, y)) {
				outButton = _buttons + i;
				return true;
			}
		}
		return false;
	}

	bool HandleEvent(const SDL_Event& event) {
		switch (event.type) {
			case SDL_EVENT_WINDOW_RESIZED: {
				int w, h;
				SDL_GetWindowSize(SDL_GetWindowFromID(event.window.windowID), &w, &h);

				for (int i = 0; i < _BUTTON_COUNT; i++) {
					_buttons[i].UpdateDrawRect(w, h);
				}

				return false;
			}

			case SDL_EVENT_FINGER_DOWN: {
				TouchButton* button;
				if (!FindButton(event.tfinger.x, event.tfinger.y, button)) return false;
				button->Press();
				return true;
			}

			case SDL_EVENT_FINGER_MOTION: {
				TouchButton *oldButton, *newButton;
				bool newButtonFound = FindButton(event.tfinger.x, event.tfinger.y, newButton);
				bool oldButtonFound = FindButton(event.tfinger.x - event.tfinger.dx, event.tfinger.y - event.tfinger.dy, oldButton);

				if (!newButtonFound && !oldButtonFound) {
					return false;

				} else if (newButtonFound && !oldButtonFound) {
					newButton->Press();

				} else if (oldButtonFound && !newButtonFound) {
					oldButton->Release();

				} else {
					newButton->Shift(*oldButton);
				}

				return true;
			}

			case SDL_EVENT_FINGER_UP: {
				TouchButton* button;
				if (!FindButton(event.tfinger.x, event.tfinger.y, button)) return false;
				button->Release();
				return true;
			}

			default:
				return false;
		}
	}

	void Reset() {
		for (int i = 0; i < _BUTTON_COUNT; i++) {
			_buttons[i].Release();
		}
	}
};