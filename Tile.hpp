#pragma once

#include <SDL.h>
#include <fstream>
#include <iostream>

#include "Json.hpp"

struct Tile {
	static const int DRAW_SOURCE_SIZE = 14.0;
	static const int DRAW_DEST_SIZE = 8.0;
	static const int DRAW_DEST_OFFSET = (DRAW_DEST_SIZE - DRAW_SOURCE_SIZE) / 2.0;

	uint32_t x = 0;
	uint32_t y = 0;
	uint16_t xAtlas = 0;
	uint16_t yAtlas = 0;
	uint16_t sourceID = 0;

	Tile() {};
	Tile(ifstream& file) {
		file.read(reinterpret_cast<char*>(&x), 4);
		file.read(reinterpret_cast<char*>(&y), 4);
		file.read(reinterpret_cast<char*>(&xAtlas), 2);
		file.read(reinterpret_cast<char*>(&yAtlas), 2);
		file.read(reinterpret_cast<char*>(&sourceID), 2);
	}

	void Draw(SDL_Surface* targetSurface, const vector<SDL_Surface*>& atlases, int xOffset, int yOffset) {
		SDL_Rect source{xAtlas * DRAW_SOURCE_SIZE, yAtlas * DRAW_SOURCE_SIZE, DRAW_SOURCE_SIZE, DRAW_SOURCE_SIZE};

		SDL_Rect destination{x * DRAW_DEST_SIZE + DRAW_DEST_OFFSET + xOffset,
							 y * DRAW_DEST_SIZE + DRAW_DEST_OFFSET + yOffset, DRAW_SOURCE_SIZE, DRAW_SOURCE_SIZE};

		int error = SDL_BlitSurface(atlases.at(sourceID), &source, targetSurface, &destination);

		if (error < 0) {
			cerr << "ERROR when caching room: " << SDL_GetError() << endl;
		}
	}
};

void from_json(const nlohmann::json& json, Tile& tile) {
	tile.x = json.at("x");
	tile.y = json.at("y");
	tile.xAtlas = json.at("atlas_x");
	tile.yAtlas = json.at("atlas_y");
	tile.sourceID = json.at("source_id");
}

std::ostream& operator<<(std::ostream& out, const Tile& tile) {
	out << "( " << tile.x << ", " << tile.y << " )";
	return out;
}