#pragma once

#include <SDL.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "engine/Math.hpp"
#include "engine/world/TileBase.hpp"
#include "engine/world/WorldConstants.hpp"

struct SpikeTile : public TileBase {
	int GetDrawSourceSize() const override { return 8; }
	int GetDrawDestOffset() const override { return (WorldConstants::TILE_SIZE_F - GetDrawSourceSize()) / 2.0; }

	SpikeTile(std::ifstream& file) {
		file.read((char*)&x, 2);
		file.read((char*)&y, 2);
		file.read((char*)&xAtlas, 2);
		file.read((char*)&yAtlas, 2);
	}
};