#pragma once

#include "game/player/Player.hpp"
#include "game/player/movement/IMovementState.hpp"

class MovementStateDuck : public IMovementState {
	const float SQUISH_DUCK = 0.5;
	const float SQUISH_STAND_UP = 1.5;
	const float SQUISH_TWERK_DOWN = 0.9;
	const float SQUISH_TWERK_UP = 1.1;
	const float DUCK_GRAVITY = 0.01;
	const float DUCK_JUMP_FORCE = 250.0;

	void Init(Player& p) const override {
		p.Unbuffer(Player::BUFFER_SLIDE);

		p.SetShortCollision(true);
		p.SetSquish(p.TimerActive(Player::TIMER_TWERK) ? SQUISH_TWERK_DOWN : SQUISH_DUCK);

		if (p.TimerActive(Player::TIMER_TWERK)) p.PlayAnimation(Player::ANIM_TWERK_DOWN);
		p.IncrementTwerkTimer();
	}

	void Process(Player& p, float delta) const override {
		p.velocity = Vector2{0.0, DUCK_GRAVITY * delta};

		if (!p.TimerActive(Player::TIMER_TWERK)) p.PlayAnimation(Player::ANIM_DUCK);

		// sliding
		if (p.GetInput().GetDir().x != 0.0 && (p.GetInput().IsDown(ACTION_DIVE) || p.IsCloseToCeiling()) &&
			!p.CooldownActive(Player::COOLDOWN_SLIDE)) {
			// duck double-jump
			if (!p.IsCloseToCeiling() && p.GetInput().IsTapped(ACTION_JUMP)) {
				p.velocity.y = -DUCK_JUMP_FORCE;
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
		p.SetSquish(p.TimerActive(Player::TIMER_TWERK) ? SQUISH_TWERK_UP : SQUISH_STAND_UP);
		p.SetShortCollision(false);
	}
};