#pragma once

#include <SDL.h>

#include "IDrawable.hpp"

class IDrawableRect : public IDrawable {
  public:
	virtual SDL_FRect GetRect() const = 0;
};