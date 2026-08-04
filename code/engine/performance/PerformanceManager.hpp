#pragma once

#include "engine/performance/PerformanceManagerBase.hpp"

#if SDL_PLATFORM_PSP
#include "engine/performance/PerformanceManagerPSP.hpp"
#endif

namespace PerformanceManager {
PerformanceManagerBase* instance = nullptr;

void Init() {
#if SDL_PLATFORM_PSP
	instance = new PerformanceManagerPSP;
#else
	instance = new PerformanceManagerBase;
#endif
}
}  // namespace PerformanceManager