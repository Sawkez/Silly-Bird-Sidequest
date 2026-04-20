#pragma once

#include <iostream>
#include <vector>

#include "engine/IProcessable.hpp"
#include "engine/ResourceManager.hpp"
#include "engine/Vector2.hpp"
#include "engine/physics/CollisionRect.hpp"
#include "engine/world/IRoomObject.hpp"
#include "engine/world/RoomChunk.hpp"
#include "engine/world/RoomNeighbor.hpp"
#include "game/physics/SpikeCollider.hpp"
#include "game/player/IPlayer.hpp"
#include "game/world/objects/RoomObjectFactory.hpp"
#include "yyjson.h"

using namespace std;

class Room {
   private:
	vector<CollisionRect> _colliders;
	vector<SpikeCollider> _spikeColliders;
	vector<SDL_Point> _ledges;
	int _width;
	int _height;
	int _targetWidth;
	int _targetHeight;
	int _xPosition;
	int _yPosition;
	vector<RoomChunk> _chunks;
	vector<RoomNeighbor> _neighbors;
	vector<Vector2> _checkpoints;
	vector<IRoomObject*> _roomObjects;

   public:
	Room(const string& folderPath, SDL_Renderer* renderer, vector<SDL_Surface*> atlases, SDL_Surface* spikeAtlas)
		: Room(folderPath, ResourceManager::LoadJson(folderPath + "/room.json"), renderer, atlases, spikeAtlas) {}

	Room(const string& folderPath, yyjson_doc* jsonDoc, SDL_Renderer* renderer, vector<SDL_Surface*> atlases, SDL_Surface* spikeAtlas)
		: Room(folderPath, yyjson_doc_get_root(jsonDoc), renderer, atlases, spikeAtlas) {
		yyjson_doc_free(jsonDoc);
	}

	Room(const string& folderPath, yyjson_val* roomJson, SDL_Renderer* renderer, vector<SDL_Surface*> atlases, SDL_Surface* spikeAtlas)
		: _colliders(LoadColliders(yyjson_obj_get(roomJson, "collisions"))),
		  _spikeColliders(LoadSpikeColliders(folderPath, yyjson_get_int(yyjson_obj_get(roomJson, "spike_count")))),
		  _chunks(LoadChunks(folderPath, yyjson_obj_get(roomJson, "chunks"), renderer, atlases, spikeAtlas)),
		  _ledges(LoadLedges(yyjson_obj_get(roomJson, "ledges"))),
		  _width(yyjson_get_num(yyjson_obj_get(roomJson, "width"))),
		  _height(yyjson_get_num(yyjson_obj_get(roomJson, "height"))),
		  _targetWidth(yyjson_get_num(yyjson_obj_get(roomJson, "target_width"))),
		  _targetHeight(yyjson_get_num(yyjson_obj_get(roomJson, "target_height"))),
		  _xPosition(yyjson_get_num(yyjson_obj_get(roomJson, "position_x"))),
		  _yPosition(yyjson_get_num(yyjson_obj_get(roomJson, "position_y"))),
		  _neighbors(LoadNeighbors(yyjson_obj_get(roomJson, "neighbors"))),
		  _checkpoints(LoadCheckpoints(yyjson_obj_get(roomJson, "checkpoints"))),
		  _roomObjects(LoadRoomObjects(yyjson_obj_get(roomJson, "room_objects"))) {
		cout << SDL_GetTicks() << ": finished room load" << endl;
	}

	Room(const Room&) = delete;
	Room& operator=(const Room&) = delete;

	Room& operator=(Room&& other) noexcept = default;

	vector<CollisionRect> LoadColliders(yyjson_val* collidersJson) const {
		cout << SDL_GetTicks() << ": loading colliders" << endl;
		vector<CollisionRect> colliders;

		size_t idx, max;
		yyjson_val* collider;
		yyjson_arr_foreach(collidersJson, idx, max, collider) { colliders.emplace_back(collider); }

		return colliders;
	}

