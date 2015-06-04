#include "BFCBlockManagerFactory.hpp"
#include "BFCCullableHandle.hpp"
#include "BFCCullableObject.hpp"

#include "Utils/Debug.hpp"

namespace AGE
{
	BFCCullableHandle BFCBlockManagerFactory::createItem(BFCCullableObject *object)
	{
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
		AGE_ASSERT(handle.invalid() == false);

		auto itemId = handle.getItemId();
		
		AGE_ASSERT(itemId._blockManagerID < MaxCullableTypeID);
		AGE_ASSERT(itemId._blockManagerID < _managers.size());

		_managers[itemId._blockManagerID].deleteItem(itemId._blockID, itemId._itemID);
	}
}