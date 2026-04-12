#pragma once

#include <vector>

#include "engine/IProcessable.hpp"
#include "engine/Vector2.hpp"
#include "engine/graphics/IDrawableRect.hpp"

class InputManager;
class CollisionRect;
class Room;
class SDL_Point;
class SpikeCollider;

class IPlayer : public IDrawableRect, public IProcessable {
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

	enum Cooldown { COOLDOWN_LEDGE, COOLDOWN_SLIDE, COOLDOWN_INTERACT, COOLDOWN_WALLRUN, _COOLDOWN_COUNT };
	enum Buffer { BUFFER_JUMP, BUFFER_DIVE, BUFFER_DASH, BUFFER_SLIDE, BUFFER_LEDGE_JUMP, BUFFER_INTERACT, BUFFER_WALLJUMP, _BUFFER_COUNT };

	Vector2 position;
	Vector2 velocity;

	virtual const InputManager& GetInput() const = 0;
	virtual const CollisionRect& GetCollision() const = 0;
	virtual Vector2 GetCollisionOffset() const = 0;
	virtual void SetState(int state) = 0;
	virtual void SetShortCollision(bool isShort) = 0;
	virtual void SetRoom(Room& room) = 0;
	virtual void SetTimer(int timer) = 0;
	virtual void SetTimer(int timer, float time) = 0;
	virtual void UnsetTimer(int timer) = 0;
	virtual bool TimerActive(int timer) const = 0;
	virtual float GetTimer(int timer) const = 0;
	virtual void Buffer(int buffer) = 0;
	virtual void Unbuffer(int buffer) = 0;
	virtual bool BufferActive(int buffer) const = 0;
	virtual float GetBuffer(int buffer) const = 0;
	virtual bool UseBuffer(int buffer) = 0;
	virtual void SetCooldown(int cooldown) = 0;
	virtual void SetCooldown(int cooldown, float time) = 0;
	virtual void UnsetCooldown(int cooldown) = 0;
	virtual bool CooldownActive(int cooldown) const = 0;
	virtual bool HasUpgrade(int upgrade) const = 0;
	virtual void GiveUpgrade(int upgrade) = 0;
	virtual void FlipSprite(bool left) = 0;
	virtual void UnloadDive() = 0;
	virtual void ReloadDive(bool invisible = false) = 0;
	virtual void UnloadDash() = 0;
	virtual void ReloadDash() = 0;
	virtual void CeilingDash() = 0;
	virtual bool IsShortCollision() const = 0;
	virtual bool IsCloseToCeiling() const = 0;
	virtual bool IsPushingCeiling() const = 0;
	virtual bool IsCloseToFloor() const = 0;
	virtual bool IsPushingFloor() const = 0;
	virtual bool IsPushingWall() const = 0;
	virtual bool IsDashAvailable() const = 0;
	virtual bool IsDiveAvailable() const = 0;
	virtual bool IsQuickClimbActive() const = 0;
	virtual void EnableQuickClimb() = 0;
	virtual void DisableQuickClimb() = 0;
	virtual bool IsFacingLeft() const = 0;
	virtual float GetSquish() const = 0;
	virtual void SetSquish(float squish) = 0;
	virtual void PlayAnimation(int animation, float speed) = 0;
	virtual void PlayAnimationFromStart(int animation, float speed) = 0;
	virtual void PlayAnimationLastFrame(int animation, float speed) = 0;
	virtual const Room& GetRoom() const = 0;
	virtual void SetLedgeTile(const SDL_Point& tile) = 0;
	virtual void SetLedgeTile(int x, int y) = 0;
	virtual const SDL_Point& GetLedgeTile() const = 0;
	virtual float GetCurrentDiveGravity() const = 0;
	virtual void SetCurrentDiveGravity(float gravity) = 0;
	virtual void SetSpriteRotationRadians(float radians) = 0;
	virtual void SetSpriteRotationDegrees(float degress) = 0;
	virtual float GetLastDownVelocity() const = 0;
	virtual void ResetLastDownVelocity() = 0;
	virtual void UpdateLedgeTile() = 0;
	virtual bool CanGrabLedge() const = 0;
	virtual const std::vector<CollisionRect>& GetStaticColliders() const = 0;
	virtual const std::vector<SpikeCollider>& GetSpikeColliders() const = 0;
	virtual void Respawn() = 0;
	virtual void SetRespawnPosition(Vector2 respawnPosition) = 0;
	virtual void PushOutOfColliders() = 0;
	virtual void ShowScarf() = 0;
	virtual void HideScarf() = 0;
};