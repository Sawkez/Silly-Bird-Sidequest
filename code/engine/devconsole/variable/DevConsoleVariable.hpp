#pragma once

#include <sstream>
#include <string>

#include "engine/PlatformDefines.hpp"
#include "engine/devconsole/DevConsoleFlags.hpp"
#include "engine/devconsole/variable/DevConsoleVariableManager.hpp"
#include "engine/devconsole/variable/IDevConsoleVariable.hpp"

#define CONVAR_STR_HELPER(x) #x
#define CONVAR_STR(x) CONVAR_STR_HELPER(x)

#define CONVAR_GLUE_HELPER(x, y) x##_##y
#define CONVAR_GLUE(x, y) CONVAR_GLUE_HELPER(x, y)

#define CONVAR_NAME_STR(cat, id) CONVAR_STR(cat) "_" CONVAR_STR(id)

#define CONVAR(type, varName, id, defaultValue, flags, ...)                                                            \
	static inline auto varName = DevConsoleVariable<type>(CONVAR_NAME_STR(CONVAR_CATEGORY, id),                        \
														  DevConsoleVariableManager::CONVAR_GLUE(CONVAR_CATEGORY, id), \
														  defaultValue, flags, ##__VA_ARGS__)

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

	std::string GetString() const override {
		std::ostringstream oss;
		oss << _current;
		return oss.str();
	}

	bool IsCheat() const override { return _flags & DC_FLAG_CHEAT; }
	bool IsUnsafe() const override { return _flags & DC_FLAG_UNSAFE; }

	const Type& operator*() const { return _current; }
	const Type* operator->() const { return &_current; }
	Type Get() const { return _current; }
};