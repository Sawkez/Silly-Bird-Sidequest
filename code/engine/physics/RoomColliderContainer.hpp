#pragma once

#include <vector>

#include "engine/Vector2.hpp"
#include "engine/graphics/IDrawableRect.hpp"
#include "engine/physics/CollisionRect.hpp"
#include "engine/physics/CollisionResult.hpp"
#include "engine/physics/Raycast.hpp"
#include "engine/physics/TileRange.hpp"
#include "engine/world/WorldConstants.hpp"

class RoomColliderContainer : public IDrawableRect {
   private:
	static inline const int TILE_EDGE_GROW = 32;

	const char* _data = nullptr;
	int _width = 0;
	int _height = 0;
	Sint64 _roomX = 0;
	Sint64 _roomY = 0;

   public:
	RoomColliderContainer() {}
	RoomColliderContainer(const char* data, int width, int height, Sint64 roomX, Sint64 roomY)
		: _data(data), _width(width), _height(height), _roomX(roomX), _roomY(roomY) {}

	bool IsTilePresent(const SDL_Point& tile) const {
		int x = clamp(tile.x, 0, _width - 1);
		int y = clamp(tile.y, 0, _height - 1);

		int bitIndex = y * _width + x;
		int byteIndex = bitIndex / 8;
		int localBit = bitIndex - byteIndex * 8;

		return (_data[byteIndex] & (1 << localBit)) != 0;
	}

	CollisionRect GetCollider(const SDL_Point& tile) const {
		CollisionRect rect(tile.x * WorldConstants::TILE_SIZE + _roomX, tile.y * WorldConstants::TILE_SIZE + _roomY,
						   WorldConstants::TILE_SIZE, WorldConstants::TILE_SIZE);

		return rect;
	}

	bool OverlapsCircle(const Vector2& center, float radius) const {
		SDL_FRect potentialRect{center.x - radius, center.y - radius, radius * 2, radius * 2};
		TileRange potentialTiles(potentialRect, _roomX, _roomY);

		for (SDL_Point tile : potentialTiles) {
			if (!IsTilePresent(tile)) continue;
			if (GetCollider(tile).OverlapsCircle(center, radius)) return true;
		}

		return false;
	}

	bool OverlapsRect(const SDL_FRect& rect) const {
		TileRange potentialTiles(rect, _roomX, _roomY);

		for (SDL_Point tile : potentialTiles) {
			if (!IsTilePresent(tile)) continue;

			CollisionRect collider = GetCollider(tile);

			if (SDL_HasRectIntersectionFloat(&rect, &collider)) return true;
		}

		return false;
	}

	bool CheckRaycast(const Raycast& raycast) const {
		Vector2 points[2];
		points[0] = raycast.GetGlobalPosition();
		points[1] = raycast.GetGlobalTarget();

		SDL_FRect potentialRect;
		SDL_GetRectEnclosingPointsFloat(points, 2, nullptr, &potentialRect);

		TileRange potentialTiles(potentialRect, _roomX, _roomY);

		for (SDL_Point tile : potentialTiles) {
			if (raycast.CheckCollision(GetCollider(tile))) return true;
		}

		return false;
	}

	CollisionResult SweptAABBCollision(const CollisionRect& movingRect, const Vector2& velocity) const {
		SDL_FRect potentialRect,
			movedRect{movingRect.x + velocity.x, movingRect.y + velocity.y, movingRect.w, movingRect.h};
		SDL_GetRectUnionFloat(&movingRect, &movedRect, &potentialRect);

		TileRange potentialTiles(potentialRect, _roomX, _roomY);

		CollisionResult firstHit;

		for (SDL_Point tile : potentialTiles) {
			if (!IsTilePresent(tile)) continue;

			CollisionResult newHit = GetCollider(tile).SweptAABBCollision(movingRect, velocity);
			if (newHit.depth < firstHit.depth) firstHit = newHit;
		}

		return firstHit;
	}

	bool Draw(SDL_Renderer* renderer, const SDL_FRect& drawTargetRect, Vector2 drawOffset) const override {
		bool drawn = false;
		for (int x = 0; x < _width; x++) {
			for (int y = 0; y < _height; y++) {
				if (!IsTilePresent(SDL_Point{x, y})) continue;
				drawn |= GetCollider(SDL_Point{x, y}).Draw(renderer, drawTargetRect, drawOffset);
			}
		}

		return drawn;
	}
};