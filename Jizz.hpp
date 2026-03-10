#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <fstream>
#include <string>
#include <vector>

#include "Animation.hpp"
#include "PlaybackPosition.hpp"
#include "yyjson.h"

class Jizz {
  public:
	static const int PALETTE_SIZE = 10;
	static const vector<std::string> PLAYER_ANIMATIONS;

  private:
	yyjson_doc* _json;
	std::string _stylePath;
	SDL_Palette* _palette;
	SDL_Renderer* _renderer;
	vector<vector<Vector2>> _scarfPositions;

  public:
	Jizz(const std::string& stylePath, yyjson_doc* styleJson, SDL_Renderer* renderer)
		: _json(styleJson), _stylePath(stylePath), _renderer(renderer),
		  _palette(LoadPalette(yyjson_obj_get(yyjson_doc_get_root(styleJson), "colors"))),
		  _scarfPositions(LoadScarfPositions(yyjson_obj_get(yyjson_doc_get_root(styleJson), "scarf_positions"))) {}

	Jizz(const std::string& stylePath, SDL_Renderer* renderer) : Jizz(stylePath, LoadJson(stylePath), renderer) {}

	yyjson_doc* LoadJson(const string& stylePath) const {
		ifstream jsonFile(stylePath + "/skin.json");
		if (!jsonFile.good()) {
			cerr << "ERROR opening skin.json file in " << stylePath << endl;
		}

		std::string jsonString((istreambuf_iterator<char>(jsonFile)), (istreambuf_iterator<char>()));

		yyjson_doc* json = yyjson_read(jsonString.data(), jsonString.length(), 0);
		return json;
	}

	SDL_Palette* LoadPalette(yyjson_val* json) const {
		SDL_Palette* palette = SDL_AllocPalette(PALETTE_SIZE + 1);
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
		return {Animation(LoadTexture("duck"), yyjson_arr_get(animations, 0)),
				Animation(LoadTexture("fly"), yyjson_arr_get(animations, 1)),
				Animation(LoadTexture("idle"), yyjson_arr_get(animations, 2)),
				Animation(LoadTexture("jump"), yyjson_arr_get(animations, 3)),
				Animation(LoadTexture("ledge_flip"), yyjson_arr_get(animations, 4)),
				Animation(LoadTexture("ledge_unflip"), yyjson_arr_get(animations, 5)),
				Animation(LoadTexture("run"), yyjson_arr_get(animations, 6)),
				Animation(LoadTexture("slow_run"), yyjson_arr_get(animations, 7)),
				Animation(LoadTexture("slide"), yyjson_arr_get(animations, 8)),
				Animation(LoadTexture("twerk_down"), yyjson_arr_get(animations, 9)),
				Animation(LoadTexture("twerk_up"), yyjson_arr_get(animations, 10))};
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

	Vector2 GetScarfPosition(PlaybackPosition playback) const {
		return _scarfPositions.at(playback.animation).at(playback.frame);
	}

	SDL_Texture* LoadTexture(const std::string& textureName) const {
		SDL_Surface* surface = IMG_Load((_stylePath + "/" + textureName + ".png").data());
		std::cout << "Loading " << _stylePath + "/" + textureName + ".png" << std::endl;

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