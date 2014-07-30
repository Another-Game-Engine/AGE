#pragma once

#include <Utils/Containers/Vector.hpp>
#include <cstdint>

namespace AGE
{
	template <typename T>
	class IWorker
	{
	private:
		AGE::Vector<T> _pool;
		std::uint64_t  _cursor;
	public:
		IWorker()
			: _cursor(std::uint64_t(-1))
		{}

		virtual ~IWorker()
		{}

		void update(float deltaTime)
		{
			while (_cursor != std::uint64_t(-1))
			{
				_pool[cursor](deltaTime);
				--_cursor;
			}
		}

		template <typename... Args>
		T *create(Args ...args)
		{
			if (_pool.size() <= _cursor)
			{
				++_cursor;
				_pool.emplace_back<T>(args...);
				return &(_pool[_cursor]);
			}
			_pool[_cursor] = T(args...);
			++_cursor;
			return &(_pool[_cursor - 1]);
		}
	};
}