#pragma once

#include <iostream>
#include <vector>

#include "engine/IProcessable.hpp"
#include "engine/Vector2.hpp"
#include "engine/graphics/IDrawableRect.hpp"
#include "engine/physics/CollisionRect.hpp"
#include "engine/resource/BinaryReader.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "engine/resource/StorageIO.hpp"
#include "engine/world/IRoomObject.hpp"
#include "engine/world/RoomChunk.hpp"
#include "engine/world/RoomNeighbor.hpp"
#include "game/physics/SpikeCollider.hpp"
#include "game/player/IPlayer.hpp"
#include "game/world/objects/RoomObjectFactory.hpp"
#include "yyjson.h"

using namespace std;

class Room : IDrawableRect {
   private:
	BinaryReader _binary;
	Sint64 _xPosition;
	Sint64 _yPosition;
	Uint16 _width;
	Uint16 _height;
	Uint16 _targetWidth;
	Uint16 _targetHeight;
	vector<RoomChunk> _chunks;
	vector<CollisionRect> _colliders;
	vector<SpikeCollider> _spikeColliders;
	vector<SDL_Point> _ledges;
	vector<RoomNeighbor> _neighbors;
	vector<Vector2> _checkpoints;
	vector<IRoomObject*> _roomObjects;

   public:
	Room(SDL_Storage* storage, const std::string& path, SDL_Renderer* renderer, SDL_Texture* spikeAtlas)
		: _binary(storage, path) {
		dc::msg << SDL_GetTicks() << ": Loading room " << path << " properties" << dc::endl;
		_binary.FindSection("PROP");
		_binary.Read(8, &_xPosition);
		_binary.Read(8, &_yPosition);
		_binary.Read(2, &_width);
		_binary.Read(2, &_height);
		_binary.Read(2, &_targetWidth);
		_binary.Read(2, &_targetHeight);

		Uint8 chunkCount;
		_binary.Read(1, &chunkCount);
		Uint32 colliderCount;
		_binary.Read(4, &colliderCount);
		Uint32 spikeColliderCount;
		_binary.Read(4, &spikeColliderCount);
		Uint8 checkpointCount;
		_binary.Read(1, &checkpointCount);
		Uint8 neighborCount;
		_binary.Read(1, &neighborCount);
		Uint32 ledgeCount;
		_binary.Read(4, &ledgeCount);
		Uint16 objectCount;
		_binary.Read(2, &objectCount);

		dc::msg << SDL_GetTicks() << ": Room size is " << _width << "x" << _height << dc::endl;

		std::unordered_map<Uint8, SDL_Texture*> atlases;
		for (int i = 0; i < chunkCount; i++) {
			dc::msg << SDL_GetTicks() << ": Loading chunk " << i << dc::endl;
			_chunks.emplace_back(storage, _binary, renderer, atlases, spikeAtlas);
		}

		dc::msg << SDL_GetTicks() << ": Loading " << colliderCount << " tile colliders" << dc::endl;
		_binary.FindSection("TLCL");
		_colliders.reserve(colliderCount);
		for (int i = 0; i < colliderCount; i++) {
			CollisionRect collider;
			_binary.Read(4, &collider.x);
			_binary.Read(4, &collider.y);
			_binary.Read(4, &collider.w);
			_binary.Read(4, &collider.h);
			_colliders.push_back(collider);
		}

		dc::msg << SDL_GetTicks() << ": Loading " << spikeColliderCount << " spike colliders" << dc::endl;
		_binary.FindSection("SKCL");
		_spikeColliders.reserve(spikeColliderCount);
		for (int i = 0; i < spikeColliderCount; i++) {
			_spikeColliders.emplace_back(_binary);
		}

		dc::msg << SDL_GetTicks() << ": Loading " << checkpointCount << " checkpoints" << dc::endl;
		_binary.FindSection("CKPT");
		_checkpoints.reserve(checkpointCount);
		for (int i = 0; i < checkpointCount; i++) {
			Vector2 checkpoint;
			_binary.Read(4, &checkpoint.x);
			_binary.Read(4, &checkpoint.y);
			_checkpoints.push_back(checkpoint);
		}

		dc::msg << SDL_GetTicks() << ": Loading " << neighborCount << " neighbors" << dc::endl;
		_binary.FindSection("NGBR");
		_neighbors.reserve(neighborCount);
		for (int i = 0; i < neighborCount; i++) {
			_neighbors.emplace_back(_binary);
		}

		dc::msg << SDL_GetTicks() << ": Loading " << ledgeCount << " ledges" << dc::endl;
		_binary.FindSection("LEGE");
		_ledges.reserve(ledgeCount);
		for (int i = 0; i < ledgeCount; i++) {
			SDL_Point ledge;
			_binary.Read(8, &ledge.x);
			_binary.Read(8, &ledge.y);
			_ledges.push_back(ledge);
		}

		dc::msg << SDL_GetTicks() << ": Loading " << objectCount << " room objects" << dc::endl;
		_roomObjects.reserve(objectCount);
		for (int i = 0; i < objectCount; i++) {
			_binary.FindNextSection("OBJT");
			_roomObjects.push_back(RoomObjectFactory::MakeRoomObject(_binary));
		}

		SDL_SetRenderTarget(renderer, nullptr);
		dc::msg << SDL_GetTicks() << ": Room load done!" << dc::endl;
	}

