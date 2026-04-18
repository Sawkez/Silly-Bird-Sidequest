#include <SDL3/SDL_main.h>

#include "engine/Game.hpp"
#include "game/player/movement/MovementStateManager.hpp"

int main(int argc, char* argv[]) {
	Game game(argc, argv);
	return game.Run();
}