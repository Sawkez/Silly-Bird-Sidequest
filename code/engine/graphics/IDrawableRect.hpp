#pragma once

#include <SDL.h>

class IDrawableRect {
   public:
	virtual bool Draw(SDL_Renderer* renderer, const SDL_FRect& drawTargetRect, Vector2 drawOffset) const = 0;
};