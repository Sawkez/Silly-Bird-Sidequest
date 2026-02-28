#pragma once

#include <SDL.h>
#include <fstream>
#include <vector>

#include "CollisionRect.hpp"
#include "GameState.hpp"
#include "IDrawable.hpp"
#include "IProcessable.hpp"
#include "InputManager.hpp"
#include "Json.hpp"
#include "Player.hpp"
#include "RenderChunk.hpp"
#include "Room.hpp"
#include "RoomCamera.hpp"
#include "RoomNeighbor.hpp"
#include "TileSheetSource.hpp"

using namespace std;
using json = nlohmann::json;

class Level : IProcessable, IDrawable {
  private:
	string _path;
	SDL_Renderer* _renderer;
	vector<SDL_Surface*> _atlases;
	Room _currentRoom;
	Player _player;
	RoomCamera _roomCamera;
	vector<RenderChunk> _renderChunks;
	GameState& _state;

  public:
	Level(const json& levelProperties, const string& pathToFolder, SDL_Renderer* renderer,
		  const InputManager& inputManager, SDL_Window* window, GameState& state)
		: _path(pathToFolder), _currentRoom(LoadRoom(levelProperties.at("starting_room"))), _renderer(renderer),
		  _atlases(LoadAtlases(levelProperties, pathToFolder)),
		  _player(Player(inputManager, renderer, _currentRoom.GetColliders())),
		  _roomCamera(_player, _currentRoom, window), _renderChunks(CreateRenderChunks(_currentRoom, renderer)),
		  _state(state) {
		cout << "Finished loading level " << pathToFolder << "!!!" << endl;
		_player.position.x = levelProperties.at("player_x");
		_player.position.y = levelProperties.at("player_y");

		_currentRoom.CacheTiles(renderer, _atlases);
	}

	Level(const string& pathToFolder, SDL_Renderer* renderer, const InputManager& inputManager, SDL_Window* window,
		  GameState& state)
		: Level(LoadJson(pathToFolder), pathToFolder, renderer, inputManager, window, state) {}

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

	Room LoadRoom(int index) {
		cout << SDL_GetTicks64() << ": starting room load" << endl;
		return Room(_path + "/rooms/" + to_string(index));
	}

	void Process(float delta) override {
		_player.Process(delta);
		_roomCamera.Process(delta);

		for (const auto& neighbor : _currentRoom.GetNeighbors()) {
			// TODO prevent player from getting stuck in a loop of re-entering the same 2 rooms
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
				chunk.Draw(renderer, drawOffset, zoom);
			}
		}
	}

	void SetCurrentRoom(int room) {
		_state.Pause();
		_currentRoom = LoadRoom(room);

		_currentRoom.CacheTiles(_renderer, _atlases);
		_player.SetStaticColliders(_currentRoom.GetColliders());
		_roomCamera.SetRoom(_currentRoom);

		DestroyRenderChunks();
		_renderChunks = CreateRenderChunks(_currentRoom, _renderer);
		_state.Unpause();
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