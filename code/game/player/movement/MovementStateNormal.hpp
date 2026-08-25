#pragma once

#include <cmath>

#include "engine/Math.hpp"
#include "engine/devconsole/DevConsole.hpp"
#include "engine/devconsole/DevConsoleFlags.hpp"
#include "engine/devconsole/variable/DevConsoleVariable.hpp"
#include "engine/input/InputManager.hpp"
#include "game/player/Player.hpp"
#include "game/player/movement/IMovementState.hpp"

#define CONVAR_CATEGORY PLAYER_STATE_NORMAL

struct MovementStateNormal : public IMovementState {
	// clang-format off
	CONVAR(float,	_acceleration,				ACCELERATION,					900.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_topSpeed,					TOP_SPEED,						125.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_friction,					FRICTION,						1200.0f,	DC_FLAG_CHEAT);
	CONVAR(float,	_airFriction,				AIR_FRICTION,					60.0f,		DC_FLAG_CHEAT);

	CONVAR(float,	_weakGravity,				WEAK_GRAVITY,					600.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_gravity,					GRAVITY,						900.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_fastFallGravity,			FAST_FALL_GRAVITY,				1200.0f,	DC_FLAG_CHEAT);
	
	CONVAR(float,	_fastFallWindow,			FAST_FALL_WINDOW,				105.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_fallSpeedCap,				FALL_SPEED_CAP,					200.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_jumpForce,					JUMP_FORCE,						250.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_maxDiveBufferYVelocity,	MAX_DIVE_BUFFER_Y_VELOCITY,		250.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_slowRunSpeed,				SLOW_RUN_SPEED,					100.0f,		DC_FLAG_CHEAT);

	// clang-format on

	void Init(Player& p) const override {}

