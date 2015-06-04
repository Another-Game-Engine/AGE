#pragma once

#include <cstdint>
#include <functional>

#include "BFCItemID.hpp"

namespace AGE
{
	struct BFCCullableObject;
	class BFCBlockManagerFactory;

	class BFCCullableHandle
	{
	public:
		bool invalid() const
		{
			return (_itemID.isValid() == false || _elementPtr == nullptr);
		}

		bool operator==(const BFCCullableHandle &o) const
		{
			return o._itemID == _itemID && o._elementPtr == _elementPtr;
		}

		const BFCCullableObject *getPtr() const { return _elementPtr; }
		const BFCItemID getItemId() const { return _itemID; }
	private:
		BFCItemID              _itemID;
		BFCCullableObject*     _elementPtr = nullptr;

		friend class BFCBlockManagerFactory;
	};
}