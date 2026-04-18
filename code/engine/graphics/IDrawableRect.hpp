#pragma once

#include <SDL3/SDL.h>

class IDrawableRect {
   public:
	virtual bool Draw(SDL_Renderer* renderer, const SDL_FRect& drawTargetRect, Vector2 drawOffset) const = 0;
};