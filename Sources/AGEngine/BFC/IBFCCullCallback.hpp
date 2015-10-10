#pragma once

#include "Utils/Containers/LFList.hpp"
#include "Utils/Debug.hpp"

#include "BFC/BFCItem.hpp"
#include "BFC/BFCItemId.hpp"

namespace AGE
{
	template <typename T>
	class BFCArray
	{
	public:
		inline void push(const T &item)
		{
			AGE_ASSERT(_index < MaxItemID);

			_array[_index++] = item;
		}

		inline T &operator[](const ItemID i)
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

	typedef BFCArray<BFCItem> BFCCullVector;

	//// used as callback for traditionnal BFCCulling
	//class IBFCCullCallback
	//{
	//public:
	//	virtual void operator()(LFList<BFCItem> &result, std::size_t blockId) = 0;
	//};

	// used for custom culling
	class IBFCCuller
	{
	public:
		virtual void operator()() = 0;
		inline void push(const BFCItem &item) { _array.push(item); }
	protected:
		BFCCullVector      _array;
	};
}