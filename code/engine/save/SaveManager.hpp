#pragma once

#include "engine/save/ISaveManager.hpp"

#if __PSP__
#include "engine/save/SaveManagerPSP.hpp"
#else
#include "engine/save/SaveManagerPC.hpp"
#endif

namespace SaveManager {
static ISaveManager* instance;

void Init() {
#if __PSP__
	instance = new SaveManagerPSP;
#else
	instance = new SaveManagerPC;
#endif
}

}  // namespace SaveManager