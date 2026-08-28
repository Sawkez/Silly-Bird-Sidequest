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
	static void Init() {
		SDL_Storage* modStorage = ZipStorage::Open(ResourceManager::gameData, "content/sidequest-hidden.sbsq");
		_builtin = std::make_unique<ResourceMod>(modStorage, "content/sidequest-hidden.sbsq");
	}

	static void LoadLevelModFromStorage(SDL_Storage* modStorage, const std::string& modPath) {
		_level = std::make_unique<LevelMod>(modStorage, modPath);
	}

	static void LoadLevelModFromFile(const std::string& modPath) {
		SDL_Storage* modStorage = ZipStorage::Open(modPath);
		LoadLevelModFromStorage(modStorage, modPath);
	}

	static void LoadLevelModFromFolder(SDL_Storage* modFolderStorage, const std::string& modPath) {
		SDL_Storage* modStorage = ZipStorage::Open(modFolderStorage, modPath);
		LoadLevelModFromStorage(modStorage, modPath);
	}

	static void LoadSkinModFromStorage(SDL_Storage* modStorage, const std::string& modPath) {
		_skin = std::make_unique<SkinMod>(modStorage, modPath);
	}

	static void LoadSkinModFromFile(const std::string& modPath) {
		SDL_Storage* modStorage = ZipStorage::Open(modPath);
		LoadSkinModFromStorage(modStorage, modPath);
	}

	static void LoadSkinModFromFolder(SDL_Storage* modFolderStorage, const std::string& modPath) {
		SDL_Storage* modStorage = ZipStorage::Open(modFolderStorage, modPath);
		LoadSkinModFromStorage(modStorage, modPath);
	}

	static void LoadSkin(SDL_Renderer* renderer, int index) {
		_jizz = std::make_unique<Jizz>(_skin->GetStorage(), _skin->GetSkinPath(index), renderer);
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

	static SDL_Storage* GetBuiltinStorage() { return _builtin->GetStorage(); }
	static SDL_Storage* GetLevelStorage() { return _level->GetStorage(); }
	static SDL_Storage* GetSkinStorage() { return _level->GetStorage(); }

	static SDL_Texture* LoadTileSource(SDL_Renderer* renderer, uint8_t sourceID) {
		if (sourceID & BUILTIN_TILE_BIT) {
			return ResourceManager::LoadTexture(renderer, GetBuiltinStorage(),
												_builtin->GetTileSourcePath(sourceID & ~BUILTIN_TILE_BIT));
		} else {
			return ResourceManager::LoadTexture(renderer, GetLevelStorage(), _level->GetTileSourcePath(sourceID));
		}
	}

	static SDL_Surface* LoadTileSourceAsSurface(SDL_Renderer* renderer, Uint8 sourceID) {
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

	static const Jizz& GetJizz() { return *_jizz; }

	static int GetLevelIndexFromPath(const std::string& path) {
		for (int i = 0; i < GetLevelCount(); i++) {
			if (_level->GetShallowPath(i) == path) {
				return i;
			}
		}

		return -1;
	}

	static int GetSkinIndexFromPath(const std::string& path) {
		for (int i = 0; i < GetLevelCount(); i++) {
			if (_skin->GetShallowPath(i) == path) {
				return i;
			}
		}

		return -1;
	}
};