#pragma once

#include <atomic>

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
		LFListBase(std::size_t nextPtrPadding)
			: _nextPtrPadding(nextPtrPadding)
		{
			AGE_ASSERT(nextPtrPadding != 0);
			AGE_ASSERT(nextPtrPadding % 8 == 0);
			_datas.rawData.down = _datas.rawData.up = 0;
		}

		void push(void *element)
		{
			void *elementNext = (void*)((std::size_t)element + _nextPtrPadding);

			while (true)
			{
				Int128 oldDatas = _datas.rawData;
				Data datas = _datas;

				*(std::size_t*)(elementNext) = (std::size_t)datas.head;

				datas.head = element;
				datas.size += 1;

				if (_InterlockedCompareExchange128(
					&_datas.rawData.up,
					datas.rawData.down,
					datas.rawData.up,
					&oldDatas.up
					) == 1)
				{
					return;
				}
			}
		}
	private:
		std::size_t _nextPtrPadding;
		Data        _datas;
	};
}