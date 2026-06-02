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

	SDL_Texture* _downTexture = nullptr;
	SDL_Texture* _upTexture = nullptr;

	bool _down = false;

   public:
	TouchButton(SDL_FPoint positionNormalized, SDL_FPoint sizeNormalized, InputManager& inputManager, std::vector<int> actions)
		: _logicRect{positionNormalized.x, positionNormalized.y, sizeNormalized.x, sizeNormalized.y},
		  _inputManager(inputManager),
		  _actions(std::move(actions)) {}

	void UpdateDrawRect(float windowWidth, float windowHeight) {
		_drawRect = {_logicRect.x * windowWidth, _logicRect.y * windowHeight, _logicRect.w * windowWidth, _logicRect.h * windowHeight};
	}

	void LoadIcons(SDL_Renderer* renderer, int id) {
		std::string downPath = "content/textures/icons/touch/down/" + std::to_string(id) + ".png";
		std::string upPath = "content/textures/icons/touch/up/" + std::to_string(id) + ".png";

		std::cout << "Loading touch icon " << id << ": " << downPath << ", " << upPath << std::endl;
		_downTexture = IMG_LoadTexture(renderer, downPath.c_str());
		_upTexture = IMG_LoadTexture(renderer, upPath.c_str());
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
			SDL_RenderTexture(renderer, _downTexture, NULL, &_drawRect);
		} else {
			SDL_RenderTexture(renderer, _upTexture, NULL, &_drawRect);
		}

		SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
	}
};