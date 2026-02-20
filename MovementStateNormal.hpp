#pragma once

#include <cmath>
#include "InputManager.hpp"

const float ACCELERATION = 600.0;
const float TOP_SPEED = 125.0;
const float FRICTION = 1200.0;
const float AIR_FRICTION = 60.0;

const float WEAK_GRAVITY = 600.0;
const float GRAVITY = 900.0;
const float FAST_FALL_GRAVITY = 1200.0;

const float FAST_FALL_WINDOW = GRAVITY * 10.0 / 120.0; // 10 frames total, 5 frames either direction at 60 fps
const float FALL_SPEED_CAP = 200.0;

const float JUMP_FORCE = 250.0;

const float MAX_DIVE_BUFFER_Y_VELOCITY = 250.0;

const float SLOW_RUN_SPEED = 100.0;

void Player::NormalInit() { }

void Player::NormalProcess(float delta) {
    if (_shortCollision && !_closeToCeiling) {
        SetShortCollision(false);
    }

    // friction
    // trying to turn around
    if (velocity.x * _input.GetDir().x < 0.0) {
        // make sure we don't overcompensate
        float activeFrict = fminf(FRICTION * delta, abs(velocity.x)); 
        velocity.x -= copysignf32(activeFrict, velocity.x);
    }

    // still accelerating (do nothing)
    else if (abs(velocity.x < TOP_SPEED)) { }

    // going barely too fast, lock to top speed
    else if (abs(velocity.x) - TOP_SPEED < FRICTION * delta) {
        velocity.x = copysignf32(TOP_SPEED, velocity.x);
    }

    // going way too fast
    else {
        float activeFrict = _pushingFloor? FRICTION : AIR_FRICTION;
        velocity.x -= copysignf32(velocity.x, activeFrict * delta);
    }

    // accelerating
    if (abs(velocity.x < TOP_SPEED)) {
        velocity.x += ACCELERATION * _input.GetDir().x * delta;
    }

    // starting a fastfall
    // not fastfalling, do nothing
    if (!_input.IsTapped(ACTION_DOWN)) { }

    // fastfalling around peak of jump, maximize downwards velocity
    else if (!_pushingFloor && abs(velocity.y) < FAST_FALL_WINDOW) {

        float timeFalling = abs(velocity.y) / GRAVITY;
        float timeFastFalling = sqrtf32(GRAVITY * timeFalling * timeFalling / FAST_FALL_GRAVITY);

        velocity.y = FAST_FALL_GRAVITY * timeFastFalling;
    }

    // fastfalling normally, cancel up velocity
    else {
        velocity.y = fmaxf(velocity.y, 0.0);
    }

    // applying gravity
    if (velocity.y > FALL_SPEED_CAP || TimerActive(TIMER_GRAVITY_FREEZE)) {
        velocity.y += WEAK_GRAVITY * delta;
    }

    else if (_input.IsDown(ACTION_DOWN)) {
        velocity.y += FAST_FALL_GRAVITY * delta;
    }

    else {
        velocity.y += GRAVITY * delta;
    }

    // resetting coyote timer
    if (_pushingFloor) {
        SetTimer(TIMER_COYOTE);
    }

    // jump / dash buffer
    if (!_input.IsTapped(ACTION_JUMP)) { }

    else if (_closeToFloor || TimerActive(TIMER_COYOTE)) {
        Buffer(BUFFER_JUMP);
    }

    else {
        Buffer(BUFFER_DASH);
        // TODO dashblocker
    }

    // dashing
    bool dashFirst = !BufferActive(BUFFER_DIVE) || _buffers[BUFFER_DASH] < _buffers[BUFFER_DIVE] || !_diveAvailable;

    if (!BufferActive(BUFFER_DASH)) { }

    else if (dashFirst && velocity.y > GRAVITY * delta && HasUpgrade(UPGRADE_DASH) && _dashAvailable) {
        SetState(MOVEMENT_STATE_DASH);
        return;
    }

    // dive buffer
    bool wantsToDive = velocity.y < GRAVITY * delta || !_closeToFloor;
    bool wantsToUltraslide = velocity.y > MAX_DIVE_BUFFER_Y_VELOCITY && _input.IsDown(ACTION_DOWN);

    if (!_input.IsTapped(ACTION_DIVE)) { }

    else if (wantsToDive && !wantsToUltraslide) {
        Buffer(BUFFER_DIVE);
        // TODO implement diveblocker
    }

    else {
        Buffer(BUFFER_SLIDE);
    }

    // sliding
    bool inputtingSlide = _input.IsDown(ACTION_DIVE) || BufferActive(BUFFER_SLIDE);
    bool slideAvailable = HasUpgrade(UPGRADE_SLIDE) && !CooldownActive(COOLDOWN_SLIDE);

    if (!_pushingFloor || !inputtingSlide || !slideAvailable) { }

    else if (velocity.x != 0.0) {
        SetState(MOVEMENT_STATE_SLIDE);
        return;
    }

    // ducking
    else {
        SetState(MOVEMENT_STATE_DUCK);
        return;
    }

    // ducking with down key
    bool canDuck = _pushingFloor && !BufferActive(BUFFER_JUMP) && HasUpgrade(UPGRADE_SLIDE);

    if (canDuck && velocity.x == 0.0 && _input.GetDir() == Vector2{ 0.0, 1.0 }) {
        SetState(MOVEMENT_STATE_DUCK);
        return;
    }

    // diving
    if (BufferActive(BUFFER_DIVE) && velocity.y > GRAVITY * delta && HasUpgrade(UPGRADE_DIVE) && _diveAvailable) {
        SetState(MOVEMENT_STATE_DIVE);
        return;
    }

    // jumping
    if (TimerActive(TIMER_COYOTE) && UseBuffer(BUFFER_JUMP)) {
        UnsetTimer(TIMER_COYOTE);
        velocity.y = -JUMP_FORCE;

        // TODO squishy
        // TODO moving platforms
    }

    // TODO grabbing ledges

    // TODO updating animation

    // TODO flipping sprite

}

void Player::NormalDeinit() { }