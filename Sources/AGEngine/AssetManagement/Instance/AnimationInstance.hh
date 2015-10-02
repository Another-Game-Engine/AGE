#pragma once

#include <Utils/Containers/Vector.hpp>
#include <memory>
#include <glm/glm.hpp>

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

		inline const std::vector<glm::mat4> &getBones() const { return transformations; }
		inline std::shared_ptr<Skeleton> getSkeleton() const { return skeleton; }
		inline std::shared_ptr<AnimationData> getAnimation() const { return animationData; }
		void update(float t);
		inline float getTimeMultiplier() const { return _timeMultiplier; }
		inline void setTimeMultiplier(float val) { _timeMultiplier = val; }
		inline std::size_t getInstanceCounter() const { return _instanceCounter; }
		inline bool isShared() const { return _isShared; }
		inline bool shareSameAnimation(const std::shared_ptr<AnimationData> &anim) const { return anim == animationData; }

//todo private:
		std::shared_ptr<AnimationData> animationData;
		float time;
		std::shared_ptr<Skeleton> skeleton;
		AGE::Vector<glm::mat4> transformations;
		AGE::Vector<glm::mat4> bindPoses;
		float _timeMultiplier = 10.0f;
		std::size_t _instanceCounter = 0;
		bool _isShared = false;
	};

}