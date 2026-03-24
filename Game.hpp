#include <SDL.h>
#include <SDL_image.h>

#include <iostream>

#include "CollisionRect.hpp"
#include "GameState.hpp"
#include "InputManager.hpp"
#include "Level.hpp"
#include "Player.hpp"
#include "Random.hpp"

/*
#if __PSP__
#include <pspdebug.h>
#include <pspkernel.h>
#include <pspsdk.h>
#endif
*/

using namespace std;

const float MAX_DELTA = 1.0;

#if __PSP__
#define INITIAL_WINDOW_RES 480, 272
#else
#define INITIAL_WINDOW_RES 960, 540
#endif

struct Game {
	SDL_Window* mainWindow;
	SDL_Renderer* mainRenderer;
	InputManager input;
	GameState state;
	Level level;

	Game()
		: mainWindow(SDL_CreateWindow("SBS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, INITIAL_WINDOW_RES, SDL_WINDOW_RESIZABLE)),
		  mainRenderer(SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)),
		  input(),
		  state(),
		  level("mods/test-sbmaker-project", mainRenderer, input, mainWindow, state) {}

	int Run(int argc, char* argv[]) {
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | IMG_INIT_PNG);

		Random::Init();

		while (state.IsRunning()) {
			GameLoopIteration();
		}

		SDL_DestroyRenderer(mainRenderer);
		SDL_DestroyWindow(mainWindow);
		SDL_Quit();

		return 0;
	}

	void GameLoopIteration() {
		unsigned long lastFrameTimeMs = state.GetFrameEndMs() - state.GetFrameStartMs();
		float delta = float(lastFrameTimeMs / 1000.0);
		if (delta > MAX_DELTA) {
			delta = state.frameDuration / 1000.0;
		}

		state.UpdateFrameStart();

		// event handling
		SDL_Event event;

		while (SDL_PollEvent(&event) != 0) {
			if (input.HandleEvent(event)) continue;

			if (event.type == SDL_QUIT) {
				state.SetRunning(false);
				continue;
			}

			if (event.type == SDL_WINDOWEVENT) {
				level.GetCamera().UpdateZoom();
				continue;
			}
		}
		input.UpdateDir();

		// game logic
		level.Process(delta * 3.0);

		input.UpdateTapStates();

		// render
		SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 0);
		SDL_RenderClear(mainRenderer);

		level.Draw(mainRenderer);

		SDL_RenderPresent(mainRenderer);

		// frame limiting
		state.UpdateFrameEnd();
		unsigned long frameTimeMs = state.GetFrameEndMs() - state.GetFrameStartMs();

		if (frameTimeMs < state.frameDuration) {
			SDL_Delay(state.frameDuration - frameTimeMs);
			state.UpdateFrameEnd();
		}
	}
};