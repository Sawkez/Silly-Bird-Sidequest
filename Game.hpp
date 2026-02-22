#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "CollisionRect.hpp"
#include "InputManager.hpp"
#include "Level.hpp"
#include "Player.hpp"

using namespace std;

const unsigned long frameDuration = 1000 / 60;
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
	bool running;
	unsigned long frameStartMs;
	unsigned long frameEndMs;
	Level level;

	Game()
		: mainWindow(SDL_CreateWindow("SBS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, INITIAL_WINDOW_RES,
									  SDL_WINDOW_RESIZABLE)),
		  mainRenderer(SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)),
		  input(), running(true), frameStartMs(SDL_GetTicks64()), frameEndMs(frameStartMs + frameDuration),
		  level("mods/test-sbmaker-project", mainRenderer, input, mainWindow) {}

	int Run(int argc, char* argv[]) {
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | IMG_INIT_PNG);

		while (running) {
			GameLoopIteration();
		}

		SDL_DestroyRenderer(mainRenderer);
		SDL_DestroyWindow(mainWindow);
		SDL_Quit();

		return 0;
	}

	void GameLoopIteration() {
		unsigned long lastFrameTimeMs = frameEndMs - frameStartMs;
		float delta = min(float(lastFrameTimeMs / 1000.0), MAX_DELTA);

		frameStartMs = SDL_GetTicks64();

		// event handling
		SDL_Event event;

		while (SDL_PollEvent(&event) != 0) {
			if (input.HandleEvent(event))
				continue;

			if (event.type == SDL_QUIT) {
				running = false;
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

		// render
		SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 0);
		SDL_RenderClear(mainRenderer);

		level.Draw(mainRenderer);

		SDL_RenderPresent(mainRenderer);

		// frame limiting
		frameEndMs = SDL_GetTicks64();
		unsigned long frameTimeMs = frameEndMs - frameStartMs;

		if (frameTimeMs < frameDuration) {
			SDL_Delay(frameDuration - frameTimeMs);
			frameEndMs = SDL_GetTicks64();
		}

		// cout << "Frame time: " << lastFrameTimeMs << endl;
	}
};