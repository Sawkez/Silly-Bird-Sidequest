#pragma once

#include <SDL3/SDL.h>

#include "engine/Vector2.hpp"

class IDrawable {
   public:
	virtual void Draw(SDL_Renderer* renderer, Vector2 drawOffset) const = 0;
};