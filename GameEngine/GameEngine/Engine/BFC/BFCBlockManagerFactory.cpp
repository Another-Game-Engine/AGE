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

	void BFCBlockManagerFactory::cullOnChannel(CullableTypeID channel, LFList<BFCItem> &result, const Frustum &frustum)
	{
		SCOPE_profile_cpu_function("BFC");

		AGE_ASSERT(channel < MaxCullableTypeID);

		if (channel >= _managers.size())
		{
			return;
		}

		auto &manager = _managers[channel];

		for (auto &block : manager._blocks)
		{
			for (auto &item : block->_items)
			{
				if (item.getDrawable() && frustum.checkCollision(item.getPosition()))
				{
					result.push(&item);
				}
			}
		}
	}

	void BFCBlockManagerFactory::cullOnBlock(CullableTypeID channel, LFList<BFCItem> &result, const Frustum &frustum, std::size_t blockId)
	{
		SCOPE_profile_cpu_function("BFC");

		AGE_ASSERT(channel < MaxCullableTypeID);
		auto &manager = _managers[channel];
		AGE_ASSERT(blockId < manager._blocks.size());
		auto &block = manager._blocks[blockId];
		for (auto &item : block->_items)
		{
			if (item.getDrawable() && frustum.checkCollision(item.getPosition()))
			{
				result.push(&item);
			}
		}
	}

	std::size_t BFCBlockManagerFactory::getBlockNumberToCull(CullableTypeID channel) const
	{
		AGE_ASSERT(channel < MaxCullableTypeID);
		return _managers[channel]._blocks.size();
	}

}