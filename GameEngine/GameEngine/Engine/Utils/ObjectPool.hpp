#pragma once

#include <vector>
#include <queue>

namespace AGE
{
	class ObjectPoolBase
	{
	public:
		ObjectPoolBase(std::size_t chunkSize);
		ObjectPoolBase() = delete;
		virtual ~ObjectPoolBase();
		virtual void destroy(void *ptr) = 0;
		virtual void *createVoidPtr() = 0;
	protected:
		std::size_t _chunkSize;
		std::size_t _objectSize;
	};

	template <typename Type>
	class ObjectPool : public ObjectPoolBase
	{
	private:
		struct ChunkHeader
		{
			ChunkHeader()
				:used(false)
			{}
			bool used;
		};

		template <typename T>
		struct Chunk
		{
			Chunk()
				: data(nullptr)
				, unalignedData(nullptr)
				, from(0)
				, to(0)
				, sizeOfT(0)
				, sizeOfObj(0)
				, blockNumber(0)
			{
			}

			void init(std::size_t size)
			{
				assert(data == nullptr);

				blockNumber = size;

				auto headerSize = sizeof(ChunkHeader);

				sizeOfT = sizeof(T);
				sizeOfObj = sizeOfT + headerSize;

				auto memSize = size * headerSize + size * sizeOfT;
				auto alignMemSize = memSize + 16;
				unalignedData = new char[alignMemSize];

				std::size_t mask = 16 - 1;
				std::size_t misalignment = (std::size_t)(unalignedData) & mask;
				auto adjustment = 16 - misalignment;
				std::cout << mask << " " << misalignment << " " << adjustment;
				data = (char*)(unalignedData + adjustment);

				for (auto i = 0; i < size; ++i)
				{
					new (data + sizeOfObj * i) ChunkHeader();
					trash.push(i);
				}
				from = (std::size_t)(data);
				to = from + memSize;
			}

			void release()
			{
				if (unalignedData)
				{
					for (auto i = 0; i < blockNumber; ++i)
					{
						auto header = (ChunkHeader*)(data + i * sizeOfObj);
						if (header->used)
						{
							auto ptr = (T*)(data + i * sizeOfObj + sizeof(ChunkHeader));
							ptr->~T();
						}
					}
					delete[]unalignedData;
					unalignedData = nullptr;
					data = nullptr;
				}
			}

			Chunk(Chunk &&o)
				: data(nullptr)
				, unalignedData(nullptr)
				, from(0)
				, to(0)
				, sizeOfT(0)
				, sizeOfObj(0)
			{
				std::swap(o.data, data);
				std::swap(o.unalignedData, unalignedData);
				sizeOfT = std::move(o.sizeOfT);
				from = std::move(o.from);
				to = std::move(o.to);
				sizeOfObj = std::move(o.sizeOfObj);
				blockNumber = std::move(o.blockNumber);
			}

			Chunk(const Chunk &o) = delete;
			Chunk &operator=(const Chunk &o) = delete;
			Chunk &operator=(Chunk &&o) = delete;
			
			~Chunk()
			{
			}

			void destroy(T *ptr)
			{
				delete ptr;
				return;
				ptr->~T();
				auto addr = std::size_t(ptr);
				((ChunkHeader*)((addr - sizeof(ChunkHeader))))->used = false;
				trash.push((addr - from - sizeof(ChunkHeader)) / sizeOfObj);
			}

			T *create()
			{
				return new T();
				auto index = trash.front();
				trash.pop();
				Type *res = new (data + sizeOfObj * index + sizeof(ChunkHeader)) Type();
				auto header = (ChunkHeader*)(std::size_t(res) - sizeof(ChunkHeader));
				header->used = true;
				return res;
			}

			char *data;
			char *unalignedData;
			std::queue < std::size_t > trash;
			std::size_t sizeOfT;
			std::size_t sizeOfObj;
			std::size_t from;
			std::size_t to;
			std::size_t blockNumber;
			inline bool hasEmptyPlace() { return trash.size() > 0; }
			inline bool isIn(std::size_t addr) { return from <= addr && to >= addr; }

		};
	public:
		ObjectPool(std::size_t chunkSize = 1024)
			: ObjectPoolBase(chunkSize)
		{}

		virtual ~ObjectPool()
		{
			for (auto &e : _chunks)
			{
				e.release();
			}
		}

		virtual void destroy(void *ptr) final
		{
			destroy((Type*)(ptr));
		}

		void destroy(Type *ptr)
		{
			if (!ptr)
				return;
			size_t addr = (size_t)(ptr);
			for (auto &e : _chunks)
			{
				if (e.isIn(addr))
				{
					e.destroy(ptr);
					return;
				}
			}
			assert(false);
		}

		virtual void* createVoidPtr()
		{
			return (void*)(create());
		}

		Type *create()
		{
			Chunk<Type> *chunk = nullptr;

			for (auto &e : _chunks)
			{
				if (e.hasEmptyPlace())
				{
					chunk = &e;
					break;
				}
			}

			if (chunk == nullptr)
			{
				_chunks.push_back(Chunk<Type>());
				chunk = &_chunks.back();
				chunk->init(_chunkSize);
			}

			return chunk->create();
		}

		std::vector <Chunk<Type>> _chunks;
	};
}