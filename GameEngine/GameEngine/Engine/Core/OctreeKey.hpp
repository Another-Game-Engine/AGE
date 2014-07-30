#pragma once

#include <cstdint>

namespace AGE
{
	struct OctreeKey
	{
		enum Type
		{
			no_type = 0
			, Camera
			, Cullable
		};
		
		static const std::uint8_t NO_TYPE = Type::no_type;
		static const std::uint32_t NO_ID = (std::uint32_t)(-1);

		typedef std::uint8_t OctreeObjectType;
		typedef std::uint32_t OctreeObjectId;

		OctreeObjectType type = NO_TYPE;
		OctreeObjectId id = NO_ID;

		bool invalid() const
		{
			return (type == NO_TYPE || id == NO_ID);
		}

		bool operator==(const OctreeKey &o)
		{
			return o.id == id && o.type == type;
		}
	};
}