#pragma once

#include <SDL.h>
#include <fstream>
#include <vector>

#include "CollisionRect.hpp"
#include "IDrawable.hpp"
#include "IProcessable.hpp"
#include "InputManager.hpp"
#include "Json.hpp"
#include "Player.hpp"
#include "RenderChunk.hpp"
#include "Room.hpp"
#include "RoomCamera.hpp"
#include "TileSheetSource.hpp"

using namespace std;
using json = nlohmann::json;

class Level : IProcessable, IDrawable {
  private:
	string _path;
	int _currentRoom = 0;
	SDL_Renderer* _renderer;
	vector<SDL_Surface*> _atlases;
	vector<Room> _rooms;
	Player _player;
	RoomCamera _roomCamera;
	vector<RenderChunk> _renderChunks;

  public:
	Level(const json& levelProperties, const string& pathToFolder, SDL_Renderer* renderer,
		  const InputManager& inputManager, SDL_Window* window)
		: _path(pathToFolder), _currentRoom(levelProperties.at("starting_room")), _renderer(renderer),
		  _rooms(LoadRooms(levelProperties, pathToFolder)), _atlases(LoadAtlases(levelProperties, pathToFolder)),
		  _player(Player(inputManager, renderer, _rooms.front().GetColliders())),
		  _roomCamera(_player, _rooms.at(_currentRoom), window),
		  _renderChunks(CreateRenderChunks(_rooms.at(_currentRoom), renderer)) {
		cout << "Finished loading level " << pathToFolder << "!!!" << endl;
		_player.position.x = levelProperties.at("player_x");
		_player.position.y = levelProperties.at("player_y");

		_rooms.at(_currentRoom).CacheTiles(renderer, _atlases);
	}

	Level(const string& pathToFolder, SDL_Renderer* renderer, const InputManager& inputManager, SDL_Window* window)
		: Level(LoadJson(pathToFolder), pathToFolder, renderer, inputManager, window) {}

	json LoadJson(const string& pathToFolder) const {
		ifstream jsonFile(pathToFolder + "/level.json");
		if (!jsonFile.good()) {
			cerr << "ERROR opening level.json file in " << pathToFolder << endl;
		}

		return json::parse(jsonFile);
	}

	vector<SDL_Surface*> LoadAtlases(const json& levelProperties, const string& pathToFolder) const {
		vector<TileSheetSource> sources = levelProperties.at("tilesheet_sources").get<vector<TileSheetSource>>();
		vector<SDL_Surface*> atlases;

		for (auto source : sources) {
			SDL_Surface* surface = IMG_Load(source.GetPath(pathToFolder).data());
			if (surface == NULL) {
				cerr << "ERROR: couldn't load tilesheet atlas " << source.GetPath(pathToFolder) << ": "
					 << SDL_GetError() << endl;
			}
			atlases.push_back(surface);
		}

		return atlases;
	};

	vector<Room> LoadRooms(const json& levelProperties, const string& pathToFolder) const {
		int roomCount = levelProperties["room_count"];
		cout << "Loading " << roomCount << " rooms" << endl;

		vector<Room> rooms;

		for (int i = 0; i < roomCount; i++) {
			rooms.push_back(Room(pathToFolder + "/rooms/" + to_string(i) + ".json"));
		}

		return rooms;
	};

	void Process(float delta) override {
		_player.Process(delta);
		_roomCamera.Process(delta);

		for (int i = 0; i < _rooms.size(); i++) {
			SDL_FRect roomRect = _rooms.at(i).GetFRect();
			if (SDL_HasIntersectionF(&_player.GetCollision(), &roomRect)) {
				SetCurrentRoom(i);
			}
		}
	}

	void Draw(SDL_Renderer* renderer, Vector2 drawOffset = {}) const override {
		drawOffset += _roomCamera.GetDrawOffset();

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);

		SDL_Rect camRect = _roomCamera.GetRect();
		float zoom = _roomCamera.GetZoom();

		for (const auto& chunk : _renderChunks) {
			SDL_Rect chunkRect = chunk.GetRect();
			if (SDL_HasIntersection(&camRect, &chunkRect)) {
				chunk.DrawRoom(renderer);
				chunk.DrawObject(renderer, _player, _rooms.at(_currentRoom).GetPosition());
				chunk.Draw(renderer, drawOffset, zoom);
			}
		}
	}

	void SetCurrentRoom(int room) {
		if (room == _currentRoom) {
			return;
		}

		cout << "Entered room " << room << endl;

		_rooms.at(_currentRoom).UncacheTiles();
		_currentRoom = room;
		Room& newRoom = _rooms.at(_currentRoom);
		newRoom.CacheTiles(_renderer, _atlases);
		_player.SetStaticColliders(newRoom.GetColliders());
		_roomCamera.SetRoom(newRoom);

		DestroyRenderChunks();
		_renderChunks = CreateRenderChunks(_rooms.at(_currentRoom), _renderer);
	}

	vector<RenderChunk> CreateRenderChunks(const Room& room, SDL_Renderer* renderer) const {
		vector<RenderChunk> renderChunks;
		const vector<RoomChunk>& chunks = room.GetChunks();
		renderChunks.reserve(chunks.size());

		for (const auto& roomChunk : room.GetChunks()) {
			renderChunks.emplace_back(roomChunk, renderer);
		}

		return renderChunks;
	}

	void DestroyRenderChunks() { _renderChunks.clear(); }

	RoomCamera& GetCamera() { return _roomCamera; }
};