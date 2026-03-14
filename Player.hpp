#pragma once

#include <functional>
#include <vector>

#include "AnimatedSpriteOverlay.hpp"
#include "CollisionRect.hpp"
#include "CollisionResult.hpp"
#include "DiveParticle.hpp"
#include "IDrawableRect.hpp"
#include "IProcessable.hpp"
#include "InputManager.hpp"
#include "Jizz.hpp"
#include "ParticleSpawner.hpp"
#include "Room.hpp"
#include "Scarf.hpp"
#include "Vector2.hpp"
#include "WorldConstants.hpp"

// Player movement state functions are defined in headers included at the bottom
// of this one

using namespace std;

class Player : public IProcessable, public IDrawableRect {
	enum PlayerAnimation {
		ANIM_DUCK,
		ANIM_FLY,
		ANIM_IDLE,
		ANIM_JUMP,
		ANIM_LEDGE_FLIP,
		ANIM_LEDGE_UNFLIP,
		ANIM_RUN,
		ANIM_SLOW_RUN,
		ANIM_SLIDE,
		ANIM_TWERK_DOWN,
		ANIM_TWERK_UP
	};

	enum MovementStateID {
		MOVEMENT_STATE_NORMAL,
		MOVEMENT_STATE_LEDGE,
		MOVEMENT_STATE_DIVE,
		MOVEMENT_STATE_DASH,
		MOVEMENT_STATE_SLIDE,
		MOVEMENT_STATE_DUCK,
		MOVEMENT_STATE_DEAD,
		_MOVEMENT_STATE_COUNT
	};

	enum Upgrade { UPGRADE_DIVE, UPGRADE_DASH, UPGRADE_SLIDE, UPGRADE_DIVEBOOST, UPGRADE_POWERCORD, UPGRADE_REJUVENATOR };

	enum Timer {
		TIMER_COYOTE,
		TIMER_PLATFORM,
		TIMER_V_RESET,
		TIMER_GRAVITY_FREEZE,
		TIMER_BORED,
		TIMER_TWERK,
		TIMER_DIVE,
		TIMER_DASH,
		TIMER_SLIDE,
		_TIMER_COUNT
	};

	const float TIMER_DURATIONS[_TIMER_COUNT] = {
		10.0 / 60.0,  // coyote
		30.0 / 60.0,  // platform
		5.0 / 60.0,	  // v reset
		30.0 / 60.0,  // gravity freeze
		NAN,		  // bored (should be randomized)
		NAN,		  // twerk (use min & max)
		15.0 / 60.0,  // dive
		10.0 / 60.0,  // dash
		15.0 / 60.0	  // slide
	};

	enum Cooldown { COOLDOWN_LEDGE, COOLDOWN_SLIDE, COOLDOWN_INTERACT, _COOLDOWN_COUNT };

	const float COOLDOWN_DURATIONS[_COOLDOWN_COUNT] = {
		NAN,		  // ledge (use up & down)
		15.0 / 60.0,  // slide
		5.0 / 60.0	  // interact
	};

	enum Buffer { BUFFER_JUMP, BUFFER_DIVE, BUFFER_DASH, BUFFER_SLIDE, BUFFER_LEDGE_JUMP, BUFFER_INTERACT, _BUFFER_COUNT };

	const float BUFFER_DURATIONS[_BUFFER_COUNT] = {
		10.0 / 60.0,  // jump
		30.0 / 60.0,  // dive
		30.0 / 60.0,  // dash
		10.0 / 60.0,  // slide
		10.0 / 60.0,  // ledge jump
		15.0 / 60.0	  // interact
	};

	const Vector2 BODY_CENTER{8.0, 8.0};
	const Vector2 FEET_POS{8.0, 16.0};

	const float SQUISH_BASE_X_VELOCITY = 200.0;
	const float SQUISH_BASE_Y_VELOCITY = 250.0;
	const float SQUISH_ACCEL = 180.0;
	const float MAX_SQUISH_VELOCITY = 15.0;
	const float SQUISH_DAMPENING = pow(0.85, 60.0);
	const float X_SQUISH_MIN = 0.25;
	const float X_SQUISH_MAX = 1.5;
	const float Y_SQUISH_MAX = 1.25;

