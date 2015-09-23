#pragma once

#include <Utils/Containers/Vector.hpp>
#include <glm/fwd.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <Utils/Serialization/SerializationMacros.hpp>

SERIALIZATION_ARCHIVE_FORWARD_DECLARATION();

namespace AGE
{
	struct Bone
	{
		std::string name;
		glm::mat4 offset;
		std::uint32_t index;
		glm::mat4 transformation;
		AGE::Vector<std::uint32_t> children;
		std::uint32_t parent = (std::uint32_t)(-1);
	};
}

SERIALIZATION_SERIALIZE_DECLARATION(AGE::Bone);