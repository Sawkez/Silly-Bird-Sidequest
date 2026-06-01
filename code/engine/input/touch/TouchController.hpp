#pragma once

#include "engine/input/InputManager.hpp"
#include "engine/input/touch/TouchButton.hpp"

class TouchController {
   private:
	TouchButton _buttons[1];

   public:
	TouchController(InputManager& inputManager) : _buttons{TouchButton(SDL_FPoint{0.0, 0.0}, 0.5, inputManager, -1, -1, -1)} {}
};