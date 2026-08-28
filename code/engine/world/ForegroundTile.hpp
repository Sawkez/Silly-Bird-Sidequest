#pragma once

#include <SDL3/SDL.h>

#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "engine/Math.hpp"
#include "engine/mods/ModManager.hpp"
#include "engine/resource/BinaryReader.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "engine/world/TileBase.hpp"
#include "engine/world/WorldConstants.hpp"

struct ForegroundTile : public TileBase {
	uint8_t sourceID = 0;

	int GetDrawSourceSize() const override { return 14; }
	int GetDrawDestOffset() const override { return (WorldConstants::TILE_SIZE_F - GetDrawSourceSize()) / 2.0; }

	ForegroundTile(Uint16 X, Uint16 Y, Uint16 XAtlas, Uint16 YAtlas, Uint8 SourceID)
		: TileBase{X, Y, XAtlas, YAtlas}, sourceID(SourceID) {}

	ForegroundTile(BinaryReader& binary) {
		binary.Read(2, &x);
		binary.Read(2, &y);
		binary.Read(2, &xAtlas);
		binary.Read(2, &yAtlas);
		binary.Read(1, &sourceID);
	}

	void Draw(SDL_Renderer* renderer, const std::unordered_map<uint8_t, SDL_Texture*>& atlases, int xOffset,
			  int yOffset) {
		TileBase::Draw(renderer, atlases.at(sourceID), xOffset, yOffset);
	}

	void EnsureAtlasLoaded(SDL_Renderer* renderer, SDL_Storage* storage,
						   std::unordered_map<uint8_t, SDL_Texture*>& atlases) const {
		if (atlases.find(sourceID) != atlases.end()) return;

		atlases[sourceID] = ModManager::LoadTileSource(renderer, sourceID);
	}
};