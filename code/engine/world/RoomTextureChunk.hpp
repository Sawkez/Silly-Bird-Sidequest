#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

#include "engine/graphics/IDrawableRect.hpp"
#include "engine/resource/BinaryReader.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "engine/resource/StorageIO.hpp"
#include "engine/world/ForegroundTile.hpp"
#include "engine/world/WorldConstants.hpp"
#include "yyjson.h"

class RoomTextureChunk : public IDrawableRect {
	const int OVERLAP_OFFSET = 8;

   private:
	SDL_Rect _rect;
	SDL_Texture* _cache = NULL;

	static inline SDL_Vertex SPIKE_VERTICES[4096 * 4];
	static inline int VERTEX_INDICES[4096 * 6];

   public:
	RoomTextureChunk() : _rect({0, 0, 0, 0}) {}

	RoomTextureChunk(SDL_Storage* storage, BinaryReader& binary, SDL_Renderer* renderer,
					 std::unordered_map<Uint8, SDL_Texture*>& atlases, SDL_Texture* spikeAtlas) {
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

		_cache = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB1555, SDL_TEXTUREACCESS_TARGET, _rect.w, _rect.h);
		SDL_SetRenderTarget(renderer, _cache);

		// SDL_RenderClear currently bugged on PSP
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_FRect fill{0.0, 0.0, float(_rect.w), float(_rect.h)};
		SDL_RenderFillRect(renderer, &fill);

		dc::msg << SDL_GetTicks() << ": Loading and caching " << tileCount << " tiles" << dc::endl;
		binary.FindSection("TLFG");
		for (int i = 0; i < tileCount; i++) {
			ForegroundTile tile(binary);
			tile.EnsureAtlasLoaded(renderer, storage, atlases);
			tile.Draw(renderer, atlases, -_rect.x + OVERLAP_OFFSET, -_rect.y + OVERLAP_OFFSET);
		}

		dc::msg << SDL_GetTicks() << ": Loading and caching " << spikeCount << " spikes" << dc::endl;
		binary.FindSection("SPIK");

		for (int i = 0; i < spikeCount; i++) {
			Uint16 xSpike, ySpike;
			binary.Read(2, &xSpike);
			binary.Read(2, &ySpike);

			float xSpikeChunk = xSpike * WorldConstants::TILE_SIZE_F + OVERLAP_OFFSET - _rect.x;
			float ySpikeChunk = ySpike * WorldConstants::TILE_SIZE_F + OVERLAP_OFFSET - _rect.y;

			Uint8 spikeMask;
			binary.Read(1, &spikeMask);

			float xAtlas = (spikeMask & 15) / 16.0f;  //	% 16
			float yAtlas = (spikeMask >> 4) / 16.0f;  //	/ 16

			// clang-format off
			SPIKE_VERTICES[i * 4] = SDL_Vertex {
				SDL_FPoint{xSpikeChunk, ySpikeChunk},
				SDL_FColor{1.0, 1.0, 1.0, 1.0},
				SDL_FPoint{xAtlas, yAtlas}
			};

			SPIKE_VERTICES[i * 4 + 1] = SDL_Vertex {
				SDL_FPoint{xSpikeChunk + WorldConstants::TILE_SIZE_F, ySpikeChunk},
				SDL_FColor{1.0, 1.0, 1.0, 1.0},
				SDL_FPoint{xAtlas + 1.0f / 16.0f, yAtlas}
			};

			SPIKE_VERTICES[i * 4 + 2] = SDL_Vertex {
				SDL_FPoint{xSpikeChunk, ySpikeChunk + WorldConstants::TILE_SIZE_F},
				SDL_FColor{1.0, 1.0, 1.0, 1.0},
				SDL_FPoint{xAtlas, yAtlas + 1.0f / 16.0f}
			};

			SPIKE_VERTICES[i * 4 + 3] = SDL_Vertex {
				SDL_FPoint{xSpikeChunk + WorldConstants::TILE_SIZE_F, ySpikeChunk + WorldConstants::TILE_SIZE_F},
				SDL_FColor{1.0, 1.0, 1.0, 1.0},
				SDL_FPoint{xAtlas + 1.0f / 16.0f, yAtlas + 1.0f / 16.0f}
			};

			// clang-format on

			VERTEX_INDICES[i * 6] = i * 4;
			VERTEX_INDICES[i * 6 + 1] = i * 4 + 1;
			VERTEX_INDICES[i * 6 + 2] = i * 4 + 2;
			VERTEX_INDICES[i * 6 + 3] = i * 4 + 1;
			VERTEX_INDICES[i * 6 + 4] = i * 4 + 2;
			VERTEX_INDICES[i * 6 + 5] = i * 4 + 3;
		}

		SDL_RenderGeometry(renderer, spikeAtlas, SPIKE_VERTICES, spikeCount * 4, VERTEX_INDICES, spikeCount * 6);
	}

	RoomTextureChunk(const RoomTextureChunk&) = delete;
	RoomTextureChunk& operator=(const RoomTextureChunk&) = delete;

	RoomTextureChunk(RoomTextureChunk&& other) noexcept : _rect(other._rect), _cache(other._cache) {
		other._cache = NULL;
	}

	RoomTextureChunk& operator=(RoomTextureChunk&& other) noexcept {
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
		SDL_FRect destination{float(_rect.x) + drawOffset.x - WorldConstants::TILE_SIZE_F,
							  float(_rect.y) + drawOffset.y - WorldConstants::TILE_SIZE_F, float(_rect.w),
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

	~RoomTextureChunk() { UncacheTiles(); }
};