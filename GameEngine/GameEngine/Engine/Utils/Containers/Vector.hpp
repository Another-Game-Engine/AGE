#pragma once

#include "Config.hpp"

#ifdef USE_AGE_CONTAINER
	#include <vector>
namespace AGE
{
	template<typename T>
	using Vector = ::std::vector<T>;
}
#else
	#include <vector>
namespace AGE
{
	template<typename T>
	using Vector = ::std::vector<T>;
}
#endif