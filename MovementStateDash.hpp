#pragma once

#include "IMovementState.hpp"
#include "Player.hpp"

class MovementStateDash : public IMovementState {
	static inline constexpr float DASH_X_VELOCITY = 200.0;
	static inline constexpr float DASH_Y_VELOCITY = -200.0;
	static inline constexpr float DASH_RETURN_TIME = 8.99 / 60.0;

	void Init(Player& p) const override {
		p.UnloadDash();
		p.Unbuffer(Player::BUFFER_DASH);
		p.SetTimer(Player::TIMER_DASH);
		p.SetSquish(Player::X_SQUISH_MAX);
		p.PlayAnimationFromStart(Player::ANIM_JUMP);

		p.velocity.y = DASH_Y_VELOCITY;
		p.velocity.x = max(DASH_X_VELOCITY, abs(p.velocity.x));

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

		if (p.GetTimer(Player::TIMER_DASH) < DASH_RETURN_TIME) {
			p.SetState(Player::MOVEMENT_STATE_NORMAL);
		}
	}

	void Deinit(Player& p) const override { p.UnsetCooldown(Player::COOLDOWN_WALLRUN); }
};