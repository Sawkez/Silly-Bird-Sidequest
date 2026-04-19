#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

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
#include "game/player/movement/MovementStateManager.hpp"
#include "game/ui/Menus.hpp"
#include "game/ui/title/TitleScreenMenu.hpp"

using namespace std;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);

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

	GameState::Unpause();

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
	if (!GameState::IsRunning()) return SDL_APP_SUCCESS;
	if (!GameState::ShouldProcess()) return SDL_APP_CONTINUE;

	GameState::UpdateFrameEnd();
	float delta = GameState::GetDelta();

	// per-frame event updates
	if (GameState::GetInput().IsTapped(ACTION_PAUSE)) UIManager::Push(UIManager::MENU_PAUSE);

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

	else {
		SaveManager::instance->Draw();
	}

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
	if (SaveManager::instance->OverrideDrawing()) return SDL_APP_CONTINUE;

	if (event->type == SDL_EVENT_WINDOW_RESIZED) WorldManager::GetLevel().GetCamera().UpdateZoom();

	if (UIManager::HandleEvent(*event)) return SDL_APP_CONTINUE;
	if (GameState::GetInput().HandleEvent(*event)) return SDL_APP_CONTINUE;

	if (event->type == SDL_EVENT_QUIT) return SDL_APP_SUCCESS;

	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) { GameState::Deinit(); }