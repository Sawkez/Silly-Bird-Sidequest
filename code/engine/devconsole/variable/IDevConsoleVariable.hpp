#pragma once

#include <string>

class IDevConsoleVariable {
   public:
	virtual void Set(const std::string& value) = 0;
};