#include "BFCBlockManagerFactory.hpp"
#include "BFCCullableHandle.hpp"

#include "Utils/Debug.hpp"

namespace AGE
{
	BFCCullableHandle BFCBlockManagerFactory::createItem(
		CullableTypeID typeId
		, BFCCullableObject *object)
	{
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
}