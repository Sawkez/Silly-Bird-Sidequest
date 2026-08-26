#pragma once

#include <SDL3/SDL.h>

#include <fstream>
#include <iostream>

#include "engine/Math.hpp"
#include "engine/world/WorldConstants.hpp"

struct TileBase {
	uint16_t x = 0;
	uint16_t y = 0;
	uint16_t xAtlas = 0;
	uint16_t yAtlas = 0;

	TileBase() {}

	TileBase(Uint16 X, Uint16 Y, Uint16 XAtlas, Uint16 YAtlas) : x(X), y(Y), xAtlas(XAtlas), yAtlas(YAtlas) {}

	virtual int GetDrawSourceSize() const = 0;
	virtual int GetDrawDestOffset() const = 0;

	void Draw(SDL_Renderer* renderer, SDL_Texture* atlas, int xOffset, int yOffset) {
		SDL_FRect source{float(xAtlas) * GetDrawSourceSize(), float(yAtlas) * GetDrawSourceSize(),
						 float(GetDrawSourceSize()), float(GetDrawSourceSize())};

		SDL_FRect destination{float(x * WorldConstants::TILE_SIZE + GetDrawDestOffset() + xOffset),
							  float(y * WorldConstants::TILE_SIZE + GetDrawDestOffset() + yOffset),
							  float(GetDrawSourceSize()), float(GetDrawSourceSize())};

		if (!SDL_RenderTexture(renderer, atlas, &source, &destination)) {
			dc::err << "ERROR when caching tile: " << SDL_GetError() << dc::endl;
		}
	}
};

std::ostream& operator<<(std::ostream& out, const TileBase& tile) {
	out << "( " << tile.x << ", " << tile.y << " )";
	return out;
}