#pragma once

#include <SDL3/SDL.h>

#include <vector>

#include "engine/Vector2.hpp"
#include "engine/graphics/AnimatedSprite.hpp"

class AnimatedSpriteOverlay : public AnimatedSprite {
   private:
	Uint8 _overlayRed;
	Uint8 _overlayBlue;
	Uint8 _overlayGreen;
	bool _showOverlay = true;
	vector<SDL_Texture*> _overlayTextures;

   public:
	AnimatedSpriteOverlay(const vector<Animation>& animations, const vector<SDL_Texture*>& overlayTextures, Uint8 overlayRed, Uint8 overlayGreen,
						  Uint8 overlayBlue, Vector2 offset = Vector2::ZERO, Vector2 scaleOrigin = Vector2::ZERO,
						  Vector2 rotateOrigin = Vector2{0.0, 0.0})
		: AnimatedSprite(animations, offset, scaleOrigin, rotateOrigin),
		  _overlayTextures(overlayTextures),
		  _overlayRed(overlayRed),
		  _overlayGreen(overlayGreen),
		  _overlayBlue(overlayBlue) {}

	bool Draw(SDL_Renderer* renderer, const SDL_FRect& drawTargetRect, Vector2 drawOffset = {}) const override {
		if (!AnimatedSprite::Draw(renderer, drawTargetRect, drawOffset)) {
			return false;
		}

		if (_showOverlay) {
			SDL_FRect destination = _destination;
			destination.x += drawOffset.x;
			destination.y += drawOffset.y;

			SDL_Texture* texture = _overlayTextures.at(_current);
			SDL_SetTextureColorMod(texture, _overlayRed, _overlayGreen, _overlayBlue);

			int error = SDL_RenderTextureRotated(renderer, _overlayTextures.at(_current), &_source, &destination, _rotation, &_rotateOrigin, _flip);

			if (error < 0) {
				std::cerr << "ERROR: couldn't draw sprite overlay: " << SDL_GetError() << std::endl;
				return false;
			}
		}

		return true;
	}

	void SetOverlayColor(Uint8 red, Uint8 green, Uint8 blue) {
		_overlayRed = red;
		_overlayGreen = green;
		_overlayBlue = blue;
	}

	void SetOverlayColor(const SDL_Color& color) {
		_overlayRed = color.r;
		_overlayGreen = color.g;
		_overlayBlue = color.b;
	}

	void EnableOverlay() { _showOverlay = true; }

	void DisableOverlay() { _showOverlay = false; }
};