#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "engine/graphics/IDrawableRect.hpp"
#include "engine/resource/BinaryReader.hpp"
#include "engine/resource/StorageIO.hpp"
#include "engine/world/ForegroundTile.hpp"
#include "engine/world/SpikeTile.hpp"
#include "yyjson.h"

class RoomChunk : public IDrawableRect {
	const int OVERLAP_OFFSET = 8;

   private:
	SDL_Rect _rect;
	SDL_Texture* _cache = NULL;

   public:
	RoomChunk() : _rect({0, 0, 0, 0}) {}

	RoomChunk(SDL_Storage* storage, BinaryReader& binary, SDL_Renderer* renderer,
			  std::unordered_map<Uint8, SDL_Surface*>& atlases, SDL_Surface* spikeAtlas) {
		binary.FindSection("CHNK");

		Uint16 num;
		binary.Read(2, &num);
		_rect.x = num;
		binary.Read(2, &num);
		_rect.y = num;
		binary.Read(2, &num);
		_rect.w = num;
		binary.Read(2, &num);
		_rect.h = num;

		Uint32 tileCount;
		binary.Read(4, &tileCount);
		Uint32 spikeCount;
		binary.Read(4, &spikeCount);

		dc::msg << SDL_GetTicks() << ": Creating cache surface" << dc::endl;
		auto* cacheSurface = SDL_CreateSurface(_rect.w, _rect.h, SDL_PIXELFORMAT_ARGB1555);
		SDL_ClearSurface(cacheSurface, 1.0, 0.0, 0.0, 0.0);

		dc::msg << SDL_GetTicks() << ": Loading and caching " << tileCount << " tiles" << dc::endl;
		binary.FindSection("TLFG");
		for (int i = 0; i < tileCount; i++) {
			ForegroundTile tile(binary);
			tile.EnsureAtlasLoaded(storage, atlases);
			tile.Draw(cacheSurface, atlases, -_rect.x + OVERLAP_OFFSET, -_rect.y + OVERLAP_OFFSET);
		}

		dc::msg << SDL_GetTicks() << ": Loading and caching " << spikeCount << " spikes" << dc::endl;
		binary.FindSection("SPIK");
		for (int i = 0; i < spikeCount; i++) {
			SpikeTile spike(binary);
			spike.Draw(cacheSurface, spikeAtlas, -_rect.x + OVERLAP_OFFSET, -_rect.y + OVERLAP_OFFSET);
		}

		dc::msg << SDL_GetTicks() << ": Converting cache surface to texture" << dc::endl;
		_cache = SDL_CreateTextureFromSurface(renderer, cacheSurface);
	}

	RoomChunk(const RoomChunk&) = delete;
	RoomChunk& operator=(const RoomChunk&) = delete;

	RoomChunk(RoomChunk&& other) noexcept : _rect(other._rect), _cache(other._cache) { other._cache = NULL; }

	RoomChunk& operator=(RoomChunk&& other) noexcept {
		if (this != &other) {
			if (_cache) SDL_DestroyTexture(_cache);

			_rect = other._rect;
			_cache = other._cache;

			other._cache = NULL;
		}
		return *this;
	}

	void UncacheTiles() {
		SDL_DestroyTexture(_cache);
		_cache = NULL;
	}

	bool Draw(SDL_Renderer* renderer, const SDL_FRect& drawTargetRect, Vector2 drawOffset) const override {
		SDL_FRect destination{float(_rect.x) + drawOffset.x, float(_rect.y) + drawOffset.y, float(_rect.w),
							  float(_rect.h)};

		if (!SDL_HasRectIntersectionFloat(&drawTargetRect, &destination)) {
			return false;
		}

		SDL_RenderTexture(renderer, _cache, nullptr, &destination);
		return true;
	}

	/*
	void Draw(SDL_Renderer* renderer) const {
		if (_cache == NULL) {
			dc::err << "ERROR: chunk not cached!" << dc::endl;
		}

		SDL_FRect destination{0, 0, float(_rect.w), float(_rect.h)};

		SDL_RenderTexture(renderer, _cache, NULL, &destination);
	}
	*/

	int GetWidth() const { return _rect.w; }
	int GetHeight() const { return _rect.h; }

	const SDL_Rect& GetRect() const { return _rect; }

	SDL_FRect GetFRect() const { return SDL_FRect{float(_rect.x), float(_rect.y), float(_rect.w), float(_rect.h)}; }

	~RoomChunk() { UncacheTiles(); }
};