#pragma once

#include <SDL.h>

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

	ForegroundTile(std::ifstream& file) {
		file.read((char*)&x, 2);
		file.read((char*)&y, 2);
		file.read((char*)&xAtlas, 2);
		file.read((char*)&yAtlas, 2);
		file.read((char*)&sourceID, 2);
	}

	void Draw(SDL_Surface* targetSurface, const std::vector<SDL_Surface*>& atlases, int xOffset, int yOffset) {
		TileBase::Draw(targetSurface, atlases[sourceID], xOffset, yOffset);
	}
};