#pragma once

const float SQUISH_DUCK = 0.5;
const float SQUISH_STAND_UP = 1.5;
const float DUCK_GRAVITY = 0.01;
const float DUCK_JUMP_FORCE = 250.0;

void Player::DuckInit() {
    Unbuffer(BUFFER_SLIDE);

    SetShortCollision(true);
    _sprite.scale.x = SQUISH_DUCK;
    _sprite.Play(ANIM_DUCK);
}

void Player::DuckProcess(float delta) {
    velocity = Vector2 {0.0, DUCK_GRAVITY * delta};

    // sliding
    if (_input.GetDir().x != 0.0 && (_input.IsDown(ACTION_DIVE) || _closeToCeiling) && !CooldownActive(COOLDOWN_SLIDE)) {

        // duck double-jump
        if (!_closeToCeiling && _input.IsTapped(ACTION_JUMP)) {
            velocity.y = -DUCK_JUMP_FORCE;
            SetState(MOVEMENT_STATE_NORMAL);
            return;
        }

        velocity.x = _input.GetDir().x;
        SetState(MOVEMENT_STATE_SLIDE);
        return;
    }

    if (_closeToCeiling) { return; }

    // unducking
    if (!_pushingFloor || !(_input.IsDown(ACTION_DIVE) || _input.GetDir() == Vector2{0.0, 1.0})) {
        SetState(MOVEMENT_STATE_NORMAL);
        return;
    }

    // jumping
    if (_input.IsTapped(ACTION_JUMP)) {
        SetCooldown(COOLDOWN_SLIDE);
        Buffer(BUFFER_JUMP);
        SetState(MOVEMENT_STATE_NORMAL);
    }
}

void Player::DuckDeinit() {
    SetTimer(TIMER_COYOTE);
    _sprite.scale.x = SQUISH_STAND_UP;
    SetShortCollision(false);
}