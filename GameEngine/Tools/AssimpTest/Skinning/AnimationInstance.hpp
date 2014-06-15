#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace AGE
{
	struct Skeleton;
	struct Animation;

	struct AnimationInstance
	{
		AnimationInstance(Skeleton *_skeleton, Animation *_animation = nullptr);

		Animation *animation;
		float time;
		Skeleton *skeleton;
		std::vector<glm::mat4> transformations;
	};

}