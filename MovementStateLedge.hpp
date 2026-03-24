#pragma once

#include "IMovementState.hpp"
#include "Player.hpp"
#include "Vector2.hpp"

class MovementStateLedge : public IMovementState {
	static inline constexpr float LEDGE_OFFSET_HEIGHT = 10.0;
	static inline constexpr float LEDGE_OFFSET_LEFT = 12.0;
	static inline constexpr float LEDGE_OFFSET_RIGHT = -4.0;

	static inline const Vector2 LEDGE_SIDE_JUMP_FORCE{200.0, -125.0};
	static inline constexpr float LEDGE_UP_JUMP_FORCE = 250.0;
	static inline constexpr float LEDGE_UP_COOLDOWN = 5.0 / 60.0;
	static inline constexpr float LEDGE_DOWN_COOLDOWN = 10.0 / 60.0;

	void Init(Player& p) const override {
		p.position = p.GetLedgeTile();
		p.position.y += LEDGE_OFFSET_HEIGHT;

		if (p.IsFacingLeft()) {
			p.position.x += LEDGE_OFFSET_LEFT;
		}

		else {
			p.position.x += LEDGE_OFFSET_RIGHT;
		}

		p.velocity = Vector2::ZERO;
		p.EnableQuickClimb();

		p.PlayAnimationLastFrame(Player::Player::ANIM_LEDGE_UNFLIP);
		p.SetSquish(Player::X_SQUISH_MAX);
	}

	void Process(Player& p, float delta) const override {
		if (p.GetInput().IsTapped(ACTION_JUMP) || p.GetInput().IsTapped(ACTION_UP) || p.BufferActive(Player::BUFFER_DASH) ||
			p.BufferActive(Player::BUFFER_LEDGE_JUMP)) {
			p.Unbuffer(Player::BUFFER_DASH);
			p.Unbuffer(Player::BUFFER_LEDGE_JUMP);

			if (p.GetInput().GetDir().x != 0.0 && (p.GetInput().GetDir().x < 0.0 != p.IsFacingLeft())) {
				p.velocity = {LEDGE_SIDE_JUMP_FORCE.x * p.GetInput().GetDir().x, LEDGE_SIDE_JUMP_FORCE.y};
			}

			else {
				p.velocity.y = -LEDGE_UP_JUMP_FORCE;
			}

			p.SetCooldown(Player::COOLDOWN_LEDGE, LEDGE_UP_COOLDOWN);
			p.SetState(Player::MOVEMENT_STATE_NORMAL);
		}

		else if (p.GetInput().IsTapped(ACTION_DOWN)) {
			p.SetCooldown(Player::COOLDOWN_LEDGE, LEDGE_DOWN_COOLDOWN);
			p.SetState(Player::MOVEMENT_STATE_NORMAL);
		}

		else if (p.GetInput().GetDir().x == 0.0 || (p.GetInput().GetDir().x < 0.0 == p.IsFacingLeft())) {
			p.PlayAnimation(Player::ANIM_LEDGE_UNFLIP);
		}

		else {
			p.PlayAnimation(Player::ANIM_LEDGE_FLIP);
		}
	}

	void Deinit(Player& p) const override {
		p.SetSquish(Player::X_SQUISH_MIN);
		p.UnsetTimer(Player::TIMER_COYOTE);
	}
};