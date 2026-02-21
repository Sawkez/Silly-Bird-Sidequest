#pragma once

#include "Vector2.hpp"

struct CollisionResult {
    float depth = 1.0;
    Vector2 normal {0.0, 0.0};
};