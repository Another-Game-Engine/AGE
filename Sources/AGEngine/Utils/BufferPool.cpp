#include "BufferPool.hpp"

namespace AGE
{

	// BufferPool
	BufferPool::BufferPool()
		: _objectPerChunk(0)
		, _objectSize(0)
		, _freeObjectNumber(0)
		, _chunkAlignement(0)
		, _objectAlignement(0)
	{
	}

	BufferPool::~BufferPool()
	{
		_destroy();
	}

	bool BufferPool::_allocateChunk()
	{
		auto chunkSize = _getChunkSize();
		auto newChunk = (Chunk*)malloc(chunkSize);
		if (!newChunk)
			return false;
		newChunk->emptySlotsNumber = _objectPerChunk;
		newChunk->emptySlotsList.init();
		_chunks.push_front(newChunk);
		_freeObjectNumber += _objectPerChunk;

		auto data = (unsigned char*)(newChunk + 1);
		data += _chunkAlignement;

		for (std::size_t i = 0; i < _objectPerChunk; ++i)
		{
			((ChunkHeader*)(data))->index = i;

			data += sizeof(ChunkHeader);

			newChunk->emptySlotsList.push((Link*)(data));
			data += _objectSize + _objectAlignement;
		}
		return true;
	}

	bool BufferPool::_allocateObject(void *&addr)
	{
		if (_freeObjectNumber == 0)
		{
			auto noError = _allocateChunk();
			assert(noError);

		}
		for (auto &e : _chunks)
		{
			if (e->emptySlotsNumber != 0)
			{
				auto ptr = e->emptySlotsList.pop();
				--(e->emptySlotsNumber);
				--_freeObjectNumber;

				addr = ptr;
				return true;
			}
		}
		return false;
	}

	bool BufferPool::_dealocateObject(void *addr)
	{
		auto data = (unsigned char *)(addr);
		data -= sizeof(ChunkHeader);

		auto chunk = (Chunk*)(data - (((ChunkHeader*)(data))->index * (sizeof(ChunkHeader) + _objectSize + _objectAlignement)) - _chunkAlignement) - 1;
		++(chunk->emptySlotsNumber);
		chunk->emptySlotsList.push((Link*)(addr));
		++_freeObjectNumber;

		if (chunk->emptySlotsNumber == _objectPerChunk)
		{
			_freeObjectNumber -= _objectPerChunk;
			free(chunk);
			_chunks.remove(chunk);
		}

		return false;
	}

	void BufferPool::_destroy()
	{
		for (auto &e : _chunks)
		{
			free(e);
		}
	}

	bool BufferPool::_init(std::size_t objectSize, std::size_t objectAlignment, std::size_t chunkSize /* object per chunk */)
	{
		_objectSize = sizeof(Link);
		_objectPerChunk = chunkSize;

		if (objectSize > _objectSize)
			_objectSize = objectSize;

		_objectAlignement = objectAlignment - ((sizeof(ChunkHeader) + _objectSize) % objectAlignment);
		if (_objectAlignement == objectAlignment)
			_objectAlignement = 0;

		_chunkAlignement = objectAlignment - ((sizeof(ChunkHeader) + sizeof(Chunk)) % objectAlignment);
		if (_chunkAlignement == objectAlignment)
			_chunkAlignement = 0;
		return true;
	}

	// End BufferPool
	// BufferPool::Link
	BufferPool::Link::Link() :
		next(nullptr)
	{
	}
	// End BufferPool::Link
}