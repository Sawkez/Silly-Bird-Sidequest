#pragma once

#include <SDL3/SDL.h>

#include "engine/input/InputManager.hpp"

class TouchButton {
   private:
	int _actions[3];
	SDL_FPoint _screenRelativePosition;
	float _size;
	InputManager& _inputManager;

   public:
	TouchButton(SDL_FPoint screenRelativePosition, float size, InputManager& inputManager, int action0, int action1, int action2)
		: _actions{action0, action1, action2}, _screenRelativePosition(screenRelativePosition), _size(size), _inputManager(inputManager) {}
};