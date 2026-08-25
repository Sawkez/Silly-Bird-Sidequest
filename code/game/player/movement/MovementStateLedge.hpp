#pragma once

#include "engine/Vector2.hpp"
#include "engine/devconsole/DevConsoleFlags.hpp"
#include "engine/devconsole/variable/DevConsoleVariable.hpp"
#include "game/player/Player.hpp"
#include "game/player/movement/IMovementState.hpp"

#define CONVAR_CATEGORY PLAYER_STATE_LEDGE

class MovementStateLedge : public IMovementState {
	// clang-format off
	CONVAR(float,	_offsetHeight,		OFFSET_HEIGHT,		10.0f,			DC_FLAG_CHEAT);
	CONVAR(float,	_offsetLeft,		OFFSET_LEFT,		12.0f,			DC_FLAG_CHEAT);
	CONVAR(float,	_offsetRight,		OFFSET_RIGHT,		-4.0f,			DC_FLAG_CHEAT);

	CONVAR(float,	_sideJumpForceX,	SIDE_JUMP_FORCE_X,	200.0f,			DC_FLAG_CHEAT);
	CONVAR(float,	_sideJumpForceY,	SIDE_JUMP_FORCE_Y,	-200.0f,		DC_FLAG_CHEAT);
	CONVAR(float,	_upJumpForce,		UP_JUMP_FORCE,		250.0f,			DC_FLAG_CHEAT);
	CONVAR(float,	_upCooldown,		UP_COOLDOWN,		5.0f / 60.0f,	DC_FLAG_CHEAT);
	CONVAR(float,	_downCooldown,		DOWN_COOLDOWN,		10.0f / 60.0f,	DC_FLAG_CHEAT);
	// clang-format on

	void Init(Player& p) const override {
		p.position = p.GetLedgeTile();
		p.position.y += *_offsetHeight;

		if (p.IsFacingLeft()) {
			p.position.x += *_offsetLeft;
		}

		else {
			p.position.x += *_offsetRight;
		}

		p.velocity = Vector2::ZERO;
		p.EnableQuickClimb();

		p.PlayAnimationLastFrame(Player::Player::ANIM_LEDGE_UNFLIP, 0.0);
		p.SetSquish(*PlayerSquish::xMax);
	}

	void Process(Player& p, float delta) const override {
		if (p.GetInput().IsTapped(ACTION_JUMP) || p.GetInput().IsTapped(ACTION_UP) ||
			p.BufferActive(Player::BUFFER_DASH) || p.BufferActive(Player::BUFFER_LEDGE_JUMP)) {
			p.Unbuffer(Player::BUFFER_DASH);
			p.Unbuffer(Player::BUFFER_LEDGE_JUMP);

			if (p.GetInput().GetDir().x != 0.0 && (p.GetInput().GetDir().x < 0.0 != p.IsFacingLeft())) {
				p.velocity = {*_sideJumpForceX * p.GetInput().GetDir().x, *_sideJumpForceY};
			}

			else {
				p.velocity.y = -*_upJumpForce;
			}

			p.SetCooldown(Player::COOLDOWN_LEDGE, *_upCooldown);
			p.SetState(Player::MOVEMENT_STATE_NORMAL);
		}

		else if (p.GetInput().IsTapped(ACTION_DOWN)) {
			p.SetCooldown(Player::COOLDOWN_LEDGE, *_downCooldown);
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
		if (p.IsCloseToCeiling()) {
			p.SetShortCollision(true);
		}
		p.SetSquish(*PlayerSquish::xMin);
		p.UnsetTimer(Player::TIMER_COYOTE);
		p.UnsetCooldown(Player::COOLDOWN_WALLRUN);
	}
};

#undef CONVAR_CATEGORY