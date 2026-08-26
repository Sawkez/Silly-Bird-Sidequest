#pragma once

#include <SDL3/SDL.h>

#include <unordered_map>

#include "engine/Vector2.hpp"
#include "engine/world/RoomChunkBase.hpp"
#include "engine/world/WorldConstants.hpp"

class RoomMeshChunk : public RoomChunkBase {
   private:
	// clang-format off
	static inline const Vector2 TOP_LEFT				= {0.0, 0.0};
	static inline const Vector2 TOP_RIGHT				= {1.0, 0.0};
	static inline const Vector2 BOTTOM_LEFT				= {0.0, 1.0};
	static inline const Vector2 BOTTOM_RIGHT			= {1.0, 1.0};

	static inline const Vector2 TILE_TOP_LEFT			= TOP_LEFT		* WorldConstants::TILE_TEXTURE_SIZE_F;
	static inline const Vector2 TILE_TOP_RIGHT			= TOP_RIGHT		* WorldConstants::TILE_TEXTURE_SIZE_F;
	static inline const Vector2 TILE_BOTTOM_LEFT		= BOTTOM_LEFT	* WorldConstants::TILE_TEXTURE_SIZE_F;
	static inline const Vector2 TILE_BOTTOM_RIGHT		= BOTTOM_RIGHT	* WorldConstants::TILE_TEXTURE_SIZE_F;

	static inline const Vector2 SPIKE_TOP_LEFT			= TOP_LEFT		* WorldConstants::TILE_SIZE_F;
	static inline const Vector2 SPIKE_TOP_RIGHT			= TOP_RIGHT		* WorldConstants::TILE_SIZE_F;
	static inline const Vector2 SPIKE_BOTTOM_LEFT		= BOTTOM_LEFT	* WorldConstants::TILE_SIZE_F;
	static inline const Vector2 SPIKE_BOTTOM_RIGHT		= BOTTOM_RIGHT	* WorldConstants::TILE_SIZE_F;
	// clang-format on

	SDL_Texture* _megaAtlas;
	SDL_Vertex* _vertices;
	int* _indices;
	int _quadCount;

	struct RoomMeshChunkCustomData {
		const std::unordered_map<Uint8, int>& offsets;
		int tileCount;
		Vector2 megaAtlasRes;
	};

   public:
	RoomMeshChunk(BinaryReader& binary, const std::unordered_map<Uint8, int>& offsets, SDL_Texture* megaAtlas)
		: _megaAtlas(megaAtlas) {
		auto data = RoomMeshChunkCustomData{offsets, 0, Vector2(megaAtlas->w, megaAtlas->h)};
		BuildChunk(binary, &data);
	}

	void PrepareCache(int tileCount, int spikeCount, void* customData) override {
		auto* data = (RoomMeshChunkCustomData*)customData;

		_quadCount = tileCount + spikeCount;

		_vertices = new SDL_Vertex[_quadCount * 4];
		_indices = new int[_quadCount * 6];

		data->tileCount = tileCount;
	}

	void CacheTile(int index, float x, float y, float xAtlas, float yAtlas, Uint8 sourceID, void* customData) override {
		auto* data = (RoomMeshChunkCustomData*)customData;
		int offset = data->offsets.find(sourceID)->second;

		Vector2 destPos = Vector2(x, y);
		Vector2 sourcePos = Vector2(xAtlas, yAtlas);
		sourcePos.x += float(offset);

		int i = index * 4;

		// clang-format off
        _vertices[i + 0] = {
			destPos + TILE_TOP_LEFT,
			SDL_FColor{1.0f, 1.0f, 1.0f, 1.0f},
			(sourcePos + TILE_TOP_LEFT) / data->megaAtlasRes
		};

		_vertices[i + 1] = {
			destPos + TILE_TOP_RIGHT,
			SDL_FColor{1.0f, 1.0f, 1.0f, 1.0f},
			(sourcePos + TILE_TOP_RIGHT) / data->megaAtlasRes
		};

		_vertices[i + 2] = {
			destPos + TILE_BOTTOM_LEFT,
			SDL_FColor{1.0f, 1.0f, 1.0f, 1.0f},
			(sourcePos + TILE_BOTTOM_LEFT) / data->megaAtlasRes
		};

		_vertices[i + 3] = {
			destPos + TILE_BOTTOM_RIGHT,
			SDL_FColor{1.0f, 1.0f, 1.0f, 1.0f},
			(sourcePos + TILE_BOTTOM_RIGHT) / data->megaAtlasRes
		};
		// clang-format on

		int j = index * 6;
		_indices[j + 0] = i + 0;
		_indices[j + 1] = i + 1;
		_indices[j + 2] = i + 2;

		_indices[j + 3] = i + 1;
		_indices[j + 4] = i + 2;
		_indices[j + 5] = i + 3;
	}

	void CacheSpike(int index, float x, float y, float xAtlas, float yAtlas, void* customData) override {
		auto* data = (RoomMeshChunkCustomData*)customData;

		Vector2 sourcePos(xAtlas, yAtlas);
		Vector2 destPos(x, y);

		int i = (index + data->tileCount) * 4;

		// clang-format off
		_vertices[i + 0] = {
			destPos + SPIKE_TOP_LEFT,
			SDL_FColor{1.0f, 1.0f, 1.0f, 1.0f},
			(sourcePos + SPIKE_TOP_LEFT) / data->megaAtlasRes
		};

		_vertices[i + 1] = {
			destPos + SPIKE_TOP_RIGHT,
			SDL_FColor{1.0f, 1.0f, 1.0f, 1.0f},
			(sourcePos + SPIKE_TOP_RIGHT) / data->megaAtlasRes
		};

		_vertices[i + 2] = {
			destPos + SPIKE_BOTTOM_LEFT,
			SDL_FColor{1.0f, 1.0f, 1.0f, 1.0f},
			(sourcePos + SPIKE_BOTTOM_LEFT) / data->megaAtlasRes
		};

		_vertices[i + 3] = {
			destPos + SPIKE_BOTTOM_RIGHT,
			SDL_FColor{1.0f, 1.0f, 1.0f, 1.0f},
			(sourcePos + SPIKE_BOTTOM_RIGHT) / data->megaAtlasRes
		};
		// clang-format on

		int j = (index + data->tileCount) * 6;
		_indices[j + 0] = i + 0;
		_indices[j + 1] = i + 1;
		_indices[j + 2] = i + 2;

		_indices[j + 3] = i + 1;
		_indices[j + 4] = i + 2;
		_indices[j + 5] = i + 3;
	}

	void FinalizeCache(void* customData, int tileCount, int spikeCount) override {}

	bool Draw(SDL_Renderer* renderer, const SDL_FRect& drawTargetRect, Vector2 drawOffset) const override {
		SDL_FRect destinationF{float(_rect.x) + drawOffset.x - OVERLAP_OFFSET,
							   float(_rect.y) + drawOffset.y - OVERLAP_OFFSET, float(_rect.w), float(_rect.h)};

		if (!SDL_HasRectIntersectionFloat(&drawTargetRect, &destinationF)) {
			return false;
		}

		SDL_Rect destination{_rect.x + drawOffset.x - OVERLAP_OFFSET, _rect.y + drawOffset.y - OVERLAP_OFFSET, _rect.w,
							 _rect.h};

		SDL_SetRenderViewport(renderer, &destination);
		SDL_RenderGeometry(renderer, _megaAtlas, _vertices, _quadCount * 4, _indices, _quadCount * 6);
		SDL_SetRenderViewport(renderer, nullptr);

		return true;
	}
};