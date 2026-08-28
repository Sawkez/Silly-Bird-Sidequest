#pragma once

#include <functional>

#include "engine/Vector2.hpp"
#include "engine/devconsole/variable/DevConsoleVariable.hpp"
#include "game/player/Player.hpp"

#define CONVAR_CATEGORY DRAW

class Camera {
   private:
	static inline const float TARGET_SPEED = 100.0;
	static inline const float TARGET_DIST = 0.5;
	static inline const float FOLLOW_SPEED = 60.0 / 15.0;
	static inline const float MIN_FOLLOW_SPEED = 10.0;
	static inline const float ZOOM_SPEED = 8.0;
	static inline const float ZOOM_SNAP = 0.0001;
	static inline const float FREE_CAM_SPEED = 200.0;

	SDL_Window* _window;
	SDL_Renderer* _renderer;

	const Player& _player;
	SDL_Point _pixelSize;
	std::reference_wrapper<const Room> _room;

#ifdef PLATFORM_HAS_CAMERA_PIXEL_TEXTURE
	SDL_Texture* _pixelTexture = nullptr;
	CONVAR(bool, _pixelate, PIXELATE, true, 0, "Render the game as low-resolution to align the pixels");
#endif
	CONVAR(bool, _pixelate, PIXELATE, false, 0, "Render the game as low-resolution to align the pixels");

	SDL_Point _pixelTextureSize{0, 0};

	Vector2 _roomStart;
	Vector2 _roomEnd;

	bool _shouldSnap;
	bool _zoomedOut = false;
	Vector2 _position;
	Vector2 _target;

	float _zoom = 1.0;

	unsigned int _pressedCallbackID;
	void PressedCallback() { GameState::GetInput().BlockInputs(); }

	unsigned int _releasedCallbackID;
	void ReleasedCallback() { GameState::GetInput().UnblockInputs(); }

	unsigned int _doubleTapCallbackID;
	void DoubleTapCallback() {
		_zoomedOut = !_zoomedOut;
		UpdateZoom(true);
	}

   public:
	Camera(SDL_Renderer* renderer, SDL_Window* window, const Player& player, const Room& room)
		: _renderer(renderer), _window(window), _player(player), _room(room), _pixelSize{240, 136} {
		UpdateZoom();
		InputManager& input = GameState::GetInput();

		_pressedCallbackID = input.AddPressedCallback(ACTION_CAMERA, [this]() { PressedCallback(); });
		_releasedCallbackID = input.AddReleasedCallback(ACTION_CAMERA, [this]() { ReleasedCallback(); });
		_doubleTapCallbackID = input.AddDoubleTapCallback(ACTION_CAMERA, [this]() { DoubleTapCallback(); });
	}

	void SetRoom(const Room& room) {
		_room = std::ref(room);
		UpdateZoom();
	}

	void UpdateZoom(bool smooth = false) {
		int windowWidth, windowHeight;
		SDL_GetWindowSize(_window, &windowWidth, &windowHeight);

		float aspect = float(windowWidth) / float(windowHeight);

		Vector2 targetRes = _zoomedOut ? _room.get().GetSize() : _room.get().GetTargetSize();
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

		Vector2 maxRes = _zoomedOut ? Vector2{float(windowWidth), float(windowHeight)}
									: Vector2{float(windowWidth) * 0.5f, float(windowHeight) * 0.5f};

		int xOld = _pixelSize.x;

		if (maxRes.LengthSquared() < targetRes.LengthSquared()) {
			_pixelSize.x = int(maxRes.x);
			_pixelSize.y = int(maxRes.y);
		}

		else {
			_pixelSize.x = int(targetRes.x);
			_pixelSize.y = int(targetRes.y);
		}

		if (smooth) _zoom = float(xOld) / float(_pixelSize.x);

		SDL_Point newTextureRes{Math::CeilPowerOfTwo(windowWidth + 16), Math::CeilPowerOfTwo(windowHeight + 16)};

		if (newTextureRes == _pixelTextureSize) {
			return;
		}

		_pixelTextureSize = newTextureRes;

#ifdef PLATFORM_HAS_CAMERA_PIXEL_TEXTURE
		if (_pixelTexture != nullptr) SDL_DestroyTexture(_pixelTexture);
		_pixelTexture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_ARGB1555, SDL_TEXTUREACCESS_TARGET,
										  _pixelTextureSize.x, _pixelTextureSize.y);
#endif
	}

	Vector2 GetGlobalCenter() const {
		Vector2 center;

		Vector2 roomStart = _room.get().GetPosition();
		Vector2 roomEnd = roomStart + _room.get().GetSize();

		roomStart -= _player.position;
		roomEnd -= _player.position;

		roomStart += Vector2(_pixelSize) * 0.5f * _zoom;
		roomEnd -= Vector2(_pixelSize) * 0.5f * _zoom;

		roomEnd.x = max(roomStart.x, roomEnd.x);
		roomEnd.y = max(roomStart.y, roomEnd.y);

		center.x = clamp(_position.x, roomStart.x, roomEnd.x);
		center.y = clamp(_position.y, roomStart.y, roomEnd.y);

		center += _player.position;
		if (_shouldSnap) {
			center.x = roundf(center.x);
			center.y = roundf(center.y);
		}

		return center;
	}

	void Process(float delta) {
		if (GameState::GetInput().IsDownNoBlock(ACTION_CAMERA)) {
			_target += GameState::GetInput().GetDirNoBlock() * FREE_CAM_SPEED * delta;

			Vector2 roomStart = _room.get().GetPosition();
			Vector2 roomEnd = roomStart + _room.get().GetSize();

			roomStart += Vector2(_pixelSize) * 0.5f * _zoom;
			roomEnd -= Vector2(_pixelSize) * 0.5f * _zoom;

			_target.x = clamp(_target.x, roomStart.x, roomEnd.x);
			_target.y = clamp(_target.y, roomStart.y, roomEnd.y);
		}

		else {
			_target.MoveToward(_player.position, TARGET_SPEED * delta);
			float targetDist = round(min(_pixelSize.x, _pixelSize.y) * TARGET_DIST);
			_shouldSnap = (_zoom == 1.0f && _target.PinLength(_player.position, targetDist));
		}

		Vector2 targetPlayerRelative = _target - _player.position;

		float dist = _position.Distance(targetPlayerRelative);
		_position.MoveToward(targetPlayerRelative, max(dist * FOLLOW_SPEED, MIN_FOLLOW_SPEED) * delta);

		// interpolating zoom toward 1.0
		_zoom = 1.0f + (_zoom - 1.0f) * expf(-ZOOM_SPEED * delta);
		if (abs(_zoom - 1.0f) < ZOOM_SNAP) _zoom = 1.0f;
	}

	void Draw(SDL_Renderer* renderer) const {
#ifdef PLATFORM_HAS_CAMERA_PIXEL_TEXTURE
		if (*_pixelate) {
			DrawPixelate(renderer);
			return;
		}
#endif

		DrawSharp(renderer);
	}

