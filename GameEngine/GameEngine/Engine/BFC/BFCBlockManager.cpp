#include "BFCBlockManager.hpp"
#include "BFCBlock.hpp"
#include "BFCCullableObject.hpp"

#include "Utils/Debug.hpp"


namespace AGE
{
	void BFCBlockManager::createItem(BFCCullableObject *object, BlockID &blockID, ItemID &itemId)
	{
		for (std::size_t i = 0; i < _blockCounter; ++i)
		{
			auto &block = _blocks[i];
			if (block->isFull())
			{
				continue;
			}
			itemId = block->createItem(object);
			blockID = std::uint8_t(i);
			return;
		}
		_blockCounter++;
		AGE_ASSERT(_blockCounter >= MaxBlockID);
		createItem(object, blockID, itemId);
	}

	void BFCBlockManager::deleteItem(BlockID &blockID, ItemID &itemId)
	{
		AGE_ASSERT(blockID < _blockCounter);
		_blocks[blockID]->deleteItem(itemId);
	}
}