#include "engine/Game.hpp"
#include "game/player/movement/MovementStateManager.hpp"

#if SDL_PLATFORM_PSP
#include <pspkernel.h>
PSP_MODULE_INFO("Silly Bird Sidequest", 0, 1, 1);
#endif

int main(int argc, char* argv[]) {
	Game game(argc, argv);
	return game.Run();
}