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
};