#pragma once

#include "BFCBlockManager.hpp"
#include "BFCItemID.hpp"

namespace AGE
{
	struct BFCCullableObject;
	class  BFCCullableHandle;
	class  BFCItem;

	static const CullableTypeID MaxCullableTypeID = MaxBlockManagerID;

	class BFCBlockManagerFactory
	{
	public:
		BFCCullableHandle createItem(BFCCullableObject *object);
		void deleteItem(const BFCCullableHandle &handle);
		BFCItem &getItem(const BFCItemID &id);
	private:
		std::vector<BFCBlockManager> _managers;
	};
}