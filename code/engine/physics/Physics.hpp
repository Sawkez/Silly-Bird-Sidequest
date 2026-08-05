#pragma once

#include <SDL3/SDL.h>

#include "engine/Math.hpp"
#include "engine/Vector2.hpp"
#include "engine/physics/CollisionResult.hpp"

namespace Physics {

const float X_PRIORITY = 0.1;
const float MIN_COLLISION_DEPTH = -0.1;

CollisionResult SweptAABBCollision(const SDL_FRect& movingRect, float staticX, float staticY, float staticW,
								   float staticH, const Vector2& velocity, float xInvVel, float yInvVel) {
	float xInverseEntry = staticX - (movingRect.x + movingRect.w);
	float xInverseExit = (staticX + staticW) - movingRect.x;

	if (velocity.x == 0.0 && (xInverseEntry >= 0.0 || xInverseExit <= 0.0)) {
		return CollisionResult{};
	}

	if (velocity.x < 0.0) {
		float temp = xInverseExit;
		xInverseExit = xInverseEntry;
		xInverseEntry = temp;
	}

	float yInverseEntry = staticY - (movingRect.y + movingRect.h);
	float yInverseExit = (staticY + staticH) - movingRect.y;

	if (velocity.y == 0.0 && (yInverseEntry >= 0.0 || yInverseExit <= 0.0)) {
		return CollisionResult{};
	}

	if (velocity.y < 0.0) {
		float temp = yInverseExit;
		yInverseExit = yInverseEntry;
		yInverseEntry = temp;
	}

	float xEntry = velocity.x == 0.0 ? -std::numeric_limits<float>::max() : xInverseEntry * xInvVel;
	float xExit = velocity.x == 0.0 ? std::numeric_limits<float>::max() : xInverseExit * xInvVel;
	float yEntry = velocity.y == 0.0 ? -std::numeric_limits<float>::max() : yInverseEntry * yInvVel;
	float yExit = velocity.y == 0.0 ? std::numeric_limits<float>::max() : yInverseExit * yInvVel;

	float entryTime = std::max(xEntry, yEntry);
	float exitTime = std::min(xExit, yExit);

	if (entryTime > exitTime || entryTime < MIN_COLLISION_DEPTH || entryTime > 1.0) {
		return CollisionResult{};
	}

	if (xEntry > yEntry + X_PRIORITY)
		return CollisionResult{entryTime, Vector2{-Math::SignOrZero(velocity.x), 0.0}};

	else
		return CollisionResult{entryTime, Vector2{0.0, -Math::SignOrZero(velocity.y)}};
}

}  // namespace Physics