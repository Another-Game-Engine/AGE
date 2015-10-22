#include "BFCOutput.hpp"

namespace AGE
{
	IBFCOutput::IBFCOutput()
	{
		_counter.store(0);
		_resultQueue = nullptr;
	}

	void IBFCOutput::treatCulledChunk(const BFCCullArray *array)
	{
		_treatCulledChunk(array);
		std::size_t count = _counter.fetch_sub(1);
		// equal one when have cull stuff
		// equal 0 when there was nothig to cull
		if (count <= 1)
		{
			treatCulledResult();
		}
	}

	void IBFCOutput::treatCulledResult()
	{
		_treatCulledResult();
		_resultQueue->enqueue(this);
	}
	
	void IBFCOutput::setNumberOfBlocks(const std::size_t number)
	{
		_counter = number;
	}
}