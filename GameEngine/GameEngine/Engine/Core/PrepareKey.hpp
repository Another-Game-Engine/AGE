#pragma once

#include <cstdint>

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

		bool operator==(const PrepareKey &o)
		{
			return o.id == id && o.type == type;
		}
	};
}