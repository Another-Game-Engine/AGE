#include "BFCBlockManagerFactory.hpp"
#include "BFCCullableHandle.hpp"
#include "BFCCullableObject.hpp"
#include "BFCBlock.hpp"

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

}