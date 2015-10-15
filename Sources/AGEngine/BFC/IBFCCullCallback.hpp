#pragma once

#include "BFCArray.hpp"

namespace AGE
{
	class IBFCCuller
	{
	public:
		virtual void operator()() = 0;
		inline void push(const BFCItem &item) { _array.push(item); }
	protected:
		BFCCullArray      _array;
	};
}