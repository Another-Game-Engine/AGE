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
		BufferPool();

		struct Link
		{
			Link *next;
			Link();
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

		bool _allocateChunk();

		bool _allocateObject(void *&addr);
		bool _dealocateObject(void *addr);

		void _destroy();
		inline std::size_t _getChunkSize() const { return _chunkAlignement + sizeof(Chunk) + _objectPerChunk * (_objectSize + _objectAlignement + sizeof(ChunkHeader)); }

		bool _init(std::size_t objectSize, std::size_t objectAlignment, std::size_t chunkSize /* object per chunk */);

		public:
			virtual ~BufferPool();

			virtual void destroy(void *ptr) = 0;
			virtual void *allocateObject() = 0;
	};
}
