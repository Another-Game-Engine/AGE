#include "BFCBlock.hpp"
#include <Utils/Debug.hpp>

namespace AGE
{
	BFCBlock::BFCBlock()
		: _counter(0)
	{}

	bool BFCBlock::isEmpty() const
	{
		return _counter == 0;
	}

	bool BFCBlock::isFull() const
	{
		AGE_ASSERT(_counter <= BlockMaxSize);
		return _counter == BlockMaxSize - 1;
	}
}