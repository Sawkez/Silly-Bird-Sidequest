#pragma once

#include <functional>
#include <vector>

#include "engine/IProcessable.hpp"
#include "engine/Vector2.hpp"
#include "engine/graphics/AnimatedSpriteOverlay.hpp"
#include "engine/graphics/IDrawableRect.hpp"
#include "engine/graphics/ParticleSpawner.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/physics/CollisionRect.hpp"
#include "engine/physics/CollisionResult.hpp"
#include "engine/world/Room.hpp"
#include "engine/world/WorldConstants.hpp"
#include "game/player/IPlayer.hpp"
#include "game/player/graphics/DiveParticle.hpp"
#include "game/player/graphics/Jizz.hpp"
#include "game/player/graphics/Scarf.hpp"
#include "game/player/movement/IMovementState.hpp"

using namespace std;

class Player : public IPlayer {
   public:
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
		ANIM_TWERK_UP,
		ANIM_WALLRUN,
		_ANIM_COUNT
	};

	enum MovementStateID {
		MOVEMENT_STATE_NORMAL,
		MOVEMENT_STATE_LEDGE,
		MOVEMENT_STATE_DIVE,
		MOVEMENT_STATE_DASH,
		MOVEMENT_STATE_SLIDE,
		MOVEMENT_STATE_DUCK,
		MOVEMENT_STATE_DEAD,
		MOVEMENT_STATE_WALLRUN,
		_MOVEMENT_STATE_COUNT
	};

	enum Upgrade { UPGRADE_DIVE, UPGRADE_DASH, UPGRADE_SLIDE, UPGRADE_DIVEBOOST, UPGRADE_REJUVENATOR, UPGRADE_WALLRUN, _UPGRADE_COUNT };

   public:
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

   private:
	static inline constexpr float TIMER_DURATIONS[_TIMER_COUNT] = {
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

   public:
	enum Cooldown { COOLDOWN_LEDGE, COOLDOWN_SLIDE, COOLDOWN_INTERACT, COOLDOWN_WALLRUN, _COOLDOWN_COUNT };

   private:
	static inline constexpr float COOLDOWN_DURATIONS[_COOLDOWN_COUNT] = {
		NAN,		  // ledge (use up & down)
		15.0 / 60.0,  // slide
		5.0 / 60.0,	  // interact
		15.0 / 60.0	  // wallrun
	};

   public:
	enum Buffer { BUFFER_JUMP, BUFFER_DIVE, BUFFER_DASH, BUFFER_SLIDE, BUFFER_LEDGE_JUMP, BUFFER_INTERACT, BUFFER_WALLJUMP, _BUFFER_COUNT };

   private:
	static inline constexpr float BUFFER_DURATIONS[_BUFFER_COUNT] = {
		10.0 / 60.0,  // jump
		30.0 / 60.0,  // dive
		30.0 / 60.0,  // dash
		10.0 / 60.0,  // slide
		10.0 / 60.0,  // ledge jump
		15.0 / 60.0,  // interact
		30.0 / 60.0	  // wall jump
	};

   public:
	static inline const Vector2 BODY_CENTER{8.0, 8.0};
	static inline const Vector2 FEET_POS{8.0, 16.0};

	static inline constexpr float SQUISH_BASE_X_VELOCITY = 200.0;
	static inline constexpr float SQUISH_BASE_Y_VELOCITY = 250.0;
	static inline constexpr float SQUISH_ACCEL = 180.0;
	static inline constexpr float MAX_SQUISH_VELOCITY = 15.0;
	static inline constexpr float MIN_SQUISH_VELOCITY = 0.25;
	static inline constexpr float SQUISH_DAMPENING = 0.000058228f;	// 0.85 ^ 60.0
	static inline constexpr float X_SQUISH_MIN = 0.25;
	static inline constexpr float X_SQUISH_MAX = 1.5;
	static inline constexpr float X_SQUISH_RESET = 0.075;
	static inline constexpr float Y_SQUISH_MAX = 1.25;

	static inline constexpr float LEDGE_CHECK_OFFSET_LEFT = -1.2;
	static inline constexpr float LEDGE_CHECK_OFFSET_RIGHT = 0.8;
	static inline constexpr float LEDGE_CHECK_OFFSET_UP = -1.5;

   private:
	static inline constexpr float V_RESET_GRAVITY = 1800.0;

	// static inline constexpr float FLOOR_RAY_LENGTH = 12.0;

	static inline const CollisionRect FULL_COLLISION = CollisionRect(0.0, 0.0, 8.0, 13.0);
	static inline const CollisionRect SHORT_COLLISION = CollisionRect(0.0, 0.0, 8.0, 6.75);
	static inline constexpr int COLLISION_ITERATIONS = 3;
	static inline constexpr float MIN_COLLISION_TIME = 0.1;
	static inline const Vector2 COLLISION_OFFSET_FULL{-4.0, -13.0};
	static inline const Vector2 COLLISION_OFFSET_SHORT{COLLISION_OFFSET_FULL.x, COLLISION_OFFSET_FULL.y + FULL_COLLISION.h - SHORT_COLLISION.h};
	static inline const Vector2 FLOOR_CHECK_OFFSET{-4.0, 0.0};
	static inline const Vector2 CEILING_CHECK_OFFSET = COLLISION_OFFSET_FULL;

	static inline constexpr float CEILING_DASH_VELOCITY = 200.0;

	// objects
	const InputManager& _input;
	Jizz _jizz;
	CollisionRect _collision = CollisionRect(FULL_COLLISION);
	CollisionRect _ceilingCheck{0.0, 0.0, 8.0, 6.0};
	CollisionRect _floorCheck{0.0, 0.0, 8.0, 12.0};
	reference_wrapper<Room> _room;
	AnimatedSpriteOverlay _sprite;
	Scarf _scarf;
	static const IMovementState* _movementStates[_MOVEMENT_STATE_COUNT];

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
	float _lastDownVelocity = 0.0;
	bool _quickClimb = false;
	bool _facingLeft = false;
	bool _shortCollision = false;
	SDL_Point _ledgeTile{0, 0};

	// visual variables
	float _squishVelocity = 0.0;

	// upgrades            ??wrbs^>
	Uint8 _upgradeBits = 0b00111111;
	float _currentDiveGravity = 0.0;
	bool _diveAvailable = true;
	bool _dashAvailable = true;

	// miscellaneous
	Vector2 _respawnPosition{0.0, 0.0};

   public:
	Vector2 velocity{0.0, 0.0};
	Vector2 position{0.0, 0.0};

	Player(const InputManager& input, SDL_Renderer* renderer, Room& room, Uint8 upgrades)
		: _input(input),
		  _jizz("content/sidequest/skins/classic", renderer),
		  _room(room),
		  _scarf(position, room.GetColliders()),
		  _sprite(_jizz.GetAnimations(), _jizz.GetOverlayTextures(renderer), 255, 0, 0, BODY_CENTER - FEET_POS, FEET_POS, BODY_CENTER),
		  _diveParticles({-2500.0, -2500.0, 5000.0, 5000.0}, IMG_LoadTexture(renderer, "content/textures/particles/feather.png")),
		  _upgradeBits(upgrades) {}

	const InputManager& GetInput() const override { return _input; }

	const CollisionRect& GetCollision() const override { return _collision; }

	Vector2 GetCollisionOffset() const override { return _shortCollision ? COLLISION_OFFSET_SHORT : COLLISION_OFFSET_FULL; }

	void SetState(int state) override {
		_movementStates[_movementStateID]->Deinit(*this);
		_movementStateID = state;
		_movementStates[_movementStateID]->Init(*this);
	}

	void SetShortCollision(bool isShort) override {
		_shortCollision = isShort;
		_collision = isShort ? SHORT_COLLISION : FULL_COLLISION;
	}

	void SetRoom(Room& room) override {
		_room = ref(room);
		_scarf.SetColliders(room.GetColliders());
	}

	void SetTimer(int timer, float time) override { _timers[timer] = time; }

	void SetTimer(int timer) override {
		if (isnan(TIMER_DURATIONS[timer])) {
			cerr << "ERROR: timer " << timer << " duration is NAN" << endl;
		}
		SetTimer(timer, TIMER_DURATIONS[timer]);
	}

	void UnsetTimer(int timer) override { _timers[timer] = 0.0; }

	bool TimerActive(int timer) const override { return _timers[timer] > 0.0; }

	float GetTimer(int timer) const override { return _timers[timer]; }

	void Buffer(int buffer) override {
		if (isnan(BUFFER_DURATIONS[buffer])) {
			cerr << "ERROR: buffer " << buffer << " duration is NAN" << endl;
		}
		_buffers[buffer] = BUFFER_DURATIONS[buffer];
	}

	void Unbuffer(int buffer) override { _buffers[buffer] = 0.0; }

	bool BufferActive(int buffer) const override { return _buffers[buffer] > 0.0; }

	float GetBuffer(int buffer) const override { return _buffers[buffer]; }

	bool UseBuffer(int buffer) override {
		bool active = BufferActive(buffer);
		Unbuffer(buffer);
		return active;
	}

	void SetCooldown(int cooldown, float time) override { _cooldowns[cooldown] = time; }

	void SetCooldown(int cooldown) override { SetCooldown(cooldown, COOLDOWN_DURATIONS[cooldown]); }

	void UnsetCooldown(int cooldown) override { _cooldowns[cooldown] = 0.0; }

	bool CooldownActive(int cooldown) const override { return _cooldowns[cooldown] > 0.0; }

	bool HasUpgrade(int upgrade) const override { return (_upgradeBits & (1 << upgrade)) > 0; }

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
		_movementStates[_movementStateID]->Process(*this, delta);

		// conserving down velocity for ultraslides
		_lastDownVelocity = max(_lastDownVelocity, velocity.y);

		if (velocity.y > V_RESET_GRAVITY * delta) {
			SetTimer(TIMER_V_RESET);
		}

		if (!TimerActive(TIMER_V_RESET)) {
			_lastDownVelocity = 0.0;
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
		Vector2 collisionOffset = GetCollisionOffset();

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

		// dying horribly to spikes
		for (const auto& spike : GetSpikeColliders()) {
			if (spike.HasIntersection(_collision)) {
				SetState(MOVEMENT_STATE_DEAD);
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

		if (abs(targetSquish - _sprite.scale.x) < X_SQUISH_RESET && abs(_squishVelocity) < MIN_SQUISH_VELOCITY) {
			_sprite.scale.x = targetSquish;
			_squishVelocity = 0.0;
		}

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

	bool Draw(SDL_Renderer* renderer, const SDL_FRect& drawTargetRect, Vector2 drawOffset = {}) const override {
		bool parts = _diveParticles.Draw(renderer, drawTargetRect, drawOffset);
		bool scarf = _scarf.Draw(renderer, drawTargetRect, drawOffset);
		bool sprite = _sprite.Draw(renderer, drawTargetRect, drawOffset);

		// _collision.Draw(renderer, drawTargetRect, drawOffset);

		return parts || scarf || sprite;
	}

	void FlipSprite(bool left) override {
		if (_facingLeft == left) return;

		_sprite.scale.x = X_SQUISH_MIN;
		_facingLeft = left;
		_sprite.SetFlip(left ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
	}

	void UnloadDive() override {
		_diveAvailable = false;
		_diveParticles.StartEmitting();
	}

	void ReloadDive(bool invisible = false) override {
		_diveAvailable = true;
		_diveParticles.StopEmitting();
	}

	void UnloadDash() override {
		if (!_dashAvailable) return;
		_dashAvailable = false;
		_scarf.Unload();
	}

	void ReloadDash() override {
		if (_dashAvailable) return;
		_dashAvailable = true;
		_scarf.Load();
	}

	void CeilingDash() override {
		UnsetTimer(TIMER_DASH);
		velocity.x += copysignf(CEILING_DASH_VELOCITY, velocity.x);
		SetTimer(TIMER_GRAVITY_FREEZE);
	}

	bool IsShortCollision() const override { return _shortCollision; }
	bool IsCloseToCeiling() const override { return _closeToCeiling; }
	bool IsPushingCeiling() const override { return _pushingCeiling; }
	bool IsCloseToFloor() const override { return _closeToFloor; }
	bool IsPushingFloor() const override { return _pushingFloor; }
	bool IsPushingWall() const override { return _pushingWall; }

	bool IsDashAvailable() const override { return _dashAvailable; }
	bool IsDiveAvailable() const override { return _diveAvailable; }

	bool IsQuickClimbActive() const override { return _quickClimb; }
	void EnableQuickClimb() override { _quickClimb = true; }
	void DisableQuickClimb() override { _quickClimb = false; }

	bool IsFacingLeft() const override { return _facingLeft; }

	float GetSquish() const override { return _sprite.scale.x; }
	void SetSquish(float squish) override { _sprite.scale.x = squish; }

	void PlayAnimation(int animation, float speed = 1.0) override { _sprite.Play(animation, speed); }
	void PlayAnimationFromStart(int animation, float speed = 1.0) override { _sprite.PlayFromStart(animation, speed); }
	void PlayAnimationLastFrame(int animation, float speed = 1.0) override { _sprite.PlayLastFrame(animation, speed); }

	const Room& GetRoom() const override { return _room; }

	void SetLedgeTile(const SDL_Point& tile) override { _ledgeTile = tile; }
	void SetLedgeTile(int x, int y) override { _ledgeTile = SDL_Point{x, y}; }
	const SDL_Point& GetLedgeTile() const override { return _ledgeTile; }

	float GetCurrentDiveGravity() const override { return _currentDiveGravity; }
	void SetCurrentDiveGravity(float gravity) override { _currentDiveGravity = gravity; }

	void SetSpriteRotationRadians(float radians) override { _sprite.SetRotationRadians(radians); }
	void SetSpriteRotationDegrees(float degrees) override { _sprite.SetRotationDegrees(degrees); }

	float GetLastDownVelocity() const override { return _lastDownVelocity; }
	void ResetLastDownVelocity() override { _lastDownVelocity = 0.0; }

	void UpdateLedgeTile() override {
		float tileX = roundf(position.x / WorldConstants::TILE_SIZE_F + (IsFacingLeft() ? LEDGE_CHECK_OFFSET_LEFT : LEDGE_CHECK_OFFSET_RIGHT));
		float tileY = roundf(position.y / WorldConstants::TILE_SIZE_F + LEDGE_CHECK_OFFSET_UP);

		_ledgeTile.x = static_cast<int>(tileX) * WorldConstants::TILE_SIZE;
		_ledgeTile.y = static_cast<int>(tileY) * WorldConstants::TILE_SIZE;
	}

	bool CanGrabLedge() const override {
		if (IsPushingFloor()) return false;
		if (!IsQuickClimbActive() && velocity.y <= 0.0) return false;
		if (CooldownActive(COOLDOWN_LEDGE)) return false;
		if (GetInput().GetDir().y == 1.0) return false;

		const vector<SDL_Point>& ledges = GetRoom().GetLedges();

		return std::find(ledges.begin(), ledges.end(), _ledgeTile) != ledges.end();
	}

	const vector<CollisionRect>& GetStaticColliders() const override { return _room.get().GetColliders(); }
	const vector<SpikeCollider>& GetSpikeColliders() const override { return _room.get().GetSpikeColliders(); }

	void Respawn() override {
		position = _respawnPosition;
		velocity = Vector2();
		ReloadDash();
		ReloadDive();
	}
	void SetRespawnPosition(Vector2 respawnPosition) override { _respawnPosition = respawnPosition; }

	void PushOutOfColliders() override {
		Vector2 collisionOffset = GetCollisionOffset();

		for (const auto& collider : GetStaticColliders()) {
			Vector2 push = collider.PushOut(_collision);
			position += push;
		}
	}
};