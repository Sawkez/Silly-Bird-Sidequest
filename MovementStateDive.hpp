#pragma once

const float DIVE_INITIAL_VELOCITY = 200.0;
const float DIVE_INITIAL_GRAVITY = 60.0;
const float DIVE_GRAVITY_MULT = powf(1.1, 60.0);
const float DIVE_GRAVITY_MAX = 900.0;
const float DIVE_CANCEL_DURATION = 5.0 / 60.0;
const float DIVEBOOST_FORCE = 200.0;
const float DIVE_FRICTION = 300.0;

void Player::DiveInit() {
	UnloadDive();
	Unbuffer(BUFFER_DIVE);
	SetTimer(TIMER_DIVE);
	_currentDiveGravity = DIVE_INITIAL_GRAVITY;

	velocity.x = max(DIVE_INITIAL_VELOCITY, abs(velocity.x));
	velocity.y = 0.0;

	if (_facingLeft) {
		velocity.x = -velocity.x;
	}

	_quickClimb = false;

	_sprite.Play(ANIM_FLY);
}

void Player::DiveProcess(float delta) {
	// friction
	if (abs(velocity.x) > DIVE_INITIAL_VELOCITY) {
		velocity.x -= copysignf(DIVE_FRICTION * delta, velocity.x);
	}

	// keeping top speed
	else {
		velocity.x = copysignf(DIVE_INITIAL_VELOCITY, velocity.x);
	}

	// gravity
	velocity.y += _currentDiveGravity * delta;
	_currentDiveGravity = min(_currentDiveGravity * powf(DIVE_GRAVITY_MULT, delta), DIVE_GRAVITY_MAX);

	// rotating
	// FIXME rotation broken during diveboost
	_sprite.SetRotationRadians(atanf(copysignf(velocity.y, velocity.x) / abs(velocity.x)));

	// diveboost buffer
	if (_input.IsTapped(ACTION_JUMP)) {
		Buffer(BUFFER_DASH);
	}

	// diveboost
	if (BufferActive(BUFFER_DASH) && HasUpgrade(UPGRADE_DIVEBOOST) && _dashAvailable && _input.IsDown(ACTION_DIVE)) {
		UnloadDash();
		Unbuffer(BUFFER_DASH);

		velocity.y = -DIVEBOOST_FORCE;
		_currentDiveGravity = DIVE_INITIAL_GRAVITY;
	}

	// pivoting
	if (_input.GetDir().x * velocity.x < 0.0 && _timers[TIMER_DIVE] > DIVE_CANCEL_DURATION) {
		velocity.x = -velocity.x;
		FlipSprite(!_facingLeft);
		_timers[TIMER_DIVE] = DIVE_CANCEL_DURATION;
	}

	// undiving
	if (_pushingFloor || _pushingWall) {
		SetState(MOVEMENT_STATE_NORMAL);
	}

	else if (!TimerActive(TIMER_DIVE) && !_input.IsDown(ACTION_DIVE)) {
		_diveAvailable = _diveAvailable || _timers[TIMER_DIVE] > DIVE_CANCEL_DURATION;
		SetState(MOVEMENT_STATE_NORMAL);
	}
}

void Player::DiveDeinit() {
	_sprite.SetRotationDegrees(0.0);
}