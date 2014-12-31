#pragma once

namespace AGE
{
	template < typename T >
	unsigned int countOne(T v)
	{
		unsigned int res = 0;
		while (v)
		{
			v &= v - 1;
			++res;
		}
		return res;
	}

	template < typename T >
	unsigned int countZero(T v)
	{
		return (sizeof(T) - countOne<T>());
	}
}