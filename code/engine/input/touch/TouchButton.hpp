#pragma once

#include <SDL3/SDL.h>

#include "engine/input/InputManager.hpp"

class TouchButton {
   private:
	int _actions[3];
	SDL_FPoint _position;
	float _size;
	InputManager& _inputManager;

   public:
	TouchButton(SDL_FPoint position, float size, InputManager& inputManager, int action0, int action1, int action2)
		: _actions{action0, action1, action2}, _position(position), _size(size), _inputManager(inputManager) {}

	void Draw(SDL_Renderer* renderer) {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		SDL_FRect rect{_position.x, _position.y, _size, _size};
		SDL_SetRenderDrawColor(renderer, 0, 128, 128, 128);
		SDL_RenderFillRect(renderer, &rect);

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	}
};