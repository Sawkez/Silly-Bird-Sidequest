#pragma once

#include "engine/input/InputManager.hpp"
#include "engine/input/touch/TouchButton.hpp"

class TouchController {
   private:
	enum TouchButtonID { BUTTON_TEST, _BUTTON_COUNT };

	TouchButton _buttons[_BUTTON_COUNT];

   public:
	// clang-format off
	
	TouchController(InputManager& inputManager) : _buttons{
		TouchButton(SDL_FPoint{0.0, 0.0}, SDL_FPoint{0.2, 0.2}, inputManager, -1, -1, -1)
	} {}

	// clang-format on

	void Init(SDL_Window* window) {
		for (int i = 0; i < _BUTTON_COUNT; i++) {
			int w, h;
			SDL_GetWindowSize(window, &w, &h);
			_buttons[i].UpdateDrawRect(w, h);
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

				if (newButtonFound) newButton->Press();
				if (oldButtonFound && oldButton != newButton) oldButton->Release();
				return oldButtonFound || newButtonFound;
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
};