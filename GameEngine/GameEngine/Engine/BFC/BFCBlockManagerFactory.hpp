#pragma once

#include <list>

#include "BFCBlockManager.hpp"
#include "BFCItemID.hpp"

#include "Utils/Containers/LFList.hpp"
#include "BFC/BFCItem.hpp"

namespace AGE
{
	struct BFCCullableObject;
	class  BFCCullableHandle;
	struct DRBData;
	class Frustum;

	static const CullableTypeID MaxCullableTypeID = MaxBlockManagerID;

	class BFCBlockManagerFactory
	{
	public:
		BFCCullableHandle createItem(BFCCullableObject *object);
		void deleteItem(const BFCCullableHandle &handle);
		BFCItem &getItem(const BFCItemID &id);
		void cullOnChannel(CullableTypeID channel, LFList<BFCItem> &result, const Frustum &frustum);
	private:
		std::vector<BFCBlockManager> _managers;
	};
}