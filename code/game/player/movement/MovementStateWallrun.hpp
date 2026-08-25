#pragma once

#include <math.h>

#include "engine/Math.hpp"
#include "engine/devconsole/DevConsoleFlags.hpp"
#include "engine/devconsole/variable/DevConsoleVariable.hpp"
#include "engine/physics/Raycast.hpp"
#include "game/player/Player.hpp"
#include "game/player/movement/IMovementState.hpp"

#define CONVAR_CATEGORY PLAYER_STATE_WALLRUN

class MovementStateWallrun : public IMovementState {
	// clang-format off
	CONVAR(float,	_stickAccel,		STICK_ACCEL,		1800.0f,	DC_FLAG_CHEAT);
	CONVAR(float,	_dropAccel,			DROP_ACCEL,			450.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_stickVelocity,		STICK_VELOCITY,		150.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_gravity,			GRAVITY,			500.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_maxDist,			MAX_DIST,			6.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_initialVelocity,	INITIAL_VELOCITY,	250.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_jumpForceX,		JUMP_FORCE_X,		250.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_jumpForceY,		JUMP_FORCE_Y,		200.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_dropVelocity,		DROP_VELOCITY,		300.0f,		DC_FLAG_CHEAT);
	// clang-format on

	void Init(Player& p) const override {
		p.PlayAnimation(Player::ANIM_WALLRUN);

		p.velocity.y = min(p.velocity.y, -*_initialVelocity);

		p.EnableQuickClimb();
	}

	void Process(Player& p, float delta) const override {
		float wallDir = p.IsFacingLeft() ? -1.0 : 1.0;

		if (p.GetInput().IsTapped(ACTION_JUMP)) {
			p.Buffer(Player::BUFFER_WALLJUMP);
		}

		if (wallDir * p.GetInput().GetDir().x < 0.0) {
			if (!p.GetInput().IsDown(ACTION_DIVE)) {
				p.velocity.x -= wallDir * *_dropAccel * delta;
			}

			if (p.BufferActive(Player::BUFFER_WALLJUMP)) {
				p.velocity.x = copysignf(*_jumpForceX, -wallDir);
				p.SetState(Player::MOVEMENT_STATE_NORMAL);
				return;
			}
		}

		else {
			p.velocity.x += wallDir * *_stickAccel * delta;
			p.velocity.x = clamp(p.velocity.x, -*_stickVelocity, *_stickVelocity);
		}

		bool shouldLetGoAtTop = !p.GetInput().IsDown(ACTION_DIVE) || p.BufferActive(Player::BUFFER_WALLJUMP);
		bool isAtTop = p.velocity.y >= 0.0;
		bool isAtBottom = p.velocity.y > *_dropVelocity || p.IsPushingFloor();

		Raycast wallCast(p.position + Vector2(0, -p.BODY_CENTER.y), p.IsFacingLeft() ? Raycast::LEFT : Raycast::RIGHT,
						 *_maxDist);

		bool hasWall = p.GetRoomColliders().CheckRaycast(wallCast);

		if (isAtTop && shouldLetGoAtTop || isAtBottom || !hasWall) {
			p.SetState(Player::MOVEMENT_STATE_NORMAL);
			return;
		}

		p.UpdateLedgeTile();
		if (p.CanGrabLedge()) {
			p.SetState(Player::MOVEMENT_STATE_LEDGE);
			return;
		}

		p.velocity.y += *_gravity * delta;
	}

	void Deinit(Player& p) const override {
		if (p.UseBuffer(Player::BUFFER_WALLJUMP)) {
			p.velocity.y = min(0.0f, p.velocity.y) - *_jumpForceY;
		}

		p.SetCooldown(Player::COOLDOWN_WALLRUN);
	}
};

#undef CONVAR_CATEGORY