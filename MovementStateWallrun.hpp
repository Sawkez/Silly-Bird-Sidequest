#pragma once

#include <math.h>

#include "IMovementState.hpp"
#include "Math.hpp"
#include "MovementStateNormal.hpp"
#include "Player.hpp"
#include "Raycast.hpp"

class MovementStateWallrun : public IMovementState {
	static inline constexpr float STICK_ACCEL = 1800;
	static inline constexpr float DROP_ACCEL = 450;
	static inline constexpr float STICK_VELOCITY = 150.0;
	static inline constexpr float GRAVITY = 500.0;
	static inline constexpr float MAX_DIST = 6.0;
	static inline constexpr float INITIAL_VELOCITY = 250.0;
	static inline const Vector2 JUMP_FORCE = Vector2(250.0, 200.0);

	void Init(Player& p) const override {
		p.PlayAnimationLastFrame(Player::ANIM_LEDGE_UNFLIP);

		p.velocity.y = min(p.velocity.y, -INITIAL_VELOCITY);

		p.EnableQuickClimb();
	}

	void Process(Player& p, float delta) const override {
		float wallDir = p.IsFacingLeft() ? -1.0 : 1.0;

		Raycast ray(p.position, p.IsFacingLeft() ? Raycast::LEFT : Raycast::RIGHT, MAX_DIST);

		if (wallDir * p.GetInput().GetDir().x < 0.0) {
			if (!p.GetInput().IsDown(ACTION_DIVE)) {
				p.velocity.x -= wallDir * DROP_ACCEL * delta;
			}

			if (p.GetInput().IsTapped(ACTION_JUMP)) {
				p.velocity.x = copysignf(JUMP_FORCE.x, -wallDir);
				p.velocity.y -= JUMP_FORCE.y;
				p.SetState(Player::MOVEMENT_STATE_NORMAL);
				return;
			}
		}

		else {
			p.velocity.x += wallDir * STICK_ACCEL * delta;
			p.velocity.x = clamp(p.velocity.x, -STICK_VELOCITY, STICK_VELOCITY);

			if (p.GetInput().IsTapped(ACTION_JUMP)) p.Buffer(Player::BUFFER_DASH);
		}

		p.velocity.y += GRAVITY * delta;
		if (p.velocity.y > 0.0 || !ray.CheckCollision(p.GetStaticColliders())) {
			p.SetState(Player::MOVEMENT_STATE_NORMAL);
			return;
		}

		p.UpdateLedgeTile();
		if (p.CanGrabLedge()) {
			p.SetState(Player::MOVEMENT_STATE_LEDGE);
			return;
		}
	}

	void Deinit(Player& p) const override { p.SetCooldown(Player::COOLDOWN_WALLRUN); }
};