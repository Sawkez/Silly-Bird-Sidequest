#pragma once

#include <vector>

#include "engine/Vector2.hpp"
#include "engine/graphics/IDrawableRect.hpp"
#include "engine/physics/CollisionRect.hpp"
#include "engine/physics/CollisionResult.hpp"
#include "engine/physics/Raycast.hpp"
#include "engine/physics/SpikeCollider.hpp"
#include "engine/physics/TileRange.hpp"
#include "engine/world/WorldConstants.hpp"

class SpikeColliderContainer {
   private:
	static inline const int TILE_EDGE_GROW = 32;

	const char* _data = nullptr;
	int _width = 0;
	int _height = 0;
	Sint64 _roomX = 0;
	Sint64 _roomY = 0;

   public:
	SpikeColliderContainer() {}
	SpikeColliderContainer(const char* data, int width, int height, Sint64 roomX, Sint64 roomY)
		: _data(data), _width(width), _height(height), _roomX(roomX), _roomY(roomY) {}

	SpikeCollider GetCollider(const SDL_Point& tile) const {
		if (tile.x < 0 || tile.x >= _width || tile.y < 0 || tile.y >= _height) {
			return SpikeCollider();
		}

		int byteIndex = tile.y * _width + tile.x;
		return SpikeCollider(tile.x + _roomX, tile.y + _roomY, _data[byteIndex]);
	}

	bool OverlapsRect(const CollisionRect& rect) const {
		TileRange potentialTiles(rect, _roomX, _roomY);

		for (SDL_Point tile : potentialTiles) {
			if (GetCollider(tile).HasIntersection(rect)) return true;
		}

		return false;
	}
};