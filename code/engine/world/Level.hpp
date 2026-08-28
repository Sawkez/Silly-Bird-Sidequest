#pragma once

#include <SDL3/SDL.h>

#include <cstring>
#include <fstream>
#include <vector>

#include "engine/GameState.hpp"
#include "engine/IProcessable.hpp"
#include "engine/graphics/IDrawable.hpp"
#include "engine/input/InputManager.hpp"
#include "engine/physics/CollisionRect.hpp"
#include "engine/save/SaveData.hpp"
#include "engine/save/SaveManager.hpp"
#include "engine/world/Camera.hpp"
#include "engine/world/Room.hpp"
#include "engine/world/RoomNeighbor.hpp"
#include "game/player/Player.hpp"
#include "yyjson.h"

using namespace std;

class Level : IProcessable, IDrawable {
   private:
	SDL_Storage* _storage;
	string _path;
	SDL_Renderer* _renderer;
	Room _currentRoom;
	Player _player;
	Camera _camera;

   public:
	Level(SDL_Storage* storage, const std::string& path, SDL_Renderer* renderer, const InputManager& inputManager,
		  SDL_Window* window, int roomIndex, Uint8 playerUpgrades)
		: _storage(storage),
		  _path(path),
		  _currentRoom(storage, GetRoomPath(roomIndex), renderer),
		  _renderer(renderer),
		  _player(inputManager, renderer, _currentRoom, playerUpgrades),
		  _camera(_renderer, GameState::GetMainWindow(), _player, _currentRoom) {}

	std::string GetRoomPath(int index) { return _path + "rooms/" + to_string(index) + ".room"; }

	void Process(float delta) override {
		_player.Process(delta);
		_currentRoom.Process(delta, _player);
		_camera.Process(delta);
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

	void Draw(SDL_Renderer* renderer, Vector2 drawOffset = {}) const override { _camera.Draw(_renderer); }

	void SetCurrentRoom(int room) {
		GameState::Pause();
		_currentRoom = Room(_storage, GetRoomPath(room), _renderer);

		_player.SetRoom(_currentRoom);
		_camera.SetRoom(_currentRoom);

		UpdateCheckpoint();
		SaveManager::instance->saveData.room = room;
		SaveManager::instance->Autosave();

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

	Camera& GetCamera() { return _camera; }

	Player& GetPlayer() { return _player; }
};