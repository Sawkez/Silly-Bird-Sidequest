#pragma once

#include "engine/devconsole/DevConsoleFlags.hpp"
#include "engine/devconsole/variable/DevConsoleVariable.hpp"
#include "game/player/Player.hpp"
#include "game/player/movement/IMovementState.hpp"

#define CONVAR_CATEGORY PLAYER_STATE_DASH

class MovementStateDash : public IMovementState {
	// clang-format off
	CONVAR(float,	_xVelocity,		X_VELOCITY,		200.0f,			DC_FLAG_CHEAT);
	CONVAR(float,	_yVelocity,		Y_VELOCITY,		-200.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_returnTime,	RETURN_TIME,	8.99f / 60.0f,	DC_FLAG_CHEAT);
	// clang-format on

	void Init(Player& p) const override {
		p.UnloadDash();
		p.Unbuffer(Player::BUFFER_DASH);
		p.SetTimer(Player::TIMER_DASH);
		p.SetSquish(*PlayerSquish::xMax);
		p.PlayAnimationFromStart(Player::ANIM_JUMP);

		p.velocity.y = *_yVelocity;
		p.velocity.x = max(*_xVelocity, abs(p.velocity.x));

		if (p.GetInput().GetDir().x < 0.0) {
			p.velocity.x = -p.velocity.x;
		}

		else if (p.IsFacingLeft()) {
			p.velocity.x = -p.velocity.x;
		}

		p.DisableQuickClimb();
	}

	void Process(Player& p, float delta) const override {
		if (p.IsPushingCeiling()) {
			p.CeilingDash();
			return;
		}

		if (p.GetTimer(Player::TIMER_DASH) < *_returnTime) {
			p.SetState(Player::MOVEMENT_STATE_NORMAL);
		}
	}

	void Deinit(Player& p) const override { p.UnsetCooldown(Player::COOLDOWN_WALLRUN); }
};

#undef CONVAR_CATEGORY