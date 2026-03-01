#pragma once

#include <SDL.h>

class GameState {
  private:
	bool _running = true;
	bool _paused = false;
	unsigned long _frameStartMs = 0;
	unsigned long _frameEndMs = 0;

  public:
	const unsigned long frameDuration = 1000 / 60;

	GameState() : _frameStartMs(SDL_GetTicks64()), _frameEndMs(_frameStartMs + frameDuration) {}

	void SetRunning(bool value) { _running = value; }
	void Pause() { _paused = true; }
	void Unpause() {
		_paused = true;
		_frameEndMs = SDL_GetTicks64();
		_frameStartMs = _frameEndMs - frameDuration;
	}

	bool IsPaused() const { return _paused; }
	bool IsRunning() const { return _running; }
	const unsigned long& GetFrameStartMs() const { return _frameStartMs; }
	const unsigned long& GetFrameEndMs() const { return _frameEndMs; }
	void UpdateFrameStart() { _frameStartMs = _frameEndMs; }
	void UpdateFrameEnd() { _frameEndMs = SDL_GetTicks64(); }
};