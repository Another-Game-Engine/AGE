#include "BFCBlockManager.hpp"
#include "BFCBlock.hpp"
#include "BFCCullableObject.hpp"

#include "Utils/Debug.hpp"


namespace AGE
{
	void BFCBlockManager::createItem(BFCCullableObject *object, BlockID &blockID, ItemID &itemId)
	{
		for (std::size_t i = _firstBlockWithSpace; i < _blocks.size(); ++i)
		{
			auto &block = _blocks[i];
			if (block->isFull())
			{
				continue;
			}
			itemId = block->createItem(object);
			blockID = std::uint8_t(i);
			_firstBlockWithSpace = i;
			return;
		}
		_firstBlockWithSpace = _blocks.size();
		_blocks.push_back(std::make_shared<BFCBlock>());
		AGE_ASSERT(_blocks.size() < MaxBlockID);
		createItem(object, blockID, itemId);
	}

	void BFCBlockManager::deleteItem(BlockID &blockID, ItemID &itemId)
	{
		AGE_ASSERT(blockID < _blocks.size());
		_firstBlockWithSpace = blockID;
		_blocks[blockID]->deleteItem(itemId);
	}
}