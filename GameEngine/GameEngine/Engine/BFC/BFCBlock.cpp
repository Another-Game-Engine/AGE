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
		ItemID index;
		if (_free.empty() == false)
		{
			index = _free.back();
			_free.pop();
		}
		else
		{
			index = _counter;
			++_counter;
		}
		
		_items[index].setDrawable(object);
		return index;
	}

	void BFCBlock::deleteItem(ItemID itemId)
	{
		AGE_ASSERT(isEmpty() == false);
		AGE_ASSERT(itemId < MaxItemID);

		_items[itemId].setDrawable(nullptr);
		_free.push(itemId);
		--_counter;
	}
}