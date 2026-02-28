#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>

#include "Animation.hpp"

class Jizz {
  public:
	static const int PALETTE_SIZE = 10;
	static const vector<std::string> PLAYER_ANIMATIONS;

  private:
	std::string _stylePath;
	SDL_Palette* _palette;
	SDL_Renderer* _renderer;

  public:
	Jizz(const std::string& stylePath, SDL_Renderer* renderer, const SDL_Color (&palette)[PALETTE_SIZE])
		: _stylePath(stylePath), _renderer(renderer), _palette(SDL_AllocPalette(PALETTE_SIZE * 2 + 1)) {
		SDL_SetPaletteColors(_palette, palette, 1, PALETTE_SIZE);
		SDL_SetPaletteColors(_palette, palette, PALETTE_SIZE + 1, PALETTE_SIZE);
	}

	std::vector<Animation> GetAnimations() const {
		return {Animation(LoadTexture("duck"), 1, 1.0f, false),
				Animation(LoadTexture("fly"), 1, 1.0f, false),
				Animation(LoadTexture("idle"), 7, 10.0f, true),
				Animation(LoadTexture("jump"), 10, 24.0f, false),
				Animation(LoadTexture("ledge_flip"), 4, 10.0f, false),
				Animation(LoadTexture("ledge_unflip"), 4, 10.0f, false),
				Animation(LoadTexture("run"), 16, 24.0f, true),
				Animation(LoadTexture("slow_run"), 16, 24.0f, true),
				Animation(LoadTexture("slide"), 1, 1.0f, false),
				Animation(LoadTexture("twerk_down"), 5, 12.0f, false),
				Animation(LoadTexture("twerk_up"), 5, 12.0f, false)};
	}

	std::vector<SDL_Texture*> GetOverlayTextures(SDL_Renderer* renderer) const {
		return {
			IMG_LoadTexture(renderer, (_stylePath + "/scarf/duck.png").data()),
			IMG_LoadTexture(renderer, (_stylePath + "/scarf/fly.png").data()),
			IMG_LoadTexture(renderer, (_stylePath + "/scarf/idle.png").data()),
			IMG_LoadTexture(renderer, (_stylePath + "/scarf/jump.png").data()),
			IMG_LoadTexture(renderer, (_stylePath + "/scarf/ledge_flip.png").data()),
			IMG_LoadTexture(renderer, (_stylePath + "/scarf/ledge_unflip.png").data()),
			IMG_LoadTexture(renderer, (_stylePath + "/scarf/run.png").data()),
			IMG_LoadTexture(renderer, (_stylePath + "/scarf/slow_run.png").data()),
			IMG_LoadTexture(renderer, (_stylePath + "/scarf/slide.png").data()),
			IMG_LoadTexture(renderer, (_stylePath + "/scarf/twerk_down.png").data()),
			IMG_LoadTexture(renderer, (_stylePath + "/scarf/twerk_up.png").data()),
		};
	}

	SDL_Texture* LoadTexture(const std::string& textureName) const {
		SDL_Surface* surface = IMG_Load((_stylePath + "/" + textureName + ".png").data());

		if (surface == NULL) {
			std::cerr << "ERROR loading character texture " << textureName << ": " << SDL_GetError() << std::endl;
		}

		SDL_SetSurfacePalette(surface, _palette);

		SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);

		if (texture == NULL) {
			std::cerr << "ERROR converting character texture " << textureName << " to surface: " << SDL_GetError()
					  << std::endl;
		}

		SDL_FreeSurface(surface);
		return texture;
	}
};