#pragma once

#include <SDL3/SDL.h>

#include <iostream>
#include <limits>

#include "engine/Math.hpp"
#include "engine/Vector2.hpp"
#include "engine/graphics/IDrawableRect.hpp"
#include "engine/physics/CollisionResult.hpp"
#include "engine/physics/Physics.hpp"
#include "yyjson.h"

struct CollisionRect : public SDL_FRect, public IDrawableRect {
	bool active = true;
	bool oneWay = false;
	Vector2 oneWayNormal{0.0, 0.0};

	CollisionRect(float xPosition, float yPosition, float width, float height, bool active_ = true,
				  bool oneWay_ = false, Vector2 oneWayNormal_ = Vector2{0.0, 0.0})
		: SDL_FRect{xPosition, yPosition, width, height},
		  active(active_),
		  oneWay(oneWay_),
		  oneWayNormal(oneWayNormal_) {}

	CollisionRect(const CollisionRect& other)
		: CollisionRect(other.x, other.y, other.w, other.h, other.active, other.oneWay, other.oneWayNormal) {}

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

	bool OverlapsCircle(const Vector2& position, const float radius) const {
		float xDist, yDist;

		if (position.x < x) {
			xDist = x - position.x;
		} else if (position.x > x + w) {
			xDist = position.x - (x + w);
		} else {
			xDist = 0;
		}

		if (position.y < y) {
			yDist = y - position.y;
		} else if (position.y > y + h) {
			yDist = position.y - (y + h);
		} else {
			yDist = 0;
		}

		return xDist * xDist + yDist * yDist <= radius * radius;
	}

	CollisionResult SweptAABBCollision(const CollisionRect& movingRect, const Vector2& velocity, float xInvVel,
									   float yInvVel) const {
		if (!active || !movingRect.active) return CollisionResult{};

		Physics::SweptAABBCollision(movingRect, x, y, w, h, velocity, xInvVel, yInvVel);
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