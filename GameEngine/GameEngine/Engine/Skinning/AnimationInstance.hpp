#pragma once

#include <Utils/Containers/Vector.hpp>
#include <memory>
#include <glm/glm.hpp>
#include <Render/Key.hh>

namespace AGE
{
	struct Skeleton;
	struct Animation;

	struct AnimationInstance
	{
		AnimationInstance() = default;
		AnimationInstance(std::shared_ptr<Skeleton> _skeleton, std::shared_ptr<Animation> _animation = nullptr);
		AnimationInstance(const AnimationInstance &) = default;
		AnimationInstance &operator=(const AnimationInstance &) = default;

		std::shared_ptr<Animation> animation;
		float time;
		std::shared_ptr<Skeleton> skeleton;
		AGE::Vector<glm::mat4> transformations;
		AGE::Vector<glm::mat4> bindPoses;
		void update(float t);
		Key<AnimationInstance> key;
	};

}