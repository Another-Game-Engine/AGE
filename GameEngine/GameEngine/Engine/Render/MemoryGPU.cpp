#include <Render/MemoryGPU.hh>
#include <iostream>
#include <stdint.h>
#include <assert.h>

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
	_sync(false),
	_isUsed(true)
{

}

MemoryBlocksGPU::~MemoryBlocksGPU()
{
}

MemoryBlocksGPU::MemoryBlocksGPU(MemoryBlocksGPU const &copy)
	: _nbrElement(copy._nbrElement),
	_startElement(copy._startElement),
	_sizeBlocks(copy._sizeBlocks),
	_baseOffset(copy._baseOffset),
	_sync(false),
	_isUsed(copy._isUsed)
{
}

MemoryBlocksGPU &MemoryBlocksGPU::operator=(MemoryBlocksGPU const &b)
{
	_isUsed = b._isUsed;
	_sync = false;
	_startElement = b._startElement;
	_nbrElement = b._nbrElement;
	_sizeBlocks = b._sizeBlocks;
	_baseOffset = b._baseOffset;
	return (*this);
}

size_t MemoryBlocksGPU::getElementStart() const
{
	return (_startElement);
}

size_t MemoryBlocksGPU::getOffset(uint8_t index) const
{
	return (_baseOffset[index]);
}

size_t MemoryBlocksGPU::getNbrBlock() const
{
	return (_sizeBlocks.size());
}

size_t MemoryBlocksGPU::getSizeBlock(uint8_t index) const
{
	return (_sizeBlocks[index]);
}

MemoryBlocksGPU &MemoryBlocksGPU::setStartElement(size_t start)
{
	_startElement = start;
	return (*this);
}

MemoryBlocksGPU &MemoryBlocksGPU::setOffset(uint8_t index, size_t offset)
{
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
	_sizeBlocks.resize(nbrBlock);
	_baseOffset.resize(nbrBlock);
	return (*this);
}

MemoryBlocksGPU &MemoryBlocksGPU::setSizeBlock(uint8_t index, size_t sizeBlock)
{
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
