#pragma once

#include <string>
#include <vector>

#include "engine/PlatformDefines.hpp"

class DevConsoleCommand {
   private:
	unsigned char _flags = 0;
	void (*_function)(const std::vector<std::string>& args) = nullptr;

#ifdef PLATFORM_HAS_STRING_COMMANDS
	std::string _description = "";
#endif

   public:
	static inline const unsigned char FLAG_CHEAT = 1 << 0;
	static inline const unsigned char FLAG_UNSAFE = 1 << 1;

	DevConsoleCommand() {}
	DevConsoleCommand(void (*function)(const std::vector<std::string>& args), unsigned char flags, const std::string& description)
		: _flags(flags),
		  _function(function),
#ifdef PLATFORM_HAS_STRING_COMMANDS
		  _description(description)
#endif
	{
	}

	void Run(const std::vector<std::string>& args, bool fromUser) const { _function(args); }
	const std::string& GetDescription() const {
#ifdef PLATFORM_HAS_STRING_COMMANDS
		return _description;
#else
		return ""
#endif
	}
};