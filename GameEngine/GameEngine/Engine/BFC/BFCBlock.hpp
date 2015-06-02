#pragma once

namespace AGE
{
	class BFCItem;

	class BFCBlock
	{
	public:
		static const int BlockMaxSize = 256;
	private:
		BFCItem _items[BlockMaxSize];
	};
}