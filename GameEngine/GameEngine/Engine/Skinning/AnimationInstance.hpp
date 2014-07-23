#pragma once

#include <Utils/Containers/Vector.hpp>
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
		AGE::Vector<glm::mat4> transformations;
		AGE::Vector<glm::mat4> bindPoses;
		void update(float t);
	};

}