#pragma once

#include <SDL.h>
#include <vector>

#include "AnimatedSprite.hpp"
#include "Vector2.hpp"

class AnimatedSpriteOverlay : public AnimatedSprite {
  private:
	Uint8 _overlayRed;
	Uint8 _overlayBlue;
	Uint8 _overlayGreen;
	bool _showOverlay = true;
	vector<SDL_Texture*> _overlayTextures;

  public:
	AnimatedSpriteOverlay(const vector<Animation>& animations, const vector<SDL_Texture*>& overlayTextures,
						  Uint8 overlayRed, Uint8 overlayGreen, Uint8 overlayBlue, Vector2 offset = Vector2::ZERO,
						  Vector2 scaleOrigin = Vector2::ZERO, Vector2 rotateOrigin = Vector2{0.0, 0.0})
		: AnimatedSprite(animations, offset, scaleOrigin, rotateOrigin), _overlayTextures(overlayTextures),
		  _overlayRed(overlayRed), _overlayGreen(overlayGreen), _overlayBlue(overlayBlue) {}

	void Draw(SDL_Renderer* renderer, Vector2 drawOffset = {}) const override {
		AnimatedSprite::Draw(renderer, drawOffset);

		if (_showOverlay) {
			SDL_FRect destination = _destination;
			destination.x += drawOffset.x;
			destination.y += drawOffset.y;

			SDL_Texture* texture = _overlayTextures.at(_current);
			SDL_SetTextureColorMod(texture, _overlayRed, _overlayGreen, _overlayBlue);

			SDL_RenderCopyExF(renderer, _overlayTextures.at(_current), &_source, &destination, _rotation,
							  &_rotateOrigin, _flip);
		}
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