#pragma once

#include <list>

#include "BFCBlockManager.hpp"
#include "BFCItemID.hpp"

namespace AGE
{
	struct BFCCullableObject;
	class  BFCCullableHandle;
	class  BFCItem;
	struct DRBData;
	class Frustum;

	static const CullableTypeID MaxCullableTypeID = MaxBlockManagerID;

	class BFCBlockManagerFactory
	{
	public:
		BFCCullableHandle createItem(BFCCullableObject *object);
		void deleteItem(const BFCCullableHandle &handle);
		BFCItem &getItem(const BFCItemID &id);
		void cullOnChannel(CullableTypeID channel, std::list<std::shared_ptr<DRBData>> &result, const Frustum &frustum);
	private:
		std::vector<BFCBlockManager> _managers;
	};
}