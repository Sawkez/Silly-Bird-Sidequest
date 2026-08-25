#pragma once

#include <string>
#include <unordered_map>

#include "engine/PlatformDefines.hpp"
#include "engine/devconsole/DevConsole.hpp"
#include "engine/devconsole/variable/IDevConsoleVariable.hpp"

class DevConsoleVariableManager {
   public:
	enum DevConsoleVariableID {
		PLAYER_STATE_DASH_X_VELOCITY,
		PLAYER_STATE_DASH_Y_VELOCITY,
		PLAYER_STATE_DASH_RETURN_TIME,

		PLAYER_STATE_DIVE_INITIAL_VELOCITY,
		PLAYER_STATE_DIVE_INITIAL_GRAVITY,
		PLAYER_STATE_DIVE_GRAVITY_MULT,
		PLAYER_STATE_DIVE_GRAVITY_MAX,
		PLAYER_STATE_DIVE_CANCEL_DURATION,
		PLAYER_STATE_DIVE_BOOST_FORCE,
		PLAYER_STATE_DIVE_FRICTION,

		PLAYER_STATE_NORMAL_ACCELERATION,
		PLAYER_STATE_NORMAL_TOP_SPEED,
		PLAYER_STATE_NORMAL_FRICTION,
		PLAYER_STATE_NORMAL_AIR_FRICTION,
		PLAYER_STATE_NORMAL_WEAK_GRAVITY,
		PLAYER_STATE_NORMAL_GRAVITY,
		PLAYER_STATE_NORMAL_FAST_FALL_GRAVITY,
		PLAYER_STATE_NORMAL_FAST_FALL_WINDOW,
		PLAYER_STATE_NORMAL_FALL_SPEED_CAP,
		PLAYER_STATE_NORMAL_JUMP_FORCE,
		PLAYER_STATE_NORMAL_MAX_DIVE_BUFFER_Y_VELOCITY,
		PLAYER_STATE_NORMAL_SLOW_RUN_SPEED,

		META_ENABLE_CHEATS,

		_VAR_COUNT
	};

   private:
	static inline IDevConsoleVariable* _variables[_VAR_COUNT];
#ifdef PLATFORM_HAS_STRING_COMMANDS
	static inline std::unordered_map<std::string, IDevConsoleVariable*> _variablesByName;
#endif

   public:
	static void Register(const std::string& name, int index, IDevConsoleVariable* variable) {
		_variables[index] = variable;
#ifdef PLATFORM_HAS_STRING_COMMANDS
		_variablesByName[name] = variable;
#endif
	}

	static void Set(const std::string& name, const std::string& value, bool fromUser) {
#ifdef PLATFORM_HAS_STRING_COMMANDS

		auto it = _variablesByName.find(name);

		if (it == _variablesByName.end()) {
			dc::err << "Variable not found: " << name << dc::endl;
			return;
		}

		IDevConsoleVariable* variable = it->second;

		if (fromUser && variable->IsCheat()) {
			auto enableCheats = _variables[META_ENABLE_CHEATS];

			if (enableCheats && enableCheats->GetString() != "1") {
				dc::err << "Cannot set cheat variables with cheats disabled. Run \"set META_ENABLE_CHEATS 1\" to "
						   "enable them."
						<< dc::endl;
				return;
			}
		}

		if (!fromUser && variable->IsUnsafe()) {
			dc::err << "Only the player may set unsafe variables." << dc::endl;
			return;
		}

		variable->Set(value);
#endif
	}
};