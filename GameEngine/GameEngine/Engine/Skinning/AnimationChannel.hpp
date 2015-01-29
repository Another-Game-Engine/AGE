#pragma once

#include <Utils/Containers/Vector.hpp>
#include <Utils/Serialization/SerializationMacros.hpp>
#include <glm/fwd.hpp>
#include "AnimationKey.hpp"

SERIALIZATION_ARCHIVE_FORWARD_DECLARATION();

namespace AGE
{
	struct AnimationChannel
	{
		std::uint32_t boneIndex;
		AGE::Vector<AnimationKey<glm::vec3>> scale;
		AGE::Vector<AnimationKey<glm::quat>> rotation;
		AGE::Vector<AnimationKey<glm::vec3>> translation;

		void findKeyIndex(float t, glm::uvec3 &keys, glm::uvec3 &nextKeys);
		void getInterpolatedTransform(float t, glm::mat4 &res);

		SERIALIZATION_SERIALIZE_METHOD_DECLARATION();
	};
}