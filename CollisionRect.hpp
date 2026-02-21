#pragma once

#include <SDL.h>
#include <iostream>

#include "Vector2.hpp"
#include "CollisionResult.hpp"
#include "Math.hpp"

struct CollisionRect : SDL_FRect, IDrawable {
    const float X_PRIORITY = 0.1;
    const float MIN_COLLISION_DEPTH = -0.1;

    bool active = true;
    bool oneWay = false;
    Vector2 oneWayNormal {0.0, 0.0};

    CollisionRect(float xPosition, float yPosition, float width, float height, bool active_ = true, bool oneWay_ = false, Vector2 oneWayNormal_ = Vector2 {0.0, 0.0}) :
    SDL_FRect{xPosition, yPosition, width, height}, active(active_), oneWay(oneWay_), oneWayNormal(oneWayNormal_)
    { }

    CollisionRect(const CollisionRect& other) :
    CollisionRect(other.x, other.y, other.w, other.h, other.active, other.oneWay, other.oneWayNormal)
    { }

    void operator=(const CollisionRect& other) {
        x = other.x;
        y = other.y;
        w = other.w;
        h = other.h;
        active = other.active;
        oneWay = other.oneWay;
        oneWayNormal = other.oneWayNormal;
    }

    void Draw(SDL_Renderer* renderer) const override {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRectF(renderer, this);
    }

    CollisionResult SweptAABBCollision(const CollisionRect& movingRect, const Vector2& velocity) const {

        if (!active || !movingRect.active) return CollisionResult{ };

        float xInverseEntry = x - (movingRect.x + movingRect.w);
        float xInverseExit = (x + w) - movingRect.x;

        if (velocity.x == 0.0 && xInverseEntry >= 0.0 || xInverseExit <= 0.0) {
            return CollisionResult{ };
        }

        if (velocity.x < 0.0) {
            float temp = xInverseExit;
            xInverseExit = xInverseEntry;
            xInverseEntry = temp;
        }

        float yInverseEntry = y - (movingRect.y + movingRect.h);
        float yInverseExit = (y + h) - movingRect.y;

        if (velocity.y == 0.0 && yInverseEntry >= 0.0 || yInverseExit <= 0.0) {
            return CollisionResult{ };
        }

        if (velocity.y < 0.0) {
            float temp = yInverseExit;
            yInverseExit = yInverseEntry;
            yInverseEntry = temp;
        }

        float xEntry = velocity.x == 0.0? -MAXFLOAT : xInverseEntry / velocity.x;
        float xExit = velocity.x == 0.0? MAXFLOAT : xInverseExit / velocity.x;
        float yEntry = velocity.y == 0.0? -MAXFLOAT : yInverseEntry / velocity.y;
        float yExit = velocity.y == 0.0? MAXFLOAT : yInverseExit / velocity.y;

        float entryTime = std::max(xEntry, yEntry);
        float exitTime = std::min(xExit, yExit);

        if (entryTime > exitTime || entryTime < MIN_COLLISION_DEPTH || entryTime > 1.0) {
            return CollisionResult{ };
        }

        if (xEntry > yEntry + X_PRIORITY) return CollisionResult {
            entryTime, Vector2 {
                -Math::SignOrZero(velocity.x),
                0.0
            }
        };

        else return CollisionResult {
            entryTime, Vector2 {
                0.0,
                -Math::SignOrZero(velocity.y)
            }
        };
    }
};

std::ostream& operator<<(std::ostream& out, const CollisionRect& rect) {
    out << "pos: " << rect.x << ", " << rect.y << std::endl <<
        "size: " << rect.w << ", " << rect.h;
        
    return out;
}