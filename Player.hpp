#pragma once

#include "IDrawable.hpp"
#include "IProcessable.hpp"
#include "InputManager.hpp"
#include "Vector2.hpp"
#include "Animation.hpp"
#include "AnimatedSprite.hpp"

class Player : IProcessable, IDrawable {

    enum PlayerAnimation {
        ANIM_DUCK, ANIM_FLY, ANIM_IDLE, ANIM_JUMP, ANIM_LEDGE_FLIP, ANIM_LEDGE_UNFLIP,
        ANIM_RUN, ANIM_SLOW_RUN, ANIM_SLIDE, ANIM_TWERK_DOWN, ANIM_TWERK_UP
    };

    enum MovementStateID {
        MOVEMENT_STATE_NORMAL, MOVEMENT_STATE_LEDGE, MOVEMENT_STATE_DIVE, MOVEMENT_STATE_DASH, MOVEMENT_STATE_SLIDE,
        MOVEMENT_STATE_DUCK, MOVEMENT_STATE_DEAD, _MOVEMENT_STATE_COUNT
    };

    enum Upgrade {
        UPGRADE_DIVE, UPGRADE_DASH, UPGRADE_SLIDE, UPGRADE_DIVEBOOST, UPGRADE_POWERCORD, UPGRADE_REJUVENATOR
    };

    enum Timer {
        TIMER_COYOTE, TIMER_PLATFORM, TIMER_V_RESET, TIMER_GRAVITY_FREEZE, TIMER_BORED, TIMER_TWERK,
        TIMER_DIVE, TIMER_DASH, TIMER_SLIDE, _TIMER_COUNT
    };

    const float TIMER_DURATIONS[_TIMER_COUNT] = {
        10.0    / 60.0,     // coyote
        30.0    / 60.0,     // platform
        5.0     / 60.0,     // v reset
        30.0    / 60.0,     // gravity freeze
        NAN,                // bored (should be randomized)
        NAN,                // twerk (use min & max)
        15.0    / 60.0,     // dive
        10.0    / 60.0,     // dash
        15.0    / 60.0      // slide
    };

    enum Cooldown {
        COOLDOWN_LEDGE, COOLDOWN_SLIDE, COOLDOWN_INTERACT, _COOLDOWN_COUNT
    };

    const float COOLDOWN_DURATIONS[_COOLDOWN_COUNT] = {
        NAN,                // ledge (use up & down)
        15.0    / 60.0,     // slide
        5.0     / 60.0      // interact
    };

    enum Buffer {
        BUFFER_JUMP, BUFFER_DIVE, BUFFER_DASH, BUFFER_SLIDE, BUFFER_LEDGE_JUMP, BUFFER_INTERACT, _BUFFER_COUNT
    };

    const float BUFFER_DURATIONS[_BUFFER_COUNT] = {
        10.0    / 60.0,     // jump
        30.0    / 60.0,     // dive
        30.0    / 60.0,     // dash
        10.0    / 60.0,     // slide
        10.0    / 60.0,     // ledge jump
        15.0    / 60.0      // interact
    };

    const SDL_FRect FULL_COLLISION {0.0, 0.0, 8.0, 13.0};
    const SDL_FRect SHORT_COLLISION {0.0, 0.0, 8.0, 6.75};

    private:

        // objects
        const InputManager& _input;
        // TODO add jizz
        SDL_FRect _collision = FULL_COLLISION;
        SDL_FRect _ceilingCheck {0.0, 0.0, 8.0, 6.0};
        AnimatedSprite _sprite;

        // collision bools
        bool _pushingFloor = false;
        bool _wasPushingFloor = false;
        bool _closeToFloor = false;
        bool _pushingCeiling = false;
        bool _closeToCeiling = false;
        bool _pushingWall = false;

        // movement variables
        int _movementStateID = MOVEMENT_STATE_NORMAL;
        float _lastVerticalVelocity = 0.0;
        bool _quickClimb = false;
        bool _facingLeft = false;
        bool _shortCollision = false;
        Vector2 _ledgeTile {0.0, 0.0};

        // visual variables
        float _squishVelocity = 0.0;

        // upgrades                    ??rpbs^>
        unsigned char _upgradeBits = 0b00000000;
        float _currentDiveGravity = 0.0;
        bool _diveAvailable = true;
        bool _dashAvailable = true;


    public:
        Vector2 velocity {0.0, 0.0};
        Vector2 position {0.0, 0.0};

        Player(InputManager input, SDL_Renderer* renderer) :
        _input(input),
        _sprite(Animation(
            renderer,
            "content/textures/gameplay/player_styles/classic/run.png",
            16, 24, true
        ))
        { }

        void Process(float delta) override {
            _sprite.Process(delta);
        }

        void Draw(SDL_Renderer* renderer) const override {
            _sprite.Draw(renderer);
        }

        const InputManager& GetInput() const {
            return _input;
        }
};