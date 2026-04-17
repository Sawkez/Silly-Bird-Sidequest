#pragma once

#include "engine/save/SaveManagerBase.hpp"

#if __PSP__
#include "engine/save/SaveManagerPSP.hpp"
#else
#include "engine/save/SaveManagerPC.hpp"
#endif

namespace SaveManager {
static SaveManagerBase* instance;

void Init() {
#if __PSP__
	instance = new SaveManagerPSP;
#else
	instance = new SaveManagerPC;
#endif

	instance->Init();
}

}  // namespace SaveManager