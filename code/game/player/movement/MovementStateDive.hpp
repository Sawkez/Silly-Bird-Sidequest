#pragma once

#include "game/player/Player.hpp"
#include "game/player/movement/IMovementState.hpp"

class MovementStateDive : public IMovementState {
	static inline constexpr float DIVE_INITIAL_VELOCITY = 200.0;
	static inline constexpr float DIVE_INITIAL_GRAVITY = 60.0;
	static inline constexpr float DIVE_GRAVITY_MULT = 304.481639541f;  // powf(1.1, 60.0);
	static inline constexpr float DIVE_GRAVITY_MAX = 900.0;
	static inline constexpr float DIVE_CANCEL_DURATION = 5.0 / 60.0;
	static inline constexpr float DIVEBOOST_FORCE = 200.0;
	static inline constexpr float DIVE_FRICTION = 300.0;

	void Init(Player& p) const override {
		p.UnloadDive();
		p.Unbuffer(Player::BUFFER_DIVE);
		p.SetTimer(Player::TIMER_DIVE);
		p.SetCurrentDiveGravity(DIVE_INITIAL_GRAVITY);

		p.velocity.x = max(DIVE_INITIAL_VELOCITY, abs(p.velocity.x));
		p.velocity.y = 0.0;

		if (p.IsFacingLeft()) {
			p.velocity.x = -p.velocity.x;
		}

		p.DisableQuickClimb();

		p.PlayAnimation(Player::ANIM_FLY);
	}

	void Process(Player& p, float delta) const override {
		// friction
		if (abs(p.velocity.x) > DIVE_INITIAL_VELOCITY) {
			p.velocity.x -= copysignf(DIVE_FRICTION * delta, p.velocity.x);
		}

		// keeping top speed
		else {
			p.velocity.x = copysignf(DIVE_INITIAL_VELOCITY, p.velocity.x);
		}

		// gravity
		p.velocity.y += p.GetCurrentDiveGravity() * delta;
		p.SetCurrentDiveGravity(min(p.GetCurrentDiveGravity() * powf(DIVE_GRAVITY_MULT, delta), DIVE_GRAVITY_MAX));

		// rotating
		p.SetSpriteRotationRadians(p.velocity.Angle() + (p.IsFacingLeft() ? M_PI : 0.0f));

		// diveboost buffer
		if (p.GetInput().IsTapped(ACTION_JUMP)) {
			p.Buffer(Player::BUFFER_DASH);
		}

		// diveboost
		if (p.BufferActive(Player::BUFFER_DASH) && p.HasUpgrade(Player::UPGRADE_DIVEBOOST) && p.IsDashAvailable() &&
			p.GetInput().IsDown(ACTION_DIVE)) {
			p.UnloadDash();
			p.Unbuffer(Player::BUFFER_DASH);

			p.velocity.y = -DIVEBOOST_FORCE;
			p.SetCurrentDiveGravity(DIVE_INITIAL_GRAVITY);
		}

		// pivoting
		if (p.GetInput().GetDir().x * p.velocity.x < 0.0 && p.GetTimer(Player::TIMER_DIVE) > DIVE_CANCEL_DURATION) {
			p.velocity.x = -p.velocity.x;
			p.FlipSprite(!p.IsFacingLeft());
			p.SetTimer(Player::TIMER_DIVE, DIVE_CANCEL_DURATION);
		}

		// undiving
		if (p.IsPushingFloor()) {
			p.SetState(Player::MOVEMENT_STATE_NORMAL);
		}

		else if (p.IsPushingWall()) {
			if (p.HasUpgrade(Player::UPGRADE_WALLRUN) && p.GetInput().IsDown(ACTION_DIVE)) {
				p.SetState(Player::MOVEMENT_STATE_WALLRUN);
				return;
			}

			if (p.GetTimer(Player::TIMER_DIVE) > DIVE_CANCEL_DURATION) {
				p.ReloadDive(true);
			}

			p.SetState(Player::MOVEMENT_STATE_NORMAL);
		}

		else if (!p.TimerActive(Player::TIMER_DIVE) && !p.GetInput().IsDown(ACTION_DIVE)) {
			p.SetState(Player::MOVEMENT_STATE_NORMAL);
		}
	}

	void Deinit(Player& p) const override { p.SetSpriteRotationDegrees(0.0); }
};