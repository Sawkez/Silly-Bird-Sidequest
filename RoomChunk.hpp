#pragma once

#include <SDL.h>
#include <SDL_image.h>
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

	void CacheTiles(SDL_Renderer* renderer, const std::vector<SDL_Surface*>& atlases) {
		SDL_Surface* cacheSurface = SDL_CreateRGBSurface(0, rect.w, rect.h, 16, 0xF000, 0x0F00, 0x00F0, 0x000F);

		if (cacheSurface == NULL) {
			std::cerr << "ERROR when caching chunk: " << SDL_GetError() << std::endl;
		}

		for (auto tile : tiles) {
			tile.Draw(cacheSurface, atlases, -rect.x + 8, -rect.y + 8);
		}

		cache = SDL_CreateTextureFromSurface(renderer, cacheSurface);
		SDL_FreeSurface(cacheSurface);
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

	SDL_FRect GetFRect() const { return SDL_FRect{float(rect.x), float(rect.y), float(rect.w), float(rect.h)}; }

	~RoomChunk() {
		cout << "Destroying room chunk!" << endl;
		UncacheTiles();
		tiles.clear();
	}
};


void from_json(const nlohmann::json& json, RoomChunk& chunk) {
	chunk.rect.x = json.at("x");
	chunk.rect.y = json.at("y");
	chunk.rect.w = json.at("width");
	chunk.rect.h = json.at("height");
	chunk.tiles = json.at("tiles").get<std::vector<Tile>>();
}