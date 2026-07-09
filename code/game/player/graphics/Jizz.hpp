#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <fstream>
#include <string>
#include <vector>

#include "engine/devconsole/DevConsole.hpp"
#include "engine/graphics/Animation.hpp"
#include "engine/graphics/PlaybackPosition.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "yyjson.h"

class Jizz {
   public:
	static const int PALETTE_SIZE = 10;
	static const vector<std::string> PLAYER_ANIMATIONS;

   private:
	SDL_Storage* _storage;
	std::string _stylePath;
	yyjson_doc* _json;
	SDL_Palette* _palette;
	SDL_Renderer* _renderer;
	vector<vector<Vector2>> _scarfPositions;

   public:
	Jizz(SDL_Storage* storage, const std::string& stylePath, yyjson_doc* styleJson, SDL_Renderer* renderer)
		: _storage(storage),
		  _json(styleJson),
		  _stylePath(stylePath),
		  _renderer(renderer),
		  _palette(LoadPalette(yyjson_obj_get(yyjson_doc_get_root(styleJson), "colors"))),
		  _scarfPositions(LoadScarfPositions(yyjson_obj_get(yyjson_doc_get_root(styleJson), "scarf_positions"))) {}

	Jizz(SDL_Storage* storage, const std::string& stylePath, SDL_Renderer* renderer)
		: Jizz(storage, stylePath, ResourceManager::LoadJson(storage, stylePath + "/skin.json"), renderer) {}

	SDL_Palette* LoadPalette(yyjson_val* json) const {
		SDL_Palette* palette = SDL_CreatePalette(PALETTE_SIZE + 1);
		SDL_Color colors[PALETTE_SIZE + 1];
		colors[0] = {0, 0, 0, 0};

		int color_idx, color_max;
		yyjson_val* colorArr;

		yyjson_arr_foreach(json, color_idx, color_max, colorArr) {
			colors[color_idx + 1].r = yyjson_get_int(yyjson_arr_get(colorArr, 0));
			colors[color_idx + 1].g = yyjson_get_int(yyjson_arr_get(colorArr, 1));
			colors[color_idx + 1].b = yyjson_get_int(yyjson_arr_get(colorArr, 2));
			colors[color_idx + 1].a = 255;
		}

		SDL_SetPaletteColors(palette, colors, 0, PALETTE_SIZE + 1);
		return palette;
	}

	vector<vector<Vector2>> LoadScarfPositions(yyjson_val* json) const {
		vector<vector<Vector2>> anims;

		int animIdx, animMax;
		yyjson_val* anim;
		yyjson_arr_foreach(json, animIdx, animMax, anim) {
			vector<Vector2> frames;

			int frameIdx, frameMax;
			yyjson_val* frame;
			yyjson_arr_foreach(anim, frameIdx, frameMax, frame) {
				frames.push_back(
					{float(yyjson_get_num(yyjson_arr_get(frame, 0))), float(yyjson_get_num(yyjson_arr_get(frame, 1)))});
			}

			anims.push_back(frames);
		}

		return anims;
	}

	std::vector<Animation> GetAnimations() const {
		yyjson_val* animations = yyjson_obj_get(yyjson_doc_get_root(_json), "animations");
		return {Animation(LoadPaletteTexture("duck"), yyjson_arr_get(animations, 0)),
				Animation(LoadPaletteTexture("fly"), yyjson_arr_get(animations, 1)),
				Animation(LoadPaletteTexture("idle"), yyjson_arr_get(animations, 2)),
				Animation(LoadPaletteTexture("jump"), yyjson_arr_get(animations, 3)),
				Animation(LoadPaletteTexture("ledge_flip"), yyjson_arr_get(animations, 4)),
				Animation(LoadPaletteTexture("ledge_unflip"), yyjson_arr_get(animations, 5)),
				Animation(LoadPaletteTexture("run"), yyjson_arr_get(animations, 6)),
				Animation(LoadPaletteTexture("slow_run"), yyjson_arr_get(animations, 7)),
				Animation(LoadPaletteTexture("slide"), yyjson_arr_get(animations, 8)),
				Animation(LoadPaletteTexture("twerk_down"), yyjson_arr_get(animations, 9)),
				Animation(LoadPaletteTexture("twerk_up"), yyjson_arr_get(animations, 10)),
				Animation(LoadPaletteTexture("wallrun"), yyjson_arr_get(animations, 11))};
	}

	std::vector<SDL_Texture*> GetOverlayTextures() const {
		return {LoadTexture("scarf/duck"),		 LoadTexture("scarf/fly"),		  LoadTexture("scarf/idle"),
				LoadTexture("scarf/jump"),		 LoadTexture("scarf/ledge_flip"), LoadTexture("scarf/ledge_unflip"),
				LoadTexture("scarf/run"),		 LoadTexture("scarf/slow_run"),	  LoadTexture("scarf/slide"),
				LoadTexture("scarf/twerk_down"), LoadTexture("scarf/twerk_up"),	  LoadTexture("scarf/wallrun")};
	}

	Vector2 GetScarfPosition(PlaybackPosition playback) const {
		return _scarfPositions.at(playback.animation).at(playback.frame);
	}

	SDL_Texture* LoadTexture(const std::string& textureName) const {
		return ResourceManager::LoadTexture(_renderer, _storage, _stylePath + "/" + textureName + ".png");
	}

	SDL_Texture* LoadPaletteTexture(const std::string& textureName) const {
		SDL_Surface* surface = ResourceManager::LoadSurface(_storage, _stylePath + "/" + textureName + ".png");
		if (surface == nullptr) {
			dc::err << "ERROR: " << SDL_GetError() << dc::endl;
		}

		if (!SDL_SetSurfacePalette(surface, _palette)) {
			dc::err << "ERROR setting palette for " << textureName << ": " << SDL_GetError() << dc::endl;
		}

		SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
		if (texture == nullptr) {
			dc::err << "ERROR: " << SDL_GetError() << dc::endl;
		}

		return texture;
	}
};