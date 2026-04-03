#pragma once

#include <SDL.h>

class GameState {
   private:
	static inline constexpr float MAX_DELTA = 1.0;

	static inline bool _running = true;
	static inline bool _paused = false;
	static inline unsigned long _frameStartMs = 0;
	static inline unsigned long _frameEndMs = 0;
	static inline unsigned long _frameDuration = 16;

   public:
	static void SetRunning(bool value) { _running = value; }
	static void Pause() { _paused = true; }
	static void Unpause() {
		_paused = true;
		_frameEndMs = SDL_GetTicks64();
		_frameStartMs = _frameEndMs - _frameDuration;
	}

	static bool IsPaused() { return _paused; }
	static bool IsRunning() { return _running; }
	static const unsigned long& GetFrameStartMs() { return _frameStartMs; }
	static const unsigned long& GetFrameEndMs() { return _frameEndMs; }
	static const unsigned long& GetFrameDuration() { return _frameDuration; }

	static float GetDelta() {
		float delta = (_frameEndMs - _frameStartMs) / 1000.0f;
		return delta < MAX_DELTA ? delta : _frameDuration / 1000.0f;
	}

	static void UpdateFrameStart() { _frameStartMs = _frameEndMs; }
	static void UpdateFrameEnd() { _frameEndMs = SDL_GetTicks64(); }

	static void SetTargetFPS(float fps) { _frameDuration = 1000UL / fps; }
};