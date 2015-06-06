#pragma once

#include <memory>
#include <vector>
#include <cstdint>

#include "BFCItemID.hpp"

namespace AGE
{
	class BFCBlock;
	struct BFCCullableObject;
	class BFCBlockManagerFactory;

	class BFCBlockManager
	{
	public:
	private:
		void createItem(BFCCullableObject *object, BlockID &blockID, ItemID &itemId);
		void deleteItem(BlockID &blockID, ItemID &itemId);
		std::size_t _firstBlockWithSpace = 0;
		std::vector<std::shared_ptr<BFCBlock>> _blocks;

		friend class BFCBlockManagerFactory;
	};
}