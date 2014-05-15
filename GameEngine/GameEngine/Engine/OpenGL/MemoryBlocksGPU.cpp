#include <OpenGL/MemoryBlocksGPU.hh>
#include <iostream>

# define WARNING_MESSAGE_SETTING(type, index) \
	std::cerr << "warning: tentative to set value to the index : [" << index << "] which is out of range on a object MemoryBlocksGPU for " << type << std::endl;\

# define WARNING_MESSAGE_GETTING(type, index) \
	std::cerr << "warning: tentative to get value to the index : [" << index << "] which is out of range on a object MemoryBlocksGPU for " << type << std::endl;\

namespace gl
{
	// the data into startblocks and sizeblocks must have nbrblock allocate
	MemoryBlocksGPU::MemoryBlocksGPU(size_t nbrBlock, size_t *startBlocks, size_t *sizeBlocks)
		: _nbrBlock(nbrBlock),
		_startBlocks(NULL),
		_sizeBlocks(NULL)
	{
		if (nbrBlock)
		{
			_startBlocks = new size_t[nbrBlock];
			_sizeBlocks = new size_t[nbrBlock];
			memcpy(_startBlocks, startBlocks, sizeof(size_t) * nbrBlock);
			memcpy(_sizeBlocks, sizeBlocks, sizeof(size_t) * nbrBlock);
		}
	}

	MemoryBlocksGPU::~MemoryBlocksGPU()
	{
		if (_nbrBlock)
		{
			delete[] _sizeBlocks;
			delete[] _startBlocks;
		}
	}

	MemoryBlocksGPU::MemoryBlocksGPU(MemoryBlocksGPU const &copy)
	{
		if (copy._nbrBlock)
		{
			if (copy._nbrBlock != _nbrBlock)
			{
				if (_nbrBlock)
				{
					delete[] _sizeBlocks;
					delete[] _startBlocks;
				}
				_nbrBlock = copy._nbrBlock;
				if (_nbrBlock)
				{
					_sizeBlocks = new size_t[_nbrBlock];
					_startBlocks = new size_t[_nbrBlock];
				}
				else
				{
					_sizeBlocks = NULL;
					_startBlocks = NULL;
				}
			}
			memcpy(_sizeBlocks, copy._sizeBlocks, sizeof(size_t) * copy._nbrBlock);
			memcpy(_startBlocks, copy._startBlocks, sizeof(size_t)* copy._nbrBlock);
		}
	}

	MemoryBlocksGPU &MemoryBlocksGPU::operator=(MemoryBlocksGPU const &b)
	{
		if (&b != this)
		{
			if (b._nbrBlock != _nbrBlock)
			{
				if (_nbrBlock)
				{
					delete[] _sizeBlocks;
					delete[] _startBlocks;
				}
				_nbrBlock = b._nbrBlock;
				if (_nbrBlock)
				{
					_sizeBlocks = new size_t[_nbrBlock];
					_startBlocks = new size_t[_nbrBlock];
				}
				else
				{
					_sizeBlocks = NULL;
					_startBlocks = NULL;
				}
			}
			memcpy(_sizeBlocks, b._sizeBlocks, sizeof(size_t)* b._nbrBlock);
			memcpy(_startBlocks, b._startBlocks, sizeof(size_t)* b._nbrBlock);
		}
		return (*this);
	}


	size_t MemoryBlocksGPU::getNbrBlock() const
	{
		return (_nbrBlock);
	}

	size_t MemoryBlocksGPU::getSizeBlock(size_t index) const
	{
		if (index >= _nbrBlock)
		{
			WARNING_MESSAGE_GETTING("size", index);
			return (-1);
		}
		return (_sizeBlocks[index]);
	}

	size_t MemoryBlocksGPU::getStartBlock(size_t index) const
	{
		if (index >= _nbrBlock)
		{
			WARNING_MESSAGE_GETTING("start", index);
			return (-1);
		}
		return (_startBlocks[index]);
	}

	// the values into the new storage are set to 0.
	void MemoryBlocksGPU::setNbrBlock(size_t nbrBlock)
	{
		if (nbrBlock != _nbrBlock)
		{
			if (_nbrBlock)
			{
				delete[] _startBlocks;
				delete[] _sizeBlocks;
			}
			_nbrBlock = nbrBlock;
			if (nbrBlock)
			{
				_sizeBlocks = new size_t[_nbrBlock];
				_startBlocks = new size_t[_nbrBlock];
				memset(_sizeBlocks, 0, sizeof(size_t) * _nbrBlock);
				memset(_startBlocks, 0, sizeof(size_t) * _nbrBlock);
			}
			else
			{
				_sizeBlocks = NULL;
				_startBlocks = NULL;
			}
		}
	}

	void MemoryBlocksGPU::setStartBlock(size_t index, size_t startBlock)
	{
		if (index >= _nbrBlock)
		{
			WARNING_MESSAGE_SETTING("start", index);
			return;
		}
		_startBlocks[index] = startBlock;
	}

	void MemoryBlocksGPU::setSizeBlock(size_t index, size_t sizeBlock)
	{
		if (index >= _nbrBlock)
		{
			WARNING_MESSAGE_SETTING("size", index);
			return;
		}
		_sizeBlocks[index] = sizeBlock;
	}
}