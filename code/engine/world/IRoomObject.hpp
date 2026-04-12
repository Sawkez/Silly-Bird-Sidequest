#pragma once

#include <SDL.h>

#include "engine/IProcessable.hpp"
#include "engine/graphics/IDrawableRect.hpp"

class Player;

class IRoomObject : public IDrawableRect {
   public:
	virtual void Process(float delta, Player& player) = 0;
};