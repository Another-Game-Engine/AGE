#pragma once

#include "BFCBlockManager.hpp"
#include "BFCItemID.hpp"

namespace AGE
{
	struct BFCCullableObject;
	class  BFCCullableHandle;

	static const CullableTypeID MaxCullableTypeID = MaxBlockManagerID;

	class BFCBlockManagerFactory
	{
	public:
		BFCCullableHandle createItem(BFCCullableObject *object);
		void deleteItem(const BFCCullableHandle &handle);
	private:
		std::vector<BFCBlockManager> _managers;
	};
}