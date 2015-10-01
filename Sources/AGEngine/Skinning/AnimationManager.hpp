#pragma once

#include <Utils/Dependency.hpp>
#include <Utils/Containers/Vector.hpp>
#include <memory>
#include <AssetManagement/Instance/AnimationInstance.hh>
#include <mutex>

namespace AGE
{
	struct AnimationData;
	struct Skeleton;

	class AnimationManager : public Dependency < AnimationManager >
	{
	public:
		AnimationManager();
		virtual ~AnimationManager();
		std::shared_ptr<AnimationInstance> createAnimationInstance(std::shared_ptr<Skeleton> skeleton, std::shared_ptr<AnimationData> animation);
		void update(float time);

	private:
		std::mutex _mutex;
	};
}