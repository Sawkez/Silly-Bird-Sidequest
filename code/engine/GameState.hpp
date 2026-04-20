#pragma once

#include <SDL3/SDL.h>

#include <memory>

#include "engine/input/InputManager.hpp"
#include "engine/input/UIInputManager.hpp"

#if SDL_PLATFORM_PSP
#define INITIAL_WINDOW_RES 480, 272
#else
#define INITIAL_WINDOW_RES 960, 540
#endif

class GameState {
   private:
	static inline constexpr float MAX_DELTA = 1.0;

	static inline auto _input = InputManager();
	static inline bool _running = true;
	static inline bool _paused = true;
	static inline unsigned long _frameStartMs = 0;
	static inline unsigned long _frameEndMs = 0;
	static inline unsigned long _frameDuration = 16;
	static inline SDL_Window* _mainWindow = NULL;
	static inline SDL_Renderer* _mainRenderer = NULL;

   public:
	static void Init() {
		_mainWindow = SDL_CreateWindow("SBS", INITIAL_WINDOW_RES, SDL_WINDOW_RESIZABLE);
		_mainRenderer = SDL_CreateRenderer(_mainWindow, NULL);
		SDL_SetRenderVSync(_mainRenderer, 1);
		SDL_SetDefaultTextureScaleMode(_mainRenderer, SDL_SCALEMODE_NEAREST);
		SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");
	}

	static void SetRunning(bool value) { _running = value; }

	static void Pause() {
		_paused = true;
		UIInputManager::Reset();
	}

	static void Unpause() {
		_paused = false;
		_frameEndMs = SDL_GetTicks();
		_frameStartMs = _frameEndMs - _frameDuration;
		_input.ResetToState();
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

	static void UpdateFrameEnd() {
		_frameStartMs = _frameEndMs;
		_frameEndMs = SDL_GetTicks();
	}

	static bool ShouldProcess() { return (SDL_GetTicks() - _frameStartMs) > _frameDuration; }

	static void SetTargetFPS(float fps) { _frameDuration = 1000UL / fps; }

	static InputManager& GetInput() { return _input; }

	static SDL_Renderer* GetMainRenderer() { return _mainRenderer; }
	static SDL_Window* GetMainWindow() { return _mainWindow; }

	static void Deinit() {
		SDL_DestroyRenderer(_mainRenderer);
		SDL_DestroyWindow(_mainWindow);
		SDL_Quit();
	}
};