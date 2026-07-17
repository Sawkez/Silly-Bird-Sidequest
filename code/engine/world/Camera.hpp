#pragma once

#include <functional>

#include "game/player/Player.hpp"

class Camera {
   public:
	const Player& _player;
	SDL_Point _pixelSize;
	std::reference_wrapper<const Room> _room;
	SDL_Texture* _pixelTexture;

	Camera(SDL_Renderer* renderer, const Player& player, const Room& room)
		: _player(player), _room(room), _pixelSize{240, 136} {
		_pixelTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB1555, SDL_TEXTUREACCESS_TARGET,
										  _pixelSize.x + 16, _pixelSize.y + 16);
	}

	void SetRoom(const Room& room) { _room = std::ref(room); }

	void Process(float delta) {}

	void DrawRoom(SDL_Renderer* renderer) const {
		Vector2 cameraPos = _player.position - Vector2(_pixelSize) / 2;
		Vector2 texturePos{floor(cameraPos.x / 8.0f) * 8.0f, floor(cameraPos.y / 8.0f) * 8.0f};

		SDL_SetRenderTarget(renderer, _pixelTexture);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		SDL_RenderClear(renderer);

		SDL_FRect visibilityRect{0.0f, 0.0f, float(_pixelSize.x) + 16.0f, float(_pixelSize.y) + 16.0f};

		_room.get().Draw(renderer, visibilityRect, -texturePos);

		_player.Draw(renderer, visibilityRect, -texturePos);

		SDL_FRect hdRenderSource{cameraPos.x - texturePos.x, cameraPos.y - texturePos.y, float(_pixelSize.x),
								 float(_pixelSize.y)};

		SDL_SetRenderTarget(renderer, nullptr);
		SDL_RenderTexture(renderer, _pixelTexture, &hdRenderSource, nullptr);
	}
};