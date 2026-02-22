#pragma once

#include <vector>
#include <iostream>

#include "Json.hpp"

#include "CollisionRect.hpp"
#include "Tile.hpp"
#include "Vector2.hpp"

using namespace std;
using json = nlohmann::json;

class Room {

    private:
        vector<CollisionRect> _colliders;
        vector<Tile> _tiles;
        vector<Vector2> _ledges;

    public:
        Room(const string& jsonPath) :
        Room(LoadJson(jsonPath))
        { }

        Room(const json& roomJson) :
        _colliders(LoadColliders(roomJson)), _tiles(LoadTiles(roomJson)), _ledges(LoadLedges(roomJson))
        { }

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

        vector<Tile> LoadTiles(json roomJson) const {
            return roomJson.at("tiles").get<vector<Tile>>();
        }

        vector<Vector2> LoadLedges(json roomJson) const {
            return roomJson.at("ledges").get<vector<Vector2>>();
        }

        const vector<CollisionRect>& GetColliders() const {
            return _colliders;
        };

        void Draw(SDL_Renderer* renderer, const vector<SDL_Texture*>& atlases) const {

            for (auto tile: _tiles) {
                tile.Draw(renderer, atlases);
            }

        }
};