	const float V_RESET_GRAVITY = 1800.0;

	// const float FLOOR_RAY_LENGTH = 12.0;

	const CollisionRect FULL_COLLISION = CollisionRect(0.0, 0.0, 8.0, 13.0);
	const CollisionRect SHORT_COLLISION = CollisionRect(0.0, 0.0, 8.0, 6.75);
	const int COLLISION_ITERATIONS = 3;
	const float MIN_COLLISION_TIME = 0.1;
	const Vector2 COLLISION_OFFSET_FULL{-4.0, -13.0};
	const Vector2 COLLISION_OFFSET_SHORT{COLLISION_OFFSET_FULL.x, COLLISION_OFFSET_FULL.y + FULL_COLLISION.h - SHORT_COLLISION.h};
	const Vector2 FLOOR_CHECK_OFFSET{-4.0, 0.0};
	const Vector2 CEILING_CHECK_OFFSET = COLLISION_OFFSET_FULL;

	const float CEILING_DASH_VELOCITY = 200.0;

   private:
	// objects
	const InputManager& _input;
	Jizz _jizz;
	CollisionRect _collision = CollisionRect(FULL_COLLISION);
	CollisionRect _ceilingCheck{0.0, 0.0, 8.0, 6.0};
	CollisionRect _floorCheck{0.0, 0.0, 8.0, 12.0};
	reference_wrapper<Room> _room;
	AnimatedSpriteOverlay _sprite;
	Scarf _scarf;

	// particles
	ParticleSpawner<DiveParticle, 5> _diveParticles;

	// timers
	float _timers[_TIMER_COUNT]{};
	float _buffers[_BUFFER_COUNT]{};
	float _cooldowns[_COOLDOWN_COUNT]{};

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
	SDL_Point _ledgeTile{0, 0};

	// visual variables
	float _squishVelocity = 0.0;

	// upgrades                    ??rpbs^>
	unsigned char _upgradeBits = 0b00111111;
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

	using StateInitFunc = void (Player::*)();
	StateInitFunc _initFuncs[_MOVEMENT_STATE_COUNT]{&Player::NormalInit, &Player::LedgeInit, &Player::DiveInit, &Player::DashInit,
													&Player::SlideInit,	 &Player::DuckInit,	 &Player::DeadInit};

	using StateProcessFunc = void (Player::*)(float);
	StateProcessFunc _processFuncs[_MOVEMENT_STATE_COUNT]{&Player::NormalProcess, &Player::LedgeProcess, &Player::DiveProcess, &Player::DashProcess,
														  &Player::SlideProcess,  &Player::DuckProcess,	 &Player::DeadProcess};

	using StateDeinitFunc = void (Player::*)();
	StateDeinitFunc _deinitFuncs[_MOVEMENT_STATE_COUNT]{&Player::NormalDeinit, &Player::LedgeDeinit, &Player::DiveDeinit, &Player::DashDeinit,
														&Player::SlideDeinit,  &Player::DuckDeinit,	 &Player::DeadDeinit};

   public:
	Vector2 velocity{0.0, 0.0};
	Vector2 position{0.0, 0.0};

	Player(const InputManager& input, SDL_Renderer* renderer, Room& room)
		: _input(input),
		  _jizz("content/sidequest/skins/classic", renderer),
		  _room(room),
		  _scarf(position, room.GetColliders()),
		  _sprite(_jizz.GetAnimations(), _jizz.GetOverlayTextures(renderer), 255, 0, 0, BODY_CENTER - FEET_POS, FEET_POS, BODY_CENTER),
		  _diveParticles({-25.0, -25.0, 50.0, 50.0}, IMG_LoadTexture(renderer, "content/textures/particles/feather.png")) {}

	const InputManager& GetInput() const { return _input; }

	const CollisionRect& GetCollision() const { return _collision; }

	void SetState(int state) {
		(this->*_deinitFuncs[_movementStateID])();
		_movementStateID = state;
		(this->*_initFuncs[_movementStateID])();
	}

	void SetShortCollision(bool isShort) {
		_shortCollision = isShort;
		_collision = isShort ? SHORT_COLLISION : FULL_COLLISION;
	}

	void SetRoom(Room& room) {
		_room = ref(room);
		_scarf.SetColliders(room.GetColliders());
	}

