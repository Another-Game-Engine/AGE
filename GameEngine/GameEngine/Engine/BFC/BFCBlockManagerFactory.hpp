#pragma once

#include "BFCBlockManager.hpp"
#include "BFCItemID.hpp"

namespace AGE
{
	struct BFCCullableObject;
	class  BFCCullableHandle;

	typedef BlockManagerID CullableTypeID;
	static const CullableTypeID MaxCullableTypeID = MaxBlockManagerID;

	class BFCBlockManagerFactory
	{
	public:
		BFCCullableHandle createItem(CullableTypeID typeId, BFCCullableObject *object);
	private:
		std::vector<BFCBlockManager> _managers;
	};
}