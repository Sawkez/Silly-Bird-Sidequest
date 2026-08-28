#pragma once

#include <SDL3/SDL.h>

#include <unordered_map>
#include <vector>

#include "engine/Vector2.hpp"
#include "engine/graphics/IDrawableRect.hpp"
#include "engine/graphics/RoomTextureChunk.hpp"
#include "engine/resource/BinaryReader.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "engine/world/WorldConstants.hpp"

class RoomTextureChunkRenderer : public IDrawableRect {
   private:
	std::vector<RoomTextureChunk> _chunks = {};

   public:
	RoomTextureChunkRenderer() {}

	RoomTextureChunkRenderer(SDL_Renderer* renderer, SDL_Storage* storage, int chunkCount, BinaryReader& binary) {
		_chunks.reserve(chunkCount);
		std::unordered_map<Uint8, SDL_Texture*> atlases;
		SDL_Texture* spikeAtlas =
			ResourceManager::LoadTexture(renderer, ModManager::GetBuiltinStorage(), "tiles/special/spikes.png");

		for (int i = 0; i < chunkCount; i++) {
			_chunks.emplace_back(storage, binary, renderer, atlases, spikeAtlas);
		}

		SDL_DestroyTexture(spikeAtlas);

		for (auto& pair : atlases) {
			SDL_DestroyTexture(pair.second);
		}
	}

	bool Draw(SDL_Renderer* renderer, const SDL_FRect& drawTargetRect, Vector2 drawOffset) const override {
		bool drawn = false;

		for (const auto& chunk : _chunks) {
			drawn |= chunk.Draw(renderer, drawTargetRect, drawOffset);
		}

		return drawn;
	}
};