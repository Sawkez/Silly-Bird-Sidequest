#pragma once

#include "IMovementState.hpp"
#include "Player.hpp"

class MovementStateWallrun : public IMovementState {
	void Init(Player& p) const override {}

	void Process(Player& p, float delta) const override {}

	void Deinit(Player& p) const override {}
};