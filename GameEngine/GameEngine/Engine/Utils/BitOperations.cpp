#include <Utils/BitOperations.hpp>
#include <math.h>

namespace AGE
{
	namespace Bits
	{
		bool isPowerOfTwo(unsigned int x)
		{
			return ((x != 0) && ((x & (~x + 1)) == x));
		}

		int32_t roundToHighestPowerOfTwo(int32_t v)
		{
			v--;
			v |= v >> 1;
			v |= v >> 2;
			v |= v >> 4;
			v |= v >> 8;
			v |= v >> 16;
			v++;
			return v;
		}

		int getFirstBitPosition(int i)
		{
			int r = 0;
			while (i)
			{
				if (i & 1)
				{
					return r;
				}
				++r;
				i >>= 1;
			}
			return -1;
		}
	}
}