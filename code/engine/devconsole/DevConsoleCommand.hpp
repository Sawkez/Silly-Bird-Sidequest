#pragma once

#include <string>
#include <vector>

#include "engine/PlatformDefines.hpp"

class DevConsoleCommand {
   private:
	unsigned char _flags = 0;
	void (*_function)(const std::vector<std::string>& args, bool fromUser) = nullptr;

#ifdef PLATFORM_HAS_STRING_COMMANDS
	std::string _description = "";
#endif

   public:
	DevConsoleCommand() {}
#ifdef PLATFORM_HAS_STRING_COMMANDS
	DevConsoleCommand(void (*function)(const std::vector<std::string>& args, bool fromUser), unsigned char flags,
					  const std::string& description)
		: _flags(flags), _function(function), _description(description) {}
#else
	DevConsoleCommand(void (*function)(const std::vector<std::string>& args), unsigned char flags,
					  const std::string& description)
		: _flags(flags), _function(function) {}
#endif

	void Run(const std::vector<std::string>& args, bool fromUser) const { _function(args, fromUser); }

	const std::string& GetDescription() const {
#ifdef PLATFORM_HAS_STRING_COMMANDS
		return _description;
#else
		return "";
#endif
	}
};