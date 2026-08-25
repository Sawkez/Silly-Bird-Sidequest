#pragma once

#include "engine/devconsole/DevConsoleFlags.hpp"
#include "engine/devconsole/variable/DevConsoleVariable.hpp"
#include "game/player/Player.hpp"
#include "game/player/graphics/PlayerSquish.hpp"
#include "game/player/movement/IMovementState.hpp"

#define CONVAR_CATEGORY PLAYER_STATE_DUCK

class MovementStateDuck : public IMovementState {
	CONVAR(float, _gravity, GRAVITY, 1.0f, DC_FLAG_CHEAT);
	CONVAR(float, _jumpForce, JUMP_FORCE, 25.0f, DC_FLAG_CHEAT);

	void Init(Player& p) const override {
		p.Unbuffer(Player::BUFFER_SLIDE);

		p.SetShortCollision(true);
		p.SetSquish(p.TimerActive(Player::TIMER_TWERK) ? *PlayerSquish::twerkDown : *PlayerSquish::duck);

		if (p.TimerActive(Player::TIMER_TWERK)) p.PlayAnimation(Player::ANIM_TWERK_DOWN);
		p.IncrementTwerkTimer();
	}

	void Process(Player& p, float delta) const override {
		p.velocity = Vector2{0.0, *_gravity * delta};

		if (!p.TimerActive(Player::TIMER_TWERK)) p.PlayAnimation(Player::ANIM_DUCK);

		// sliding
		if (p.GetInput().GetDir().x != 0.0 && (p.GetInput().IsDown(ACTION_DIVE) || p.IsCloseToCeiling()) &&
			!p.CooldownActive(Player::COOLDOWN_SLIDE)) {
			// duck double-jump
			if (!p.IsCloseToCeiling() && p.GetInput().IsTapped(ACTION_JUMP)) {
				p.velocity.y = -*_jumpForce;
				p.SetState(Player::MOVEMENT_STATE_NORMAL);
				return;
			}

			p.velocity.x = p.GetInput().GetDir().x;
			p.SetState(Player::MOVEMENT_STATE_SLIDE);
			return;
		}

		if (p.IsCloseToCeiling()) {
			return;
		}

		// unducking
		if (!p.IsPushingFloor() || !(p.GetInput().IsDown(ACTION_DIVE) || p.GetInput().GetDir() == Vector2{0.0, 1.0})) {
			p.SetState(Player::MOVEMENT_STATE_NORMAL);
			return;
		}

		// jumping
		if (p.GetInput().IsTapped(ACTION_JUMP)) {
			p.SetCooldown(Player::COOLDOWN_SLIDE);
			p.Buffer(Player::BUFFER_JUMP);
			p.SetState(Player::MOVEMENT_STATE_NORMAL);
		}
	}

	void Deinit(Player& p) const override {
		p.SetTimer(Player::TIMER_COYOTE);
		p.SetSquish(p.TimerActive(Player::TIMER_TWERK) ? *PlayerSquish::twerkUp : *PlayerSquish::standUp);
		p.SetShortCollision(false);
	}
};

#undef CONVAR_CATEGORY