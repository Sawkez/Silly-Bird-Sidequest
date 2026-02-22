#pragma once

const float DASH_X_VELOCITY = 200.0;
const float DASH_Y_VELOCITY = -200.0;
const float DASH_RETURN_TIME = 8.99 / 60.0;

void Player::DashInit() {
	UnloadDash();
	Unbuffer(BUFFER_DASH);
	SetTimer(TIMER_DASH);
	_sprite.scale.x = X_SQUISH_MAX;
	_sprite.PlayFromStart(ANIM_JUMP);

	velocity.y = DASH_Y_VELOCITY;
	velocity.x = max(DASH_X_VELOCITY, abs(velocity.x));

	if (_input.GetDir().x < 0.0) {
		velocity.x = -velocity.x;
	}

	else if (_facingLeft) {
		velocity.x = -velocity.x;
	}

	_quickClimb = false;
}

void Player::DashProcess(float delta) {
	if (_timers[TIMER_DASH] < DASH_RETURN_TIME) {
		SetState(MOVEMENT_STATE_NORMAL);
	}

	// cout << velocity << endl;
}

void Player::DashDeinit() {}