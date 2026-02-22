#pragma once

#include <fstream>
#include <vector>

#include <SDL.h>

#include "Json.hpp"

#include "Player.hpp"
#include "InputManager.hpp"
#include "Room.hpp"
#include "CollisionRect.hpp"
#include "IProcessable.hpp"
#include "IDrawable.hpp"
#include "TileSheetSource.hpp"
#include "RoomCamera.hpp"

using namespace std;
using json = nlohmann::json;

class Level : IProcessable, IDrawable {
    private:
        string _path;
        int _currentRoom = 0;
        vector<SDL_Texture*> _atlases;
        vector<Room> _rooms;
        Player _player;
        RoomCamera _roomCamera;
        SDL_Texture* _lowResRenderTexture;

    public:
        Level(const json& levelProperties, const string& pathToFolder, SDL_Renderer* renderer, const InputManager& inputManager, SDL_Window* window) :
        _path(pathToFolder),
        _currentRoom(levelProperties.at("starting_room")),
        _rooms(LoadRooms(levelProperties, pathToFolder)),
        _atlases(LoadAtlases(levelProperties, pathToFolder, renderer)),
        _player(Player(inputManager, renderer, _rooms.front().GetColliders())),
        _roomCamera(_player, _rooms.at(_currentRoom), window),
        _lowResRenderTexture(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _rooms.at(_currentRoom).GetWidth(), _rooms.at(_currentRoom).GetHeight()))
        {
            cout << "Finished loading level " << pathToFolder << "!!!" << endl;
            _player.position.x = levelProperties.at("player_x");
            _player.position.y = levelProperties.at("player_y");

            _rooms.at(_currentRoom).CacheTiles(renderer, _atlases);
        }

        Level(const string& pathToFolder, SDL_Renderer* renderer, const InputManager& inputManager, SDL_Window* window) :
        Level(LoadJson(pathToFolder), pathToFolder, renderer, inputManager, window)
        { }

        json LoadJson(const string& pathToFolder) const {
            ifstream jsonFile(pathToFolder + "/level.json");
            if (!jsonFile.good()) {
                cerr << "ERROR opening level.json file in " << pathToFolder << endl;
            }

            return json::parse(jsonFile);
        }

        vector<SDL_Texture*> LoadAtlases(const json& levelProperties, const string& pathToFolder, SDL_Renderer* renderer) const {
            vector<TileSheetSource> sources = levelProperties.at("tilesheet_sources").get<vector<TileSheetSource>>();
            vector<SDL_Texture*> atlases;

            for (auto source: sources) {
                SDL_Texture* texture = IMG_LoadTexture(renderer, source.GetPath(pathToFolder).data());
                if (texture == NULL) {
                    cerr << "ERROR: couldn't load tilesheet atlas " << source.GetPath(pathToFolder) << ": " << SDL_GetError() << endl;
                }
                atlases.push_back(texture);
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
        }

        void Draw(SDL_Renderer* renderer) const override {

            // pixel rendering
            SDL_SetRenderTarget(renderer, _lowResRenderTexture);
            SDL_RenderClear(renderer);

            _rooms[_currentRoom].Draw(renderer, _atlases);
            _player.Draw(renderer);
            
            // fullres rendering
            SDL_SetRenderTarget(renderer, NULL);

            SDL_Point drawOffset = _roomCamera.GetDrawOffset();
            float zoom = _roomCamera.GetZoom();

            SDL_FRect destination {
                drawOffset.x * zoom,
                drawOffset.y * zoom,
                _rooms.at(_currentRoom).GetWidth() * zoom,
                _rooms.at(_currentRoom).GetHeight() * zoom
            };

            SDL_RenderCopyF(
                renderer,
                _lowResRenderTexture,
                NULL,
                &destination
            );
        }

        RoomCamera& GetCamera() {
            return _roomCamera;
        }

};