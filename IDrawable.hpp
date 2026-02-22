#pragma once

#include <SDL.h>

class IDrawable {
    public:
        virtual void Draw(SDL_Renderer* renderer) const = 0;
};