#pragma once

#include <cstdint>

namespace AGE
{
	namespace Physics
	{
		enum class ColliderType : std::uint8_t
		{
			Box,
			Sphere,
			Capsule,
			Mesh
		};
	}
}