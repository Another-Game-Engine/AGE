#pragma once

#include "Utils/Debug.hpp"

#include "BFC/BFCItem.hpp"
#include "BFC/BFCItemId.hpp"

namespace AGE
{
	template <typename T>
	class BFCArray
	{
	public:
		inline bool push(const T &item)
		{
			if (_index >= MaxItemID)
			{
				_index = MaxItemID;
				return false;
			}

			_array[_index++] = item;
			return true;
		}

		inline T &operator[](const ItemID i)
		{
			AGE_ASSERT(i < _index);
			return _array[i];
		}

		inline const T &operator[](const ItemID i) const
		{
			AGE_ASSERT(i < _index);
			return _array[i];
		}

		inline void clear()
		{
			_index = 0;
		}

		inline ItemID size() const
		{
			return _index;
		}

		inline T* data()
		{
			return &(_array[0]);
		}

		inline const T* data() const
		{
			return &(_array[0]);
		}

	private:
		T _array[MaxItemID];
		ItemID  _index = 0;
	};

	typedef BFCArray<BFCItem> BFCCullArray;
}