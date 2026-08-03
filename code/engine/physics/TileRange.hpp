#pragma once

#include <SDL3/SDL.h>

#include <cmath>

#include "engine/world/WorldConstants.hpp"

struct TileRange {
	int xMin, xMax, yMin, yMax;

	TileRange(const SDL_FRect& worldRect, Sint64 roomX, Sint64 roomY)
		: xMin(std::floor((worldRect.x - roomX) / WorldConstants::TILE_SIZE_F)),
		  xMax(std::ceil((worldRect.x + worldRect.w - roomX) / WorldConstants::TILE_SIZE_F)),
		  yMin(std::floor((worldRect.y - roomY) / WorldConstants::TILE_SIZE_F)),
		  yMax(std::ceil((worldRect.y + worldRect.h - roomY) / WorldConstants::TILE_SIZE_F)) {}

	struct Iterator {
		int x, y;
		int xMin, xMax;

		SDL_Point operator*() const { return {x, y}; }

		Iterator& operator++() {
			x++;
			if (x >= xMax) {
				x = xMin;
				y++;
			}
			return *this;
		}

		bool operator!=(const Iterator& other) const { return y != other.y || x != other.x; }
	};

	Iterator begin() const { return {xMin, yMin, xMin, xMax}; }
	Iterator end() const { return {xMin, yMax, xMin, xMax}; }
};