#pragma once

#include "IMovementState.hpp"
#include "MovementStateDash.hpp"
#include "MovementStateDead.hpp"
#include "MovementStateDive.hpp"
#include "MovementStateDuck.hpp"
#include "MovementStateLedge.hpp"
#include "MovementStateNormal.hpp"
#include "MovementStateSlide.hpp"
#include "MovementStateWallrun.hpp"
#include "Player.hpp"

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