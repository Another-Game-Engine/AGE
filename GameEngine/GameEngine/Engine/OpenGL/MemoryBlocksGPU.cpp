#include <OpenGL/MemoryBlocksGPU.hh>
#include <iostream>
#include <stdint.h>

void *  __cdecl memcpy(_Out_writes_bytes_all_(_Size) void * _Dst, _In_reads_bytes_(_Size) const void * _Src, _In_ size_t _Size);

# define WARNING_MESSAGE_SETTING(type, index) \
	std::cerr << "warning: tentative to set value to the index : [" << index << "] which is out of range on a object MemoryBlocksGPU for " << type << std::endl;\

# define WARNING_MESSAGE_GETTING(type, index) \
	std::cerr << "warning: tentative to get value to the index : [" << index << "] which is out of range on a object MemoryBlocksGPU for " << type << std::endl;\

namespace gl
{
	MemoryBlocksGPU::MemoryBlocksGPU()
		: _nbrElement(0),
		_nbrBlock(0),
		_startBlocks(NULL),
		_sizeBlocks(NULL),
		_baseOffset(NULL)
	{

	}

	// the data into startblocks and sizeblocks must have nbrblock allocate
	MemoryBlocksGPU::MemoryBlocksGPU(size_t nbrElement, size_t nbrBlock, size_t *startBlocks, size_t *sizeBlocks)
		: _nbrElement(nbrElement),
		_nbrBlock(nbrBlock),
		_startBlocks(NULL),
		_sizeBlocks(NULL),
		_baseOffset(NULL)
	{
		if (nbrBlock)
		{
			_startBlocks = new size_t[nbrBlock];
			_sizeBlocks = new size_t[nbrBlock];
			_baseOffset = new size_t[nbrBlock];
			memcpy(_startBlocks, startBlocks, sizeof(size_t) * nbrBlock);
			memcpy(_sizeBlocks, sizeBlocks, sizeof(size_t) * nbrBlock);
			memset(_baseOffset, 0, sizeof(size_t) * nbrBlock);
		}
	}

	MemoryBlocksGPU::~MemoryBlocksGPU()
	{
		if (_nbrBlock)
		{
			delete[] _sizeBlocks;
			delete[] _startBlocks;
			delete[] _baseOffset;
		}
	}

	MemoryBlocksGPU::MemoryBlocksGPU(MemoryBlocksGPU const &copy)
		: _nbrElement(copy._nbrElement),
		_nbrBlock(copy._nbrBlock),
		_startBlocks(NULL),
		_sizeBlocks(NULL),
		_baseOffset(NULL)
	{
		if (_nbrBlock)
		{
			_sizeBlocks = new size_t[_nbrBlock];
			_startBlocks = new size_t[_nbrBlock];
			_baseOffset = new size_t[_nbrBlock];
		}
		for (size_t index = 0; index < copy._nbrBlock; ++index)
		{
			_sizeBlocks[index] = copy._sizeBlocks[index];
			_startBlocks[index] = copy._startBlocks[index];
			_baseOffset[index] = copy._baseOffset[index];
		}
	}

	MemoryBlocksGPU &MemoryBlocksGPU::operator=(MemoryBlocksGPU const &b)
	{
		if (&b != this)
		{
			_nbrElement = b._nbrElement;
			if (b._nbrBlock != _nbrBlock)
			{
				if (_nbrBlock)
				{
					delete[] _sizeBlocks;
					delete[] _startBlocks;
					delete[] _baseOffset;
				}
				_nbrBlock = b._nbrBlock;
				if (_nbrBlock)
				{
					_sizeBlocks = new size_t[_nbrBlock];
					_startBlocks = new size_t[_nbrBlock];
					_baseOffset = new size_t[_nbrBlock];
				}
				else
				{
					_sizeBlocks = NULL;
					_startBlocks = NULL;
					_baseOffset = NULL;
				}
			}
			for (size_t index = 0; index < b._nbrBlock; ++index)
			{
				_sizeBlocks[index] = b._sizeBlocks[index];
				_startBlocks[index] = b._startBlocks[index];
				_baseOffset[index] = b._baseOffset[index];
			}
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

	size_t MemoryBlocksGPU::getOffset(size_t index) const
	{
		if (index >= _nbrBlock)
		{
			WARNING_MESSAGE_GETTING("start", index);
			return (-1);
		}
		return (_baseOffset[index]);
	}

	// the values into the new storage are set to 0.
	MemoryBlocksGPU &MemoryBlocksGPU::setNbrBlock(size_t nbrBlock)
	{
		if (nbrBlock != _nbrBlock)
		{
			if (_nbrBlock)
			{
				delete[] _startBlocks;
				delete[] _sizeBlocks;
				delete[] _baseOffset;
			}
			_nbrBlock = nbrBlock;
			if (nbrBlock)
			{
				_sizeBlocks = new size_t[_nbrBlock];
				_startBlocks = new size_t[_nbrBlock];
				_baseOffset = new size_t[_nbrBlock];
				memset(_sizeBlocks, 0, sizeof(size_t) * _nbrBlock);
				memset(_startBlocks, 0, sizeof(size_t) * _nbrBlock);
				memset(_baseOffset, 0, sizeof(size_t)* _nbrBlock);
			}
			else
			{
				_sizeBlocks = NULL;
				_startBlocks = NULL;
				_baseOffset = NULL;
			}
		}
		return (*this);
	}

	MemoryBlocksGPU &MemoryBlocksGPU::setStartBlock(size_t index, size_t startBlock)
	{
		if (index >= _nbrBlock)
		{
			WARNING_MESSAGE_SETTING("start", index);
			return (*this);
		}
		_startBlocks[index] = startBlock;
		return (*this);
	}

	MemoryBlocksGPU &MemoryBlocksGPU::setSizeBlock(size_t index, size_t sizeBlock)
	{
		if (index >= _nbrBlock)
		{
			WARNING_MESSAGE_SETTING("size", index);
			return (*this);
		}
		_sizeBlocks[index] = sizeBlock;
		return (*this);
	}

	MemoryBlocksGPU &MemoryBlocksGPU::setOffset(size_t index, size_t offset)
	{
		if (index >= _nbrBlock)
		{
			WARNING_MESSAGE_SETTING("size", index);
			return (*this);
		}
		_baseOffset[index] = offset;
		return (*this);
	}

	MemoryBlocksGPU &MemoryBlocksGPU::setNbrElement(size_t nbrElement)
	{
		_nbrElement = nbrElement;
		return (*this);
	}

	size_t MemoryBlocksGPU::getNbrElement() const
	{
		return (_nbrElement);
	}
}