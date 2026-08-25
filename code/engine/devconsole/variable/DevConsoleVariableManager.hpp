#pragma once

#include <string>
#include <unordered_map>

#include "engine/PlatformDefines.hpp"
#include "engine/devconsole/DevConsole.hpp"
#include "engine/devconsole/variable/IDevConsoleVariable.hpp"

class DevConsoleVariableManager {
   public:
	enum DevConsoleVariableID { PLAYER_STATE_NORMAL_GRAVITY, PLAYER_STATE_NORMAL_FASTFALL_GRAVITY, _VAR_COUNT };

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

		auto variable = _variablesByName.find(name);

		if (variable == _variablesByName.end()) {
			dc::err << "Variable not found: " << name << dc::endl;
		}

		variable->second->Set(value);
#endif
	}
};