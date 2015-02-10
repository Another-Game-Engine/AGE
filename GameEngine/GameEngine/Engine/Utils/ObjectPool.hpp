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
	protected:
		std::size_t _chunkSize;
		std::size_t _objectSize;
	};

	template <typename Type>
	class ObjectPool : public ObjectPoolBase
	{
	private:
		template <typename T>
		struct Chunk
		{
			Chunk(std::size_t size)
			{
				data = new char[sizeof(T) * size];
				for (auto i = 0; i < size; ++i)
				{
					trash.push(i);
				}
				sizeOfT = sizeof(T);
				from = (std::size_t)(data);
				to = from + sizeOfT * size;
			}
			
			~Chunk()
			{
				delete []data;
			}

			char *data;
			std::queue < std::size_t > trash;
			std::size_t sizeOfT;
			std::size_t from;
			std::size_t to;
			inline bool hasEmptyPlace() { return trash.size() > 0; }
			inline bool isIn(std::size_t addr) { return from <= addr && to >= addr; }

		};
	public:
		ObjectPool(std::size_t chunkSize = 1024)
			: ObjectPoolBase(chunkSize)
		{}

		virtual ~ObjectPool()
		{
			// very very dirty
			// for the moment it's leaking
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
					ptr->~Type();
					e.trash.push((addr - e.from) / e.sizeOfT);
					return;
				}
			}
			assert(false);
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
				_chunks.emplace_back<Chunk<Type>>(_chunkSize);
				chunk = &_chunks.back();
			}

			auto index = chunk->trash.front();
			chunk->trash.pop();
			Type *res = new (chunk->data + chunk->sizeOfT * index) Type();
			return res;
		}

		std::vector <Chunk<Type>> _chunks;
	};
}