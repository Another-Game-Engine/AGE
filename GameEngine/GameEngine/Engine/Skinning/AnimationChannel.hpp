#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>

#include "AnimationKey.hpp"

namespace AGE
{
	struct AnimationChannel
	{
		std::uint32_t boneIndex;
		std::vector<AnimationKey<glm::vec3>> scale;
		std::vector<AnimationKey<glm::quat>> rotation;
		std::vector<AnimationKey<glm::vec3>> translation;

		void findKeyIndex(float t, glm::uvec3 &keys, glm::uvec3 &nextKeys);
		void getInterpolatedTransform(float t, glm::mat4 &res);
	};

	template <class Archive>
	void serialize(Archive &ar, AnimationChannel &value)
	{
		ar(cereal::make_nvp("bone", value.boneIndex));
		ar(cereal::make_nvp("scale", value.scale));
		ar(cereal::make_nvp("rotation", value.rotation));
		ar(cereal::make_nvp("translation", value.translation));
	}
}