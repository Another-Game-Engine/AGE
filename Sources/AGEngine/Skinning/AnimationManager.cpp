#include "AnimationManager.hpp"

#include <Utils/Profiler.hpp>

namespace AGE
{
	AnimationManager::AnimationManager()
	{}

	AnimationManager::~AnimationManager()
	{}

	std::shared_ptr<AnimationInstance> AnimationManager::createAnimationInstance(std::shared_ptr<Skeleton> skeleton, std::shared_ptr<AnimationData> animation)
	{
		std::lock_guard<std::mutex> lock(_mutex); //dirty lock not definitive, to test purpose

		auto instance = std::make_shared<AnimationInstance>(skeleton, animation);
		return instance;
	}

	void AnimationManager::update(float time)
	{
		SCOPE_profile_cpu_function("Animations");

		std::lock_guard<std::mutex> lock(_mutex); //dirty lock not definitive, to test purpose

		//for (auto &e : _list)
		//{
		//	e->update(time);
		//}
	}
}