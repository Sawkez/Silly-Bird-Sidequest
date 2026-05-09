#pragma once

#include <SDL3/SDL.h>

#include <fstream>
#include <iostream>
#include <map>
#include <vector>

#include "engine/Math.hpp"
#include "engine/mods/ModManager.hpp"
#include "engine/world/TileBase.hpp"
#include "engine/world/WorldConstants.hpp"

struct ForegroundTile : public TileBase {
	uint8_t sourceID = 0;

	int GetDrawSourceSize() const override { return 14; }
	int GetDrawDestOffset() const override { return (WorldConstants::TILE_SIZE_F - GetDrawSourceSize()) / 2.0; }

	ForegroundTile(SDL_IOStream* file) {
		SDL_ReadIO(file, &x, 2);
		SDL_ReadIO(file, &y, 2);
		SDL_ReadIO(file, &xAtlas, 2);
		SDL_ReadIO(file, &yAtlas, 2);
		SDL_ReadIO(file, &sourceID, 1);
	}

	void Draw(SDL_Surface* targetSurface, const std::map<uint8_t, SDL_Surface*>& atlases, int xOffset, int yOffset) {
		TileBase::Draw(targetSurface, atlases.at(sourceID), xOffset, yOffset);
	}

	void EnsureAtlasLoaded(std::map<uint8_t, SDL_Surface*>& atlases) const {
		if (atlases.find(sourceID) != atlases.end()) return;

		atlases[sourceID] = SDL_LoadSurface(ModManager::GetTileSourcePath(sourceID).c_str());
	}
};