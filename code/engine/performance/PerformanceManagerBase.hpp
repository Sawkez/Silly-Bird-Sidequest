#pragma once

class PerformanceManagerBase {
   public:
	enum PerformanceProfile {
		PROFILE_POWER_SAVER,
		PROFILE_GAMEPLAY,
		PROFILE_DEFAULT,
		PROFILE_LOADING,
		PROFILE_UI,
		PROFILE_BOOST,
		_PROFILE_COUNT
	};

	virtual void SetProfile(PerformanceProfile profile) {}
};