	void SetTimer(int timer) {
		if (isnan(TIMER_DURATIONS[timer])) {
			cerr << "ERROR: timer " << timer << " duration is NAN" << endl;
		}
		_timers[timer] = TIMER_DURATIONS[timer];
	}

	void UnsetTimer(int timer) { _timers[timer] = 0.0; }

	bool TimerActive(int timer) const { return _timers[timer] > 0.0; }

	void Buffer(int buffer) {
		if (isnan(BUFFER_DURATIONS[buffer])) {
			cerr << "ERROR: buffer " << buffer << " duration is NAN" << endl;
		}
		_buffers[buffer] = BUFFER_DURATIONS[buffer];
	}

	void Unbuffer(int buffer) { _buffers[buffer] = 0.0; }

	bool BufferActive(int buffer) const { return _buffers[buffer] > 0.0; }

	bool UseBuffer(int buffer) {
		bool active = BufferActive(buffer);
		Unbuffer(buffer);
		return active;
	}

	void SetCooldown(int cooldown) { _cooldowns[cooldown] = COOLDOWN_DURATIONS[cooldown]; }

	void UnsetCooldown(int cooldown) { _cooldowns[cooldown] = 0.0; }

	bool CooldownActive(int cooldown) const { return _cooldowns[cooldown] > 0.0; }

	bool HasUpgrade(int upgrade) { return (_upgradeBits & (1 << upgrade)) > 0; }

