#pragma once

#include <cstdint>
#include <functional>

namespace AGE
{
	struct PrepareKey
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

#ifdef AGE_BFC

		typedef std::uint8_t TypeId;
		typedef std::uint8_t ItemId;
		typedef std::uint8_t BlockId;
		typedef std::uint8_t Padding;

		static const TypeId NO_TYPE = Type::no_type;
		static const BlockId INVALID_BLOCK = (BlockId)(-1);

		ItemId itemId = 0;
		BlockId blockId = INVALID_BLOCK;
		TypeId type = NO_TYPE;
		Padding padding;


		bool invalid() const
		{
			return (type == NO_TYPE || blockId == INVALID_BLOCK);
		}

		bool operator==(const PrepareKey &o) const
		{
			return o.itemId == itemId && o.blockId == blockId && o.type == type;
		}
#else
		typedef std::uint8_t OctreeObjectType;
		typedef std::uint32_t OctreeObjectId;

		static const OctreeObjectType NO_TYPE = Type::no_type;
		static const OctreeObjectId NO_ID = (OctreeObjectId)(-1);

		OctreeObjectType type = NO_TYPE;
		OctreeObjectId id = NO_ID;

		bool invalid() const
		{
			return (type == NO_TYPE || id == NO_ID);
		}

		bool operator==(const PrepareKey &o) const
		{
			return o.id == id && o.type == type;
		}
#endif
	};
}

#ifdef AGE_BFC
namespace std
{
	template <>
	struct hash < AGE::PrepareKey > final
	{
		std::size_t operator()(const AGE::PrepareKey &key) const
		{
			// totaly random hash haha
			return std::hash<decltype(key.type)>()(key.type) ^ std::hash<decltype(key.itemId)>()(key.itemId) << 1 ^ std::hash<decltype(key.blockId)>()(key.blockId) << 2;
		}
	};
}

#else
namespace std
{
	template <>
	struct hash < AGE::PrepareKey > final
	{
		std::size_t operator()(const AGE::PrepareKey &key) const
		{
			return std::hash<decltype(key.type)>()(key.type) ^ (std::hash<decltype(key.id)>()(key.id) << 1);
		}
	};
}
#endif