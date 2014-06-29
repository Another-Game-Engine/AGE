#pragma once

#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace AGE
{
	struct Skeleton;
	struct Animation;

	struct AnimationInstance
	{
		AnimationInstance(std::shared_ptr<Skeleton> _skeleton, std::shared_ptr<Animation> _animation = nullptr);

		std::shared_ptr<Animation> animation;
		float time;
		std::shared_ptr<Skeleton> skeleton;
		std::vector<glm::mat4> transformations;
		std::vector<glm::mat4> bindPoses;
		void update(float t);
	};

}