#pragma once

#include <SDL.h>

#include "engine/world/IRoomObject.hpp"
#include "game/player/IPlayer.hpp"

class PlayerDetector : public IRoomObject {
   protected:
	SDL_FRect _rect;
	bool _active = true;

	virtual void Activated(IPlayer& player) {};

   public:
	PlayerDetector(const SDL_FRect& rect) : _rect(rect) {}

	void Process(float delta, IPlayer& player) override {
		if (!_active) return;

		if (SDL_HasIntersectionF(&player.GetCollision(), &_rect)) {
			this->Activated(player);
		}
	}
};