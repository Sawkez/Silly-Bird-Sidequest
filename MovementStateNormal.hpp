#pragma once

void Player::NormalInit() {

}

void Player::NormalProcess(float delta) {
    position += _input.GetDir() * 60.0 * delta;
}

void Player::NormalDeinit() {

}