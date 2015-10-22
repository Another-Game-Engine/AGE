#pragma once

#include "BFCBlockManager.hpp"
#include "BFCItemID.hpp"
#include "IBFCCullCallback.hpp"
#include "BFCBlock.hpp"

#include <Utils/Containers/LFList.hpp>

namespace AGE
{
	struct BFCCullableObject;
	class  BFCCullableHandle;
	struct DRBData;
	class Frustum;
	class IBFCOutput;

	static const CullableTypeID MaxCullableTypeID = MaxBlockManagerID;

	class BFCBlockManagerFactory
	{
	public:
		BFCCullableHandle createItem(BFCCullableObject *object);
		void deleteItem(const BFCCullableHandle &handle);
		BFCItem &getItem(const BFCItemID &id);
		std::size_t cullOnBlock(CullableTypeID channel, LFList<BFCItem> &result, const Frustum &frustum, std::size_t blockIdFrom, std::size_t numberOfBlocks);
		std::size_t cullOnBlock(CullableTypeID channel, const Frustum &frustum, std::size_t blockIdFrom, std::size_t numberOfBlocks, IBFCCuller *culler);
		// do not cull, all tests are accepted
		std::size_t fillOnBlock(CullableTypeID channel, LFList<BFCItem> &result, std::size_t blockIdFrom, std::size_t numberOfBlocks, IBFCCuller *callback = nullptr);
		// return the number of block to treat (each in on job)
		std::size_t getBlockNumberToCull(CullableTypeID channel) const;

		template <typename CullerType>
		void cullOnBlock(
			CullableTypeID channel
			, CullerType *culler
			, std::size_t from
			, std::size_t numberOfBlocks
			, std::vector<IBFCOutput*> &outputs)
		{
			SCOPE_profile_cpu_function("BFC");

			AGE_ASSERT(channel < MaxCullableTypeID);

			if (channel < _managers.size())
			{
				auto &manager = _managers[channel];

				size_t i = 0;
				while (i < numberOfBlocks)
				{
					auto blockId = from + i;
					if (blockId >= manager._blocks.size())
						break;
					auto &block = manager._blocks[blockId];
					for (auto &item : block->_items)
					{
						culler->cullItem(item);
					}
					++i;
				}
			}
			for (auto &output : outputs)
			{
				auto array = (culler == nullptr ? nullptr : &culler->getArray());
				output->treatCulledChunk(array);
			}
		}
	private:
		std::vector<BFCBlockManager> _managers;
	};
}