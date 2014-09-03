#include <Render/MemoryGPU.hh>
#include <iostream>
#include <stdint.h>
#include <assert.h>

namespace gl
{
	MemoryGPU::MemoryGPU()
		: size(0),
		offset(0),
		stride(0)
	{
	}

	MemoryGPU::MemoryGPU(MemoryGPU const &copy)
		: size(copy.size),
		offset(copy.offset),
		stride(copy.stride)
	{
	}

	MemoryGPU::~MemoryGPU()
	{
	}

	MemoryGPU::MemoryGPU(size_t size, size_t offset, size_t stride)
		: size(size),
		offset(offset),
		stride(stride)
	{
	}

	MemoryGPU &MemoryGPU::operator=(MemoryGPU const &m)
	{
		if (this != &m)
		{
			size = m.size;
			offset = m.offset;
			stride = m.stride;
		}
		return (*this);
	}

	bool MemoryGPU::operator<(MemoryGPU const &m) const
	{
		return (offset < m.offset);
	}

	bool MemoryGPU::operator>(MemoryGPU const &m) const
	{
		return (offset > m.offset);
	}

	bool MemoryGPU::operator>=(MemoryGPU const &m) const
	{
		return (offset >= m.offset);
	}

	bool MemoryGPU::operator<=(MemoryGPU const &m) const
	{
		return (offset <= m.offset);
	}

	bool MemoryGPU::operator==(MemoryGPU const &m) const
	{
		return (offset == m.offset);
	}

	bool MemoryGPU::operator!=(MemoryGPU const &m) const
	{
		return (offset != m.offset);
	}

	MemoryBlocksGPU::MemoryBlocksGPU()
		: _nbrElement(0),
		_startElement(0),
		_nbrBlock(0),
		_sizeBlocks(NULL),
		_baseOffset(NULL),
		_sync(false),
		_isUsed(true)
	{

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
		_startElement(copy._startElement),
		_nbrBlock(copy._nbrBlock),
		_sizeBlocks(NULL),
		_baseOffset(NULL),
		_sync(false),
		_isUsed(copy._isUsed)
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
			_isUsed = b._isUsed;
			_sync = false;
			_startElement = b._startElement;
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

	size_t MemoryBlocksGPU::getElementStart() const
	{
		return (_startElement);
	}

	size_t MemoryBlocksGPU::getOffset(uint8_t index) const
	{
		if (index >= _nbrBlock)
		{
			assert(0);
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
			assert(0);
			return (-1);
		}
		return (_sizeBlocks[index]);
	}

	MemoryBlocksGPU &MemoryBlocksGPU::setStartElement(size_t start)
	{
		_startElement = start;
		return (*this);
	}

	MemoryBlocksGPU &MemoryBlocksGPU::setOffset(uint8_t index, size_t offset)
	{
		if (index >= _nbrBlock)
		{
			assert(0);
			return (*this);
		}
		_baseOffset[index] = offset;
		return (*this);
	}

	MemoryBlocksGPU &MemoryBlocksGPU::setIsUsed(bool isused)
	{
		_isUsed = isused;
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
			assert(0);
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

	MemoryBlocksGPU &MemoryBlocksGPU::setSync(bool sync)
	{
		_sync = sync;
		return (*this);
	}

	bool MemoryBlocksGPU::getSync() const
	{
		return (_sync);
	}
	
	bool MemoryBlocksGPU::getIsUsed() const
	{
		return (_isUsed);
	}
}