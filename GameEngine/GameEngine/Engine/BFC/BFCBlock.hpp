#pragma once

#include <cstddef>
#include <queue>

#include "BFCItem.hpp"
#include "BFCItemID.hpp"

namespace AGE
{
	class BFCBlockManagerFactory;

	class BFCBlock
	{
	public:
		BFCBlock();
		bool isEmpty() const;
		bool isFull() const;
		ItemID createItem(BFCCullableObject *object);
		void deleteItem(ItemID itemId);
	private:
		BFCItem _items[MaxItemID];
		std::size_t _counter;
		std::queue<ItemID> _free;

		friend class BFCBlockManagerFactory;
	};
}