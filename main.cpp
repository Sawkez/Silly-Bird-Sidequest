#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

#include "InputManager.hpp"
#include "Player.hpp"
#include "Level.hpp"
#include "CollisionRect.hpp"

using namespace std;

const unsigned long frameDuration = 1000 / 60;

#if __PSP__
    #define INITIAL_WINDOW_RES 480, 272
#else
    #define INITIAL_WINDOW_RES 960, 540
#endif

int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow(
        "SBS",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        INITIAL_WINDOW_RES,
        SDL_WINDOW_RESIZABLE
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    InputManager input;

    bool running = true;

    unsigned long frameStartMs = SDL_GetTicks64();
    unsigned long frameEndMs = frameStartMs + frameDuration;

    Level level("mods/test-sbmaker-project", renderer, input, window);

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
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        
        level.Draw(renderer);

        SDL_RenderPresent(renderer);
        
        // frame limiting
        frameEndMs = SDL_GetTicks64();
        unsigned long frameTimeMs = frameEndMs - frameStartMs;

        if (frameTimeMs < frameDuration) {
            SDL_Delay(frameDuration - frameTimeMs);
            frameEndMs = SDL_GetTicks64();
        }

        //cout << "Frame time: " << lastFrameTimeMs << endl;
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}