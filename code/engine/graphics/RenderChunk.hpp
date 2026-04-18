#pragma once

#include <SDL.h>

#include <functional>

#include "engine/graphics/IDrawableRect.hpp"
#include "engine/world/RoomChunk.hpp"

class RenderChunk {
	std::reference_wrapper<const RoomChunk> _roomChunk;
	SDL_Texture* _renderTexture;

   public:
	RenderChunk(const RoomChunk&) = delete;
	RenderChunk& operator=(const RoomChunk&) = delete;

	RenderChunk(RenderChunk&& other) noexcept : _roomChunk(other._roomChunk), _renderTexture(other._renderTexture) { other._renderTexture = NULL; }

	RenderChunk& operator=(RenderChunk&& other) noexcept {
		if (this != &other) {
			if (_renderTexture) SDL_DestroyTexture(_renderTexture);

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
		  _renderTexture(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA5551, SDL_TEXTUREACCESS_TARGET, _roomChunk.get().GetWidth(),
										   _roomChunk.get().GetHeight())) {
		SDL_SetTextureBlendMode(_renderTexture, SDL_BLENDMODE_BLEND);
	}

	void DrawRoom(SDL_Renderer* renderer) const {
		SDL_SetRenderTarget(renderer, _renderTexture);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		SDL_RenderClear(renderer);

		_roomChunk.get().Draw(renderer);
	}

	void DrawObject(SDL_Renderer* renderer, const IDrawableRect& object, const Vector2& roomPos) const {
		SDL_FRect roomRect = _roomChunk.get().GetFRect();

		Vector2 drawOffset{-roomRect.x - roomPos.x + 8.0f, -roomRect.y - roomPos.y + 8.0f};

		SDL_FRect localTarget = {8.0f, 8.0f, roomRect.w, roomRect.h};

		object.Draw(renderer, localTarget, drawOffset);
	}

	void Draw(SDL_Renderer* renderer, Vector2 drawOffset, float zoom) const {
		SDL_SetRenderTarget(renderer, NULL);

		SDL_Rect destination = _roomChunk.get().GetRect();

		SDL_FRect FDestination{(float(destination.x) + drawOffset.x - 8) * zoom, (float(destination.y) + drawOffset.y - 8) * zoom,
							   float(destination.w) * zoom, float(destination.h) * zoom};

		SDL_RenderTexture(renderer, _renderTexture, NULL, &FDestination);
	}

	SDL_Rect GetRect() const { return _roomChunk.get().GetRect(); }
};