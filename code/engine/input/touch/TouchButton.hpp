#pragma once

#include <SDL3/SDL.h>

#include <vector>

#include "engine/input/InputManager.hpp"

class TouchButton {
   private:
	std::vector<int> _actions;
	InputManager& _inputManager;

	SDL_FRect _logicRect;
	SDL_FRect _drawRect;

	bool _down = false;

   public:
	TouchButton(SDL_FPoint positionNormalized, SDL_FPoint sizeNormalized, InputManager& inputManager, std::vector<int> actions)
		: _logicRect{positionNormalized.x, positionNormalized.y, sizeNormalized.x, sizeNormalized.y},
		  _inputManager(inputManager),
		  _actions(std::move(actions)) {}

	void UpdateDrawRect(float windowWidth, float windowHeight) {
		_drawRect = {_logicRect.x * windowWidth, _logicRect.y * windowHeight, _logicRect.w * windowWidth, _logicRect.h * windowHeight};
	}

	bool HasPoint(float x, float y) {
		SDL_FPoint point{x, y};
		return SDL_PointInRectFloat(&point, &_logicRect);
	}

	void Press() {
		_down = true;
		for (auto action : _actions) {
			_inputManager.SimulateAction(action, true);
		}
	}

	void Release() {
		_down = false;
		for (auto action : _actions) {
			_inputManager.SimulateAction(action, false);
		}
	}

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