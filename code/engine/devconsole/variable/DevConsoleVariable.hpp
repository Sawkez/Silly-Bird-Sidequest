#pragma once

#include <sstream>
#include <string>

#include "engine/PlatformDefines.hpp"
#include "engine/devconsole/DevConsoleFlags.hpp"
#include "engine/devconsole/variable/DevConsoleVariableManager.hpp"
#include "engine/devconsole/variable/IDevConsoleVariable.hpp"

#define CONVAR(type, varName, id, defaultValue, flags, ...) \
	static inline auto varName =                            \
		DevConsoleVariable<type>(#id, DevConsoleVariableManager::id, defaultValue, flags, ##__VA_ARGS__)

template <typename Type>
class DevConsoleVariable : public IDevConsoleVariable {
   private:
	unsigned char _flags;
	Type _current;
	Type _default;
	Type _saved;

#ifdef PLATFORM_HAS_STRING_COMMANDS
	std::string _description = "";
#endif

   public:
	DevConsoleVariable(const std::string& name, int index, const Type& defaultValue, unsigned char flags,
					   const std::string& description = "")
		: _flags(flags),
		  _current(defaultValue),
		  _default(defaultValue),
#ifdef PLATFORM_HAS_STRING_COMMANDS
		  _description(description),
#endif
		  _saved(defaultValue) {
		DevConsoleVariableManager::Register(name, index, this);
	}

	void Set(const std::string& newValue) override {
		std::istringstream iss(newValue);
		iss >> _current;
	}

	const Type& operator*() const { return _current; }
	Type Get() const { return _current; }
};