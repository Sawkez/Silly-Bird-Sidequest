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

using namespace std;
using json = nlohmann::json;

class Level : IProcessable, IDrawable {
    private:
        string _path;
        vector<SDL_Texture*> _atlases;
        vector<Room> _rooms;
        Player _player;
        int _currentRoom = 0;

    public:
        Level(const json& levelProperties, const string& pathToFolder, SDL_Renderer* renderer, const InputManager& inputManager) :
        _path(pathToFolder),
        _rooms(LoadRooms(levelProperties, pathToFolder)),
        _atlases(LoadAtlases(levelProperties, pathToFolder, renderer)),
        _player(Player(inputManager, renderer, _rooms.front().GetColliders()))
        {
            cout << "Finished loading level " << pathToFolder << "!!!" << endl;
            _player.position.x = levelProperties.at("player_x");
            _player.position.y = levelProperties.at("player_y");
        }

        Level(const string& pathToFolder, SDL_Renderer* renderer, const InputManager& inputManager) :
        Level(LoadJson(pathToFolder), pathToFolder, renderer, inputManager)
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
        }

        void Draw(SDL_Renderer* renderer, SDL_Point drawOffset = {0, 0}) const override {
            
            _rooms[_currentRoom].Draw(renderer, _atlases);

            _player.Draw(renderer, drawOffset);
        }

};