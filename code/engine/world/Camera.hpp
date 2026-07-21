#pragma once

#include <functional>

#include "engine/Vector2.hpp"
#include "game/player/Player.hpp"

class Camera {
   private:
	static inline const float TARGET_SPEED = 100.0f;
	static inline const float TARGET_DIST = 0.5f;
	static inline const float FOLLOW_SPEED = 60.0 / 15.0;
	static inline const float MIN_FOLLOW_SPEED = 10.0;

	SDL_Window* _window;
	SDL_Renderer* _renderer;

	const Player& _player;
	SDL_Point _pixelSize;
	std::reference_wrapper<const Room> _room;
	SDL_Texture* _pixelTexture = nullptr;
	SDL_Point _pixelTextureSize{0, 0};

	Vector2 _roomStart;
	Vector2 _roomEnd;

	bool _shouldSnap;
	Vector2 _position;
	Vector2 _target;

   public:
	Camera(SDL_Renderer* renderer, SDL_Window* window, const Player& player, const Room& room)
		: _renderer(renderer), _window(window), _player(player), _room(room), _pixelSize{240, 136} {
		UpdateZoom(false);
	}

	void SetRoom(const Room& room) {
		_room = std::ref(room);
		UpdateZoom(false);
	}

	void UpdateZoom(bool zoomedOut = false) {
		int windowWidth, windowHeight;
		SDL_GetWindowSize(_window, &windowWidth, &windowHeight);

		float aspect = float(windowWidth) / float(windowHeight);

		Vector2 targetRes = zoomedOut ? _room.get().GetSize() : _room.get().GetTargetSize();
		float targetAspect = targetRes.x / targetRes.y;

		Vector2 roomRes = _room.get().GetSize();

		if (aspect > targetAspect) {
			targetRes.x = targetRes.y * aspect;				  // grow target to match window aspect
			targetRes *= min(1.0f, roomRes.x / targetRes.x);  // shrink target to fit in room
		}

		else {
			targetRes.y = targetRes.x / aspect;				  // grow target to match window aspect
			targetRes *= min(1.0f, roomRes.y / targetRes.y);  // shrink target to fit in room
		}

		Vector2 maxRes = zoomedOut ? Vector2{float(windowWidth), float(windowHeight)}
								   : Vector2{float(windowWidth) * 0.5f, float(windowHeight) * 0.5f};

		if (maxRes.LengthSquared() < targetRes.LengthSquared()) {
			_pixelSize.x = int(maxRes.x);
			_pixelSize.y = int(maxRes.y);
		}

		else {
			_pixelSize.x = int(targetRes.x);
			_pixelSize.y = int(targetRes.y);
		}

		SDL_Point newTextureRes{Math::CeilPowerOfTwo(_pixelSize.x), Math::CeilPowerOfTwo(_pixelSize.y)};

		if (newTextureRes == _pixelTextureSize) {
			return;
		}

		_pixelTextureSize = newTextureRes;

		if (_pixelTexture != nullptr) SDL_DestroyTexture(_pixelTexture);
		_pixelTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB1555, SDL_TEXTUREACCESS_TARGET,
										  _pixelTextureSize.x, _pixelTextureSize.y);
	}

	Vector2 GetTopLeft() const {
		Vector2 topLeft;

		Vector2 roomStart = _room.get().GetPosition();
		Vector2 roomEnd = roomStart + _room.get().GetSize();

		roomStart -= _player.position;
		roomEnd -= _player.position;

		roomStart += Vector2(_pixelSize) * 0.5f;
		roomEnd -= Vector2(_pixelSize) * 0.5f;

		roomEnd.x = max(roomStart.x, roomEnd.x);
		roomEnd.y = max(roomStart.y, roomEnd.y);

		topLeft.x = clamp(_position.x, roomStart.x, roomEnd.x);
		topLeft.y = clamp(_position.y, roomStart.y, roomEnd.y);

		topLeft += _player.position - Vector2(_pixelSize) * 0.5;
		if (_shouldSnap) {
			topLeft.x = roundf(topLeft.x);
			topLeft.y = roundf(topLeft.y);
		}

		return topLeft;
	}

	void Process(float delta) {
		_target.MoveToward(_player.position, TARGET_SPEED * delta);
		float targetDist = min(_pixelSize.x, _pixelSize.y) * TARGET_DIST;
		_shouldSnap = _target.PinLength(_player.position, targetDist);

		Vector2 targetPlayerRelative = _target - _player.position;

		float dist = _position.Distance(targetPlayerRelative);
		_position.MoveToward(targetPlayerRelative, max(dist * FOLLOW_SPEED, MIN_FOLLOW_SPEED) * delta);
	}

	void Draw(SDL_Renderer* renderer) const {
		Vector2 topLeft = GetTopLeft();

		Vector2 texturePos{floor(topLeft.x / 8.0f) * 8.0f, floor(topLeft.y / 8.0f) * 8.0f};

		SDL_SetRenderTarget(renderer, _pixelTexture);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		SDL_RenderClear(renderer);

		SDL_FRect visibilityRect{0.0f, 0.0f, float(_pixelSize.x) + 16.0f, float(_pixelSize.y) + 16.0f};

		_room.get().Draw(renderer, visibilityRect, -texturePos);

		_player.Draw(renderer, visibilityRect, -texturePos);

		SDL_FRect hdRenderSource{topLeft.x - texturePos.x, topLeft.y - texturePos.y, float(_pixelSize.x),
								 float(_pixelSize.y)};

		if (_shouldSnap) {
			hdRenderSource.x = roundf(hdRenderSource.x);
			hdRenderSource.y = roundf(hdRenderSource.y);
		}

		SDL_SetRenderTarget(renderer, nullptr);
		SDL_RenderTexture(renderer, _pixelTexture, &hdRenderSource, nullptr);
	}
};