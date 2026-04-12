#pragma once

#include <SDL.h>

#include <fstream>
#include <vector>

#include "engine/GameState.hpp"
#include "engine/IProcessable.hpp"
#include "engine/graphics/IDrawable.hpp"
#include "engine/graphics/RenderChunk.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/physics/CollisionRect.hpp"
#include "engine/world/Room.hpp"
#include "engine/world/RoomCamera.hpp"
#include "engine/world/RoomNeighbor.hpp"
#include "game/player/Player.hpp"
#include "yyjson.h"

using namespace std;

class Level : IProcessable, IDrawable {
   private:
	string _path;
	SDL_Renderer* _renderer;
	vector<SDL_Surface*> _atlases;
	SDL_Surface* _spikeAtlas;
	Room _currentRoom;
	Player _player;
	RoomCamera _roomCamera;
	vector<RenderChunk> _renderChunks;

   public:
	Level(const string& pathToFolder) : Level(pathToFolder, GameState::GetMainRenderer(), GameState::GetInput(), GameState::GetMainWindow()) {}

	Level(const string& pathToFolder, SDL_Renderer* renderer, const InputManager& inputManager, SDL_Window* window)
		: Level(LoadJson(pathToFolder), pathToFolder, renderer, inputManager, window) {}

	Level(yyjson_doc* json, const string& pathToFolder, SDL_Renderer* renderer, const InputManager& inputManager, SDL_Window* window)
		: Level(yyjson_doc_get_root(json), pathToFolder, renderer, inputManager, window) {
		yyjson_doc_free(json);
	}

	Level(yyjson_val* levelProperties, const string& pathToFolder, SDL_Renderer* renderer, const InputManager& inputManager, SDL_Window* window)
		: _path(pathToFolder),
		  _atlases(LoadAtlases(yyjson_obj_get(levelProperties, "tilesheet_sources"), pathToFolder)),
		  _spikeAtlas(IMG_Load("content/sidequest/tiles/special/spikes.png")),
		  _currentRoom(GetRoomPath(yyjson_get_int(yyjson_obj_get(levelProperties, "starting_room"))), _renderer, _atlases, _spikeAtlas),
		  _renderer(renderer),
		  _player(Player(inputManager, renderer, _currentRoom, yyjson_get_int(yyjson_obj_get(levelProperties, "starting_upgrades")))),
		  _roomCamera(_player, _currentRoom, window),
		  _renderChunks(CreateRenderChunks(_currentRoom, renderer)) {
		cout << "Finished loading level " << pathToFolder << "!!!" << endl;
		_player.position.x = (float)yyjson_get_num(yyjson_obj_get(levelProperties, "player_x"));
		_player.position.y = (float)yyjson_get_num(yyjson_obj_get(levelProperties, "player_y"));

		_player.SetRespawnPosition(_currentRoom.GetNearestCheckpoint(_player.position));

		GameState::Unpause();
	}

	yyjson_doc* LoadJson(const string& pathToFolder) const {
		ifstream jsonFile(pathToFolder + "/level.json");
		if (!jsonFile.good()) {
			cerr << "ERROR opening level.json file in " << pathToFolder << endl;
		}

		std::string jsonString((istreambuf_iterator<char>(jsonFile)), (istreambuf_iterator<char>()));

		return yyjson_read(jsonString.data(), jsonString.length(), 0);
	}

	vector<SDL_Surface*> LoadAtlases(yyjson_val* sources, const string& pathToFolder) const {
		vector<SDL_Surface*> atlases;

		size_t idx, max;
		yyjson_val* source;
		yyjson_arr_foreach(sources, idx, max, source) {
			bool custom = yyjson_get_bool(yyjson_obj_get(source, "custom"));

			string path = custom ? pathToFolder : "content/sidequest";
			path += "/tiles/fg/";
			path += yyjson_get_str(yyjson_obj_get(source, "name"));

			atlases.push_back(IMG_Load(path.data()));
		}

		return atlases;
	}

	std::string GetRoomPath(int index) { return _path + "/rooms/" + to_string(index); }

	void Process(float delta) override {
		_player.Process(delta);
		_roomCamera.Process(delta);
		CheckRoomTransition();
	}

	void CheckRoomTransition() {
		SDL_FRect currentRoomRect = _currentRoom.GetFRect();

		if (SDL_HasIntersectionF(&_player.GetCollision(), &currentRoomRect)) {
			return;
		}

		for (const auto& neighbor : _currentRoom.GetNeighbors()) {
			if (SDL_HasIntersectionF(&_player.GetCollision(), &neighbor)) {
				SetCurrentRoom(neighbor.index);
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
				chunk.DrawObject(renderer, _player, _currentRoom.GetPosition());

				for (const auto& object : _currentRoom.GetRoomObjects()) {
					chunk.DrawObject(renderer, *object, _currentRoom.GetPosition());
				}

				chunk.Draw(renderer, drawOffset, zoom);
			}
		}
	}

	void SetCurrentRoom(int room) {
		cout << "Entered room " << room << endl;
		GameState::Pause();
		_currentRoom = Room(GetRoomPath(room), _renderer, _atlases, _spikeAtlas);

		_player.SetRoom(_currentRoom);
		_player.SetRespawnPosition(_currentRoom.GetNearestCheckpoint(_player.position));
		_roomCamera.SetRoom(_currentRoom);

		DestroyRenderChunks();
		_renderChunks = CreateRenderChunks(_currentRoom, _renderer);
		GameState::Unpause();
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

	Player& GetPlayer() { return _player; }
};