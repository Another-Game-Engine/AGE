#include "BFCBlockManagerFactory.hpp"
#include "BFCCullableHandle.hpp"
#include "BFCCullableObject.hpp"
#include "BFCBlock.hpp"

#include "Utils/Frustum.hh"

#include "Utils/Debug.hpp"
#include "Utils/Profiler.hpp"


namespace AGE
{
	BFCCullableHandle BFCBlockManagerFactory::createItem(BFCCullableObject *object)
	{
		SCOPE_profile_cpu_function("BFC");

		auto typeId = object->getBFCType();

		AGE_ASSERT(typeId < MaxCullableTypeID);
		AGE_ASSERT(object != nullptr);

		if (typeId >= _managers.size())
		{
			_managers.resize(typeId + 1);
		}

		BFCCullableHandle result;

		result._elementPtr = object;
		result._itemID._blockManagerID = typeId;

		auto &manager = _managers[typeId];

		manager.createItem(object, result._itemID._blockID, result._itemID._itemID);

		return result;
	}

	void BFCBlockManagerFactory::deleteItem(const BFCCullableHandle &handle)
	{
		SCOPE_profile_cpu_function("BFC");

		AGE_ASSERT(handle.invalid() == false);

		auto itemId = handle.getItemId();
		
		AGE_ASSERT(itemId._blockManagerID < MaxCullableTypeID);
		AGE_ASSERT(itemId._blockManagerID < _managers.size());

		_managers[itemId._blockManagerID].deleteItem(itemId._blockID, itemId._itemID);
	}

	BFCItem &BFCBlockManagerFactory::getItem(const BFCItemID &id)
	{
		SCOPE_profile_cpu_function("BFC");

		AGE_ASSERT(id._blockManagerID < _managers.size());
		return _managers[id._blockManagerID]._blocks[id._blockID]->_items[id._itemID];
	}

	std::size_t BFCBlockManagerFactory::cullOnBlock(CullableTypeID channel, LFList<BFCItem> &result, const Frustum &frustum, std::size_t blockIdFrom, std::size_t numberOfBlocks)
	{
		SCOPE_profile_cpu_function("BFC");

		AGE_ASSERT(channel < MaxCullableTypeID);

		if (channel >= _managers.size())
		{
			return 0;
		}
		auto &manager = _managers[channel];

		size_t i = 0;
		while (i < numberOfBlocks)
		{
			auto blockId = blockIdFrom + i;
			if (blockId >= manager._blocks.size())
				break;
			auto &block = manager._blocks[blockId];
			for (auto &item : block->_items)
			{
				if (item.getDrawable() && frustum.checkCollision(item.getPosition()))
				{
					result.push(&item);
				}
			}
			++i;
		}
		return i;
	}

	std::size_t BFCBlockManagerFactory::cullOnBlock(CullableTypeID channel, const Frustum &frustum, std::size_t blockIdFrom, std::size_t numberOfBlocks, IBFCCuller *culler)
	{
		SCOPE_profile_cpu_function("BFC");

		AGE_ASSERT(channel < MaxCullableTypeID);

		if (channel >= _managers.size())
		{
			return 0;
		}
		auto &manager = _managers[channel];

		size_t i = 0;
		while (i < numberOfBlocks)
		{
			auto blockId = blockIdFrom + i;
			if (blockId >= manager._blocks.size())
				break;
			auto &block = manager._blocks[blockId];
			for (auto &item : block->_items)
			{
				if (item.getDrawable() && frustum.checkCollision(item.getPosition()))
				{
					culler->push(item);
				}
			}
			(*culler)();
			++i;
		}
		return i;
	}


	std::size_t BFCBlockManagerFactory::fillOnBlock(CullableTypeID channel, LFList<BFCItem> &result, std::size_t blockIdFrom, std::size_t numberOfBlocks, IBFCCuller *callback)
	{
		SCOPE_profile_cpu_function("BFC");

		AGE_ASSERT(channel < MaxCullableTypeID);
		if (channel >= _managers.size())
		{
			return 0;
		}
		auto &manager = _managers[channel];
		size_t i = 0;
		while (i < numberOfBlocks)
		{
			auto blockId = blockIdFrom + i;
			if (blockId >= manager._blocks.size())
				break;
			auto &block = manager._blocks[blockId];
			for (auto &item : block->_items)
			{
				if (item.getDrawable())
				{
					result.push(&item);
				}
			}
			if (callback)
			{
				(*callback)();
			}
			++i;
		}
		return i;
	}

	std::size_t BFCBlockManagerFactory::getBlockNumberToCull(CullableTypeID channel) const
	{
		AGE_ASSERT(channel < MaxCullableTypeID);
		if (channel >= _managers.size())
		{
			return 0;
		}
		return _managers[channel]._blocks.size();
	}
}