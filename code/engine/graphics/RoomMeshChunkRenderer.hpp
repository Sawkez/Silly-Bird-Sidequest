#pragma once

#include <SDL3/SDL.h>

#include <unordered_map>

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

		int megaAtlasWidth = spikeAtlas->w;
		int megaAtlasHeight = spikeAtlas->h;

		std::unordered_map<Uint8, SDL_Surface*> atlases;

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

				if (atlases.find(sourceID) != atlases.end()) continue;

				SDL_Surface* newAtlas = ModManager::LoadTileSourceAsSurface(renderer, sourceID);

				megaAtlasWidth += newAtlas->w;
				megaAtlasHeight = std::max(megaAtlasHeight, newAtlas->h);

				atlases[sourceID] = newAtlas;
			}
		}

		SDL_Surface* megaAtlas = SDL_CreateSurface(megaAtlasWidth, megaAtlasHeight, SDL_PIXELFORMAT_ARGB1555);
		SDL_ClearSurface(megaAtlas, 0, 0, 0, 0);

		SDL_Rect destination{0, 0, spikeAtlas->w, spikeAtlas->h};
		SDL_BlitSurface(spikeAtlas, nullptr, megaAtlas, &destination);

		int currentOffset = spikeAtlas->w;
		std::unordered_map<Uint8, int> offsets;

		SDL_DestroySurface(spikeAtlas);

		for (const auto& pair : atlases) {
			offsets[pair.first] = currentOffset;
			destination.x = currentOffset;
			currentOffset += pair.second->w;
			destination.w = pair.second->w;
			destination.h = pair.second->h;
			SDL_BlitSurface(pair.second, nullptr, megaAtlas, &destination);
			SDL_DestroySurface(pair.second);
		}

		_megaTexture = SDL_CreateTextureFromSurface(renderer, megaAtlas);
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