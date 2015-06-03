#pragma once

#include <cstdint>
#include <functional>

namespace AGE
{
	class BFCRootType;

	struct BFCKey
	{
		typedef std::uint8_t ItemId;
		typedef std::uint8_t BlockId;

		static const BlockId INVALID_BLOCK = (BlockId)(-1);

		ItemId           itemId = 0;
		BlockId          blockId = INVALID_BLOCK;
		BFCRootType*     elementPtr = nullptr;


		bool invalid() const
		{
			return (blockId == INVALID_BLOCK || elementPtr == nullptr);
		}

		bool operator==(const BFCKey &o) const
		{
			return o.itemId == itemId && o.blockId == blockId && o.elementPtr == elementPtr;
		}
	};
}

namespace std
{
	template <>
	struct hash < AGE::BFCKey > final
	{
		std::size_t operator()(const AGE::BFCKey &key) const
		{
			// totally random hash function hahahahaha (cesar ^^)
			return std::hash<decltype(key.elementPtr)>()(key.elementPtr) ^ std::hash<decltype(key.itemId)>()(key.itemId) << 1 ^ std::hash<decltype(key.blockId)>()(key.blockId) << 2;
		}
	};
}