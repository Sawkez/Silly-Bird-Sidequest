#pragma once

#include <SDL3/SDL.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "engine/Math.hpp"
#include "engine/world/TileBase.hpp"
#include "engine/world/WorldConstants.hpp"

struct ForegroundTile : public TileBase {
	uint16_t sourceID = 0;

	int GetDrawSourceSize() const override { return 14; }
	int GetDrawDestOffset() const override { return (WorldConstants::TILE_SIZE_F - GetDrawSourceSize()) / 2.0; }

	ForegroundTile(SDL_IOStream* file) {
		SDL_ReadIO(file, &x, 2);
		SDL_ReadIO(file, &y, 2);
		SDL_ReadIO(file, &xAtlas, 2);
		SDL_ReadIO(file, &yAtlas, 2);
		SDL_ReadIO(file, &sourceID, 2);
	}

	void Draw(SDL_Surface* targetSurface, const std::vector<SDL_Surface*>& atlases, int xOffset, int yOffset) {
		TileBase::Draw(targetSurface, atlases.at(sourceID), xOffset, yOffset);
	}
};