	vector<SpikeCollider> LoadSpikeColliders(const string& folderPath, int spikeCount) const {
		cout << SDL_GetTicks() << ": loading spike colliders" << endl;
		cout << "Spike count now " << spikeCount << endl;
		vector<SpikeCollider> spikes;

		SDL_IOStream* file = SDL_IOFromFile((folderPath + "/spikes.ow").c_str(), "rb");

		if (file == NULL) {
			cout << "ERROR: could not open spikes.ow: " << SDL_GetError() << endl;
		}

		for (int i = 0; i < spikeCount; i++) {
			spikes.emplace_back(file);
		}

		SDL_CloseIO(file);

		return spikes;
	}

	vector<RoomChunk> LoadChunks(const string& folderPath, yyjson_val* chunksJson, SDL_Renderer* renderer, vector<SDL_Surface*> atlases,
								 SDL_Surface* spikeAtlas) const {
		vector<RoomChunk> chunks;

		size_t idx, max;
		yyjson_val* chunk;

		yyjson_arr_foreach(chunksJson, idx, max, chunk) {
			cout << SDL_GetTicks() << ": loading chunk " << idx << endl;
			chunks.emplace_back(folderPath + "/" + to_string(idx), chunk, renderer, atlases, spikeAtlas);
		}

		return chunks;
	}

	vector<SDL_Point> LoadLedges(yyjson_val* ledgesJson) const {
		cout << SDL_GetTicks() << ": loading ledges" << endl;
		vector<SDL_Point> ledges;

		size_t idx, max;
		yyjson_val* ledge;

		yyjson_arr_foreach(ledgesJson, idx, max, ledge) {
			ledges.push_back({yyjson_get_int(yyjson_obj_get(ledge, "x")), yyjson_get_int(yyjson_obj_get(ledge, "y"))});
		}
		return ledges;
	}

	vector<RoomNeighbor> LoadNeighbors(yyjson_val* neighborsJson) const {
		cout << SDL_GetTicks() << ": loading neighbors" << endl;
		vector<RoomNeighbor> neighbors;

		size_t idx, max;
		yyjson_val* neighbor;

		yyjson_arr_foreach(neighborsJson, idx, max, neighbor) { neighbors.emplace_back(neighbor); }
		return neighbors;
	}

	vector<Vector2> LoadCheckpoints(yyjson_val* checkpointsJson) const {
		cout << SDL_GetTicks() << ": loading checkpoints" << endl;
		vector<Vector2> checkpoints;

		size_t idx, max;
		yyjson_val* checkpoint;

		yyjson_arr_foreach(checkpointsJson, idx, max, checkpoint) { checkpoints.emplace_back(checkpoint); }

		return checkpoints;
	}

	vector<IRoomObject*> LoadRoomObjects(yyjson_val* objectsJson) const {
		cout << SDL_GetTicks() << ": loading room objects" << endl;
		vector<IRoomObject*> objects;

		size_t idx, max;
		yyjson_val* object;

		yyjson_arr_foreach(objectsJson, idx, max, object) { objects.push_back(RoomObjectFactory::MakeRoomObject(object)); }

		return objects;
	}

	const vector<CollisionRect>& GetColliders() const { return _colliders; };
	const vector<SpikeCollider>& GetSpikeColliders() const { return _spikeColliders; }

	void Draw(SDL_Renderer* renderer) const {
		for (const auto& chunk : _chunks) {
			chunk.Draw(renderer);
		}
	}

	Vector2 GetPosition() const { return Vector2{float(_xPosition), float(_yPosition)}; }

	Vector2 GetSize() const { return Vector2{float(_width), float(_height)}; }

	int GetWidth() const { return _width; }

	int GetHeight() const { return _height; }

	SDL_FRect GetFRect() const { return SDL_FRect{float(_xPosition), float(_yPosition), float(_width), float(_height)}; }

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

	Vector2 GetNearestCheckpointPosition(const Vector2& position) { return GetCheckpoint(GetNearestCheckpoint(position)); }

	const vector<IRoomObject*>& GetRoomObjects() const { return _roomObjects; }

	void Process(float delta, IPlayer& player) {
		for (auto object : _roomObjects) {
			object->Process(delta, player);
		}
	}

	~Room() {
		_colliders.clear();
		_ledges.clear();
		_chunks.clear();
		_neighbors.clear();
	}
};