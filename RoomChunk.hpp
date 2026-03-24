#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "Tile.hpp"
#include "yyjson.h"

class RoomChunk {
	const int OVERLAP_OFFSET = 8;

   private:
	SDL_Rect _rect;
	SDL_Texture* _cache = NULL;

   public:
	RoomChunk() : _rect({0, 0, 0, 0}) {}

	RoomChunk(const string& chunkFilePath, yyjson_val* chunkJson, SDL_Renderer* renderer, vector<SDL_Surface*> atlases) :
		_rect{
			yyjson_get_int(yyjson_obj_get(chunkJson, "x")),
			yyjson_get_int(yyjson_obj_get(chunkJson, "y")),
			yyjson_get_int(yyjson_obj_get(chunkJson, "width")),
			yyjson_get_int(yyjson_obj_get(chunkJson, "height")),
		}, _cache(
			CacheTiles(
				renderer, 
				atlases, 
				chunkFilePath, 
				yyjson_get_int(
					yyjson_obj_get(
						chunkJson, 
						"tile_count"
					)
				)
			)
		)
		{
	}

	RoomChunk(const RoomChunk&) = delete;
	RoomChunk& operator=(const RoomChunk&) = delete;

	RoomChunk(RoomChunk&& other) noexcept : _rect(other._rect), _cache(other._cache) { other._cache = NULL; }

	RoomChunk& operator=(RoomChunk&& other) noexcept {
		if (this != &other) {
			if (_cache) SDL_DestroyTexture(_cache);

			_rect = other._rect;
			_cache = other._cache;

			other._cache = NULL;
		}
		return *this;
	}

	vector<Tile> LoadTiles(const string& chunkFilePath, int tileCount) {
		ifstream file;
		file.open(chunkFilePath, ios::out | ios::binary);
		if (!file.good()) {
			std::cerr << "ERROR: chunk file " << chunkFilePath << " is YUCKY" << std::endl;
		}

		vector<Tile> tiles;

		for (int i = 0; i < tileCount; i++) {
			tiles.push_back(Tile(file));
		}

		file.close();

		return tiles;
	}

	SDL_Texture* CacheTiles(SDL_Renderer* renderer, const std::vector<SDL_Surface*>& atlases, const string& chunkFilePath, int tileCount) {
		SDL_Surface* cacheSurface = SDL_CreateRGBSurface(0, _rect.w, _rect.h, 16, 0xF000, 0x0F00, 0x00F0, 0x000F);

		if (cacheSurface == NULL) {
			std::cerr << "ERROR when caching chunk: " << SDL_GetError() << std::endl;
		}

		vector<Tile> tiles = LoadTiles(chunkFilePath, tileCount);

		for (auto tile : tiles) {
			tile.Draw(cacheSurface, atlases, -_rect.x + OVERLAP_OFFSET, -_rect.y + OVERLAP_OFFSET);
		}

		SDL_Texture* cache = SDL_CreateTextureFromSurface(renderer, cacheSurface);
		SDL_FreeSurface(cacheSurface);

		return cache;
	}

	void UncacheTiles() {
		SDL_DestroyTexture(_cache);
		_cache = NULL;
	}

	void Draw(SDL_Renderer* renderer) const {
		if (_cache == NULL) {
			std::cerr << "ERROR: chunk not cached!" << std::endl;
		}

		SDL_Rect destination = _rect;
		destination.x = 0;
		destination.y = 0;

		SDL_RenderCopy(renderer, _cache, NULL, &destination);
	}

	int GetWidth() const { return _rect.w; }
	int GetHeight() const { return _rect.h; }

	const SDL_Rect& GetRect() const { return _rect; }

	SDL_FRect GetFRect() const { return SDL_FRect{float(_rect.x), float(_rect.y), float(_rect.w), float(_rect.h)}; }

	~RoomChunk() { UncacheTiles(); }
};