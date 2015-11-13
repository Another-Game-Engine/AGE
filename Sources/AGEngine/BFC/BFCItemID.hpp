#pragma once

#include <stdint.h>

namespace AGE
{
	// represent :
	// - the block manager ID
	// - the block ID
	// - the item ID

	typedef uint16_t ItemID;
	static const ItemID MaxItemID = ItemID(0x400); // 1024
	typedef uint8_t BlockID;
	static const BlockID MaxBlockID = BlockID(0xFF); // 255
	typedef uint8_t BlockManagerID;
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
		BlockID        _blockID = 0;
		ItemID         _itemID = 0;

		friend class BFCBlockManagerFactory;
	};
}