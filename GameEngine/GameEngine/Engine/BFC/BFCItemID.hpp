#pragma once

#include <stdint.h>

namespace AGE
{
	// represent :
	// - the block manager ID
	// - the block ID
	// - the item ID

	typedef uint8_t ItemID; // In fact it's : 6
	static const ItemID MaxItemID = ItemID(0x3F); // 63
	typedef uint8_t BlockID; // In fact it's : 10
	static const BlockID MaxBlockID = BlockID(0x3FF); // 1023
	typedef uint8_t BlockManagerID; // It's really 8
	static const BlockManagerID MaxBlockManagerID = BlockManagerID(0xFF); // 255

	typedef BlockManagerID CullableTypeID;

	class BFCBlockManagerFactory;

	class BFCItemID
	{
	public:
		inline bool isValid() const { return _blockManagerID != MaxBlockManagerID; }
		bool operator==(const BFCItemID &o) const
		{
			return (_blockManagerID == o._blockManagerID && _blockID == o._blockID && _itemID == o._itemID);
		}
	private:
		BlockManagerID _blockManagerID = MaxBlockManagerID;
		BlockID        _blockID = 0;
		ItemID         _itemID = 0;

		friend class BFCBlockManagerFactory;
	};
}