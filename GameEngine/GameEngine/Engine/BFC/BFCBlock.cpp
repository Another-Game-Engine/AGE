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

	std::uint8_t BFCBlock::createItem(void *drawable)
	{
		AGE_ASSERT(_free.empty() == false);
		std::uint8_t index = _free.back();
		_free.pop();
		_items[index].setDrawable(drawable);
		++_counter;
		return index;
	}
}