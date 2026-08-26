#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <fstream>
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

#include "engine/PlatformDefines.hpp"
#include "engine/graphics/IDrawableRect.hpp"
#include "engine/resource/BinaryReader.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "engine/resource/StorageIO.hpp"
#include "engine/world/ForegroundTile.hpp"
#include "engine/world/RoomChunkBase.hpp"
#include "engine/world/WorldConstants.hpp"
#include "yyjson.h"

class RoomTextureChunk : public RoomChunkBase {
   private:
	struct RoomTextureChunkCustomData {
		SDL_Renderer* renderer;
		SDL_Storage* storage;
		std::unordered_map<Uint8, SDL_Texture*>& atlases;
		SDL_Texture* spikeAtlas;
	};

	SDL_Texture* _cache = NULL;

	static inline SDL_Vertex SPIKE_VERTICES[PLATFORM_SPIKES_PER_CHUNK * 4];
	static inline int VERTEX_INDICES[PLATFORM_SPIKES_PER_CHUNK * 6];

   public:
	RoomTextureChunk(SDL_Storage* storage, BinaryReader& binary, SDL_Renderer* renderer,
					 std::unordered_map<Uint8, SDL_Texture*>& atlases, SDL_Texture* spikeAtlas) {
		RoomTextureChunkCustomData data{renderer, storage, atlases, spikeAtlas};
		BuildChunk(binary, &data);
	}

	void PrepareCache(int tileCount, int spikeCount, void* customData) override {
		auto* data = (RoomTextureChunkCustomData*)customData;
		_cache =
			SDL_CreateTexture(data->renderer, SDL_PIXELFORMAT_ARGB1555, SDL_TEXTUREACCESS_TARGET, _rect.w, _rect.h);
		SDL_SetRenderTarget(data->renderer, _cache);

		// SDL_RenderClear currently bugged on PSP
		SDL_SetRenderDrawColor(data->renderer, 255, 255, 255, 255);
		SDL_FRect fill{0.0, 0.0, float(_rect.w), float(_rect.h)};
		SDL_RenderFillRect(data->renderer, &fill);

		if (spikeCount > PLATFORM_SPIKES_PER_CHUNK) {
			dc::err << "ERROR: Too many spikes in this chunk. Some will be invisible." << dc::endl;
		}
	}

	void CacheTile(int index, float x, float y, float xAtlas, float yAtlas, Uint8 sourceID, void* customData) override {
		auto* data = (RoomTextureChunkCustomData*)customData;
		auto pair = data->atlases.find(sourceID);

		SDL_Texture* atlas;

		if (pair == data->atlases.end()) {
			atlas = ModManager::LoadTileSource(data->renderer, sourceID);
			data->atlases[sourceID] = atlas;
		} else {
			atlas = pair->second;
		}

		SDL_FRect source{xAtlas, yAtlas, WorldConstants::TILE_TEXTURE_SIZE_F, WorldConstants::TILE_TEXTURE_SIZE_F};
		SDL_FRect destination{x, y, WorldConstants::TILE_TEXTURE_SIZE_F, WorldConstants::TILE_TEXTURE_SIZE_F};

		SDL_RenderTexture(data->renderer, atlas, &source, &destination);
	}

	virtual void CacheSpike(int index, float x, float y, float xAtlas, float yAtlas, void* customData) override {
		auto* data = (RoomTextureChunkCustomData*)customData;

		if (index > PLATFORM_SPIKES_PER_CHUNK - 1) return;

		Vector2 spikeTextureSize(data->spikeAtlas->w, data->spikeAtlas->h);

		// clang-format off
		SPIKE_VERTICES[index * 4] = SDL_Vertex {
			SDL_FPoint{x, y},
			SDL_FColor{1.0, 1.0, 1.0, 1.0},
			Vector2{xAtlas, yAtlas} / spikeTextureSize
		};

		SPIKE_VERTICES[index * 4 + 1] = SDL_Vertex {
			SDL_FPoint{x + WorldConstants::TILE_SIZE_F, y},
			SDL_FColor{1.0, 1.0, 1.0, 1.0},
			Vector2{xAtlas + 1.0f / 16.0f, yAtlas} / spikeTextureSize
		};

		SPIKE_VERTICES[index * 4 + 2] = SDL_Vertex {
			SDL_FPoint{x, y + WorldConstants::TILE_SIZE_F},
			SDL_FColor{1.0, 1.0, 1.0, 1.0},
			Vector2{xAtlas, yAtlas + 1.0f / 16.0f} / spikeTextureSize
		}; 

		SPIKE_VERTICES[index * 4 + 3] = SDL_Vertex {
			SDL_FPoint{x + WorldConstants::TILE_SIZE_F, y + WorldConstants::TILE_SIZE_F},
			SDL_FColor{1.0, 1.0, 1.0, 1.0},
			Vector2{xAtlas + 1.0f / 16.0f, yAtlas + 1.0f / 16.0f} / spikeTextureSize
		};
		// clang-format on

		VERTEX_INDICES[index * 6] = index * 4;
		VERTEX_INDICES[index * 6 + 1] = index * 4 + 1;
		VERTEX_INDICES[index * 6 + 2] = index * 4 + 2;
		VERTEX_INDICES[index * 6 + 3] = index * 4 + 1;
		VERTEX_INDICES[index * 6 + 4] = index * 4 + 2;
		VERTEX_INDICES[index * 6 + 5] = index * 4 + 3;
	}

	void FinalizeCache(void* customData, int tileCount, int spikeCount) override {
		auto* data = (RoomTextureChunkCustomData*)customData;

		SDL_RenderGeometry(data->renderer, data->spikeAtlas, SPIKE_VERTICES, spikeCount * 4, VERTEX_INDICES,
						   spikeCount * 6);
	}

	void UncacheTiles() {
		SDL_DestroyTexture(_cache);
		_cache = NULL;
	}

	bool Draw(SDL_Renderer* renderer, const SDL_FRect& drawTargetRect, Vector2 drawOffset) const override {
		SDL_FRect destination{float(_rect.x) + drawOffset.x - OVERLAP_OFFSET,
							  float(_rect.y) + drawOffset.y - OVERLAP_OFFSET, float(_rect.w), float(_rect.h)};

		if (!SDL_HasRectIntersectionFloat(&drawTargetRect, &destination)) {
			return false;
		}

		SDL_RenderTexture(renderer, _cache, nullptr, &destination);
		return true;
	}

	~RoomTextureChunk() { UncacheTiles(); }
};