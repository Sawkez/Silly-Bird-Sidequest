#pragma once

#include "engine/devconsole/DevConsoleFlags.hpp"
#include "engine/devconsole/variable/DevConsoleVariable.hpp"
#include "game/player/Player.hpp"
#include "game/player/movement/IMovementState.hpp"

#define CONVAR_CATEGORY PLAYER_STATE_DIVE

class MovementStateDive : public IMovementState {
	// clang-format off
	CONVAR(float,	_initialVelocity,	INITIAL_VELOCITY,	200.0f,				DC_FLAG_CHEAT);
	CONVAR(float,	_initialGravity,	INITIAL_GRAVITY,	60.0f,				DC_FLAG_CHEAT);
	CONVAR(float,	_gravityMult,		GRAVITY_MULT,		304.481639541f,		DC_FLAG_CHEAT); // powf(1.1, 60.0)
	CONVAR(float,	_gravityMax,		GRAVITY_MAX,		900.0f,				DC_FLAG_CHEAT);
	CONVAR(float,	_cancelDuration,	CANCEL_DURATION,	5.0f / 60.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_boostForce,		BOOST_FORCE,		200.0f,				DC_FLAG_CHEAT);
	CONVAR(float,	_friction,			FRICTION,			300.0f,				DC_FLAG_CHEAT);
	// clang-format on

	void Init(Player& p) const override {
		p.UnloadDive();
		p.Unbuffer(Player::BUFFER_DIVE);
		p.SetTimer(Player::TIMER_DIVE);
		p.SetCurrentDiveGravity(*_initialGravity);

		p.velocity.x = max(*_initialVelocity, abs(p.velocity.x));
		p.velocity.y = 0.0;

		if (p.IsFacingLeft()) {
			p.velocity.x = -p.velocity.x;
		}

		p.DisableQuickClimb();

		p.PlayAnimation(Player::ANIM_FLY);
	}

	void Process(Player& p, float delta) const override {
		// friction
		if (abs(p.velocity.x) > *_initialVelocity) {
			p.velocity.x -= copysignf(*_friction * delta, p.velocity.x);
		}

		// keeping top speed
		else {
			p.velocity.x = copysignf(*_initialVelocity, p.velocity.x);
		}

		// gravity
		p.velocity.y += p.GetCurrentDiveGravity() * delta;
		p.SetCurrentDiveGravity(min(p.GetCurrentDiveGravity() * powf(*_gravityMult, delta), *_gravityMax));

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

			p.velocity.y = -*_boostForce;
			p.SetCurrentDiveGravity(*_initialGravity);
		}

		// pivoting
		if (p.GetInput().GetDir().x * p.velocity.x < 0.0 && p.GetTimer(Player::TIMER_DIVE) > *_cancelDuration) {
			p.velocity.x = -p.velocity.x;
			p.FlipSprite(!p.IsFacingLeft());
			p.SetTimer(Player::TIMER_DIVE, *_cancelDuration);
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

			if (p.GetTimer(Player::TIMER_DIVE) > *_cancelDuration) {
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

#undef CONVAR_CATEGORY