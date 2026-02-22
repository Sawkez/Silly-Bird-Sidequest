#pragma once

#include <string>

#include "Json.hpp"

struct TileSheetSource {
    bool custom;
    std::string name;

    TileSheetSource() :
    custom(false), name("")
    { }

    std::string GetPath(const std::string& modPath) const {
        if (custom) return modPath + "/tiles/fg/" + name;
        else return "content/sidequest/tiles/fg/" + name;
    }
};

void from_json(const nlohmann::json& json, TileSheetSource& source) {
    source.custom = json.at("custom");
    source.name = json.at("name");
}