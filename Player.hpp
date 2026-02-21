#pragma once

#include <vector>

#include "IDrawable.hpp"
#include "IProcessable.hpp"
#include "InputManager.hpp"
#include "Vector2.hpp"
#include "Animation.hpp"
#include "AnimatedSprite.hpp"
#include "CollisionResult.hpp"
#include "CollisionRect.hpp"

// Player movement state functions are defined in headers included at the bottom of this one

using namespace std;

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
        NAN,                // twerk (use min &Player:: max)
        15.0    / 60.0,     // dive
        10.0    / 60.0,     // dash
        15.0    / 60.0      // slide
    };

    enum Cooldown {
        COOLDOWN_LEDGE, COOLDOWN_SLIDE, COOLDOWN_INTERACT, _COOLDOWN_COUNT
    };

    const float COOLDOWN_DURATIONS[_COOLDOWN_COUNT] = {
        NAN,                // ledge (use up &Player:: down)
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

    const Vector2 BODY_CENTER {8.0, 8.0};
    const Vector2 FEET_POS {8.0, 16.0};

    const float FLOOR_RAY_LENGTH = 12.0;

    const CollisionRect FULL_COLLISION = CollisionRect(0.0, 0.0, 8.0, 13.0);
    const CollisionRect SHORT_COLLISION = CollisionRect(0.0, 0.0, 8.0, 6.75);
    const int COLLISION_ITERATIONS = 3;
    const float MIN_COLLISION_TIME = 0.1;
    const Vector2 COLLISION_OFFSET_FULL {-4.0, -13.0};
    const Vector2 COLLISION_OFFSET_SHORT {
        COLLISION_OFFSET_FULL.x,
        COLLISION_OFFSET_FULL.y + FULL_COLLISION.h - SHORT_COLLISION.h
    };

    private:

        // objects
        const InputManager& _input;
        // TODO add jizz
        CollisionRect _collision = CollisionRect(FULL_COLLISION);
        SDL_FRect _ceilingCheck {0.0, 0.0, 8.0, 6.0};
        const vector<CollisionRect>& _staticColliders;
        AnimatedSprite _sprite;

        // timers
        float _timers[_TIMER_COUNT] {};
        float _buffers[_BUFFER_COUNT] {};
        float _cooldowns[_COOLDOWN_COUNT] {};

        // collision info
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

        // movement state functions
        // implemented in MovementStateNormal.hpp, included at the bottom
        void NormalInit();
        void NormalProcess(float delta);
        void NormalDeinit();

        // implemented in MovementStateLedge.hpp, included at the bottom
        void LedgeInit();
        void LedgeProcess(float delta);
        void LedgeDeinit();

        // implemented in MovementStateDive.hpp, included at the bottom
        void DiveInit();
        void DiveProcess(float delta);
        void DiveDeinit();

        // implemented in MovementStateDash.hpp, included at the bottom
        void DashInit();
        void DashProcess(float delta);
        void DashDeinit();

        // implemented in MovementStateSlide.hpp, included at the bottom
        void SlideInit();
        void SlideProcess(float delta);
        void SlideDeinit();

        // implemented in MovementStateDuck.hpp, included at the bottom
        void DuckInit();
        void DuckProcess(float delta);
        void DuckDeinit();

        // implemented in MovementStateDead.hpp, included at the bottom
        void DeadInit();
        void DeadProcess(float delta);
        void DeadDeinit();

        using StateInitFunc = void(Player::*)();
        StateInitFunc _initFuncs[_MOVEMENT_STATE_COUNT] {&Player::NormalInit, &Player::LedgeInit, &Player::DiveInit, &Player::DashInit, &Player::SlideInit, &Player::DuckInit, &Player::DeadInit};

        using StateProcessFunc = void(Player::*)(float);
        StateProcessFunc _processFuncs[_MOVEMENT_STATE_COUNT] {&Player::NormalProcess, &Player::LedgeProcess, &Player::DiveProcess, &Player::DashProcess, &Player::SlideProcess, &Player::DuckProcess, &Player::DeadProcess};

        using StateDeinitFunc = void(Player::*)();
        StateDeinitFunc _deinitFuncs[_MOVEMENT_STATE_COUNT] {&Player::NormalDeinit, &Player::LedgeDeinit, &Player::DiveDeinit, &Player::DashDeinit, &Player::SlideDeinit, &Player::DuckDeinit, &Player::DeadDeinit};

    public:
        Vector2 velocity {0.0, 0.0};
        Vector2 position {0.0, 0.0};

        Player(InputManager& input, SDL_Renderer* renderer, vector<CollisionRect>& staticColliders) :
        _input(input), _staticColliders(staticColliders),
        _sprite(
            Animation(
                renderer,
                "content/textures/gameplay/player_styles/classic/run.png",
                16, 24, true
            ), BODY_CENTER - FEET_POS, FEET_POS, BODY_CENTER
        )
        { }

        const InputManager& GetInput() const {
            return _input;
        }

        void SetState(int state) {
            (this->*_deinitFuncs[_movementStateID])();
            _movementStateID = state;
            (this->*_initFuncs[_movementStateID])();
        }

        void SetShortCollision(bool isShort) {
            _shortCollision = isShort;
            _collision = isShort? SHORT_COLLISION : FULL_COLLISION;
        }

        void SetTimer(int timer) {
            if (isnanf(TIMER_DURATIONS[timer])) {
                cerr << "ERROR: timer " << timer << " duration is NAN" << endl;
            }
            _timers[timer] = TIMER_DURATIONS[timer];
        }

        void UnsetTimer(int timer) {
            _timers[timer] = 0.0;
        }

        bool TimerActive(int timer) const {
            return _timers[timer] > 0.0;
        }

        void Buffer(int buffer) {
            if (isnanf(BUFFER_DURATIONS[buffer])) {
                cerr << "ERROR: buffer " << buffer << " duration is NAN" << endl;
            }
            _buffers[buffer] = BUFFER_DURATIONS[buffer];
        }

        void Unbuffer(int buffer) {
            _buffers[buffer] = 0.0;
        }

        bool BufferActive(int buffer) const {
            return _buffers[buffer] > 0.0;
        }

        bool UseBuffer(int buffer) {
            bool active = BufferActive(buffer);
            Unbuffer(buffer);
            return active;
        }

        bool HasUpgrade(int upgrade) {
            return (_upgradeBits & (1 << upgrade)) > 0; 
        }

        bool CooldownActive(int cooldown) const {
            return _cooldowns[cooldown] > 0;
        }

        void Process(float delta) override {
            _input.GetDir();

            // calling movement state function
            (this->*_processFuncs[_movementStateID])(delta);

            // moving and colliding
            _wasPushingFloor = _pushingFloor;
            _pushingFloor = false;
            _closeToFloor = false;
            _pushingWall = false;
            _pushingCeiling = false;
            _closeToCeiling = false;

            float timeLeft = 1.0;
            Vector2 collisionOffset = _shortCollision? COLLISION_OFFSET_SHORT : COLLISION_OFFSET_FULL;

            for (int i = 0; i < COLLISION_ITERATIONS; i++) {
                _collision.x = position.x + collisionOffset.x;
                _collision.y = position.y + collisionOffset.y;

                CollisionResult firstCollision{ };
                Vector2 frameVelocity = velocity * delta;

                for (auto& collider: _staticColliders) {
                    CollisionResult newCollision = collider.SweptAABBCollision(_collision, frameVelocity);

                    if (newCollision.depth < firstCollision.depth) {
                        firstCollision = newCollision;
                    }
                }

                float depth = std::min(firstCollision.depth, timeLeft);

                Vector2 keepDir {
                    1.0f - abs(firstCollision.normal.x),
                    1.0f - abs(firstCollision.normal.y)
                };

                position += velocity * depth * delta;
                velocity *= keepDir;

                if (firstCollision.normal.y == -1.0) {
                    _pushingFloor = true;
                }

                else if (firstCollision.normal.y == 1.0) {
                    _pushingCeiling = true;
                }

                if (firstCollision.normal.x != 0.0) {
                    _pushingWall = true;
                }

                timeLeft -= depth;
                if (timeLeft < MIN_COLLISION_TIME) break;
            }

            // updating children
            _sprite.position = position;
            _sprite.Process(delta);

            cout << velocity << endl;
        }

        void Draw(SDL_Renderer* renderer) const override {
            _sprite.Draw(renderer);
            //_collision.Draw(renderer);
        }
};

#include "MovementStateNormal.hpp"
#include "MovementStateLedge.hpp"
#include "MovementStateDive.hpp"
#include "MovementStateDash.hpp"
#include "MovementStateSlide.hpp"
#include "MovementStateDuck.hpp"
#include "MovementStateDead.hpp"