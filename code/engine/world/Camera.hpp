#pragma once

#include <functional>

#include "engine/Vector2.hpp"
#include "game/player/Player.hpp"

class Camera {
   private:
	static inline const float SPEED_DISTANCE_MULT = 1.0 / 15.0 * 60.0;
	static inline const float PLAYER_RECT_MULT = 0.05;
	static inline const float SNAP_DIST = 16.0;

	const Player& _player;
	SDL_Point _pixelSize;
	std::reference_wrapper<const Room> _room;
	SDL_Texture* _pixelTexture;

	bool _shouldSnap;
	Vector2 _position;

   public:
	Camera(SDL_Renderer* renderer, const Player& player, const Room& room)
		: _player(player), _room(room), _pixelSize{240, 136} {
		_pixelTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB1555, SDL_TEXTUREACCESS_TARGET,
										  _pixelSize.x + 16, _pixelSize.y + 16);
	}

	void SetRoom(const Room& room) { _room = std::ref(room); }

	Vector2 GetTopLeft() const {
		Vector2 topLeft = _position - Vector2(_pixelSize) * 0.5;
		if (_shouldSnap) {
			topLeft.x = roundf(topLeft.x);
			topLeft.y = roundf(topLeft.y);
		}

		return topLeft;
	}

	void Process(float delta) {
		Vector2 target = _player.position;

		Vector2 roomStart = _room.get().GetPosition();
		Vector2 roomEnd = _room.get().GetSize() + roomStart;

		roomStart += Vector2(_pixelSize) * 0.5f;
		roomEnd -= Vector2(_pixelSize) * 0.5f;

		bool playerIsFast = abs(_player.velocity.x) > 125.0 || _player.velocity.y > 200.0;

		// clamping target to room bounds
		if (!playerIsFast) {
			target.x = clamp(target.x, roomStart.x, roomEnd.x);
			target.y = clamp(target.y, roomStart.y, roomEnd.y);
		}

		float dist = _position.Distance(target);
		_shouldSnap = dist > SNAP_DIST;

		_position.MoveToward(target, dist * SPEED_DISTANCE_MULT * delta);

		float playerRectMargin = max(_pixelSize.x * PLAYER_RECT_MULT, _pixelSize.y * PLAYER_RECT_MULT);
		Vector2 playerRect = Vector2(_pixelSize) * 0.5f - Vector2(playerRectMargin, playerRectMargin);

		// clamping position so player is guaranteed visible and out-of-bounds are guaranteed not
		if (playerIsFast) {
			float xMin = clamp(_player.position.x - playerRect.x, roomStart.x, roomEnd.x);
			float xMax = clamp(_player.position.x + playerRect.x, roomStart.x, roomEnd.x);

			float yMin = clamp(_player.position.y - playerRect.y, roomStart.y, roomEnd.y);
			float yMax = clamp(_player.position.y + playerRect.y, roomStart.y, roomEnd.y);

			_position.x = clamp(_position.x, xMin, xMax);
			_position.y = clamp(_position.y, yMin, yMax);
		} else {
			_position.x = clamp(_position.x, roomStart.x, roomEnd.x);
			_position.y = clamp(_position.y, roomStart.y, roomEnd.y);
		}
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