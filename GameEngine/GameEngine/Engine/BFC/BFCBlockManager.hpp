#pragma once

#include <memory>

namespace AGE
{
	class BFCBlock;

	class BFCBlockManager
	{
	public:
		static const std::size_t BlockMaxNumber = 256;
	private:
		std::size_t _blockCounter;
		std::vector<std::shared_ptr<BFCBlock>> _blocks;
	};
}