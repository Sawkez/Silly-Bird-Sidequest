#pragma once

#include <SDL3/SDL.h>

#include <unordered_map>

#include "engine/PlatformDefines.hpp"
#include "engine/graphics/IDrawableRect.hpp"
#include "engine/graphics/RoomMeshChunk.hpp"
#include "engine/mods/ModManager.hpp"
#include "engine/resource/BinaryReader.hpp"
#include "engine/resource/ResourceManager.hpp"

class RoomMeshChunkRenderer : public IDrawableRect {
   private:
	std::vector<RoomMeshChunk> _chunks = {};
	SDL_Texture* _megaTexture = nullptr;

   public:
	RoomMeshChunkRenderer() {}

	RoomMeshChunkRenderer(SDL_Renderer* renderer, SDL_Storage* storage, int chunkCount, BinaryReader& binary) {
		SDL_Surface* spikeAtlas =
			ResourceManager::LoadSurface(ModManager::GetBuiltinStorage(), "tiles/special/spikes.png");

		binary.EnsureSection("CHNK");
		const char* chunkDataStart = binary.GetCurrentPosition();

		SDL_Point currentOffset = {spikeAtlas->w, 0};
		int rowHeight = spikeAtlas->h;
		SDL_Point subAtlasPosition{spikeAtlas->w, spikeAtlas->h};

		SDL_Surface* megaAtlas =
			SDL_CreateSurface(PLATFORM_MEGA_ATLAS_MAX_WIDTH, PLATFORM_MEGA_ATLAS_MAX_HEIGHT, SDL_PIXELFORMAT_ARGB1555);
		std::unordered_map<Uint8, SDL_Point> offsets;

		SDL_Rect destination = {0, 0, spikeAtlas->w, spikeAtlas->h};

		SDL_BlitSurface(spikeAtlas, NULL, megaAtlas, &destination);

		for (int i = 0; i < chunkCount; i++) {
			binary.EnsureSection("CHNK");
			binary.Skip(2 + 2 + 2 + 2);

			Uint32 tileCount;
			binary.Read(4, &tileCount);

			binary.EnsureSection("TLFG");
			for (int j = 0; j < tileCount; j++) {
				binary.Skip(2 + 2 + 2 + 2);
				Uint8 sourceID;
				binary.Read(1, &sourceID);

				if (offsets.find(sourceID) != offsets.end()) continue;

				SDL_Surface* newAtlas = ModManager::LoadTileSourceAsSurface(renderer, sourceID);

				currentOffset.x += newAtlas->w;

				if (currentOffset.x + newAtlas->w > PLATFORM_MEGA_ATLAS_MAX_WIDTH) {
					currentOffset.y += rowHeight;
					currentOffset.x = 0;
					rowHeight = 0;

					if (currentOffset.y > PLATFORM_MEGA_ATLAS_MAX_HEIGHT) {
						dc::err << "ERROR: Mega atlas too big! some tiles will not render" << dc::endl;
						break;
					}
				}

				destination = {currentOffset.x, currentOffset.y, newAtlas->w, newAtlas->h};
				rowHeight = std::max(rowHeight, newAtlas->h);

				offsets[sourceID] = currentOffset;

				SDL_BlitSurface(newAtlas, NULL, megaAtlas, &destination);
			}
		}

		_megaTexture = SDL_CreateTextureFromSurface(renderer, megaAtlas);

		if (_megaTexture == nullptr) {
			dc::err << "Failed to create mega atlas texture: " << SDL_GetError() << dc::endl;
		}

		SDL_DestroySurface(megaAtlas);

		binary.GoToChunkAtPosition(chunkDataStart);

		_chunks.reserve(chunkCount);
		for (int i = 0; i < chunkCount; i++) {
			_chunks.emplace_back(binary, offsets, _megaTexture);
		}
	}

	~RoomMeshChunkRenderer() override { SDL_DestroyTexture(_megaTexture); }

	bool Draw(SDL_Renderer* renderer, const SDL_FRect& drawTargetRect, Vector2 drawOffset) const override {
		bool drawn = false;

		for (const auto& chunk : _chunks) {
			drawn |= chunk.Draw(renderer, drawTargetRect, drawOffset);
		}

		return drawn;
	}
};