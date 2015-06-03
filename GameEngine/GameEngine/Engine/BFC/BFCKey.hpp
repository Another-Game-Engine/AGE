#pragma once

#include <cstdint>
#include <functional>

namespace AGE
{
	struct BFCKey
	{
		enum Type
		{
			no_type = 0
			, Camera
			, Drawable
			, Mesh
			, PointLight
			, SpotLight
			, DirectionalLight
		};

		typedef std::uint8_t TypeId;
		typedef std::uint8_t ItemId;
		typedef std::uint8_t BlockId;
		typedef std::uint8_t Padding;

		static const TypeId NO_TYPE = Type::no_type;
		static const BlockId INVALID_BLOCK = (BlockId)(-1);

		ItemId   itemId = 0;
		BlockId  blockId = INVALID_BLOCK;
		TypeId   type = NO_TYPE;
		Padding  padding;
		void*    elementPtr = nullptr;


		bool invalid() const
		{
			return (type == NO_TYPE || blockId == INVALID_BLOCK);
		}

		bool operator==(const BFCKey &o) const
		{
			return o.itemId == itemId && o.blockId == blockId && o.type == type;
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
			// totaly random hash haha
			return std::hash<decltype(key.type)>()(key.type) ^ std::hash<decltype(key.itemId)>()(key.itemId) << 1 ^ std::hash<decltype(key.blockId)>()(key.blockId) << 2;
		}
	};
}