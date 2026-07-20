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

	virtual int GetDrawSourceSize() const = 0;
	virtual int GetDrawDestOffset() const = 0;

	void Draw(SDL_Renderer* renderer, SDL_Texture* atlas, int xOffset, int yOffset) {
		SDL_FRect source{xAtlas * GetDrawSourceSize(), yAtlas * GetDrawSourceSize(), GetDrawSourceSize(),
						 GetDrawSourceSize()};

		SDL_FRect destination{x * WorldConstants::TILE_SIZE + GetDrawDestOffset() + xOffset,
							  y * WorldConstants::TILE_SIZE + GetDrawDestOffset() + yOffset, GetDrawSourceSize(),
							  GetDrawSourceSize()};

		if (!SDL_RenderTexture(renderer, atlas, &source, &destination)) {
			dc::err << "ERROR when caching tile: " << SDL_GetError() << dc::endl;
		}
	}
};

std::ostream& operator<<(std::ostream& out, const TileBase& tile) {
	out << "( " << tile.x << ", " << tile.y << " )";
	return out;
}