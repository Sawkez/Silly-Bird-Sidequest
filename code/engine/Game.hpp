#include <SDL.h>
#include <SDL_image.h>

#include <iostream>

#include "engine/GameState.hpp"
#include "engine/Random.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/physics/CollisionRect.hpp"
#include "engine/save/SaveManager.hpp"
#include "engine/ui/UIManager.hpp"
#include "engine/world/Level.hpp"
#include "engine/world/WorldManager.hpp"
#include "game/player/Player.hpp"
#include "game/ui/Menus.hpp"
#include "game/ui/title/TitleScreenMenu.hpp"

/*
#if __PSP__
#include <pspdebug.h>
#include <pspkernel.h>
#include <pspsdk.h>
#endif
*/

using namespace std;

struct Game {
	Uint64 lastPerfCounter = 0;

	Game(int argc, char* argv[]) {
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | IMG_INIT_PNG);

		GameState::Init();
		UIManager::Init(GameState::GetMainRenderer(), GameState::GetMainWindow());
		Random::Init();
		SaveManager::Init();

		if (argc < 2) {
			WorldManager::LoadLevel("content/title-screen-bg");
			UIManager::Push(UIManager::MENU_TITLE);
		}

		else {
			WorldManager::LoadLevel(argv[1]);
		}
	}

	int Run() {
		GameState::Unpause();
		while (GameState::IsRunning()) {
			GameLoopIteration();
		}

		GameState::Deinit();
		return 0;
	}

	void GameLoopIteration() {
		float delta = GameState::GetDelta();

		GameState::UpdateFrameStart();

		// event handling
		SDL_Event event;

		while (SDL_PollEvent(&event) != 0) {
			if (SaveManager::instance->OverrideDrawing()) continue;

			if (event.type == SDL_WINDOWEVENT) WorldManager::GetLevel().GetCamera().UpdateZoom();

			if (UIManager::HandleEvent(event)) continue;
			if (GameState::GetInput().HandleEvent(event)) continue;

			if (event.type == SDL_QUIT) {
				GameState::SetRunning(false);
				continue;
			}
		}

		if (GameState::GetInput().IsTapped(ACTION_PAUSE)) {
			UIManager::Push(UIManager::MENU_PAUSE);
		}

		GameState::GetInput().UpdateDir();

		// game logic
		if (!GameState::IsPaused()) {
			WorldManager::GetLevel().Process(delta);
		}

		GameState::GetInput().UpdateTapStates();

		UIManager::Process();

		// render

		if (!SaveManager::instance->OverrideDrawing()) {
			SDL_SetRenderDrawColor(GameState::GetMainRenderer(), 0, 0, 0, 0);
			SDL_RenderClear(GameState::GetMainRenderer());

			WorldManager::GetLevel().Draw(GameState::GetMainRenderer());

			UIManager::Draw();

			SDL_RenderPresent(GameState::GetMainRenderer());
		}

		SaveManager::instance->Draw();

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