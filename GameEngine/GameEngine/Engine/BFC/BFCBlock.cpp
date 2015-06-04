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
		AGE_ASSERT(_counter < MaxItemID);
		return _counter == MaxItemID - 1;
	}

	ItemID BFCBlock::createItem(BFCCullableObject *object)
	{
		AGE_ASSERT(_free.empty() == false);
		ItemID index = _free.back();
		_free.pop();
		_items[index].setDrawable(object);
		++_counter;
		return index;
	}
}