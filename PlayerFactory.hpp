#pragma once

#include <SDL.h>

#include "IMovementState.hpp"
#include "MovementStateNormal.hpp"
#include "Player.hpp"

namespace PlayerFactory {

Player NewPlayer(const InputManager& input, SDL_Renderer* renderer, Room& room) {
	// i'll unhardcode the 7 later man
	// also add other movement states
	IMovementState** states =
		new IMovementState*[7]{new MovementStateNormal(), new MovementStateNormal(), new MovementStateNormal(), new MovementStateNormal(),
							   new MovementStateNormal(), new MovementStateNormal(), new MovementStateNormal()};

	return Player(input, renderer, room, states);
}

}  // namespace PlayerFactory