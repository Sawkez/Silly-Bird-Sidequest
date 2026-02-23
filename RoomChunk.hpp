#pragma once

#include <SDL.h>
#include <iostream>
#include <vector>

#include "Json.hpp"
#include "Tile.hpp"

struct RoomChunk {
	SDL_Rect rect;
	SDL_Texture* cache = NULL;
	std::vector<Tile> tiles;

	RoomChunk() : rect({0, 0, 0, 0}) {}

	RoomChunk(const RoomChunk&) = delete;
	RoomChunk& operator=(const RoomChunk&) = delete;

	RoomChunk(RoomChunk&& other) noexcept : rect(other.rect), cache(other.cache), tiles(std::move(other.tiles)) {
		other.cache = NULL;
	}

	RoomChunk& operator=(RoomChunk&& other) noexcept {
		if (this != &other) {
			if (cache)
				SDL_DestroyTexture(cache);

			rect = other.rect;
			cache = other.cache;
			tiles = std::move(other.tiles);

			other.cache = NULL;
		}
		return *this;
	}

	void CacheTiles(SDL_Renderer* renderer, const std::vector<SDL_Texture*>& atlases) {
		std::cout << "Caching chunk " << rect.x << " " << rect.y << " " << rect.w << " " << rect.h << std::endl;

		cache = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA5551, SDL_TEXTUREACCESS_TARGET, rect.w, rect.h);

		if (cache == NULL) {
			std::cerr << "ERROR when caching chunk: " << SDL_GetError() << std::endl;
		}

		SDL_SetRenderTarget(renderer, cache);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		for (auto tile : tiles) {
			std::cout << "Caching tile" << tile << std::endl;
			tile.Draw(renderer, atlases, -rect.x, -rect.y);
		}
	}

	void UncacheTiles() {
		SDL_DestroyTexture(cache);
		cache = NULL;
	}

	void Draw(SDL_Renderer* renderer) const {
		if (cache == NULL) {
			std::cerr << "ERROR: chunk not cached!" << std::endl;
		}

		SDL_Rect destination = rect;
		destination.x = 0;
		destination.y = 0;

		SDL_RenderCopy(renderer, cache, NULL, &destination);
	}
};


void from_json(const nlohmann::json& json, RoomChunk& chunk) {
	chunk.rect.x = json.at("x");
	chunk.rect.y = json.at("y");
	chunk.rect.w = json.at("width");
	chunk.rect.h = json.at("height");
	chunk.tiles = json.at("tiles").get<std::vector<Tile>>();
}