#pragma once

#include <stdint.h>

namespace AGE
{
	// represent :
	// - the block manager ID
	// - the block ID
	// - the item ID

	typedef uint8_t ItemID; // In fact it's : 6
	static const ItemID MaxItemID = ItemID(0x1FF); // 511
	typedef uint16_t BlockID;
	static const BlockID MaxBlockID = BlockID(0x400); // 1024
	typedef uint8_t BlockManagerID; // It's really 8
	static const BlockManagerID MaxBlockManagerID = BlockManagerID(0xFF); // 255

	typedef BlockManagerID CullableTypeID;

	class BFCBlockManagerFactory;

	class BFCItemID
	{
	public:
		inline bool isValid() const { return _blockManagerID != MaxBlockManagerID; }
		bool operator==(const BFCItemID &o) const;
	private:
		BlockManagerID _blockManagerID = MaxBlockManagerID;
		ItemID         _itemID = 0;
		BlockID        _blockID = 0;

		friend class BFCBlockManagerFactory;
	};
}