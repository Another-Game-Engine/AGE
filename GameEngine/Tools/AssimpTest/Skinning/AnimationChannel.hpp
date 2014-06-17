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
		unsigned int boneIndex;
		std::vector<AnimationKey<glm::vec3>> scale;
		std::vector<AnimationKey<glm::quat>> rotation;
		std::vector<AnimationKey<glm::vec3>> translation;

		void findKeyIndex(float t, glm::uvec3 &keys, glm::uvec3 &nextKeys);
		void getInterpolatedTransform(float t, glm::mat4 &res);
	};
}