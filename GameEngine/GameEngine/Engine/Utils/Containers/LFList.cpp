#include "LFList.hpp"

#include <atomic>
#include "Utils/Debug.hpp"

AGE_NOT_OPTIMIZED_BLOCK_BEGIN

namespace AGE
{
	LFListBase::LFListBase(std::size_t nextPtrPadding)
		: _nextPtrPadding(nextPtrPadding)
	{
		AGE_ASSERT(nextPtrPadding != 0);
		AGE_ASSERT(nextPtrPadding % 8 == 0);
		_datas.rawData.down = _datas.rawData.up = 0;
	}

	void LFListBase::push(void *element)
	{
		void *elementNext = (void*)((std::size_t)element + _nextPtrPadding);

		while (true)
		{
			Int128 oldDatas = _datas.rawData;
			Data datas = _datas;

			*(std::size_t*)(elementNext) = (std::size_t)datas.head;
			AGE_ASSERT((std::size_t)(elementNext) % 8 == 0);
			AGE_ASSERT(datas->head != );

			datas.head = element;
			datas.size += 1;

			if (_InterlockedCompareExchange128(
				&_datas.rawData.up,
				datas.rawData.up,
				datas.rawData.down,
				&oldDatas.up
				) == 1)
			{
				return;
			}
		}
	}

	void *LFListBase::pop()
	{
		while (true)
		{
			Int128 oldDatas = _datas.rawData;
			Data datas = _datas;

			if (datas.head != nullptr)
			{
				void *res = (void*)(datas.head);

				datas.size -= 1;
				datas.rawData.down = *(__int64*)((void*)((size_t)(res)+_nextPtrPadding));

				if (_InterlockedCompareExchange128(
					&_datas.rawData.up,
					datas.rawData.down,
					datas.rawData.up,
					&oldDatas.up
					) == 1)
				{
					return res;
				}
			}
			else
			{
				return nullptr;
			}
		}
	}
}

AGE_NOT_OPTIMIZED_BLOCK_END
