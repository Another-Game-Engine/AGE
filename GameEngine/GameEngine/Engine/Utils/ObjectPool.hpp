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
			Chunk()
				: data(nullptr)
				, from(0)
				, to(0)
				, sizeOfT(0)
			{
			}

			void init(std::size_t size)
			{
				assert(data == nullptr);
				data = new char[sizeof(T) * size];
				for (auto i = 0; i < size; ++i)
				{
					trash.push(i);
				}
				sizeOfT = sizeof(T);
				from = (std::size_t)(data);
				to = from + sizeOfT * size;
			}

			Chunk(Chunk &&o)
				: data(nullptr)
				, from(0)
				, to(0)
				, sizeOfT(0)
			{
				std::swap(o.data, data);
				sizeOfT = std::move(o.sizeOfT);
				from = std::move(o.from);
				to = std::move(o.to);
			}

			Chunk(const Chunk &o) = delete;
			Chunk &operator=(const Chunk &o) = delete;
			Chunk &operator=(Chunk &&o) = delete;
			
			~Chunk()
			{
				if (data)
				{
					delete[]data;
				}
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
				_chunks.push_back(Chunk<Type>());
				chunk = &_chunks.back();
				chunk->init(_chunkSize);
			}

			auto index = chunk->trash.front();
			chunk->trash.pop();
			Type *res = new (chunk->data + chunk->sizeOfT * index) Type();
			return res;
		}

		std::vector <Chunk<Type>> _chunks;
	};
}