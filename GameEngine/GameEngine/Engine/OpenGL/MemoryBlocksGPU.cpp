#include <OpenGL/MemoryBlocksGPU.hh>
#include <iostream>
#include <stdint.h>

# define WARNING_MESSAGE_SETTING(type, index) \
	std::cerr << "warning: tentative to set value to the index : [" << index << "] which is out of range on a object MemoryBlocksGPU for " << type << std::endl;\

# define WARNING_MESSAGE_GETTING(type, index) \
	std::cerr << "warning: tentative to get value to the index : [" << index << "] which is out of range on a object MemoryBlocksGPU for " << type << std::endl;\

namespace gl
{
	MemoryBlocksGPU::MemoryBlocksGPU()
		: _nbrElement(0),
		_nbrBlock(0),
		_sizeBlocks(NULL),
		_baseOffset(NULL),
		sync(false)
	{

	}

	// the data into startblocks and sizeblocks must have nbrblock allocate
	MemoryBlocksGPU::MemoryBlocksGPU(size_t nbrElement, size_t nbrBlock, size_t *startBlocks, size_t *sizeBlocks)
		: _nbrElement(nbrElement),
		_nbrBlock(nbrBlock),
		_sizeBlocks(NULL),
		_baseOffset(NULL),
		sync(false)
	{
		if (nbrBlock)
		{
			_sizeBlocks = new size_t[nbrBlock];
			_baseOffset = new size_t[nbrBlock];
			memcpy(_sizeBlocks, sizeBlocks, sizeof(size_t) * nbrBlock);
			memset(_baseOffset, 0, sizeof(size_t) * nbrBlock);
		}
	}

	MemoryBlocksGPU::~MemoryBlocksGPU()
	{
		if (_nbrBlock)
		{
			delete[] _sizeBlocks;
			delete[] _baseOffset;
		}
	}

	MemoryBlocksGPU::MemoryBlocksGPU(MemoryBlocksGPU const &copy)
		: _nbrElement(copy._nbrElement),
		_nbrBlock(copy._nbrBlock),
		_sizeBlocks(NULL),
		_baseOffset(NULL),
		sync(false)
	{
		if (_nbrBlock)
		{
			_sizeBlocks = new size_t[_nbrBlock];
			_baseOffset = new size_t[_nbrBlock];
		}
		for (size_t index = 0; index < copy._nbrBlock; ++index)
		{
			_sizeBlocks[index] = copy._sizeBlocks[index];
			_baseOffset[index] = copy._baseOffset[index];
		}
	}

	MemoryBlocksGPU &MemoryBlocksGPU::operator=(MemoryBlocksGPU const &b)
	{
		if (&b != this)
		{
			sync = false;
			_nbrElement = b._nbrElement;
			if (b._nbrBlock != _nbrBlock)
			{
				if (_nbrBlock)
				{
					delete[] _sizeBlocks;
					delete[] _baseOffset;
				}
				_nbrBlock = b._nbrBlock;
				if (_nbrBlock)
				{
					_sizeBlocks = new size_t[_nbrBlock];
					_baseOffset = new size_t[_nbrBlock];
				}
				else
				{
					_baseOffset = NULL;
					_sizeBlocks = NULL;
				}
			}
			for (size_t index = 0; index < b._nbrBlock; ++index)
			{
				_sizeBlocks[index] = b._sizeBlocks[index];
				_baseOffset[index] = b._baseOffset[index];
			}
		}
		return (*this);
	}

	size_t MemoryBlocksGPU::getOffset(uint8_t index) const
	{
		if (index >= _nbrBlock)
		{
			WARNING_MESSAGE_GETTING("getOffset", index);
			return (-1);
		}
		return (_baseOffset[index]);
	}

	size_t MemoryBlocksGPU::getNbrBlock() const
	{
		return (_nbrBlock);
	}

	size_t MemoryBlocksGPU::getSizeBlock(uint8_t index) const
	{
		if (index >= _nbrBlock)
		{
			WARNING_MESSAGE_GETTING("size", index);
			return (-1);
		}
		return (_sizeBlocks[index]);
	}

	MemoryBlocksGPU &MemoryBlocksGPU::setOffset(uint8_t index, size_t offset)
	{
		if (index >= _nbrBlock)
		{
			WARNING_MESSAGE_SETTING("setOffset", index);
			return (*this);
		}
		_baseOffset[index] = offset;
		return (*this);
	}

	// the values into the new storage are set to 0.
	MemoryBlocksGPU &MemoryBlocksGPU::setNbrBlock(size_t nbrBlock)
	{
		if (nbrBlock != _nbrBlock)
		{
			if (_nbrBlock)
			{
				delete[] _baseOffset;
				delete[] _sizeBlocks;
			}
			_nbrBlock = nbrBlock;
			if (nbrBlock)
			{
				_baseOffset = new size_t[_nbrBlock];
				_sizeBlocks = new size_t[_nbrBlock];
				memset(_sizeBlocks, 0, sizeof(size_t) * _nbrBlock);
				memset(_baseOffset, 0, sizeof(size_t)* _nbrBlock);
			}
			else
			{
				_baseOffset = NULL;
				_sizeBlocks = NULL;
			}
		}
		return (*this);
	}

	MemoryBlocksGPU &MemoryBlocksGPU::setSizeBlock(uint8_t index, size_t sizeBlock)
	{
		if (index >= _nbrBlock)
		{
			WARNING_MESSAGE_SETTING("size", index);
			return (*this);
		}
		_sizeBlocks[index] = sizeBlock;
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