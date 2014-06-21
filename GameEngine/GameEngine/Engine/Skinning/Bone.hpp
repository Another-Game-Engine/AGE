#pragma once

#include <vector>
#include <glm/glm.hpp>

#include <Utils/GlmSerialization.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

namespace AGE
{
	struct Bone
	{
		std::string name;
		glm::mat4 offset;
		std::uint32_t index;
		glm::mat4 transformation;
		std::vector<unsigned int> children;
		unsigned int parent = (unsigned int)(-1);
	};

	template <class Archive>
	void serialize(Archive &ar, Bone &value)
	{
		ar(cereal::make_nvp("name", value.name));
		ar(cereal::make_nvp("offset", value.offset));
		ar(cereal::make_nvp("index", value.index));
		ar(cereal::make_nvp("transformation", value.transformation));
		ar(cereal::make_nvp("children", value.children));
		ar(cereal::make_nvp("parent", value.parent));
	}

}