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

	///////////////////
	// Group

	bool BFCCullableHandleGroup::invalid() const
	{
		return (_handles.empty() == false);
	}
}