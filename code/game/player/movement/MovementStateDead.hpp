#pragma once

#include "game/player/Player.hpp"
#include "game/player/movement/IMovementState.hpp"

class MovementStateDead : public IMovementState {
	void Init(Player& p) const override { p.SetState(Player::MOVEMENT_STATE_NORMAL); }

	void Process(Player& p, float delta) const override {}

	void Deinit(Player& p) const override { p.Respawn(); }
};