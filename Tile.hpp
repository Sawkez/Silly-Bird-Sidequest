#pragma once

#include <SDL.h>

#include <fstream>
#include <iostream>

#include "Math.hpp"
#include "WorldConstants.hpp"

struct Tile {
	static const int DRAW_SOURCE_SIZE = 14.0;
	static const int DRAW_DEST_OFFSET =
		(WorldConstants::TILE_SIZE_F - DRAW_SOURCE_SIZE) / 2.0;

	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t xAtlas = 0;
	uint16_t yAtlas = 0;
	uint16_t sourceID = 0;

	Tile() {};
	Tile(ifstream& file) {
		file.read(reinterpret_cast<char*>(&x), 2);
		file.read(reinterpret_cast<char*>(&y), 2);
		file.read(reinterpret_cast<char*>(&xAtlas), 2);
		file.read(reinterpret_cast<char*>(&yAtlas), 2);
		file.read(reinterpret_cast<char*>(&sourceID), 2);
	}

	void Draw(SDL_Surface* targetSurface, const vector<SDL_Surface*>& atlases,
			  int xOffset, int yOffset) {
		SDL_Rect source{xAtlas * DRAW_SOURCE_SIZE, yAtlas * DRAW_SOURCE_SIZE,
						DRAW_SOURCE_SIZE, DRAW_SOURCE_SIZE};

		SDL_Rect destination{
			x * WorldConstants::TILE_SIZE + DRAW_DEST_OFFSET + xOffset,
			y * WorldConstants::TILE_SIZE + DRAW_DEST_OFFSET + yOffset,
			DRAW_SOURCE_SIZE, DRAW_SOURCE_SIZE};

		int error = SDL_BlitSurface(atlases.at(sourceID), &source,
									targetSurface, &destination);

		if (error < 0) {
			cerr << "ERROR when caching tile from source " << sourceID << ": "
				 << SDL_GetError() << endl;
		}
	}
};

std::ostream& operator<<(std::ostream& out, const Tile& tile) {
	out << "( " << tile.x << ", " << tile.y << " )";
	return out;
}