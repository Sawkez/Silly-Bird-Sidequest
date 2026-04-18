#pragma once

#include <SDL3/SDL.h>

#include <iostream>
#include <limits>

#include "engine/Math.hpp"
#include "engine/Vector2.hpp"
#include "engine/graphics/IDrawableRect.hpp"
#include "engine/physics/CollisionResult.hpp"
#include "yyjson.h"

struct CollisionRect : SDL_FRect, IDrawableRect {
	const float X_PRIORITY = 0.1;
	const float MIN_COLLISION_DEPTH = -0.1;

	bool active = true;
	bool oneWay = false;
	Vector2 oneWayNormal{0.0, 0.0};

	CollisionRect(float xPosition, float yPosition, float width, float height, bool active_ = true, bool oneWay_ = false,
				  Vector2 oneWayNormal_ = Vector2{0.0, 0.0})
		: SDL_FRect{xPosition, yPosition, width, height}, active(active_), oneWay(oneWay_), oneWayNormal(oneWayNormal_) {}

	CollisionRect(const CollisionRect& other) : CollisionRect(other.x, other.y, other.w, other.h, other.active, other.oneWay, other.oneWayNormal) {}

	CollisionRect() : CollisionRect(0.0, 0.0, 0.0, 0.0) {}

	CollisionRect(yyjson_val* json)
		: SDL_FRect{
			  float(yyjson_get_num(yyjson_obj_get(json, "x"))),
			  float(yyjson_get_num(yyjson_obj_get(json, "y"))),
			  float(yyjson_get_num(yyjson_obj_get(json, "width"))),
			  float(yyjson_get_num(yyjson_obj_get(json, "height"))),
		  } {}

	void operator=(const CollisionRect& other) {
		x = other.x;
		y = other.y;
		w = other.w;
		h = other.h;
		active = other.active;
		oneWay = other.oneWay;
		oneWayNormal = other.oneWayNormal;
	}

	bool Draw(SDL_Renderer* renderer, const SDL_FRect& drawTargetRect, Vector2 drawOffset = {}) const override {
		SDL_FRect destination{x + drawOffset.x, y + drawOffset.y, w, h};

		if (!SDL_HasRectIntersectionFloat(&drawTargetRect, &destination)) return false;

		SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255);
		SDL_RenderRect(renderer, &destination);
		return true;
	}

	CollisionResult SweptAABBCollision(const CollisionRect& movingRect, const Vector2& velocity) const {
		if (!active || !movingRect.active) return CollisionResult{};

		float xInverseEntry = x - (movingRect.x + movingRect.w);
		float xInverseExit = (x + w) - movingRect.x;

		if (velocity.x == 0.0 && (xInverseEntry >= 0.0 || xInverseExit <= 0.0)) {
			return CollisionResult{};
		}

		if (velocity.x < 0.0) {
			float temp = xInverseExit;
			xInverseExit = xInverseEntry;
			xInverseEntry = temp;
		}

		float yInverseEntry = y - (movingRect.y + movingRect.h);
		float yInverseExit = (y + h) - movingRect.y;

		if (velocity.y == 0.0 && (yInverseEntry >= 0.0 || yInverseExit <= 0.0)) {
			return CollisionResult{};
		}

		if (velocity.y < 0.0) {
			float temp = yInverseExit;
			yInverseExit = yInverseEntry;
			yInverseEntry = temp;
		}

		float xEntry = velocity.x == 0.0 ? -std::numeric_limits<float>::max() : xInverseEntry / velocity.x;
		float xExit = velocity.x == 0.0 ? std::numeric_limits<float>::max() : xInverseExit / velocity.x;
		float yEntry = velocity.y == 0.0 ? -std::numeric_limits<float>::max() : yInverseEntry / velocity.y;
		float yExit = velocity.y == 0.0 ? std::numeric_limits<float>::max() : yInverseExit / velocity.y;

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

	Vector2 PushOut(const CollisionRect& pushee) const {
		if (!active || !pushee.active) return Vector2();

		float pLeft = pushee.x;
		float pRight = pLeft + pushee.w;
		float pTop = pushee.y;
		float pBottom = pTop + pushee.h;

		float sLeft = x;
		float sRight = sLeft + w;
		float sTop = y;
		float sBottom = sTop + h;

		if (pRight <= sLeft) return Vector2();
		if (pTop <= sBottom) return Vector2();
		if (sRight <= pLeft) return Vector2();
		if (sTop <= pBottom) return Vector2();

		float xPenetration = std::min(pRight - sLeft, sRight - pLeft);
		float yPenetration = std::min(pBottom - sTop, sBottom - pTop);

		if (xPenetration < yPenetration) {
			float dir = (pushee.x + pushee.w / 2.0f > x + w / 2.0f) ? 1.0 : -1.0;
			return {dir * xPenetration, 0.0};
		}

		else {
			float dir = (pushee.y + pushee.h / 2.0f > y + h / 2.0f) ? 1.0 : -1.0;
			return {0.0, dir * yPenetration};
		}
	}
};

std::ostream& operator<<(std::ostream& out, const CollisionRect& rect) {
	out << "pos: " << rect.x << ", " << rect.y << std::endl << "size: " << rect.w << ", " << rect.h;

	return out;
}