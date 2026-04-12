#pragma once

#include <SDL.h>

#include "engine/IProcessable.hpp"
#include "engine/graphics/IDrawableRect.hpp"
#include "game/player/IPlayer.hpp"

class IRoomObject : public IDrawableRect {
   public:
	virtual void Process(float delta, IPlayer& player) = 0;
};