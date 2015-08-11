#pragma once

#include <cstddef>

namespace AGE
{
	// Lock Free List
	class LFListBase
	{
	private:
		__declspec(align(8))
		struct Int128
		{
			__int64 up;
			__int64 down;
		};

		__declspec(align(8))
		union Data
		{
			__declspec(align(8))
			struct
			{
				volatile __int64     size;
				volatile void        *head;
			};
			Int128           rawData;
		};

	public:
		LFListBase(std::size_t nextPtrPadding);
		void push(void *element);
		void *pop();
		inline std::size_t  getSize() const { return _datas.size; }
	private:
		std::size_t _nextPtrPadding;
		Data        _datas;
	};

	template <typename T>
	class LFList : public LFListBase
	{
	public:
		LFList()
			: LFListBase(offsetof(T, next))
		{}

		inline void push(T* element)
		{
			LFListBase::push((void*)(element));
		}

		inline T* pop()
		{
			return (T*)(LFListBase::pop());
		}
	};
}