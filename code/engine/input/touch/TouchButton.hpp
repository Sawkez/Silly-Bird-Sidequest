#pragma once

#include <SDL3/SDL.h>

#include "engine/input/InputManager.hpp"

class TouchButton {
   private:
	int _actions[3];
	InputManager& _inputManager;

	SDL_FRect _logicRect;
	SDL_FRect _drawRect;

	bool _down = false;

   public:
	TouchButton(SDL_FPoint positionNormalized, SDL_FPoint sizeNormalized, InputManager& inputManager, int action0, int action1, int action2)
		: _actions{action0, action1, action2},
		  _logicRect{positionNormalized.x, positionNormalized.y, sizeNormalized.x, sizeNormalized.y},
		  _inputManager(inputManager) {}

	void UpdateDrawRect(float windowWidth, float windowHeight) {
		_drawRect = {_logicRect.x * windowWidth, _logicRect.y * windowHeight, _logicRect.w * windowWidth, _logicRect.h * windowHeight};
	}

	bool HasPoint(float x, float y) {
		SDL_FPoint point{x, y};
		return SDL_PointInRectFloat(&point, &_logicRect);
	}

	void Press() { _down = true; }

	void Release() { _down = false; }

	void Draw(SDL_Renderer* renderer) {
		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

		if (_down) {
			SDL_SetRenderDrawColor(renderer, 128, 0, 0, 128);
		} else {
			SDL_SetRenderDrawColor(renderer, 0, 128, 128, 128);
		}

		SDL_RenderFillRect(renderer, &_drawRect);

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	}
};