#pragma once

#include <SDL.h>

#include "Vector2.hpp"

class IDrawable {
  public:
	virtual void Draw(SDL_Renderer* renderer, Vector2 drawOffset) const = 0;
};