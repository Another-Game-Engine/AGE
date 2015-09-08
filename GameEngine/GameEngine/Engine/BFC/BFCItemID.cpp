#include "BFCItemID.hpp"

namespace AGE
{
	bool BFCItemID::operator==(const BFCItemID &o) const
	{
		return (_blockManagerID == o._blockManagerID && _blockID == o._blockID && _itemID == o._itemID);
	}
}