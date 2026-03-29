#pragma once

#include "game/player/Player.hpp"
#include "game/player/movement/IMovementState.hpp"
#include "game/player/movement/MovementStateDash.hpp"
#include "game/player/movement/MovementStateDead.hpp"
#include "game/player/movement/MovementStateDive.hpp"
#include "game/player/movement/MovementStateDuck.hpp"
#include "game/player/movement/MovementStateLedge.hpp"
#include "game/player/movement/MovementStateNormal.hpp"
#include "game/player/movement/MovementStateSlide.hpp"
#include "game/player/movement/MovementStateWallrun.hpp"

namespace MovementStates {
const auto normal = MovementStateNormal();
const auto ledge = MovementStateLedge();
const auto dive = MovementStateDive();
const auto dash = MovementStateDash();
const auto slide = MovementStateSlide();
const auto duck = MovementStateDuck();
const auto dead = MovementStateDead();
const auto wallrun = MovementStateWallrun();
}  // namespace MovementStates

const IMovementState* Player::_movementStates[Player::_MOVEMENT_STATE_COUNT]{&MovementStates::normal, &MovementStates::ledge,  &MovementStates::dive,
																			 &MovementStates::dash,	  &MovementStates::slide,  &MovementStates::duck,
																			 &MovementStates::dead,	  &MovementStates::wallrun};