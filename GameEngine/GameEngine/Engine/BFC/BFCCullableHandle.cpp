#include "BFCCullableHandle.hpp"

namespace AGE
{
	bool BFCCullableHandle::invalid() const
	{
		return (_itemID.isValid() == false || _elementPtr == nullptr);
	}

	bool BFCCullableHandle::operator==(const BFCCullableHandle &o) const
	{
		return o._itemID == _itemID && o._elementPtr == _elementPtr;
	}

	const BFCCullableObject *BFCCullableHandle::getPtr() const
	{
		return _elementPtr;
	}

	BFCCullableObject *BFCCullableHandle::getPtr()
	{
		return _elementPtr;
	}

	const BFCItemID BFCCullableHandle::getItemId() const
	{
		return _itemID;
	}

}