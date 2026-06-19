#pragma once

#include <SDL3/SDL.h>

#include <memory>
#include <vector>

#include "engine/mods/LevelMod.hpp"
#include "engine/mods/ResourceMod.hpp"
#include "engine/mods/SkinMod.hpp"
#include "game/player/graphics/Jizz.hpp"

class ModManager {
   private:
	static constexpr uint8_t BUILTIN_TILE_BIT = 0b10000000;

	static inline std::unique_ptr<ResourceMod> _builtin;
	static inline std::unique_ptr<LevelMod> _level;
	static inline std::unique_ptr<SkinMod> _skin;
	static inline std::unique_ptr<Jizz> _jizz;

   public:
	static void Init() { _builtin = std::make_unique<ResourceMod>("content/sidequest-hidden"); }

	static void LoadLevelMod(const std::string& path) { _level = std::make_unique<LevelMod>(path); }

	static void LoadSkinMod(const std::string& path) { _skin = std::make_unique<SkinMod>(path); }

	static void LoadSkin(SDL_Renderer* renderer, int index) {
		_jizz = std::make_unique<Jizz>(_skin->GetSkinPath(index), renderer);
	}

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

	static std::string GetTileSourcePath(uint8_t sourceID) {
		if (sourceID & BUILTIN_TILE_BIT) {
			return _builtin->GetTileSourcePath(sourceID & ~BUILTIN_TILE_BIT);
		} else {
			return _level->GetTileSourcePath(sourceID);
		}
	}

	static const Jizz& GetJizz() { return *_jizz; }
};