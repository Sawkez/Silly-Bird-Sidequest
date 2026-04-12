#pragma once

#include <math.h>

#include "engine/Math.hpp"
#include "engine/physics/Raycast.hpp"
#include "game/player/Player.hpp"
#include "game/player/movement/IMovementState.hpp"
#include "game/player/movement/MovementStateNormal.hpp"

class MovementStateWallrun : public IMovementState {
	static inline constexpr float STICK_ACCEL = 1800;
	static inline constexpr float DROP_ACCEL = 450;
	static inline constexpr float STICK_VELOCITY = 150.0;
	static inline constexpr float GRAVITY = 500.0;
	static inline constexpr float MAX_DIST = 6.0;
	static inline constexpr float INITIAL_VELOCITY = 250.0;
	static inline const Vector2 JUMP_FORCE = Vector2(250.0, 200.0);
	static inline constexpr float DROP_VELOCITY = 300.0;

	void Init(Player& p) const override {
		p.PlayAnimation(Player::ANIM_WALLRUN);

		p.velocity.y = min(p.velocity.y, -INITIAL_VELOCITY);

		p.EnableQuickClimb();
	}

	void Process(Player& p, float delta) const override {
		float wallDir = p.IsFacingLeft() ? -1.0 : 1.0;

		if (p.GetInput().IsTapped(ACTION_JUMP)) {
			p.Buffer(Player::BUFFER_WALLJUMP);
		}

		if (wallDir * p.GetInput().GetDir().x < 0.0) {
			if (!p.GetInput().IsDown(ACTION_DIVE)) {
				p.velocity.x -= wallDir * DROP_ACCEL * delta;
			}

			if (p.BufferActive(Player::BUFFER_WALLJUMP)) {
				p.velocity.x = copysignf(JUMP_FORCE.x, -wallDir);
				p.SetState(Player::MOVEMENT_STATE_NORMAL);
				return;
			}
		}

		else {
			p.velocity.x += wallDir * STICK_ACCEL * delta;
			p.velocity.x = clamp(p.velocity.x, -STICK_VELOCITY, STICK_VELOCITY);
		}

		bool shouldLetGoAtTop = !p.GetInput().IsDown(ACTION_DIVE) || p.BufferActive(Player::BUFFER_WALLJUMP);
		bool isAtTop = p.velocity.y >= 0.0;
		bool isAtBottom = p.velocity.y > DROP_VELOCITY || p.IsPushingFloor();
		bool hasWall = Raycast(p.position + Vector2(0, -p.BODY_CENTER.y), p.IsFacingLeft() ? Raycast::LEFT : Raycast::RIGHT, MAX_DIST)
						   .CheckCollision(p.GetStaticColliders());

		if (isAtTop && shouldLetGoAtTop || isAtBottom || !hasWall) {
			p.SetState(Player::MOVEMENT_STATE_NORMAL);
			return;
		}

		p.UpdateLedgeTile();
		if (p.CanGrabLedge()) {
			p.SetState(Player::MOVEMENT_STATE_LEDGE);
			return;
		}

		p.velocity.y += GRAVITY * delta;
	}

	void Deinit(Player& p) const override {
		if (p.UseBuffer(Player::BUFFER_WALLJUMP)) {
			p.velocity.y = min(0.0f, p.velocity.y) - JUMP_FORCE.y;
		}

		p.SetCooldown(Player::COOLDOWN_WALLRUN);
	}
};