#pragma once

#include "engine/input/InputManager.hpp"
#include "engine/input/touch/TouchButton.hpp"

class TouchController {
   private:
	enum TouchButtonID { BUTTON_TEST, _BUTTON_COUNT };

	TouchButton _buttons[_BUTTON_COUNT];

   public:
	TouchController(InputManager& inputManager) : _buttons{TouchButton(SDL_FPoint{0.0, 0.0}, 50, inputManager, -1, -1, -1)} {}

	void Draw(SDL_Renderer* renderer) {
		for (int i = 0; i < _BUTTON_COUNT; i++) {
			_buttons[i].Draw(renderer);
		}
	}
};