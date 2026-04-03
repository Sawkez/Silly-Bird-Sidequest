#include <SDL.h>
#include <SDL_image.h>

#include <iostream>

#include "engine/GameState.hpp"
#include "engine/Random.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/physics/CollisionRect.hpp"
#include "engine/ui/UIManager.hpp"
#include "engine/world/Level.hpp"
#include "game/player/Player.hpp"

/*
#if __PSP__
#include <pspdebug.h>
#include <pspkernel.h>
#include <pspsdk.h>
#endif
*/

using namespace std;

#if __PSP__
#define INITIAL_WINDOW_RES 480, 272
#else
#define INITIAL_WINDOW_RES 960, 540
#endif

struct Game {
	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;
	InputManager input;
	Level level;
	UIManager ui;

	Uint64 lastPerfCounter = 0;

	Game()
		: mainWindow(SDL_CreateWindow("SBS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, INITIAL_WINDOW_RES, SDL_WINDOW_RESIZABLE)),
		  mainRenderer(SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)),
		  input(),
		  level("mods/test-sbmaker-project", mainRenderer, input, mainWindow),
		  ui(mainRenderer) {}

	int Run(int argc, char* argv[]) {
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | IMG_INIT_PNG);

		Random::Init();

		while (GameState::IsRunning()) {
			GameLoopIteration();
		}

		SDL_DestroyRenderer(mainRenderer);
		SDL_DestroyWindow(mainWindow);
		SDL_Quit();

		return 0;
	}

	void GameLoopIteration() {
		float delta = GameState::GetDelta();

		GameState::UpdateFrameStart();

		// event handling
		SDL_Event event;

		while (SDL_PollEvent(&event) != 0) {
			if (input.HandleEvent(event)) continue;

			if (event.type == SDL_QUIT) {
				GameState::SetRunning(false);
				continue;
			}

			if (event.type == SDL_WINDOWEVENT) {
				level.GetCamera().UpdateZoom();
				continue;
			}
		}
		input.UpdateDir();

		// game logic
		level.Process(delta);

		input.UpdateTapStates();

		ui.Process();

		// render
		SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 0);
		SDL_RenderClear(mainRenderer);

		level.Draw(mainRenderer);

		ui.Draw(mainRenderer);

		SDL_RenderPresent(mainRenderer);

		// frame limiting
		GameState::UpdateFrameEnd();
		unsigned long frameTimeMs = GameState::GetFrameEndMs() - GameState::GetFrameStartMs();

		if (frameTimeMs < GameState::GetFrameDuration()) {
			SDL_Delay(GameState::GetFrameDuration() - frameTimeMs);
			GameState::UpdateFrameEnd();
		}

		// cout << "FPS: " << 1.0 / (double(SDL_GetPerformanceCounter() - lastPerfCounter) / double(SDL_GetPerformanceFrequency())) << endl;

		lastPerfCounter = SDL_GetPerformanceCounter();
	}
};