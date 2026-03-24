#pragma once

#include <cmath>

#include "IMovementState.hpp"
#include "InputManager.hpp"
#include "Math.hpp"
#include "Player.hpp"

// TODO unfriend

class MovementStateNormal : public IMovementState {
	static inline constexpr float ACCELERATION = 600.0;
	static inline constexpr float TOP_SPEED = 125.0;
	static inline constexpr float FRICTION = 1200.0;
	static inline constexpr float AIR_FRICTION = 60.0;

	static inline constexpr float WEAK_GRAVITY = 600.0;
	static inline constexpr float GRAVITY = 900.0;
	static inline constexpr float FAST_FALL_GRAVITY = 1200.0;

	static inline constexpr float FAST_FALL_WINDOW = GRAVITY * 10.0 / 120.0;  // 10 frames total, 5 frames either direction at 60 fps
	static inline constexpr float FALL_SPEED_CAP = 200.0;

	static inline constexpr float JUMP_FORCE = 250.0;

	static inline constexpr float MAX_DIVE_BUFFER_Y_VELOCITY = 250.0;

	static inline constexpr float LEDGE_CHECK_OFFSET_LEFT = -1.2;
	static inline constexpr float LEDGE_CHECK_OFFSET_RIGHT = 0.8;
	static inline constexpr float LEDGE_CHECK_OFFSET_UP = -1.5;

	static inline constexpr float SLOW_RUN_SPEED = 100.0;

	void Init(Player& p) const override {}

