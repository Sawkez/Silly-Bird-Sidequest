#pragma once

#include <SDL3/SDL.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "engine/Math.hpp"
#include "engine/resource/BinaryReader.hpp"
#include "engine/world/TileBase.hpp"
#include "engine/world/WorldConstants.hpp"

struct SpikeTile : public TileBase {
	int GetDrawSourceSize() const override { return 8; }
	int GetDrawDestOffset() const override { return (WorldConstants::TILE_SIZE_F - GetDrawSourceSize()) / 2.0; }

	SpikeTile(BinaryReader& binary) {
		binary.Read(2, &x);
		binary.Read(2, &y);

		Uint8 atlasID;
		binary.Read(1, &atlasID);
		xAtlas = atlasID & 0b00000011;
		yAtlas = (atlasID >> 2) & 0b00000011;
	}

	SpikeTile(SDL_IOStream* file) {
		SDL_ReadIO(file, &x, 2);
		SDL_ReadIO(file, &y, 2);
		SDL_ReadIO(file, &xAtlas, 2);
		SDL_ReadIO(file, &yAtlas, 2);
	}
};