	void Process(Player& p, float delta) const override {
		if (p.IsShortCollision() && !p.IsCloseToCeiling()) {
			p.SetShortCollision(false);
		}

		// ceiling dash
		if (p.TimerActive(Player::TIMER_DASH) && p.velocity.y < *_gravity * delta && p.IsPushingCeiling()) {
			p.CeilingDash();
		}

		// friction
		// trying to turn around
		if (p.velocity.x * p.GetInput().GetDir().x <= 0.0) {
			// make sure we don't overcompensate
			float activeFrict = fminf(*_friction * delta, abs(p.velocity.x));
			p.velocity.x -= Math::CopySignOrZero(activeFrict, p.velocity.x);
		}

		// still accelerating (do nothing)
		else if (abs(p.velocity.x) < *_topSpeed) {
		}

		// going barely too fast, lock to top speed
		else if (abs(p.velocity.x) - *_topSpeed < *_friction * delta) {
			p.velocity.x = Math::CopySignOrZero(*_topSpeed, p.velocity.x);
		}

		// going way too fast
		else {
			float activeFrict = p.IsPushingFloor() ? *_friction : *_airFriction;
			p.velocity.x -= Math::CopySignOrZero(activeFrict * delta, p.velocity.x);
		}

		// accelerating
		if (abs(p.velocity.x) < *_topSpeed) {
			p.velocity.x += *_acceleration * p.GetInput().GetDir().x * delta;
		}

		// starting a fastfall
		// not fastfalling, do nothing
		if (!p.GetInput().IsTapped(ACTION_DOWN)) {
		}

		// fastfalling around peak of jump, maximize downwards velocity
		else if (!p.IsPushingFloor() && abs(p.velocity.y) < *_fastFallWindow) {
			/*
			float timeFalling = abs(velocity.y) / GRAVITY;
			float timeFastFalling = sqrtf(GRAVITY * timeFalling * timeFalling / FAST_FALL_GRAVITY);

			velocity.y = FAST_FALL_GRAVITY * timeFastFalling;
			*/

			p.velocity.y = abs(p.velocity.y) * sqrtf(*_fastFallGravity / *_gravity);
			dc::msg << "setting velocity to " << p.velocity.y << dc::endl;
		}

		// fastfalling normally, cancel up velocity
		else {
			p.velocity.y = fmaxf(p.velocity.y, 0.0);
		}

		// applying gravity
		if (p.velocity.y > *_fallSpeedCap || p.TimerActive(Player::TIMER_GRAVITY_FREEZE)) {
			p.velocity.y += *_weakGravity * delta;
		}

		else if (p.GetInput().IsDown(ACTION_DOWN)) {
			p.velocity.y += *_fastFallGravity * delta;
		}

		else {
			p.velocity.y += *_gravity * delta;
		}

		// resetting coyote timer
		if (p.IsPushingFloor()) {
			p.SetTimer(Player::TIMER_COYOTE);
		}

		// jump / dash buffer
		if (!p.GetInput().IsTapped(ACTION_JUMP)) {
		}

		else if (p.velocity.y > 0.0 && p.IsCloseToFloor() || p.TimerActive(Player::TIMER_COYOTE)) {
			p.Buffer(Player::BUFFER_JUMP);
		}

		else {
			p.Buffer(Player::BUFFER_DASH);
			// TODO dashblocker
		}

		// dive buffer
		bool wantsToDive = p.velocity.y < *_gravity * delta || !p.IsCloseToFloor();
		bool wantsToUltraslide = p.velocity.y > *_maxDiveBufferYVelocity && p.GetInput().IsDown(ACTION_DOWN);

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
		bool canDuck =
			p.IsPushingFloor() && !p.BufferActive(Player::BUFFER_JUMP) && p.HasUpgrade(Player::UPGRADE_SLIDE);

		if (canDuck && p.velocity.x == 0.0 && p.GetInput().GetDir() == Vector2{0.0, 1.0}) {
			p.SetState(Player::MOVEMENT_STATE_DUCK);
			return;
		}

		// diving
		if (p.BufferActive(Player::BUFFER_DIVE) && p.velocity.y > *_gravity * delta &&
			p.HasUpgrade(Player::UPGRADE_DIVE) && p.IsDiveAvailable()) {
			p.SetState(Player::MOVEMENT_STATE_DIVE);
			return;
		}

		// jumping
		if (p.TimerActive(Player::TIMER_COYOTE) && p.UseBuffer(Player::BUFFER_JUMP)) {
			p.UnsetTimer(Player::TIMER_COYOTE);
			p.velocity.y = -*_jumpForce;

			p.SetSquish(Player::X_SQUISH_MIN);
			// TODO moving platforms
		}

		// dashing
		bool dashFirst = !p.BufferActive(Player::BUFFER_DIVE) ||
						 p.GetBuffer(Player::BUFFER_DASH) < p.GetBuffer(Player::BUFFER_DIVE) || !p.IsDiveAvailable();

		if (!p.BufferActive(Player::BUFFER_DASH)) {
		}

		else if (dashFirst && p.velocity.y > *_gravity * delta && p.HasUpgrade(Player::UPGRADE_DASH) &&
				 p.IsDashAvailable()) {
			p.SetState(Player::MOVEMENT_STATE_DASH);
			return;
		}

		// wallrunning
		if (p.HasUpgrade(Player::UPGRADE_WALLRUN) && !p.IsPushingFloor() && p.IsPushingWall() && p.IsDiveAvailable() &&
			p.GetInput().IsDown(ACTION_DIVE)) {
			p.UnloadDive();
			p.SetState(Player::MOVEMENT_STATE_WALLRUN);
			return;
		}

		// grabbing ledges
		p.UpdateLedgeTile();
		if (p.CanGrabLedge()) {
			p.SetState(Player::MOVEMENT_STATE_LEDGE);
			return;
		}

		// updating animation
		if (p.IsPushingFloor()) {
			if (p.velocity.x == 0.0 || p.IsPushingWall()) {
				p.PlayAnimation(p.TimerActive(Player::TIMER_TWERK) ? Player::ANIM_TWERK_UP : Player::ANIM_IDLE);
				// TODO bored animations
			}

			else if (abs(p.velocity.x) <= *_slowRunSpeed) {
				p.PlayAnimation(Player::ANIM_SLOW_RUN, abs(p.velocity.x) / *_topSpeed);
			}

			else {
				p.SetTimer(Player::TIMER_TWERK, Player::TWERK_TIMER_MIN);
				p.PlayAnimation(Player::ANIM_RUN, abs(p.velocity.x) / *_topSpeed, true);
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

#undef CONVAR_CATEGORY