	void Process(float delta) override {
		for (int i = 0; i < _TIMER_COUNT; i++) {
			_timers[i] -= delta;
		}

		for (int i = 0; i < _COOLDOWN_COUNT; i++) {
			_cooldowns[i] -= delta;
		}

		for (int i = 0; i < _BUFFER_COUNT; i++) {
			_buffers[i] -= delta;
		}

		if (_pushingFloor && !_wasPushingFloor) {
			_sprite.scale.x = X_SQUISH_MAX;
			ReloadDash();
			ReloadDive();
			_quickClimb = false;
		}

		// calling movement state function
		(this->*_processFuncs[_movementStateID])(delta);

		// conserving v velocity for ultraslides

		_lastVerticalVelocity = max(_lastVerticalVelocity, velocity.y);

		if (velocity.y > V_RESET_GRAVITY * delta) {
			SetTimer(TIMER_V_RESET);
		}

		if (!TimerActive(TIMER_V_RESET)) {
			_lastVerticalVelocity = 0.0;
		}

		// TODO moving platforms

		// TODO wallswap

		// TODO rejuvenation

		// moving and colliding
		_wasPushingFloor = _pushingFloor;
		_pushingFloor = false;
		_closeToFloor = false;
		_pushingWall = false;
		_pushingCeiling = false;
		_closeToCeiling = false;

		float timeLeft = 1.0;
		Vector2 collisionOffset = _shortCollision ? COLLISION_OFFSET_SHORT : COLLISION_OFFSET_FULL;

		for (int i = 0; i < COLLISION_ITERATIONS; i++) {
			_collision.x = position.x + collisionOffset.x;
			_collision.y = position.y + collisionOffset.y;

			CollisionResult firstCollision{};
			Vector2 frameVelocity = velocity * delta;

			for (const auto& collider : _room.get().GetColliders()) {
				CollisionResult newCollision = collider.SweptAABBCollision(_collision, frameVelocity);

				if (newCollision.depth < firstCollision.depth) {
					firstCollision = newCollision;
				}
			}

			float depth = std::min(firstCollision.depth, timeLeft);

			Vector2 keepDir{1.0f - abs(firstCollision.normal.x), 1.0f - abs(firstCollision.normal.y)};

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

		_ceilingCheck.x = position.x + CEILING_CHECK_OFFSET.x;
		_ceilingCheck.y = position.y + CEILING_CHECK_OFFSET.y;
		_floorCheck.x = position.x + FLOOR_CHECK_OFFSET.x;
		_floorCheck.y = position.y + FLOOR_CHECK_OFFSET.y;

		for (const auto& collider : _room.get().GetColliders()) {
			if (SDL_HasIntersectionF(&_ceilingCheck, &collider)) {
				_closeToCeiling = true;
				break;
			}
		}

		for (const auto& collider : _room.get().GetColliders()) {
			if (SDL_HasIntersectionF(&_floorCheck, &collider)) {
				_closeToFloor = true;
				break;
			}
		}

		// squish & stretch
		float targetSquish = 1.0;

		if (_movementStateID == MOVEMENT_STATE_NORMAL || _movementStateID == MOVEMENT_STATE_SLIDE) {
			float xAbsoluteVelocity = abs(velocity.x);
			float yAbsoluteVelocity = abs(velocity.y);

			float x = xAbsoluteVelocity / SQUISH_BASE_X_VELOCITY;
			float y = velocity.y == 0.0 ? 1.0 : SQUISH_BASE_Y_VELOCITY / yAbsoluteVelocity;

			if (xAbsoluteVelocity > SQUISH_BASE_X_VELOCITY || yAbsoluteVelocity > SQUISH_BASE_Y_VELOCITY) {
				targetSquish = x * y;
			}
		}

		float squishDist = targetSquish - _sprite.scale.x;
		_squishVelocity = clamp(_squishVelocity + SQUISH_ACCEL * squishDist * delta, -MAX_SQUISH_VELOCITY, MAX_SQUISH_VELOCITY);
		_squishVelocity *= powf(SQUISH_DAMPENING, delta);

		_sprite.scale.x = clamp(_sprite.scale.x + _squishVelocity * delta, X_SQUISH_MIN, X_SQUISH_MAX);
		_sprite.scale.y = min(1.0f / _sprite.scale.x, Y_SQUISH_MAX);

		// updating children
		_sprite.position = position;
		_sprite.Process(delta);

		_diveParticles.position = position;
		_diveParticles.position.y -= BODY_CENTER.y;
		_diveParticles.Process(delta);

		// TODO figure out why the offset
		Vector2 scarfPosition = _jizz.GetScarfPosition(_sprite.GetPlaybackPosition());	// + Vector2(-10.0, -32.0);
		scarfPosition += Vector2(-6.0, -24.0);
		_scarf.Pin(position - _sprite.TransformPoint(scarfPosition));
		_scarf.Process(delta);

		_sprite.SetOverlayColor(_scarf.GetColor());

		// cout << velocity << endl;
		//  cout << _input.GetDir() << endl;
		// cout << position << endl;
	}

	void Draw(SDL_Renderer* renderer, Vector2 drawOffset = {}) const {
		_diveParticles.Draw(renderer, drawOffset);
		_scarf.Draw(renderer, drawOffset);
		_sprite.Draw(renderer, drawOffset);
	}

	void FlipSprite(bool left) {
		if (_facingLeft == left) return;

		_sprite.scale.x = X_SQUISH_MIN;
		_facingLeft = left;
		_sprite.SetFlip(left ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
	}

	void UnloadDive() {
		_diveAvailable = false;
		_diveParticles.StartEmitting();
	}

	void ReloadDive() {
		_diveAvailable = true;
		_diveParticles.StopEmitting();
	}

	void UnloadDash() {
		if (!_dashAvailable) return;
		_dashAvailable = false;
		_scarf.Unload();
	}

	void ReloadDash() {
		if (_dashAvailable) return;
		_dashAvailable = true;
		_scarf.Load();
	}

	void CeilingDash() {
		UnsetTimer(TIMER_DASH);
		velocity.x += copysignf(CEILING_DASH_VELOCITY, velocity.x);
		SetTimer(TIMER_GRAVITY_FREEZE);
	}

	SDL_FRect GetRect() const override {
		SDL_FRect rect = _sprite.GetRect();
		SDL_FRect scarfRect = _scarf.GetRect();
		SDL_FRect diveParticleRect = _diveParticles.GetRect();
		SDL_UnionFRect(&rect, &scarfRect, &rect);
		SDL_UnionFRect(&rect, &diveParticleRect, &rect);
		return rect;
	}
};

#include "MovementStateDash.hpp"
#include "MovementStateDead.hpp"
#include "MovementStateDive.hpp"
#include "MovementStateDuck.hpp"
#include "MovementStateLedge.hpp"
#include "MovementStateNormal.hpp"
#include "MovementStateSlide.hpp"