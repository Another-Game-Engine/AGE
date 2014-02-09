#pragma once

#include <cassert>

#define MAX_TAG_NUMBER (32)

// TODO -> transform in constexpr when VS'll support them
template <int T>
const unsigned int Tag()
{
	static_assert((T < MAX_TAG_NUMBER) && (T >= 0), "Only 32 tags are alowed");
	return 1 << T;
}