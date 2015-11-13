#include "BFCBlockManager.hpp"
#include "BFCBlock.hpp"
#include "BFCCullableObject.hpp"

#include "Utils/Debug.hpp"


namespace AGE
{
	void BFCBlockManager::createItem(BFCCullableObject *object, BlockID &blockID, ItemID &itemId)
	{
		
		if (_blocksNotFull.empty() == false)
		{
			auto b = *(_blocksNotFull.begin());
			auto &block = _blocks[b];
			itemId = block->createItem(object);
			blockID = std::uint8_t(b);
			if (block->isFull())
			{
				_blocksNotFull.erase(_blocksNotFull.begin());
			}
			return;
		}
		_blocks.push_back(std::make_shared<BFCBlock>());
		AGE_ASSERT(_blocks.size() < MaxBlockID);
		_blocksNotFull.insert(_blocks.size() - 1);
		createItem(object, blockID, itemId);
	}

	void BFCBlockManager::deleteItem(BlockID &blockID, ItemID &itemId)
	{
		AGE_ASSERT(blockID < _blocks.size());
		_blocks[blockID]->deleteItem(itemId);
		_blocksNotFull.insert(blockID);
	}
}