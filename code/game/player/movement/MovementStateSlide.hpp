#pragma once

#include "game/player/Player.hpp"
#include "game/player/movement/IMovementState.hpp"

class MovementStateSlide : public IMovementState {
	static inline constexpr float SLIDE_INITIAL_SPEED = 250.0;
	static inline constexpr float SLIDE_SPEED = 200.0;
	static inline constexpr float ULTRASLIDE_VELOCITY_MULT = 0.975;	 // i can't figure out why this is different from godot
	static inline constexpr float SLIDE_GRAVITY = 0.01;
	static inline constexpr float SQUISH_SLIDE = 0.5;
	static inline constexpr float SLIDE_FRICTION = 300.0;
	static inline constexpr float SLIDE_JUMP_FORCE = 250.0;

	void Init(Player& p) const override {
		p.Unbuffer(Player::BUFFER_SLIDE);
		p.SetTimer(Player::TIMER_SLIDE);

		p.SetShortCollision(true);
		p.FlipSprite(p.velocity.x < 0.0);

		if (p.GetInput().GetDir().y <= 0.0) {
			p.ResetLastDownVelocity();
		}

		float maxSpeed = max(abs(p.velocity.x), SLIDE_INITIAL_SPEED);
		if (p.GetInput().GetDir().y == 1.0) {
			maxSpeed = max(maxSpeed, p.GetLastDownVelocity() * ULTRASLIDE_VELOCITY_MULT);
		}

		p.velocity.x = copysignf(maxSpeed, p.velocity.x);

		p.SetSquish(SQUISH_SLIDE);
		p.PlayAnimation(Player::ANIM_SLIDE);

		if (abs(p.velocity.x) > SLIDE_INITIAL_SPEED) {
			cout << "Ultrasliding with " << p.velocity.x << endl;
		}
	}

	void Process(Player& p, float delta) const override {
		// jump buffer
		if (p.GetInput().IsTapped(ACTION_JUMP)) {
			p.Buffer(Player::BUFFER_JUMP);
		}

		// jumping
		else if (p.BufferActive(Player::BUFFER_JUMP) && !p.IsCloseToCeiling()) {
			p.Unbuffer(Player::BUFFER_JUMP);

			p.velocity.y = -SLIDE_JUMP_FORCE;
			p.SetSquish(Player::X_SQUISH_MIN);

			p.SetState(Player::MOVEMENT_STATE_NORMAL);
			return;
		}

		// sliding off ledge
		else if (!p.IsPushingFloor()) {
			p.SetState(Player::MOVEMENT_STATE_NORMAL);
			return;
		}

		// crashing into a wall
		if (p.IsPushingWall()) {
			p.SetState(p.IsCloseToCeiling() ? Player::MOVEMENT_STATE_DUCK : Player::MOVEMENT_STATE_NORMAL);
			return;
		}

		// nothing happens
		else if (p.TimerActive(Player::TIMER_SLIDE)) {
		} else if (p.IsCloseToCeiling()) {
		}

		// releasing slide after time runs out
		else if (!p.GetInput().IsDown(ACTION_DIVE)) {
			p.SetState(Player::MOVEMENT_STATE_NORMAL);
			return;
		}

		// pressing opposite direction
		else if (p.GetInput().GetDir().x * p.velocity.x < 0.0) {
			p.velocity = Vector2::ZERO;
			p.SetState(Player::MOVEMENT_STATE_NORMAL);
			return;
		}

		p.velocity.y = SLIDE_GRAVITY * delta;

		p.velocity.x = abs(p.velocity.x) - SLIDE_FRICTION * delta;
		p.velocity.x = max(p.velocity.x, SLIDE_SPEED);
		if (p.IsFacingLeft()) {
			p.velocity.x = -p.velocity.x;
		}
	}

	void Deinit(Player& p) const override {
		p.SetTimer(Player::TIMER_COYOTE);
		p.velocity.y = min(p.velocity.y, 0.0f);
		if (p.IsCloseToCeiling()) {
			p.SetShortCollision(false);
		}
		p.Unbuffer(Player::BUFFER_DIVE);
		p.SetCooldown(Player::COOLDOWN_SLIDE);
	}
};