#ifdef PLATFORM_HAS_CAMERA_PIXEL_TEXTURE
	void DrawPixelate(SDL_Renderer* renderer) const {
		Vector2 center = GetGlobalCenter();
		Vector2 zoomedSize = Vector2(_pixelSize) * _zoom;
		Vector2 topLeft = center - zoomedSize * 0.5f;

		Vector2 texturePos{floor(topLeft.x / 8.0f) * 8.0f, floor(topLeft.y / 8.0f) * 8.0f};

		SDL_SetRenderTarget(renderer, _pixelTexture);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
		SDL_RenderClear(renderer);

		SDL_FRect visibilityRect{0.0f, 0.0f, zoomedSize.x + 16.0f, zoomedSize.y + 16.0f};

		_room.get().Draw(renderer, visibilityRect, -texturePos);

		_player.Draw(renderer, visibilityRect, -texturePos);

		SDL_FRect hdRenderSource{topLeft.x - texturePos.x, topLeft.y - texturePos.y, zoomedSize.x, zoomedSize.y};

		if (_shouldSnap) {
			hdRenderSource.x = roundf(hdRenderSource.x);
			hdRenderSource.y = roundf(hdRenderSource.y);
		}

		SDL_SetRenderTarget(renderer, nullptr);
		SDL_RenderTexture(renderer, _pixelTexture, &hdRenderSource, nullptr);
	}
#endif
	void DrawSharp(SDL_Renderer* renderer) const {
		Vector2 center = GetGlobalCenter();
		Vector2 zoomedSize = Vector2(_pixelSize) * _zoom;

		Vector2 topLeft = center - zoomedSize * 0.5f;

		SDL_FRect visibilityRect{0.0f, 0.0f, zoomedSize.x + 16.0f, zoomedSize.y + 16.0f};

		int winW, winH;
		SDL_GetRenderOutputSize(renderer, &winW, &winH);  // Assuming SDL3 since you used SDL_RenderTexture
		SDL_SetRenderScale(renderer, (float)winW / zoomedSize.x, (float)winH / zoomedSize.y);

		_room.get().Draw(renderer, visibilityRect, -topLeft);
		_player.Draw(renderer, visibilityRect, -topLeft);

		SDL_SetRenderScale(renderer, 1.0f, 1.0f);
	}

	~Camera() {
		GameState::GetInput().RemovePressedCallback(ACTION_CAMERA, _pressedCallbackID);
		GameState::GetInput().RemoveReleasedCallback(ACTION_CAMERA, _releasedCallbackID);
		GameState::GetInput().RemoveDoubleTapCallback(ACTION_CAMERA, _doubleTapCallbackID);

#ifdef PLATFORM_HAS_CAMERA_PIXEL_TEXTURE
		if (_pixelTexture != nullptr) SDL_DestroyTexture(_pixelTexture);
#endif
	}
};

#undef CONVAR_CATEGORY