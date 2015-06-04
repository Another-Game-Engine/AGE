#pragma once

#include <cstdint>
#include <functional>

#include "BFCItemID.hpp"

namespace AGE
{
	class BFCCullableObject;
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
	private:
		BFCItemID              _itemID;
		BFCCullableObject*     _elementPtr = nullptr;

		friend class BFCBlockManagerFactory;
	};
}