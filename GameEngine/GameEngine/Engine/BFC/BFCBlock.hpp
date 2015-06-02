#pragma once

#include <cstddef>
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
	private:
		BFCItem _items[BlockMaxSize];
		std::size_t _counter;
	};
}