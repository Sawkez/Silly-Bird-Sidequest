#pragma once

#include <math.h>

#include "IMovementState.hpp"
#include "Math.hpp"
#include "Player.hpp"
#include "Raycast.hpp"

class MovementStateWallrun : public IMovementState {
	static inline constexpr float STICK_ACCEL = 1800;
	static inline constexpr float DROP_ACCEL = 900;
	static inline constexpr float STICK_VELOCITY = 150.0;
	static inline constexpr float GRAVITY = 500.0;
	static inline constexpr float MAX_DIST = 6.0;

	void Init(Player& p) const override { p.PlayAnimationLastFrame(Player::ANIM_LEDGE_UNFLIP); }

	void Process(Player& p, float delta) const override {
		float wallDir = p.IsFacingLeft() ? -1.0 : 1.0;

		Raycast ray(p.position, p.IsFacingLeft() ? Raycast::LEFT : Raycast::RIGHT, MAX_DIST);

		if (wallDir * p.GetInput().GetDir().x < 0.0) {
			p.velocity.x -= wallDir * DROP_ACCEL * delta;

			// TODO walljump
		}

		else {
			p.velocity.x += wallDir * STICK_ACCEL * delta;
			p.velocity.x = clamp(p.velocity.x, -STICK_VELOCITY, STICK_VELOCITY);
		}

		p.velocity.y += GRAVITY * delta;
		if (p.velocity.y > 0.0 || !ray.CheckCollision(p.GetStaticColliders())) {
			p.SetState(Player::MOVEMENT_STATE_NORMAL);
			return;
		}
	}

	void Deinit(Player& p) const override {}
};