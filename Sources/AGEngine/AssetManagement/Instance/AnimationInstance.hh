#pragma once

#include <Utils/Containers/Vector.hpp>
#include <memory>
#include <glm/glm.hpp>

namespace AGE
{
	struct Skeleton;
	struct AnimationData;
	class AnimationManager;

	struct AnimationInstance
	{
		AnimationInstance() = default;
		AnimationInstance(std::shared_ptr<Skeleton> _skeleton, std::shared_ptr<AnimationData> _animation = nullptr);
		AnimationInstance(const AnimationInstance &) = default;
		AnimationInstance &operator=(const AnimationInstance &) = default;

		inline std::size_t getTransformationsIndex() const { return _transformationIndex; }
		inline const glm::mat4 *getTransformations() const { return _tranformationBuffer; }
		inline glm::mat4 *getTransformations() { return _tranformationBuffer; }
		inline std::shared_ptr<Skeleton> getSkeleton() const { return skeleton; }
		inline std::shared_ptr<AnimationData> getAnimation() const { return animationData; }
		void update(float t);
		inline float getTimeMultiplier() const { return _timeMultiplier; }
		inline void setTimeMultiplier(float val) { _timeMultiplier = val; }
		inline std::size_t getInstanceCounter() const { return _instanceCounter; }
		inline bool isShared() const { return _isShared; }
		inline bool shareSameAnimation(const std::shared_ptr<AnimationData> &anim) const { return anim == animationData; }
		inline std::size_t getTransformationBufferSize() const { return _tranformationBufferSize; }

//todo private:
		std::shared_ptr<AnimationData> animationData;
		float time;
		std::shared_ptr<Skeleton> skeleton;
		AGE::Vector<glm::mat4> bindPoses;
		float _timeMultiplier = 10.0f;
		std::size_t _instanceCounter = 0;
		bool _isShared = false;
		std::size_t _transformationIndex = -1;
		glm::mat4 *_tranformationBuffer = nullptr;
		const std::size_t _tranformationBufferSize;
	};

}