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
	};
}

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
