#pragma once

#include "engine/save/SaveManagerBase.hpp"

#if SDL_PLATFORM_PSP
#include "engine/save/SaveManagerPSP.hpp"
#elif SDL_PLATFORM_ANDROID
#include "engine/save/SaveManagerAndroid.hpp"
#else
#include "engine/save/SaveManagerPC.hpp"
#endif

namespace SaveManager {
static SaveManagerBase* instance;

void Init() {
#if SDL_PLATFORM_PSP
	instance = new SaveManagerPSP;
#elif SDL_PLATFORM_ANDROID
	instance = new SaveManagerAndroid;
#else
	instance = new SaveManagerPC;
#endif

	instance->Init();
}

}  // namespace SaveManager