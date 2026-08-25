#pragma once

#include <string>

#include "engine/PlatformDefines.hpp"

class IDevConsoleVariable {
   public:
	virtual void Set(const std::string& value) = 0;
	virtual std::string GetString() const = 0;
	virtual bool IsCheat() const = 0;
	virtual bool IsUnsafe() const = 0;

#ifdef PLATFORM_HAS_STRING_COMMANDS
	virtual const std::string& GetDescription() const = 0;
#endif
};