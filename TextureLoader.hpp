#pragma once

#include <string>
#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

namespace TextureLoader {

    SDL_Texture* LoadTexture(SDL_Renderer* renderer, std::string path) {
        SDL_Surface* pixels = IMG_Load(path.data());

        if (pixels == NULL) {
            std::cout << "FUCK !! couldn't load image: " << path << std::endl <<
                "error: " << SDL_GetError();
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, pixels);
        SDL_FreeSurface(pixels);

        return texture;
    }
}