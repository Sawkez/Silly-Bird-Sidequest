#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include <fstream>
#include <string>
#include <vector>

#include "engine/ResourceManager.hpp"
#include "engine/devconsole/DevConsole.hpp"
#include "engine/graphics/Animation.hpp"
#include "engine/graphics/FColor.hpp"
#include "engine/graphics/PlaybackPosition.hpp"
#include "yyjson.h"

class Jizz {
   public:
	static inline const int PALETTE_SIZE = 10;
	static inline const int ANIMATION_COUNT = 12;
	static inline const std::string PLAYER_ANIMATIONS[ANIMATION_COUNT] = {
		"duck", "fly",		"idle",	 "jump",	   "ledge_flip", "ledge_unflip",
		"run",	"slow_run", "slide", "twerk_down", "twerk_up",	 "wallrun"};

   private:
	std::string _stylePath;
	SDL_Palette* _palette;
	SDL_Renderer* _renderer;
	vector<vector<uint8_t>> _scarfPositions;
	std::vector<Animation> _animations;
	std::vector<SDL_Texture*> _scarfOverlays;
	bool _allowTwerk;
	float _scarfBaseWidth;
	float _scarfTipWidth;
	float _scarfWeight;
	float _scarfSegmentLength;
	FColor _scarfChargedColor;
	FColor _scarfEmptyColor;

	void LoadAnimations(yyjson_val* json) {
		_animations.reserve(ANIMATION_COUNT);
		for (int i = 0; i < ANIMATION_COUNT; i++) {
			_animations.emplace_back(LoadTexture(PLAYER_ANIMATIONS[i]),
									 yyjson_arr_get(yyjson_obj_get(json, "animations"), i));
		}
	}

	void LoadScarfOverlays() {
		_scarfOverlays.reserve(ANIMATION_COUNT);
		for (int i = 0; i < ANIMATION_COUNT; i++) {
			std::string path = _stylePath + "/scarf/" + PLAYER_ANIMATIONS[i] + ".png";
			_scarfOverlays.push_back(IMG_LoadTexture(_renderer, path.data()));
		}
	}

   public:
	Jizz(const std::string& stylePath, yyjson_val* jsonRoot, SDL_Renderer* renderer)
		: _stylePath(stylePath),
		  _renderer(renderer),
		  _palette(LoadPalette(yyjson_obj_get(jsonRoot, "colors"))),
		  _scarfPositions(LoadScarfPositions(yyjson_obj_get(jsonRoot, "scarf_positions"))),
		  _allowTwerk(yyjson_get_bool(yyjson_obj_get(jsonRoot, "allow_twerk"))),
		  _scarfBaseWidth(yyjson_get_num(yyjson_obj_get(jsonRoot, "scarf_base_width"))),
		  _scarfTipWidth(yyjson_get_num(yyjson_obj_get(jsonRoot, "scarf_tip_width"))),
		  _scarfWeight(yyjson_get_num(yyjson_obj_get(jsonRoot, "scarf_weight"))),
		  _scarfSegmentLength(yyjson_get_num(yyjson_obj_get(jsonRoot, "scarf_segment_length"))),
		  _scarfChargedColor(yyjson_obj_get(jsonRoot, "scarf_charged_color")),
		  _scarfEmptyColor(yyjson_obj_get(jsonRoot, "scarf_empty_color")) {
		LoadAnimations(jsonRoot);
		LoadScarfOverlays();
	}

	Jizz(const std::string& stylePath, yyjson_doc* jsonDoc, SDL_Renderer* renderer)
		: Jizz(stylePath, yyjson_doc_get_root(jsonDoc), renderer) {
		yyjson_doc_free(jsonDoc);
	}

	Jizz(const std::string& stylePath, SDL_Renderer* renderer)
		: Jizz(stylePath, ResourceManager::LoadJson(stylePath + "/skin.json"), renderer) {}

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

	vector<vector<uint8_t>> LoadScarfPositions(yyjson_val* json) const {
		vector<vector<uint8_t>> anims;

		int animIdx, animMax;
		yyjson_val* anim;
		yyjson_arr_foreach(json, animIdx, animMax, anim) {
			vector<uint8_t> frames;

			int frameIdx, frameMax;
			yyjson_val* frame;
			yyjson_arr_foreach(anim, frameIdx, frameMax, frame) { frames.push_back(yyjson_get_num(frame)); }

			anims.push_back(frames);
		}

		return anims;
	}

	const std::vector<Animation>& GetAnimations() const { return _animations; }

	const std::vector<SDL_Texture*>& GetOverlayTextures(SDL_Renderer* renderer) const { return _scarfOverlays; }

	Vector2 GetScarfPosition(PlaybackPosition playback) const {
		uint8_t position = _scarfPositions.at(playback.animation).at(playback.frame);

		return Vector2{float(position & 0xFF), float((position >> 4) & 0xFF)};
	}

	SDL_Texture* LoadTexture(const std::string& textureName) const {
		SDL_Surface* surface = IMG_Load((_stylePath + "/" + textureName + ".png").data());
		dc::msg << "Loading " << _stylePath + "/" + textureName + ".png" << dc::endl;

		if (surface == NULL) {
			dc::err << "ERROR loading character texture " << textureName << ": " << SDL_GetError() << dc::endl;
		}

		SDL_SetSurfacePalette(surface, _palette);

		SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);

		if (texture == NULL) {
			dc::err << "ERROR converting character texture " << textureName << " to surface: " << SDL_GetError()
					<< dc::endl;
		}

		SDL_DestroySurface(surface);
		return texture;
	}
};