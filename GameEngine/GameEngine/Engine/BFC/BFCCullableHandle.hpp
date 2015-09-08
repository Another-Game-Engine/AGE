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
		const BFCCullableObject *getPtr() const;
		BFCCullableObject *getPtr();
		const BFCItemID getItemId() const;
	private:
		BFCItemID              _itemID;
		BFCCullableObject*     _elementPtr = nullptr;

		friend class BFCBlockManagerFactory;
	};
}