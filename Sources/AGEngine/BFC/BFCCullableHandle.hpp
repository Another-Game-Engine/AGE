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
		bool invalid() const;
		bool operator==(const BFCCullableHandle &o) const;
		inline const BFCCullableObject *getPtr() const { return _elementPtr; }
		inline BFCCullableObject *getPtr() { return _elementPtr; }
		inline const BFCItemID getItemId() const { return _itemID; }

	private:
		BFCItemID              _itemID;
		BFCCullableObject*     _elementPtr = nullptr;

		friend class BFCBlockManagerFactory;
	};
}