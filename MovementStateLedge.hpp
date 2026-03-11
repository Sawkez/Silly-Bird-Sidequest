#pragma once

const float LEDGE_OFFSET_HEIGHT = 10.0;
const float LEDGE_OFFSET_LEFT = 12.0;
const float LEDGE_OFFSET_RIGHT = -4.0;

const Vector2 LEDGE_SIDE_JUMP_FORCE{200.0, -125.0};
const float LEDGE_UP_JUMP_FORCE = 250.0;
const float LEDGE_UP_COOLDOWN = 5.0 / 60.0;
const float LEDGE_DOWN_COOLDOWN = 10.0 / 60.0;

void Player::LedgeInit() {
	position = _ledgeTile;
	position.y += LEDGE_OFFSET_HEIGHT;

	if (_facingLeft) {
		position.x += LEDGE_OFFSET_LEFT;
	}

	else {
		position.x += LEDGE_OFFSET_RIGHT;
	}

	velocity = Vector2::ZERO;
	_quickClimb = true;

	_sprite.PlayLastFrame(ANIM_LEDGE_UNFLIP);
	_sprite.scale.x = X_SQUISH_MAX;
}

void Player::LedgeProcess(float delta) {
	if (_input.IsTapped(ACTION_JUMP) || _input.IsTapped(ACTION_UP) || BufferActive(BUFFER_DASH) || BufferActive(BUFFER_LEDGE_JUMP)) {
		Unbuffer(BUFFER_DASH);
		Unbuffer(BUFFER_LEDGE_JUMP);

		if (_input.GetDir().x != 0.0 && (_input.GetDir().x < 0.0 != _facingLeft)) {
			velocity = {LEDGE_SIDE_JUMP_FORCE.x * _input.GetDir().x, LEDGE_SIDE_JUMP_FORCE.y};
		}

		else {
			velocity.y = -LEDGE_UP_JUMP_FORCE;
		}

		_cooldowns[COOLDOWN_LEDGE] = LEDGE_UP_COOLDOWN;
		SetState(MOVEMENT_STATE_NORMAL);
	}

	else if (_input.IsTapped(ACTION_DOWN)) {
		_cooldowns[COOLDOWN_LEDGE] = LEDGE_DOWN_COOLDOWN;
		SetState(MOVEMENT_STATE_NORMAL);
	}

	else if (_input.GetDir().x == 0.0 || (_input.GetDir().x < 0.0 == _facingLeft)) {
		_sprite.Play(ANIM_LEDGE_UNFLIP);
	}

	else {
		_sprite.Play(ANIM_LEDGE_FLIP);
	}
}

void Player::LedgeDeinit() {
	_sprite.scale.x = X_SQUISH_MIN;
	UnsetTimer(TIMER_COYOTE);
}