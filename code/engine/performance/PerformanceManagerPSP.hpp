#pragma once

#include <psppower.h>

#include "engine/devconsole/DevConsole.hpp"
#include "engine/performance/PerformanceManagerBase.hpp"

class PerformanceManagerPSP : public PerformanceManagerBase {
   private:
	static inline const int FREQUENCIES[_PROFILE_COUNT][2]{
		{30, 15},	 // power saver
		{100, 50},	 // gameplay
		{222, 111},	 // default
		{333, 166},	 // loading
		{333, 166},	 // ui
		{333, 166}	 // boost
	};

   public:
	void SetProfile(PerformanceProfile profile) override {
		const int* profileFrequencies = FREQUENCIES[profile];

		scePowerSetClockFrequency(profileFrequencies[0], profileFrequencies[0], profileFrequencies[1]);

		dc::msg << "Setting cpu frequency to " << profileFrequencies[0] << "/" << profileFrequencies[1] << dc::endl;
	}
};