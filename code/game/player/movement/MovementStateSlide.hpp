#pragma once

#include "engine/devconsole/DevConsole.hpp"
#include "engine/devconsole/DevConsoleFlags.hpp"
#include "engine/devconsole/variable/DevConsoleVariable.hpp"
#include "game/player/Player.hpp"
#include "game/player/graphics/PlayerSquish.hpp"
#include "game/player/movement/IMovementState.hpp"

#define CONVAR_CATEGORY PLAYER_STATE_SLIDE

class MovementStateSlide : public IMovementState {
	// clang-format off
	CONVAR(float,	_initialSpeed,				INITIAL_SPEED,				250.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_speed,						SPEED,						200.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_ultraSlideVelocityMult,	ULTRA_SLIDE_VELOCITY_MULT,	0.975f,		DC_FLAG_CHEAT);
	CONVAR(float,	_gravity,					GRAVITY,					1.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_friction,					FRICTION,					300.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_jumpForce,					JUMP_FORCE,					250.0f,		DC_FLAG_CHEAT);
	// clang-format on

	void Init(Player& p) const override {
		p.Unbuffer(Player::BUFFER_SLIDE);
		p.SetTimer(Player::TIMER_SLIDE);

		p.SetShortCollision(true);
		p.FlipSprite(p.velocity.x < 0.0);

		if (p.GetInput().GetDir().y <= 0.0) {
			p.ResetLastDownVelocity();
		}

		float maxSpeed = max(abs(p.velocity.x), *_initialSpeed);
		if (p.GetInput().GetDir().y == 1.0) {
			maxSpeed = max(maxSpeed, p.GetLastDownVelocity() * *_ultraSlideVelocityMult);
		}

		p.velocity.x = copysignf(maxSpeed, p.velocity.x);

		p.SetSquish(*PlayerSquish::slide);
		p.PlayAnimation(Player::ANIM_SLIDE);

		if (abs(p.velocity.x) > *_initialSpeed) {
			dc::msg << "Ultrasliding with " << p.velocity.x << dc::endl;
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

			p.velocity.y = -*_jumpForce;
			p.SetSquish(*PlayerSquish::xMin);

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

		p.velocity.y = *_gravity * delta;

		p.velocity.x = abs(p.velocity.x) - *_friction * delta;
		p.velocity.x = max(p.velocity.x, *_speed);
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

#undef CONVAR_CATEGORY