	void Process(Player& p, float delta) const override {
		if (p._shortCollision && !p._closeToCeiling) {
			p.SetShortCollision(false);
		}

		// ceiling dash
		if (p.TimerActive(Player::TIMER_DASH) && p.velocity.y < GRAVITY * delta && p._pushingCeiling) {
			p.CeilingDash();
		}

		// friction
		// trying to turn around
		if (p.velocity.x * p._input.GetDir().x <= 0.0) {
			// make sure we don't overcompensate
			float activeFrict = fminf(FRICTION * delta, abs(p.velocity.x));
			p.velocity.x -= Math::CopySignOrZero(activeFrict, p.velocity.x);
		}

		// still accelerating (do nothing)
		else if (abs(p.velocity.x) < TOP_SPEED) {
		}

		// going barely too fast, lock to top speed
		else if (abs(p.velocity.x) - TOP_SPEED < FRICTION * delta) {
			p.velocity.x = Math::CopySignOrZero(TOP_SPEED, p.velocity.x);
		}

		// going way too fast
		else {
			float activeFrict = p._pushingFloor ? FRICTION : AIR_FRICTION;
			p.velocity.x -= Math::CopySignOrZero(activeFrict * delta, p.velocity.x);
		}

		// accelerating
		if (abs(p.velocity.x) < TOP_SPEED) {
			p.velocity.x += ACCELERATION * p._input.GetDir().x * delta;
		}

		// starting a fastfall
		// not fastfalling, do nothing
		if (!p._input.IsTapped(ACTION_DOWN)) {
		}

		// fastfalling around peak of jump, maximize downwards velocity
		else if (!p._pushingFloor && abs(p.velocity.y) < FAST_FALL_WINDOW) {
			/*
			float timeFalling = abs(velocity.y) / GRAVITY;
			float timeFastFalling = sqrtf(GRAVITY * timeFalling * timeFalling / FAST_FALL_GRAVITY);

			velocity.y = FAST_FALL_GRAVITY * timeFastFalling;
			*/

			p.velocity.y = abs(p.velocity.y) * sqrtf(GRAVITY / FAST_FALL_GRAVITY);
			std::cout << "setting velocity to " << p.velocity.y << std::endl;
		}

		// fastfalling normally, cancel up velocity
		else {
			p.velocity.y = fmaxf(p.velocity.y, 0.0);
		}

		// applying gravity
		if (p.velocity.y > FALL_SPEED_CAP || p.TimerActive(Player::TIMER_GRAVITY_FREEZE)) {
			p.velocity.y += WEAK_GRAVITY * delta;
		}

		else if (p._input.IsDown(ACTION_DOWN)) {
			p.velocity.y += FAST_FALL_GRAVITY * delta;
		}

		else {
			p.velocity.y += GRAVITY * delta;
		}

		// resetting coyote timer
		if (p._pushingFloor) {
			p.SetTimer(Player::TIMER_COYOTE);
		}

		// jump / dash buffer
		if (!p._input.IsTapped(ACTION_JUMP)) {
		}

		else if (p._closeToFloor || p.TimerActive(Player::TIMER_COYOTE)) {
			p.Buffer(Player::BUFFER_JUMP);
		}

		else {
			p.Buffer(Player::BUFFER_DASH);
			// TODO dashblocker
		}

		// dashing
		bool dashFirst =
			!p.BufferActive(Player::BUFFER_DIVE) || p._buffers[Player::BUFFER_DASH] < p._buffers[Player::BUFFER_DIVE] || !p._diveAvailable;

		if (!p.BufferActive(Player::BUFFER_DASH)) {
		}

		else if (dashFirst && p.velocity.y > GRAVITY * delta && p.HasUpgrade(Player::UPGRADE_DASH) && p._dashAvailable) {
			p.SetState(Player::MOVEMENT_STATE_DASH);
			return;
		}

		// dive buffer
		bool wantsToDive = p.velocity.y < GRAVITY * delta || !p._closeToFloor;
		bool wantsToUltraslide = p.velocity.y > MAX_DIVE_BUFFER_Y_VELOCITY && p._input.IsDown(ACTION_DOWN);

		if (!p._input.IsTapped(ACTION_DIVE)) {
		}

		else if (wantsToDive && !wantsToUltraslide) {
			p.Buffer(Player::BUFFER_DIVE);
			// TODO implement diveblocker
		}

		else {
			p.Buffer(Player::BUFFER_SLIDE);
		}

		// sliding
		bool inputtingSlide = p._input.IsDown(ACTION_DIVE) || p.BufferActive(Player::BUFFER_SLIDE);
		bool slideAvailable = p.HasUpgrade(Player::UPGRADE_SLIDE) && !p.CooldownActive(p.COOLDOWN_SLIDE);

		if (!p._pushingFloor || !inputtingSlide || !slideAvailable) {
		}

		else if (p.velocity.x != 0.0) {
			p.SetState(Player::MOVEMENT_STATE_SLIDE);
			return;
		}

		// ducking
		else {
			p.SetState(Player::MOVEMENT_STATE_DUCK);
			return;
		}

		// ducking with down key
		bool canDuck = p._pushingFloor && !p.BufferActive(Player::BUFFER_JUMP) && p.HasUpgrade(Player::UPGRADE_SLIDE);

		if (canDuck && p.velocity.x == 0.0 && p._input.GetDir() == Vector2{0.0, 1.0}) {
			p.SetState(Player::MOVEMENT_STATE_DUCK);
			return;
		}

		// diving
		if (p.BufferActive(Player::BUFFER_DIVE) && p.velocity.y > GRAVITY * delta && p.HasUpgrade(Player::UPGRADE_DIVE) && p._diveAvailable) {
			p.SetState(Player::MOVEMENT_STATE_DIVE);
			return;
		}

		// jumping
		if (p.TimerActive(Player::TIMER_COYOTE) && p.UseBuffer(Player::BUFFER_JUMP)) {
			p.UnsetTimer(Player::TIMER_COYOTE);
			p.velocity.y = -JUMP_FORCE;

			p._sprite.scale.x = Player::X_SQUISH_MIN;
			// TODO moving platforms
		}

		// grabbing ledges
		if ((p._quickClimb || p.velocity.y > GRAVITY * delta) && !p.CooldownActive(p.COOLDOWN_LEDGE) && p._input.GetDir().y < 1.0) {
			p._ledgeTile.x =
				int(roundf(p.position.x / WorldConstants::TILE_SIZE_F + (p._facingLeft ? LEDGE_CHECK_OFFSET_LEFT : LEDGE_CHECK_OFFSET_RIGHT))) *
				WorldConstants::TILE_SIZE;

			p._ledgeTile.y = int(roundf(p.position.y / WorldConstants::TILE_SIZE_F + LEDGE_CHECK_OFFSET_UP)) * WorldConstants::TILE_SIZE;

			const vector<SDL_Point>& ledges = p._room.get().GetLedges();

			if (std::find(ledges.begin(), ledges.end(), p._ledgeTile) != ledges.end()) {
				p.SetState(Player::MOVEMENT_STATE_LEDGE);
				return;
			}
		}

		// updating animation
		if (p._pushingFloor) {
			if (p.velocity.x == 0.0 || p._pushingWall) {
				// TODO bored and twerk
				p._sprite.Play(Player::ANIM_IDLE);
			}

			else if (abs(p.velocity.x) <= SLOW_RUN_SPEED) {
				p._sprite.Play(Player::ANIM_SLOW_RUN, abs(p.velocity.x) / TOP_SPEED);
			}

			else {
				p._sprite.Play(Player::ANIM_RUN, abs(p.velocity.x) / TOP_SPEED);
			}
		}

		else {
			p._sprite.Play(Player::ANIM_JUMP);
		}

		// flipping sprite
		if (p.velocity.x < 0.0 && !p._facingLeft)
			p.FlipSprite(true);
		else if (p.velocity.x > 0.0)
			p.FlipSprite(false);
	}

	void Deinit(Player& p) const override {}
};