#pragma once

#include "engine/save/SaveManagerBase.hpp"

#if SDL_PLATFORM_PSP
#include "engine/save/SaveManagerPSP.hpp"
#else
#include "engine/save/SaveManagerPC.hpp"
#endif

namespace SaveManager {
static SaveManagerBase* instance;

void Init() {
#if SDL_PLATFORM_PSP
	instance = new SaveManagerPSP;
#else
	instance = new SaveManagerPC;
#endif

	instance->Init();
}

}  // namespace SaveManager