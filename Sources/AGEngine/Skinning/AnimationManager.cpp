#include "AnimationManager.hpp"

#include <Skinning/Skeleton.hpp>

#include <Utils/Profiler.hpp>
#include <Utils/Debug.hpp>

#include <Threads/TaskScheduler.hpp>
#include <Threads/Tasks/BasicTasks.hpp>

namespace AGE
{
	AnimationManager::AnimationManager()
	{}

	AnimationManager::~AnimationManager()
	{}

	std::shared_ptr<AnimationInstance> AnimationManager::createAnimationInstance(std::shared_ptr<Skeleton> skeleton, std::shared_ptr<AnimationData> animation)
	{
		SCOPE_profile_cpu_function("Animations");

		std::lock_guard<std::mutex> lock(_mutex); //dirty lock not definitive, to test purpose

		auto instance = std::make_shared<AnimationInstance>(skeleton, animation);
		auto find = _animations.find(skeleton);
		if (find == _animations.end())
		{
			std::list<std::shared_ptr<AnimationInstance>> list;
			_animations.insert(std::make_pair(skeleton, list));
		}
		_animations[skeleton].push_back(instance);
		return instance;
	}

	void AnimationManager::deleteAnimationInstance(std::shared_ptr<AnimationInstance> animation)
	{
		SCOPE_profile_cpu_function("Animations");

		std::lock_guard<std::mutex> lock(_mutex); //dirty lock not definitive, to test purpose

		auto skeleton = animation->getSkeleton();
		AGE_ASSERT(_animations.find(skeleton) != _animations.end());
		_animations[skeleton].remove(animation);
	}

	void AnimationManager::update(float time)
	{
		SCOPE_profile_cpu_function("Animations");

		std::lock_guard<std::mutex> lock(_mutex); //dirty lock not definitive, to test purpose

		std::atomic_int16_t counter = 0;
		int taskNumber = 0;

		{
			SCOPE_profile_cpu_i("Animations", "Pushing skinning tasks");
			for (auto &s : _animations)
			{
				for (auto &a : s.second)
				{
					auto skeleton = s.first;
					EmplaceTask<Tasks::Basic::VoidFunction>([a, skeleton, &counter, time](){
						a->update(time);
						skeleton->updateSkinning(a);
						counter.fetch_add(1);
					});
					++taskNumber;
				}
			}
		}
		{
			SCOPE_profile_cpu_i("Animations", "Waiting for skinning tasks");
			while (counter < taskNumber)
			{
			}
		}
	}
}