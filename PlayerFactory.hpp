#pragma once

#include <SDL.h>

#include "IMovementState.hpp"
#include "MovementStateDash.hpp"
#include "MovementStateDead.hpp"
#include "MovementStateDive.hpp"
#include "MovementStateDuck.hpp"
#include "MovementStateLedge.hpp"
#include "MovementStateNormal.hpp"
#include "MovementStateSlide.hpp"
#include "Player.hpp"

namespace PlayerFactory {

Player NewPlayer(const InputManager& input, SDL_Renderer* renderer, Room& room) {
	IMovementState** states = new IMovementState* [Player::_MOVEMENT_STATE_COUNT] {
		new MovementStateNormal(), new MovementStateLedge(), new MovementStateDive(), new MovementStateDash(), new MovementStateSlide(),
			new MovementStateDuck(), new MovementStateDead()
	};

	return Player(input, renderer, room, states);
}

}  // namespace PlayerFactory