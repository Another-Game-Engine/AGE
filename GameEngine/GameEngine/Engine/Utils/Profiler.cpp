#include "Profiler.hpp"
#include <string>

namespace AGE
{
	uint32_t Profiler::GetColor(const char* str)
	{
		std::hash<std::string> fn;
		size_t value = fn(str);
		return value & 0xFFFFFF;
	}
}