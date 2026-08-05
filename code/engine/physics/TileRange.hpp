#pragma once

#include <SDL3/SDL.h>

#include <cmath>

#include "engine/world/WorldConstants.hpp"

struct TileRange {
   private:
	static inline int FastFloor(float val) {
		int i = (int)val;
		return (val < (float)i) ? i - 1 : i;
	}

   public:
	int xMin, xMax, yMin, yMax;

	TileRange(const SDL_FRect& worldRect, Sint64 roomX, Sint64 roomY)
		: xMin(FastFloor((worldRect.x - roomX) / WorldConstants::TILE_SIZE_F)),
		  xMax(FastFloor((worldRect.x + worldRect.w - roomX) / WorldConstants::TILE_SIZE_F) + 1),
		  yMin(FastFloor((worldRect.y - roomY) / WorldConstants::TILE_SIZE_F)),
		  yMax(FastFloor((worldRect.y + worldRect.h - roomY) / WorldConstants::TILE_SIZE_F) + 1) {}

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