#pragma once

#include <iostream>
#include <vector>

#include "engine/Vector2.hpp"
#include "engine/physics/CollisionRect.hpp"
#include "engine/world/RoomChunk.hpp"
#include "engine/world/RoomNeighbor.hpp"
#include "yyjson.h"

using namespace std;

class Room {
   private:
	vector<CollisionRect> _colliders;
	vector<SDL_Point> _ledges;
	int _width;
	int _height;
	int _targetWidth;
	int _targetHeight;
	int _xPosition;
	int _yPosition;
	vector<RoomChunk> _chunks;
	vector<RoomNeighbor> _neighbors;

   public:
	Room(const string& folderPath, SDL_Renderer* renderer, vector<SDL_Surface*> atlases, SDL_Surface* spikeAtlas)
		: Room(folderPath, LoadJson(folderPath + "/room.json"), renderer, atlases, spikeAtlas) {}
	Room(const string& folderPath, yyjson_val* roomJson, SDL_Renderer* renderer, vector<SDL_Surface*> atlases, SDL_Surface* spikeAtlas)
		: _colliders(LoadColliders(yyjson_obj_get(roomJson, "collisions"))),
		  _chunks(LoadChunks(folderPath, yyjson_obj_get(roomJson, "chunks"), renderer, atlases, spikeAtlas)),
		  _ledges(LoadLedges(yyjson_obj_get(roomJson, "ledges"))),
		  _width(yyjson_get_num(yyjson_obj_get(roomJson, "width"))),
		  _height(yyjson_get_num(yyjson_obj_get(roomJson, "height"))),
		  _targetWidth(yyjson_get_num(yyjson_obj_get(roomJson, "target_width"))),
		  _targetHeight(yyjson_get_num(yyjson_obj_get(roomJson, "target_height"))),
		  _xPosition(yyjson_get_num(yyjson_obj_get(roomJson, "position_x"))),
		  _yPosition(yyjson_get_num(yyjson_obj_get(roomJson, "position_y"))),
		  _neighbors(LoadNeighbors(yyjson_obj_get(roomJson, "neighbors"))) {
		cout << SDL_GetTicks64() << ": finished room load" << endl;
	}

	Room(const Room&) = delete;
	Room& operator=(const Room&) = delete;

	Room& operator=(Room&& other) noexcept {
		if (this != &other) {
			_colliders = move(other._colliders);
			_ledges = move(other._ledges);
			_width = other._width;
			_height = other._height;
			_targetWidth = other._targetWidth;
			_targetHeight = other._targetHeight;
			_xPosition = other._xPosition;
			_yPosition = other._yPosition;
			_chunks = move(other._chunks);
			_neighbors = move(other._neighbors);
		}

		return *this;
	}

	yyjson_val* LoadJson(const string& jsonPath) const {
		cout << SDL_GetTicks64() << ": loading JSON" << endl;
		ifstream jsonFile(jsonPath);
		if (!jsonFile.good()) {
			cerr << "ERROR opening file: " << jsonPath << endl;
		}

		std::string jsonString((istreambuf_iterator<char>(jsonFile)), (istreambuf_iterator<char>()));

		yyjson_doc* json = yyjson_read(jsonString.data(), jsonString.length(), 0);
		return yyjson_doc_get_root(json);
	}

	vector<CollisionRect> LoadColliders(yyjson_val* collidersJson) const {
		cout << SDL_GetTicks64() << ": loading colliders" << endl;
		vector<CollisionRect> colliders;

		size_t idx, max;
		yyjson_val* collider;
		yyjson_arr_foreach(collidersJson, idx, max, collider) { colliders.push_back(CollisionRect(collider)); }

		return colliders;
	}

	vector<RoomChunk> LoadChunks(const string& folderPath, yyjson_val* chunksJson, SDL_Renderer* renderer, vector<SDL_Surface*> atlases,
								 SDL_Surface* spikeAtlas) const {
		vector<RoomChunk> chunks;

		size_t idx, max;
		yyjson_val* chunk;

		yyjson_arr_foreach(chunksJson, idx, max, chunk) {
			cout << SDL_GetTicks64() << ": loading chunk " << idx << endl;
			chunks.push_back(RoomChunk(folderPath + "/" + to_string(idx), chunk, renderer, atlases, spikeAtlas));
		}

		return chunks;
	}

	vector<SDL_Point> LoadLedges(yyjson_val* ledgesJson) const {
		cout << SDL_GetTicks64() << ": loading ledges" << endl;
		vector<SDL_Point> ledges;

		size_t idx, max;
		yyjson_val* ledge;

		yyjson_arr_foreach(ledgesJson, idx, max, ledge) {
			ledges.push_back({yyjson_get_int(yyjson_obj_get(ledge, "x")), yyjson_get_int(yyjson_obj_get(ledge, "y"))});
		}
		return ledges;
	}

	vector<RoomNeighbor> LoadNeighbors(yyjson_val* neighborsJson) const {
		cout << SDL_GetTicks64() << ": loading neighbors" << endl;
		vector<RoomNeighbor> neighbors;

		size_t idx, max;
		yyjson_val* neighbor;

		yyjson_arr_foreach(neighborsJson, idx, max, neighbor) { neighbors.push_back(RoomNeighbor(neighbor)); }
		return neighbors;
	}

	const vector<CollisionRect>& GetColliders() const { return _colliders; };

	void Draw(SDL_Renderer* renderer) const {
		for (const auto& chunk : _chunks) {
			chunk.Draw(renderer);
			SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
			SDL_RenderDrawRect(renderer, &chunk.GetRect());
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

	~Room() {
		_colliders.clear();
		_ledges.clear();
		_chunks.clear();
		_neighbors.clear();
	}
};