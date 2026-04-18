#pragma once

#include <cmath>
#include <functional>

#include "engine/world/Room.hpp"
#include "game/player/Player.hpp"

using namespace std;

class RoomCamera : IProcessable {
	static const int SMALL_SCREEN_SIZE_X = 720;
	static const int SMALL_SCREEN_SIZE_Y = 405;
	static const int SMALL_SCREEN_MAX_TARGET_X = 240;
	static const int SMALL_SCREEN_MAX_TARGET_Y = 135;

   private:
	const Player& _player;
	reference_wrapper<const Room> _room;
	SDL_Window* _window;

	float _zoom = 1.0;
	Vector2 _position{0.0, 0.0};
	Vector2 _offset{0, 0};

   public:
	RoomCamera(const Player& player, const Room& room, SDL_Window* window) : _player(player), _room(ref(room)), _window(window) { UpdateZoom(); }

	void SetRoom(const Room& room) {
		_room = ref(room);
		UpdateZoom();
	}

	void UpdateZoom() {
		int windowWidth, windowHeight;
		SDL_GetWindowSize(_window, &windowWidth, &windowHeight);
		UpdateZoom(Vector2{float(windowWidth), float(windowHeight)});
	}

	void UpdateZoom(Vector2 windowRes) {
		Vector2 roomRes = _room.get().GetSize();
		Vector2 targetRes = _room.get().GetTargetSize();

		bool smallScreen = windowRes.x < SMALL_SCREEN_SIZE_X && windowRes.y < SMALL_SCREEN_SIZE_Y;
		bool bigTarget = targetRes.x > SMALL_SCREEN_MAX_TARGET_X && targetRes.y > SMALL_SCREEN_MAX_TARGET_Y;

		if (smallScreen && bigTarget) {
			targetRes = Vector2(SMALL_SCREEN_MAX_TARGET_X, SMALL_SCREEN_MAX_TARGET_Y);
		}

		bool taller = windowRes.x / windowRes.y < targetRes.x / targetRes.y;

		if (taller) {
			float yIdeal = min(targetRes.x * (windowRes.y / windowRes.x), roomRes.y);
			_zoom = windowRes.y / yIdeal;
		}

		else {
			float xIdeal = min(targetRes.y * (windowRes.x / windowRes.y), roomRes.x);
			_zoom = windowRes.x / xIdeal;
		}

		_offset = windowRes / _zoom * 0.5;
	}

	void Process(float delta) override {
		if (_player.GetInput().IsDown(ACTION_PAN_CAMERA)) {
			_position += _player.GetInput().GetDir() * 100.0 * delta;
		}

		else {
			_position = _player.position - _room.get().GetPosition();
		}
	}

	SDL_Point GetDrawOffset() const {
		Vector2 boundary = _room.get().GetSize() - _offset;

		SDL_Point drawOffset;

		if (boundary.x < _offset.x) {
			drawOffset.x = -_position.x;
		}

		else {
			drawOffset.x = -int(roundf(clamp(_position.x, _offset.x, boundary.x) - _offset.x));
		}

		if (boundary.y < _offset.y) {
			drawOffset.y = -_position.y;
		}

		else {
			drawOffset.y = -int(roundf(clamp(_position.y, _offset.y, boundary.y) - _offset.y));
		}

		return drawOffset;
	}

	SDL_Rect GetRect() const {
		int w = int(round(_offset.x * 2.0f));
		int h = int(round(_offset.y * 2.0f));

		Vector2 roomSize = _room.get().GetSize();

		float viewportX = clamp(_position.x, _offset.x, max(_offset.x, roomSize.x - _offset.x)) - _offset.x;
		float viewportY = clamp(_position.y, _offset.y, max(_offset.y, roomSize.y - _offset.y)) - _offset.y;

		return {int(round(viewportX)), int(round(viewportY)), w, h};
	}

	float GetZoom() const { return _zoom; }
};