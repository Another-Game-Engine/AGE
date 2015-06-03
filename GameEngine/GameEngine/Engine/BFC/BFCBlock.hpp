#pragma once

#include <cstddef>
#include <queue>

#include "BFCItem.hpp"

namespace AGE
{
	class BFCBlock
	{
	public:
		static const std::size_t BlockMaxSize = 256;
		BFCBlock();
		bool isEmpty() const;
		bool isFull() const;
		std::uint8_t createItem(BFCRootType *drawable);
	private:
		BFCItem _items[BlockMaxSize];
		std::size_t _counter;
		std::queue<std::uint8_t> _free;
	};
}