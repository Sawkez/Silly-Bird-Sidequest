#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

#include "InputManager.hpp"
#include "Player.hpp"
#include "CollisionRect.hpp"

using namespace std;

const unsigned long frameDuration = 1000 / 60;

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow(
        "SBS",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        480, 272,
        0
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    InputManager input;
    vector<CollisionRect> world;
    world.push_back(CollisionRect{0.0, 100.0, 300.0, 50.0});
    world.push_back(CollisionRect{300.0, 0.0, 50.0, 300.0});

    Player player(input, renderer, world);

    bool running = true;

    unsigned long frameStartMs = SDL_GetTicks64();
    unsigned long frameEndMs = frameStartMs + frameDuration;

    while (running) {

        unsigned long lastFrameTimeMs = frameEndMs - frameStartMs;
        float delta = float(lastFrameTimeMs / 1000.0);

        frameStartMs = SDL_GetTicks64();
        
        // event handling
        SDL_Event event;

        while (SDL_PollEvent(&event) != 0) {
            if (input.HandleEvent(event)) continue;

            if (event.type == SDL_QUIT) {
                running = false;
                continue;
            }
        }
        input.UpdateDir();

        // game logic
        player.Process(delta);

        input.UpdateTapStates();
        
        // render
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        player.Draw(renderer);
        
        for (auto collider: world) {
            collider.Draw(renderer);
        }

        SDL_RenderPresent(renderer);
        
        // frame limiting
        frameEndMs = SDL_GetTicks64();
        unsigned long frameTimeMs = frameEndMs - frameStartMs;

        if (frameTimeMs < frameDuration) {
            SDL_Delay(frameDuration - frameTimeMs);
            frameEndMs = SDL_GetTicks64();
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}