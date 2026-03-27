#pragma once

#include <cmath>

#include "IMovementState.hpp"
#include "InputManager.hpp"
#include "Math.hpp"
#include "Player.hpp"

// TODO unfriend

struct MovementStateNormal : public IMovementState {
	static inline constexpr float ACCELERATION = 600.0;
	static inline constexpr float TOP_SPEED = 125.0;
	static inline constexpr float FRICTION = 1200.0;
	static inline constexpr float AIR_FRICTION = 60.0;

	static inline constexpr float WEAK_GRAVITY = 600.0;
	static inline constexpr float GRAVITY = 900.0;
	static inline constexpr float FAST_FALL_GRAVITY = 1200.0;

	static inline constexpr float FAST_FALL_WINDOW = GRAVITY * 14.0 / 120.0;  // 10 frames total, 5 frames either direction at 60 fps
	static inline constexpr float FALL_SPEED_CAP = 200.0;

	static inline constexpr float JUMP_FORCE = 250.0;

	static inline constexpr float MAX_DIVE_BUFFER_Y_VELOCITY = 250.0;

	static inline constexpr float SLOW_RUN_SPEED = 100.0;

	void Init(Player& p) const override {}

	void Process(Player& p, float delta) const override {
		if (p.IsShortCollision() && !p.IsCloseToCeiling()) {
			p.SetShortCollision(false);
		}

		// ceiling dash
		if (p.TimerActive(Player::TIMER_DASH) && p.velocity.y < GRAVITY * delta && p.IsPushingCeiling()) {
			p.CeilingDash();
		}

		// friction
		// trying to turn around
		if (p.velocity.x * p.GetInput().GetDir().x <= 0.0) {
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
			float activeFrict = p.IsPushingFloor() ? FRICTION : AIR_FRICTION;
			p.velocity.x -= Math::CopySignOrZero(activeFrict * delta, p.velocity.x);
		}

		// accelerating
		if (abs(p.velocity.x) < TOP_SPEED) {
			p.velocity.x += ACCELERATION * p.GetInput().GetDir().x * delta;
		}

		// starting a fastfall
		// not fastfalling, do nothing
		if (!p.GetInput().IsTapped(ACTION_DOWN)) {
		}

		// fastfalling around peak of jump, maximize downwards velocity
		else if (!p.IsPushingFloor() && abs(p.velocity.y) < FAST_FALL_WINDOW) {
			/*
			float timeFalling = abs(velocity.y) / GRAVITY;
			float timeFastFalling = sqrtf(GRAVITY * timeFalling * timeFalling / FAST_FALL_GRAVITY);

			velocity.y = FAST_FALL_GRAVITY * timeFastFalling;
			*/

			p.velocity.y = abs(p.velocity.y) * sqrtf(FAST_FALL_GRAVITY / GRAVITY);
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

		else if (p.GetInput().IsDown(ACTION_DOWN)) {
			p.velocity.y += FAST_FALL_GRAVITY * delta;
		}

		else {
			p.velocity.y += GRAVITY * delta;
		}

		// resetting coyote timer
		if (p.IsPushingFloor()) {
			p.SetTimer(Player::TIMER_COYOTE);
		}

		// jump / dash buffer
		if (!p.GetInput().IsTapped(ACTION_JUMP)) {
		}

		else if (p.IsCloseToFloor() || p.TimerActive(Player::TIMER_COYOTE)) {
			p.Buffer(Player::BUFFER_JUMP);
		}

		else {
			p.Buffer(Player::BUFFER_DASH);
			// TODO dashblocker
		}

		// dashing
		bool dashFirst =
			!p.BufferActive(Player::BUFFER_DIVE) || p.GetBuffer(Player::BUFFER_DASH) < p.GetBuffer(Player::BUFFER_DIVE) || !p.IsDiveAvailable();

		if (!p.BufferActive(Player::BUFFER_DASH)) {
		}

		else if (dashFirst && p.velocity.y > GRAVITY * delta && p.HasUpgrade(Player::UPGRADE_DASH) && p.IsDashAvailable()) {
			p.SetState(Player::MOVEMENT_STATE_DASH);
			return;
		}

		// dive buffer
		bool wantsToDive = p.velocity.y < GRAVITY * delta || !p.IsCloseToFloor();
		bool wantsToUltraslide = p.velocity.y > MAX_DIVE_BUFFER_Y_VELOCITY && p.GetInput().IsDown(ACTION_DOWN);

		if (!p.GetInput().IsTapped(ACTION_DIVE)) {
		}

		else if (wantsToDive && !wantsToUltraslide) {
			p.Buffer(Player::BUFFER_DIVE);
			// TODO implement diveblocker
		}

		else {
			p.Buffer(Player::BUFFER_SLIDE);
		}

		// sliding
		bool inputtingSlide = p.GetInput().IsDown(ACTION_DIVE) || p.BufferActive(Player::BUFFER_SLIDE);
		bool slideAvailable = p.HasUpgrade(Player::UPGRADE_SLIDE) && !p.CooldownActive(p.COOLDOWN_SLIDE);

		if (!p.IsPushingFloor() || !inputtingSlide || !slideAvailable) {
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
		bool canDuck = p.IsPushingFloor() && !p.BufferActive(Player::BUFFER_JUMP) && p.HasUpgrade(Player::UPGRADE_SLIDE);

		if (canDuck && p.velocity.x == 0.0 && p.GetInput().GetDir() == Vector2{0.0, 1.0}) {
			p.SetState(Player::MOVEMENT_STATE_DUCK);
			return;
		}

		// diving
		if (p.BufferActive(Player::BUFFER_DIVE) && p.velocity.y > GRAVITY * delta && p.HasUpgrade(Player::UPGRADE_DIVE) && p.IsDiveAvailable()) {
			p.SetState(Player::MOVEMENT_STATE_DIVE);
			return;
		}

		// jumping
		if (p.TimerActive(Player::TIMER_COYOTE) && p.UseBuffer(Player::BUFFER_JUMP)) {
			p.UnsetTimer(Player::TIMER_COYOTE);
			p.velocity.y = -JUMP_FORCE;

			p.SetSquish(Player::X_SQUISH_MIN);
			// TODO moving platforms
		}

		// grabbing ledges
		p.UpdateLedgeTile();
		if (p.CanGrabLedge()) {
			p.SetState(Player::MOVEMENT_STATE_LEDGE);
		}

		// updating animation
		if (p.IsPushingFloor()) {
			if (p.velocity.x == 0.0 || p.IsPushingWall()) {
				// TODO bored and twerk
				p.PlayAnimation(Player::ANIM_IDLE);
			}

			else if (abs(p.velocity.x) <= SLOW_RUN_SPEED) {
				p.PlayAnimation(Player::ANIM_SLOW_RUN, abs(p.velocity.x) / TOP_SPEED);
			}

			else {
				p.PlayAnimation(Player::ANIM_RUN, abs(p.velocity.x) / TOP_SPEED);
			}
		}

		else {
			p.PlayAnimation(Player::ANIM_JUMP);
		}

		// flipping sprite
		if (p.velocity.x < 0.0 && !p.IsFacingLeft())
			p.FlipSprite(true);
		else if (p.velocity.x > 0.0)
			p.FlipSprite(false);
	}

	void Deinit(Player& p) const override {}
};