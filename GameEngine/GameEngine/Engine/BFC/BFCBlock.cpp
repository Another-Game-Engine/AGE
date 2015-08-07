#include "BFCBlock.hpp"
#include <Utils/Debug.hpp>

namespace AGE
{
	BFCBlock::BFCBlock()
	{
		for (auto i = 0; i < MaxItemID; ++i)
		{
			_free.push(i);
		}
	}

	bool BFCBlock::isFull() const
	{
		return _free.empty();
	}

	ItemID BFCBlock::createItem(BFCCullableObject *object)
	{
		ItemID index;
		index = _free.front();
		_free.pop();
		_items[index].setDrawable(object);
		return index;
	}

	void BFCBlock::deleteItem(ItemID itemId)
	{
		AGE_ASSERT(itemId < MaxItemID);

		_items[itemId].setDrawable(nullptr);
		_free.push(itemId);
	}
}