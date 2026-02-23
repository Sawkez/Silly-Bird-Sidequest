#pragma once

#include <iostream>
#include <vector>

#include "CollisionRect.hpp"
#include "Json.hpp"
#include "RoomChunk.hpp"
#include "Vector2.hpp"

using namespace std;
using json = nlohmann::json;

class Room {
  private:
	vector<CollisionRect> _colliders;
	vector<Vector2> _ledges;
	int _width;
	int _height;
	int _targetWidth;
	int _targetHeight;
	int _xPosition;
	int _yPosition;
	vector<RoomChunk> _chunks;

  public:
	Room(const string& jsonPath) : Room(LoadJson(jsonPath)) {}

	Room(const json& roomJson)
		: _colliders(LoadColliders(roomJson)), _chunks(LoadChunks(roomJson)), _ledges(LoadLedges(roomJson)),
		  _width(roomJson["width"]), _height(roomJson["height"]), _targetWidth(roomJson["target_width"]),
		  _targetHeight(roomJson["target_height"]), _xPosition(roomJson["position_x"]),
		  _yPosition(roomJson["position_y"]) {}

	json LoadJson(const string& jsonPath) const {
		ifstream jsonFile(jsonPath);
		if (!jsonFile.good()) {
			cerr << "ERROR opening file: " << jsonPath << endl;
		}

		return json::parse(jsonFile);
	}

	vector<CollisionRect> LoadColliders(json roomJson) const {
		return roomJson.at("collisions").get<vector<CollisionRect>>();
	}

	vector<RoomChunk> LoadChunks(json roomJson) const { return roomJson.at("chunks").get<vector<RoomChunk>>(); }

	vector<Vector2> LoadLedges(json roomJson) const { return roomJson.at("ledges").get<vector<Vector2>>(); }

	const vector<CollisionRect>& GetColliders() const { return _colliders; };

	void CacheTiles(SDL_Renderer* renderer, const vector<SDL_Surface*>& atlases) {
		for (auto& chunk : _chunks) {
			chunk.CacheTiles(renderer, atlases);
		}
	}

	void UncacheTiles() {
		for (auto& chunk : _chunks) {
			chunk.UncacheTiles();
		}
	}

	void Draw(SDL_Renderer* renderer) const {
		for (const auto& chunk : _chunks) {
			chunk.Draw(renderer);
			SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
			SDL_RenderDrawRect(renderer, &chunk.rect);
		}
	}

	Vector2 GetPosition() const { return Vector2{float(_xPosition), float(_yPosition)}; }

	Vector2 GetSize() const { return Vector2{float(_width), float(_height)}; }

	int GetWidth() const { return _width; }

	int GetHeight() const { return _height; }

	SDL_FRect GetFRect() const {
		return SDL_FRect{float(_xPosition), float(_yPosition), float(_width), float(_height)};
	}

	Vector2 GetTargetSize() const { return Vector2{float(_targetWidth), float(_targetHeight)}; }

	const vector<RoomChunk>& GetChunks() const { return _chunks; }
};