	Room(const Room&) = delete;
	Room& operator=(const Room&) = delete;

	Room& operator=(Room&& other) noexcept = default;

	const vector<CollisionRect>& GetColliders() const { return _colliders; };
	const vector<SpikeCollider>& GetSpikeColliders() const { return _spikeColliders; }

	Vector2 GetPosition() const { return Vector2{float(_xPosition), float(_yPosition)}; }

	Vector2 GetSize() const { return Vector2{float(_width), float(_height)}; }

	int GetWidth() const { return _width; }

	int GetHeight() const { return _height; }

	SDL_FRect GetFRect() const {
		return SDL_FRect{float(_xPosition), float(_yPosition), float(_width), float(_height)};
	}

	Vector2 GetTargetSize() const { return Vector2{float(_targetWidth), float(_targetHeight)}; }

	const vector<SDL_Point>& GetLedges() const { return _ledges; }

	const vector<RoomChunk>& GetChunks() const { return _chunks; }

	const vector<RoomNeighbor>& GetNeighbors() const { return _neighbors; }

	int GetNearestCheckpoint(const Vector2& position) {
		float dist = INFINITY;
		int nearest = -1;

		for (int i = 0; i < _checkpoints.size(); i++) {
			float newDist = _checkpoints[i].DistanceSquared(position);
			if (newDist < dist) {
				dist = newDist;
				nearest = i;
			}
		}

		return nearest;
	}

	Vector2 GetCheckpoint(int index) {
		if (index < 0) return Vector2{float(_xPosition), float(_yPosition)};
		return _checkpoints[index];
	}

	Vector2 GetNearestCheckpointPosition(const Vector2& position) {
		return GetCheckpoint(GetNearestCheckpoint(position));
	}

	const vector<IRoomObject*>& GetRoomObjects() const { return _roomObjects; }

	void Process(float delta, IPlayer& player) {
		for (auto object : _roomObjects) {
			object->Process(delta, player);
		}
	}

	bool Draw(SDL_Renderer* renderer, const SDL_FRect& drawTargetRect, Vector2 drawOffset) const override {
		bool drawn = false;

		Vector2 localOffset = drawOffset + GetPosition();

		for (const auto& chunk : _chunks) {
			drawn |= chunk.Draw(renderer, drawTargetRect, localOffset - Vector2(8, 8));
		}

		for (const auto* object : _roomObjects) {
			drawn |= object->Draw(renderer, drawTargetRect, drawOffset);
		}

		return drawn;
	}

	~Room() {
		_colliders.clear();
		_ledges.clear();
		_chunks.clear();
		_neighbors.clear();
	}
};