#pragma once

#include <cassert>
#include <forward_list>
#include <Utils/Profiler.hpp>

namespace AGE
{
	// Buffer pool largely inspired by :
	// http://www.codeproject.com/Articles/3526/Buffer-Pool-Object-Pool
	// Still need a LOOOOOOOTS of improvements

	class BufferPool
	{
	protected:
		BufferPool()
			: _objectPerChunk(0)
			, _objectSize(0)
			, _freeObjectNumber(0)
			, _chunkAlignement(0)
			, _objectAlignement(0)
		{}

		struct Link
		{
			Link *next;
			Link() : next(nullptr) {}
			inline void init() { next = nullptr; }
			inline bool empty() const { return (next == nullptr); }
			inline Link *pop()
			{
				auto res = next;
				next = next->next;
				return res;
			}
			inline void push(Link *l)
			{
				l->next = next;
				next = l;
			}
		};

		struct Chunk
		{
			std::size_t emptySlotsNumber;
			Link emptySlotsList;
		};

		struct ChunkHeader
		{
			std::size_t index;
		};

		std::forward_list<Chunk*> _chunks;
		std::size_t _objectPerChunk;
		std::size_t _objectSize;
		std::size_t _freeObjectNumber;
		std::size_t _chunkAlignement;
		std::size_t _objectAlignement;

		bool _allocateChunk()
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

		bool _allocateObject(void *&addr)
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

		bool _dealocateObject(void *addr)
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

		void _destroy()
		{
			for (auto &e : _chunks)
			{
				free(e);
			}
		}
		inline std::size_t _getChunkSize() const { return _chunkAlignement + sizeof(Chunk) + _objectPerChunk * (_objectSize + _objectAlignement + sizeof(ChunkHeader)); }

		bool _init(std::size_t objectSize, std::size_t objectAlignment, std::size_t chunkSize /* object per chunk */)
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

		public:
			virtual ~BufferPool()
			{
				_destroy();
			}

			virtual void destroy(void *ptr) = 0;
			virtual void *allocateObject() = 0;
	};
}
