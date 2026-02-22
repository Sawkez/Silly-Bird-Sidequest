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
        int _width;
        int _height;
        int _targetWidth;
        int _targetHeight;
        int _xPosition;
        int _yPosition;
        SDL_Texture* _tileCache = NULL;

    public:
        Room(const string& jsonPath) :
        Room(LoadJson(jsonPath))
        { }

        Room(const json& roomJson) :
        _colliders(LoadColliders(roomJson)), _tiles(LoadTiles(roomJson)), _ledges(LoadLedges(roomJson)),
        _width(roomJson["width"]), _height(roomJson["height"]), _targetWidth(roomJson["target_width"]), _targetHeight(roomJson["target_height"]),
        _xPosition(roomJson["position_x"]), _yPosition(roomJson["position_y"])
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

        void CacheTiles(SDL_Renderer* renderer, const vector<SDL_Texture*>& atlases) {
            _tileCache = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, _width, _height);

            SDL_SetRenderTarget(renderer, _tileCache);

            for (auto tile: _tiles) {
                tile.Draw(renderer, atlases);
            }

            SDL_SetRenderTarget(renderer, NULL);
        }

        void Draw(SDL_Renderer* renderer, const vector<SDL_Texture*>& atlases) const {

            if (_tileCache == NULL) {
                cerr << "ERROR: Tiles not cached!" << endl;
            }

            SDL_Rect destination {0, 0, _width, _height};

            SDL_RenderCopy(
                renderer,
                _tileCache,
                NULL,
                &destination
            );
        }

        Vector2 GetSize() const {
            return Vector2 {float(_width), float(_height)};
        }

        int GetWidth() const {
            return _width;
        }

        int GetHeight() const {
            return _height;
        }

        Vector2 GetTargetSize() const {
            return Vector2 {float(_targetWidth), float(_targetHeight)};
        }
};