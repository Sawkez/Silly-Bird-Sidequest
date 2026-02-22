#pragma once

#include <SDL.h>

#include "Json.hpp"

struct Tile {

    static const int DRAW_SOURCE_SIZE = 14.0;
    static const int DRAW_DEST_SIZE = 8.0;
    static const int DRAW_DEST_OFFSET = (DRAW_DEST_SIZE - DRAW_SOURCE_SIZE) / 2.0;

    int x = 0;
    int y = 0;
    int xAtlas = 0;
    int yAtlas = 0;
    int sourceID = 0;

    Tile() { };

    void Draw(SDL_Renderer* renderer, const vector<SDL_Texture*>& atlases) {

        SDL_Rect source {
            xAtlas * DRAW_SOURCE_SIZE,
            yAtlas * DRAW_SOURCE_SIZE,
            DRAW_SOURCE_SIZE,
            DRAW_SOURCE_SIZE
        };

        SDL_Rect dest {
            x * DRAW_DEST_SIZE + DRAW_DEST_OFFSET,
            y * DRAW_DEST_SIZE + DRAW_DEST_OFFSET,
            DRAW_SOURCE_SIZE,
            DRAW_SOURCE_SIZE
        };

        SDL_RenderCopy(
            renderer,
            atlases.at(sourceID),
            &source,
            &dest
        );
    }
};

void from_json(const nlohmann::json& json, Tile& tile) {
    tile.x = json.at("x");
    tile.y = json.at("y");
    tile.xAtlas = json.at("atlas_x");
    tile.yAtlas = json.at("atlas_y");
    tile.sourceID = json.at("source_id");

}