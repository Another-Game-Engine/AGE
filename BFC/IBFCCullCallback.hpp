#pragma once

#include "Utils/Containers/LFList.hpp"
#include "BFC/BFCItem.hpp"

namespace AGE
{
	class IBFCCullCallback
	{
	public:
		virtual void operator()(LFList<BFCItem> &result, std::size_t blockId) = 0;
	};
}