#pragma once

#include <memory>
#include <string>

#include "engine/world/Level.hpp"

class WorldManager {
   private:
	static inline std::unique_ptr<Level> _level;

   public:
	static void LoadLevel(const std::string& path) { _level = std::make_unique<Level>(path); }

	static Level& GetLevel() { return *_level; }
};