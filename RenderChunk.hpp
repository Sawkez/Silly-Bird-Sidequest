#pragma once

#include <SDL.h>
#include <functional>

#include "IDrawableRect.hpp"
#include "RoomChunk.hpp"

class RenderChunk {
	std::reference_wrapper<const RoomChunk> _roomChunk;
	SDL_Texture* _renderTexture;

  public:
	RenderChunk(const RoomChunk&) = delete;
	RenderChunk& operator=(const RoomChunk&) = delete;

	RenderChunk(RenderChunk&& other) noexcept : _roomChunk(other._roomChunk), _renderTexture(other._renderTexture) {
		other._renderTexture = NULL;
	}

	RenderChunk& operator=(RenderChunk&& other) noexcept {
		if (this != &other) {
			if (_renderTexture)
				SDL_DestroyTexture(_renderTexture);

			_roomChunk = other._roomChunk;
			_renderTexture = other._renderTexture;

			other._renderTexture = NULL;
		}
		return *this;
	}

	~RenderChunk() {
		if (_renderTexture != NULL) {
			SDL_DestroyTexture(_renderTexture);
		}
	}

	RenderChunk(const RoomChunk& roomChunk, SDL_Renderer* renderer)
		: _roomChunk(std::ref(roomChunk)),
		  _renderTexture(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA5551, SDL_TEXTUREACCESS_TARGET,
										   _roomChunk.get().rect.w, _roomChunk.get().rect.h)) {
		SDL_SetTextureBlendMode(_renderTexture, SDL_BLENDMODE_BLEND);
	}

	void DrawRoom(SDL_Renderer* renderer) const {
		SDL_SetRenderTarget(renderer, _renderTexture);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		SDL_RenderClear(renderer);

		_roomChunk.get().Draw(renderer);
	}

	void DrawObject(SDL_Renderer* renderer, const IDrawableRect& object, Vector2 roomPos) const {
		SDL_FRect roomRect = _roomChunk.get().GetFRect();
		SDL_FRect objectRect = object.GetRect();

		objectRect.x -= roomPos.x;
		objectRect.y -= roomPos.y;

		if (SDL_HasIntersectionF(&roomRect, &objectRect))
			object.Draw(renderer, Vector2{-roomRect.x - roomPos.x + 8.0f, -roomRect.y - roomPos.y + 8.0f});
	}

	void Draw(SDL_Renderer* renderer, Vector2 drawOffset, float zoom) const {
		SDL_SetRenderTarget(renderer, NULL);

		SDL_Rect destination = _roomChunk.get().rect;

		SDL_FRect FDestination{(float(destination.x) + drawOffset.x - 8) * zoom,
							   (float(destination.y) + drawOffset.y - 8) * zoom, float(destination.w) * zoom,
							   float(destination.h) * zoom};

		SDL_RenderCopyF(renderer, _renderTexture, NULL, &FDestination);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderDrawRectF(renderer, &FDestination);
	}

	SDL_Rect GetRect() const { return _roomChunk.get().rect; }
};