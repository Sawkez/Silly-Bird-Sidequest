#pragma once

#include <iostream>
#include <memory>
#include <unordered_set>
#include <vector>

#include "engine/IProcessable.hpp"
#include "engine/PlatformDefines.hpp"
#include "engine/PointHash.hpp"
#include "engine/Vector2.hpp"
#include "engine/graphics/IDrawableRect.hpp"
#include "engine/performance/PerformanceManager.hpp"
#include "engine/physics/CollisionRect.hpp"
#include "engine/physics/RoomColliderContainer.hpp"
#include "engine/physics/SpikeCollider.hpp"
#include "engine/physics/SpikeColliderContainer.hpp"
#include "engine/resource/BinaryReader.hpp"
#include "engine/resource/ResourceManager.hpp"
#include "engine/resource/StorageIO.hpp"
#include "engine/world/IRoomObject.hpp"
#include "engine/world/RoomNeighbor.hpp"
#include "game/player/IPlayer.hpp"
#include "game/world/objects/RoomObjectFactory.hpp"
#include "yyjson.h"

#ifdef PLATFORM_USE_TEXTURE_CHUNKS
#include "engine/graphics/RoomTextureChunkRenderer.hpp"
#else
#include "engine/graphics/RoomMeshChunkRenderer.hpp"
#endif

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
	RoomColliderContainer _colliders;
	SpikeColliderContainer _spikeColliders;
	unordered_set<SDL_Point> _ledges;
	vector<RoomNeighbor> _neighbors;
	vector<Vector2> _checkpoints;
	vector<std::unique_ptr<IRoomObject>> _roomObjects;

	std::unique_ptr<IDrawableRect> _chunkRenderer;

   public:
	Room(SDL_Storage* storage, const std::string& path, SDL_Renderer* renderer) : _binary(storage, path) {
		PerformanceManager::instance->SetProfile(PerformanceManagerBase::PROFILE_LOADING);

		dc::msg << SDL_GetTicks() << ": Loading room " << path << " properties" << dc::endl;
		_binary.EnsureSection("PROP");
		_binary.Read(8, &_xPosition);
		_binary.Read(8, &_yPosition);
		_binary.Read(2, &_width);
		_binary.Read(2, &_height);
		_binary.Read(2, &_targetWidth);
		_binary.Read(2, &_targetHeight);

		Uint8 chunkCount;
		_binary.Read(1, &chunkCount);
		Uint8 checkpointCount;
		_binary.Read(1, &checkpointCount);
		Uint8 neighborCount;
		_binary.Read(1, &neighborCount);
		Uint32 ledgeCount;
		_binary.Read(4, &ledgeCount);
		Uint16 objectCount;
		_binary.Read(2, &objectCount);

		dc::msg << SDL_GetTicks() << ": Room size is " << _width << "x" << _height << dc::endl;

#ifdef PLATFORM_USE_TEXTURE_CHUNKS
		_chunkRenderer = std::make_unique<RoomTextureChunkRenderer>(renderer, storage, chunkCount, _binary);
#else
		_chunkRenderer = std::make_unique<RoomMeshChunkRenderer>(renderer, storage, chunkCount, _binary);
#endif

		int tileCountX = _width / 8;
		int tileCountY = _height / 8;

		dc::msg << SDL_GetTicks() << ": Loading tile colliders" << dc::endl;
		_binary.EnsureSection("TLCL");
		_colliders =
			RoomColliderContainer(_binary.GetCurrentPosition(), tileCountX, tileCountY, _xPosition, _yPosition);

		dc::msg << SDL_GetTicks() << ": Loading spike colliders" << dc::endl;
		_binary.EnsureSection("SKCL");
		_spikeColliders =
			SpikeColliderContainer(_binary.GetCurrentPosition(), tileCountX, tileCountY, _xPosition, _yPosition);

		dc::msg << SDL_GetTicks() << ": Loading " << checkpointCount << " checkpoints" << dc::endl;
		_binary.EnsureSection("CKPT");
		_checkpoints.reserve(checkpointCount);
		for (int i = 0; i < checkpointCount; i++) {
			Vector2 checkpoint;
			_binary.Read(4, &checkpoint.x);
			_binary.Read(4, &checkpoint.y);
			_checkpoints.push_back(checkpoint);
		}

		dc::msg << SDL_GetTicks() << ": Loading " << neighborCount << " neighbors" << dc::endl;
		_binary.EnsureSection("NGBR");
		_neighbors.reserve(neighborCount);
		for (int i = 0; i < neighborCount; i++) {
			_neighbors.emplace_back(_binary);
		}

		dc::msg << SDL_GetTicks() << ": Loading " << ledgeCount << " ledges" << dc::endl;
		_binary.EnsureSection("LEGE");
		_ledges.reserve(ledgeCount);
		for (int i = 0; i < ledgeCount; i++) {
			Sint32 x, y;
			_binary.Read(4, &x);
			_binary.Read(4, &y);

			SDL_Point ledge{x, y};

			_ledges.emplace(ledge);
		}

		dc::msg << SDL_GetTicks() << ": Loading " << objectCount << " room objects" << dc::endl;
		_roomObjects.reserve(objectCount);
		for (int i = 0; i < objectCount; i++) {
			_binary.FindNextSection("OBJT");
			_roomObjects.push_back(RoomObjectFactory::MakeRoomObject(_binary));
		}

		SDL_SetRenderTarget(renderer, nullptr);
		dc::msg << SDL_GetTicks() << ": Room load done!" << dc::endl;
		PerformanceManager::instance->SetProfile(PerformanceManagerBase::PROFILE_GAMEPLAY);
	}

	Room(const Room&) = delete;
	Room& operator=(const Room&) = delete;

	Room& operator=(Room&& other) noexcept = default;

	const RoomColliderContainer& GetColliders() const { return _colliders; }
	const SpikeColliderContainer& GetSpikeColliders() const { return _spikeColliders; }

	Vector2 GetPosition() const { return Vector2{float(_xPosition), float(_yPosition)}; }

	Vector2 GetSize() const { return Vector2{float(_width), float(_height)}; }

	int GetWidth() const { return _width; }

	int GetHeight() const { return _height; }

	SDL_FRect GetFRect() const {
		return SDL_FRect{float(_xPosition), float(_yPosition), float(_width), float(_height)};
	}

	Vector2 GetTargetSize() const { return Vector2{float(_targetWidth), float(_targetHeight)}; }

	const unordered_set<SDL_Point>& GetLedges() const { return _ledges; }

	// const vector<RoomChunk>& GetChunks() const { return _chunks; }

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

	const vector<std::unique_ptr<IRoomObject>>& GetRoomObjects() const { return _roomObjects; }

	void Process(float delta, IPlayer& player) {
		for (auto& object : _roomObjects) {
			object->Process(delta, player);
		}
	}

	bool Draw(SDL_Renderer* renderer, const SDL_FRect& drawTargetRect, Vector2 drawOffset) const override {
		bool drawn = false;

		Vector2 localOffset = drawOffset + GetPosition();

		drawn |= _chunkRenderer->Draw(renderer, drawTargetRect, localOffset);

		for (const auto& object : _roomObjects) {
			drawn |= object->Draw(renderer, drawTargetRect, drawOffset);
		}

		// _colliders.Draw(renderer, drawTargetRect, drawOffset);

		return drawn;
	}

	~Room() {
		_ledges.clear();
		_neighbors.clear();
		_roomObjects.clear();
	}
};