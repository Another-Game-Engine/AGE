#include "AnimationManager.hpp"

namespace AGE
{
	AnimationManager::AnimationManager()
	{}

	AnimationManager::~AnimationManager()
	{}

	Key<AnimationInstance> AnimationManager::createAnimationInstance(std::shared_ptr<Skeleton> skeleton, std::shared_ptr<AnimationData> animation)
	{
		std::lock_guard<std::mutex> lock(_mutex); //dirty lock not definitive, to test purpose

		auto instance = std::make_shared<AnimationInstance>(skeleton, animation);
		_list.push_back(instance);
		instance->key = Key<AnimationInstance>::createKey(_list.size() - 1);
		return instance->key;
	}

	void AnimationManager::update(float time)
	{
		std::lock_guard<std::mutex> lock(_mutex); //dirty lock not definitive, to test purpose

		for (auto &e : _list)
		{
			e->update(time);
		}
	}

	std::vector<glm::mat4> &AnimationManager::getBones(const Key<AnimationInstance> &key)
	{
		return _list[key.getId()]->transformations;
	}
}