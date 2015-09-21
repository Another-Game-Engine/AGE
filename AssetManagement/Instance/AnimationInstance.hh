#pragma once

#include <Utils/Containers/Vector.hpp>
#include <memory>
#include <glm/fwd.hpp>
#include <Utils/Key.hh>

namespace AGE
{
	struct Skeleton;
	struct AnimationData;

	struct AnimationInstance
	{
		AnimationInstance() = default;
		AnimationInstance(std::shared_ptr<Skeleton> _skeleton, std::shared_ptr<AnimationData> _animation = nullptr);
		AnimationInstance(const AnimationInstance &) = default;
		AnimationInstance &operator=(const AnimationInstance &) = default;

		std::shared_ptr<AnimationData> animationData;
		float time;
		std::shared_ptr<Skeleton> skeleton;
		AGE::Vector<glm::mat4> transformations;
		AGE::Vector<glm::mat4> bindPoses;
		void update(float t);
		Key<AnimationInstance> key;
	};

}