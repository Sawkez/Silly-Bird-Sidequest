#pragma once

const float SLIDE_INITIAL_SPEED = 250.0;
const float SLIDE_SPEED = 200.0;
const float ULTRASLIDE_VELOCITY_MULT = 0.75;
const float SLIDE_GRAVITY = 0.01;
const float SQUISH_SLIDE = 0.5;
const float SLIDE_FRICTION = 300.0;

void Player::SlideInit() {
    Unbuffer(BUFFER_SLIDE);
    SetTimer(TIMER_SLIDE);

    SetShortCollision(true);
    FlipSprite(velocity.x < 0.0);

    if (_input.GetDir().y <= 0.0) {
        _lastVerticalVelocity = 0.0;
    }

    float maxSpeed = max(abs(velocity.x), SLIDE_INITIAL_SPEED);
    if (_input.GetDir().y == 1.0) {
        maxSpeed = max(maxSpeed, _lastVerticalVelocity * ULTRASLIDE_VELOCITY_MULT);
    }

    velocity.x = copysignf(maxSpeed, velocity.x);

    _sprite.scale.x = SQUISH_SLIDE;
    _sprite.Play(ANIM_SLIDE);
}

void Player::SlideProcess(float delta) {
    
    // jump buffer
    if (_input.IsTapped(ACTION_JUMP)) {
        Buffer(BUFFER_JUMP);
    }

    // jumping
    else if (BufferActive(BUFFER_JUMP) && !_closeToCeiling) {
        Unbuffer(BUFFER_JUMP);

        velocity.y = -JUMP_FORCE;
        _sprite.scale.x = X_SQUISH_MIN;

        SetState(MOVEMENT_STATE_NORMAL);
        return;
    }

    // sliding off ledge
    else if (!_pushingFloor) {
        SetState(MOVEMENT_STATE_NORMAL);
        return;
    }

    // crashing into a wall
    if (_pushingWall) {
        SetState(_closeToCeiling? MOVEMENT_STATE_DUCK : MOVEMENT_STATE_NORMAL);
        return;
    }

    // nothing happens
    else if (TimerActive(TIMER_SLIDE)) { }
    else if (_closeToCeiling) { }

    // releasing slide after time runs out
    else if (!_input.IsDown(ACTION_DIVE)) {
        SetState(MOVEMENT_STATE_NORMAL);
        return;
    }

    // pressing opposite direction
    else if (_input.GetDir().x * velocity.x < 0.0) {
        velocity = Vector2::ZERO;
        SetState(MOVEMENT_STATE_NORMAL);
        return;
    }

    velocity.y = SLIDE_GRAVITY * delta;

    velocity.x = abs(velocity.x) - SLIDE_FRICTION * delta;
    velocity.x = max(velocity.x, SLIDE_SPEED);
    if (_facingLeft) {
        velocity.x = -velocity.x;
    }
}

void Player::SlideDeinit() {
    SetTimer(TIMER_COYOTE);
    velocity.y = min(velocity.y, 0.0f);
    if (_closeToCeiling) {
        SetShortCollision(false);
    }
    Unbuffer(BUFFER_DIVE);
    SetCooldown(COOLDOWN_SLIDE);
}