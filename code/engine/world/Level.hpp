#pragma once

#include <SDL.h>

#include <cstring>
#include <fstream>
#include <vector>

#include "engine/GameState.hpp"
#include "engine/IProcessable.hpp"
#include "engine/graphics/IDrawable.hpp"
#include "engine/graphics/RenderChunk.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/physics/CollisionRect.hpp"
#include "engine/save/SaveData.hpp"
#include "engine/save/SaveManager.hpp"
#include "engine/world/Room.hpp"
#include "engine/world/RoomCamera.hpp"
#include "engine/world/RoomNeighbor.hpp"
#include "game/player/Player.hpp"
#include "yyjson.h"

using namespace std;

class Level : IProcessable, IDrawable {
   private:
	static inline SDL_Surface* _spikeAtlas = IMG_Load("content/sidequest/tiles/special/spikes.png");

	string _path;
	SDL_Renderer* _renderer;
	vector<SDL_Surface*> _atlases;
	Room _currentRoom;
	Player _player;
	RoomCamera _roomCamera;
	vector<RenderChunk> _renderChunks;

   public:
	Level(const std::string& path, SDL_Renderer* renderer, const InputManager& inputManager, SDL_Window* window, vector<SDL_Surface*> tileAtlases,
		  int roomIndex, Uint8 playerUpgrades)
		: _path(path),
		  _atlases(tileAtlases),
		  _currentRoom(GetRoomPath(roomIndex), renderer, _atlases, _spikeAtlas),
		  _renderer(renderer),
		  _player(inputManager, renderer, _currentRoom, playerUpgrades),
		  _roomCamera(_player, _currentRoom, window),
		  _renderChunks(CreateRenderChunks(_currentRoom, renderer)) {}

	std::string GetRoomPath(int index) { return _path + "/rooms/" + to_string(index); }

	void Process(float delta) override {
		_player.Process(delta);
		_currentRoom.Process(delta, _player);
		_roomCamera.Process(delta);
		CheckRoomTransition();
	}

	void CheckRoomTransition() {
		SDL_FRect currentRoomRect = _currentRoom.GetFRect();

		if (SDL_HasRectIntersectionFloat(&_player.GetCollision(), &currentRoomRect)) {
			return;
		}

		for (const auto& neighbor : _currentRoom.GetNeighbors()) {
			if (SDL_HasRectIntersectionFloat(&_player.GetCollision(), &neighbor)) {
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
			if (SDL_HasRectIntersection(&camRect, &chunkRect)) {
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
		_roomCamera.SetRoom(_currentRoom);

		UpdateCheckpoint();
		SaveManager::instance->saveData.room = room;
		SaveManager::instance->Autosave();

		DestroyRenderChunks();
		_renderChunks = CreateRenderChunks(_currentRoom, _renderer);
		GameState::Unpause();
	}

	void MovePlayerToCheckpoint(int checkpoint) {
		Vector2 position = _currentRoom.GetCheckpoint(checkpoint);
		_player.SetRespawnPosition(position);
		_player.Respawn();
	}

	void UpdateCheckpoint() {
		int checkpoint = _currentRoom.GetNearestCheckpoint(_player.position);
		_player.SetRespawnPosition(_currentRoom.GetCheckpoint(checkpoint));
		SaveManager::instance->saveData.checkpoint = checkpoint;
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