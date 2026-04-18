#pragma once

#include <SDL.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "engine/physics/CollisionRect.hpp"
#include "engine/world/WorldConstants.hpp"
#include "yyjson.h"

class SpikeCollider : IDrawable {
   private:
	Sint16 _x;
	Sint16 _y;
	Uint8 _bitmask;

	static constexpr float SPIKE_SIZE = 3.0;

   public:
	enum SubSpike { TOP_LEFT, TOP, TOP_RIGHT, LEFT, RIGHT, BOTTOM_LEFT, BOTTOM, BOTTOM_RIGHT, _SUB_SPIKE_COUNT };

	static inline const SDL_FPoint COLLIDER_POSITIONS[_SUB_SPIKE_COUNT]{
		{0.0, 0.0},	   // top left
		{2.667, 0.0},  // top
		{5.0, 0.0},	   // top right
		{0.0, 2.667},  // left
		{5.0, 2.667},  // right
		{0.0, 5.0},	   // bottom left
		{2.667, 5.0},  // bottom
		{5.0, 5.0}	   // bottom right
	};

	SpikeCollider(std::ifstream& file) {
		file.read((char*)&_x, 2);
		file.read((char*)&_y, 2);
		file.read((char*)&_bitmask, 1);
	}

	bool HasSubSpike(int which) const { return (_bitmask & (1U << which)) != 0; }

	bool HasIntersection(const CollisionRect& rect) const {
		for (int i = 0; i < _SUB_SPIKE_COUNT; i++) {
			if (HasSubSpike(i)) {
				SDL_FRect spikeRect{float(_x) * WorldConstants::TILE_SIZE_F + COLLIDER_POSITIONS[i].x,
									float(_y) * WorldConstants::TILE_SIZE_F + COLLIDER_POSITIONS[i].y, SPIKE_SIZE, SPIKE_SIZE};

				if (SDL_HasRectIntersectionFloat(&rect, &spikeRect)) return true;
			}
		}

		return false;
	}

	void Draw(SDL_Renderer* renderer, Vector2 drawOffset) const override {
		SDL_RenderPoint(renderer, _x * 8 + int(drawOffset.x), _y * 8 + int(drawOffset.y));
		// SDL_RenderPoint(renderer, _x * 8, _y * 8);
	}
};