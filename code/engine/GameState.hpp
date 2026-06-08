#pragma once

#include <SDL3/SDL.h>

#include <memory>

#include "engine/PlatformDefines.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/input/UIInputManager.hpp"
#include "engine/input/touch/TouchController.hpp"

#if SDL_PLATFORM_PSP
#define INITIAL_WINDOW_RES 480, 272
#else
#define INITIAL_WINDOW_RES 960, 540
#endif

#if SDL_PLATFORM_ANDROID
#define WINDOWFLAGS SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN
#else
#define WINDOWFLAGS SDL_WINDOW_RESIZABLE
#endif

class GameState {
   private:
	static inline constexpr float MAX_DELTA = 1.0;

	static inline auto _input = InputManager();
#ifdef PLATFORM_HAS_TOUCH
	static inline auto _touch = TouchController(_input);
#endif
	static inline bool _running = true;
	static inline bool _paused = true;
	static inline unsigned long _frameStartMs = 0;
	static inline unsigned long _frameEndMs = 0;
	static inline unsigned long _frameDuration = 16;
	static inline SDL_Window* _mainWindow = NULL;
	static inline SDL_Renderer* _mainRenderer = NULL;
	static inline bool _takingInput = false;
	static inline bool (*_keyboardInputCallback)(const SDL_Event& event) = nullptr;

   public:
	static void Init() {
		std::cout << "Welcome to Silly Bird Sidequest!" << std::endl;
		_mainWindow = SDL_CreateWindow("SBS", INITIAL_WINDOW_RES, WINDOWFLAGS);

		std::cout << "Available render drivers: ";

		int driverCount = SDL_GetNumRenderDrivers();
		for (int i = 0; i < driverCount; i++) {
			std::cout << SDL_GetRenderDriver(i) << "; ";
		}

		_mainRenderer = SDL_CreateRenderer(_mainWindow, NULL);

		std::cout << std::endl << "Using driver: " << SDL_GetRendererName(_mainRenderer) << std::endl << std::endl;

		SDL_SetRenderVSync(_mainRenderer, 1);
		SDL_SetDefaultTextureScaleMode(_mainRenderer, SDL_SCALEMODE_NEAREST);
		SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft LandscapeRight");

#ifdef PLATFORM_HAS_TOUCH
		_touch.Init(_mainRenderer, _mainWindow);
#endif
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
#ifdef PLATFORM_HAS_TOUCH
	static TouchController& GetTouch() { return _touch; }
#endif

	static SDL_Renderer* GetMainRenderer() { return _mainRenderer; }
	static SDL_Window* GetMainWindow() { return _mainWindow; }

	static void Deinit() {
		SDL_DestroyRenderer(_mainRenderer);
		SDL_DestroyWindow(_mainWindow);
		SDL_Quit();
	}

	static void StartTextInput(bool (*callback)(const SDL_Event& event)) {
		_takingInput = true;
		_keyboardInputCallback = callback;
		SDL_StartTextInput(_mainWindow);
	}

	static bool HandleEvent(const SDL_Event& event) {
		if (!_takingInput) return false;
		return _keyboardInputCallback(event);
	}
};