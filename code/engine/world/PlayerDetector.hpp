#pragma once

#include <SDL.h>

#include "engine/world/IRoomObject.hpp"
#include "game/player/Player.hpp"

class PlayerDetector : public IRoomObject {
   protected:
	SDL_FRect _rect;

	virtual void Activated(Player& player) {};

   public:
	PlayerDetector(const SDL_FRect& rect) : _rect(rect) {}

	void Process(float delta, Player& player) override {
		if (SDL_HasIntersectionF(&player.GetCollision(), &_rect)) {
			this->Activated(player);
		}
	}
};