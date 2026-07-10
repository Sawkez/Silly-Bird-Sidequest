#pragma once

#include <memory>
#include <vector>

#include "engine/mods/LevelMod.hpp"
#include "engine/mods/ResourceMod.hpp"

class ModManager {
   private:
	static constexpr uint8_t BUILTIN_TILE_BIT = 0b10000000;

	static inline std::unique_ptr<ResourceMod> _builtin;
	static inline std::unique_ptr<LevelMod> _level;
	// TODO add skin mods

   public:
	static void Init() { _builtin = std::make_unique<ResourceMod>("content/sidequest-hidden.sbsq"); }

	static void LoadLevelMod(const std::string& path) { _level = std::make_unique<LevelMod>(path); }

	static std::vector<std::string> GetLevelNames() {
		std::vector<std::string> names;
		names.reserve(_level->GetLevelCount());

		for (const auto& level : _level->GetLevels()) {
			names.push_back(level.GetName());
		}

		return names;
	}

	static int GetLevelCount() { return _level->GetLevelCount(); }

	static std::string GetLevelPath(int index) { return _level->GetLevelPath(index); }

	static SDL_Storage* GetBuiltinStorage() { return _builtin->GetStorage(); }
	static SDL_Storage* GetLevelStorage() { return _level->GetStorage(); }
	static SDL_Storage* GetSkinStorage() { return _level->GetStorage(); }

	static SDL_Surface* LoadTileSource(uint8_t sourceID) {
		if (sourceID & BUILTIN_TILE_BIT) {
			return ResourceManager::LoadSurface(GetBuiltinStorage(),
												_builtin->GetTileSourcePath(sourceID & ~BUILTIN_TILE_BIT));
		} else {
			return ResourceManager::LoadSurface(GetLevelStorage(), _level->GetTileSourcePath(sourceID));
		}
	}

	static std::string GetTileSourcePath(uint8_t sourceID) {
		if (sourceID & BUILTIN_TILE_BIT) {
			return _builtin->GetTileSourcePath(sourceID & ~BUILTIN_TILE_BIT);
		} else {
			return _level->GetTileSourcePath(sourceID);
		}
	}
};