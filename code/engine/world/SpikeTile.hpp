#pragma once

#include <SDL3/SDL.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "engine/Math.hpp"
#include "engine/world/TileBase.hpp"
#include "engine/world/WorldConstants.hpp"

struct SpikeTile : public TileBase {
	int GetDrawSourceSize() const override { return 8; }
	int GetDrawDestOffset() const override { return (WorldConstants::TILE_SIZE_F - GetDrawSourceSize()) / 2.0; }

	SpikeTile(SDL_IOStream* file) {
		SDL_ReadIO(file, &x, 2);
		SDL_ReadIO(file, &y, 2);
		SDL_ReadIO(file, &xAtlas, 2);
		SDL_ReadIO(file, &yAtlas, 2);
	}
};