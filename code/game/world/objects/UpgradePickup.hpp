#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <string>

#include "engine/world/PlayerDetector.hpp"
#include "game/player/IPlayer.hpp"

class UpgradePickup : public PlayerDetector {
   private:
	static inline const auto TEXTURE_SIZE = Vector2(16.0, 16.0);

	bool _firstFrame = true;
	SDL_Texture* _texture;
	SDL_FRect _drawRect;
	int _upgrade;

	static inline const std::string UPGRADE_NAMES[IPlayer::_UPGRADE_COUNT]{"dive", "dash", "slide", "diveboost", "rejuvenator", "wallrun"};

	SDL_FRect MakeRect(const Vector2& positionCentered) {
		Vector2 topLeft = positionCentered - TEXTURE_SIZE * 0.5f;
		return {topLeft.x, topLeft.y, TEXTURE_SIZE.x, TEXTURE_SIZE.y};
	}

	SDL_Texture* LoadTexture(SDL_Renderer* renderer, int upgrade) {
		std::string path = "content/textures/upgrades/" + UPGRADE_NAMES[upgrade] + ".png";
		return IMG_LoadTexture(renderer, path.data());
	}

   protected:
	void Activated(IPlayer& player) override {
		player.GiveUpgrade(_upgrade);
		_active = false;
	}

	void Process(float delta, IPlayer& player) override {
		if (_firstFrame) {
			_active = !player.HasUpgrade(_upgrade);
			_firstFrame = false;
		}

		PlayerDetector::Process(delta, player);
	}

   public:
	UpgradePickup(SDL_Renderer* renderer, const Vector2& positionCentered, const Vector2& relativePosition, int upgrade)
		: PlayerDetector(MakeRect(positionCentered), false),
		  _upgrade(upgrade),
		  _texture(LoadTexture(renderer, upgrade)),
		  _drawRect(MakeRect(relativePosition)) {}

	bool Draw(SDL_Renderer* renderer, const SDL_FRect& drawTargetRect, Vector2 drawOffset) const override {
		if (!_active) return false;
		if (!SDL_HasIntersectionF(&drawTargetRect, &_drawRect)) return false;
		SDL_RenderCopyF(renderer, _texture, NULL, &_drawRect);
		return true;
	}

	~UpgradePickup() { SDL_DestroyTexture(_texture); }
};