#pragma once

#include "engine/graphics/IDrawableRect.hpp"
#include "engine/resource/BinaryReader.hpp"
#include "engine/world/WorldConstants.hpp"

class RoomChunkBase : public IDrawableRect {
   protected:
	SDL_Rect _rect;
	static inline const int OVERLAP_OFFSET = 8;

	RoomChunkBase() {}

	void BuildChunk(SDL_Renderer* renderer, BinaryReader& binary, void* customData) {
		binary.EnsureSection("CHNK");

		Uint16 num;
		binary.Read(2, &num);
		_rect.x = num;
		binary.Read(2, &num);
		_rect.y = num;
		binary.Read(2, &num);
		_rect.w = num;
		binary.Read(2, &num);
		_rect.h = num;

		Uint32 tileCount;
		binary.Read(4, &tileCount);
		Uint32 spikeCount;
		binary.Read(4, &spikeCount);

		this->PrepareCache(renderer, tileCount, spikeCount, customData);

		binary.EnsureSection("TLFG");
		for (int i = 0; i < tileCount; i++) {
			Uint16 x, y, xAtlas, yAtlas;
			Uint8 sourceID;

			binary.Read(2, &x);
			binary.Read(2, &y);
			binary.Read(2, &xAtlas);
			binary.Read(2, &yAtlas);
			binary.Read(1, &sourceID);

			this->CacheTile(renderer, i, x, y, xAtlas, yAtlas, sourceID, customData);
		}

		binary.EnsureSection("SPIK");
		for (int i = 0; i < spikeCount; i++) {
			Uint16 xSpike, ySpike;
			binary.Read(2, &xSpike);
			binary.Read(2, &ySpike);

			float xSpikeChunk = xSpike * WorldConstants::TILE_SIZE_F + OVERLAP_OFFSET - _rect.x;
			float ySpikeChunk = ySpike * WorldConstants::TILE_SIZE_F + OVERLAP_OFFSET - _rect.y;

			Uint8 spikeMask;
			binary.Read(1, &spikeMask);

			float xAtlas = (spikeMask & 15) / 16.0f;  //	% 16
			float yAtlas = (spikeMask >> 4) / 16.0f;  //	/ 16

			this->CacheSpike(renderer, i, xSpikeChunk, ySpikeChunk, xAtlas, yAtlas, customData);
		}

		this->FinalizeCache(renderer, customData, tileCount, spikeCount);
	}

	virtual void PrepareCache(SDL_Renderer* renderer, int tileCount, int spikeCount, void* customData) = 0;
	virtual void CacheTile(SDL_Renderer* renderer, int index, Uint16 x, Uint16 y, Uint16 xAtlas, Uint16 yAtlas,
						   Uint8 sourceID, void* customData) = 0;
	virtual void CacheSpike(SDL_Renderer* renderer, int index, float x, float y, float xAtlas, float yAtlas,
							void* customData) = 0;
	virtual void FinalizeCache(SDL_Renderer* renderer, void* customData, int tileCount, int spikeCount) = 0;

   public:
	int GetWidth() const { return _rect.w; }
	int GetHeight() const { return _rect.h; }

	const SDL_Rect& GetRect() const { return _rect; }

	SDL_FRect GetFRect() const { return SDL_FRect{float(_rect.x), float(_rect.y), float(_rect.w), float(_rect.h)}; }
};