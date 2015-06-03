#include "BFCBlockManager.hpp"
#include "BFCBlock.hpp"
#include "BFCRootType.hpp"

#include "Utils/Debug.hpp"


namespace AGE
{
	void BFCBlockManager::createItem(BFCRootType *drawable, std::uint8_t &blockID, std::uint8_t &itemId)
	{
		for (std::size_t i = 0; i < _blockCounter; ++i)
		{
			auto &block = _blocks[i];
			if (block->isFull())
			{
				continue;
			}
			itemId = block->createItem(drawable);
			blockID = std::uint8_t(i);
			return;
		}
		_blockCounter++;
		AGE_ASSERT(_blockCounter >= BlockMaxNumber);
		createItem(drawable, blockID, itemId);
	}
}