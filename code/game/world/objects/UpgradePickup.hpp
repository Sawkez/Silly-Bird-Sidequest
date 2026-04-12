#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <string>

#include "engine/world/PlayerDetector.hpp"
#include "game/player/Player.hpp"

class UpgradePickup : public PlayerDetector {
   private:
	static inline const auto TEXTURE_SIZE = Vector2(16.0, 16.0);

	SDL_Texture* _texture;
	int _upgrade;

	static inline const std::string UPGRADE_NAMES[Player::_UPGRADE_COUNT]{"dive", "dash", "slide", "diveboost", "rejuvenator", "wallrun"};

	SDL_FRect MakeRect(const Vector2& positionCentered) {
		Vector2 topLeft = positionCentered - TEXTURE_SIZE;
		return {topLeft.x, topLeft.y, TEXTURE_SIZE.x, TEXTURE_SIZE.y};
	}

	SDL_Texture* LoadTexture(SDL_Renderer* renderer, int upgrade) {
		std::string path = "content/textures/upgrades/" + UPGRADE_NAMES[upgrade] + ".png";
		return IMG_LoadTexture(renderer, path.data());
	}

   protected:
	void Activated(Player& player) override { player.GiveUpgrade(_upgrade); }

   public:
	UpgradePickup(SDL_Renderer* renderer, const Vector2& positionCentered, int upgrade)
		: PlayerDetector(MakeRect(positionCentered)), _upgrade(upgrade), _texture(LoadTexture(renderer, upgrade)) {}

	bool Draw(SDL_Renderer* renderer, const SDL_FRect& drawTargetRect, Vector2 drawOffset) const override {
		if (!SDL_HasIntersectionF(&drawTargetRect, &_rect)) return false;
		SDL_RenderCopyF(renderer, _texture, NULL, &_rect);
		return true;
	}

	~UpgradePickup() { SDL_DestroyTexture(_texture); }
};