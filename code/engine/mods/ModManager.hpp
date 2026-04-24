#pragma once

#include <memory>
#include <vector>

#include "engine/mods/LevelMod.hpp"
#include "engine/mods/ResourceMod.hpp"

class ModManager {
   private:
	static inline std::unique_ptr<ResourceMod> _builtin;
	static inline std::unique_ptr<LevelMod> _level;
	// TODO add skin mods

   public:
	static void Init() { _builtin = std::make_unique<ResourceMod>("content/sidequest"); }

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
};