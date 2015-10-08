#pragma once

#include <list>

#include "BFCBlockManager.hpp"
#include "BFCItemID.hpp"

#include "IBFCCullCallback.hpp"


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
		void cullOnBlock(CullableTypeID channel, LFList<BFCItem> &result, const Frustum &frustum, std::size_t blockIdFrom, std::size_t numberOfBlocks, IBFCCullCallback *callback = nullptr);
		// do not cull, all tests are accepted
		void fillOnBlock(CullableTypeID channel, LFList<BFCItem> &result, std::size_t blockIdFrom, std::size_t numberOfBlocks, IBFCCullCallback *callback = nullptr);
		// return the number of block to treat (each in on job)
		std::size_t getBlockNumberToCull(CullableTypeID channel) const;
	private:
		std::vector<BFCBlockManager> _managers;
	};
}