#pragma once

#include <Utils/Dependency.hpp>
#include <Utils/Containers/Vector.hpp>
#include <memory>
#include <Skinning/AnimationInstance.hpp>
#include <mutex>

namespace AGE
{
	class AnimationManager : public Dependency < AnimationManager >
	{
	public:
		AnimationManager()
		{}

		virtual ~AnimationManager()
		{}

		Key<AnimationInstance> createAnimationInstance(std::shared_ptr<AGE::Skeleton> skeleton, std::shared_ptr<AGE::Animation> animation)
		{
			std::lock_guard<std::mutex> lock(_mutex); //dirty lock not definitive, to test purpose

			auto instance = AGE::AnimationInstance(skeleton, animation);
			instance.key = Key<AGE::AnimationInstance>::createKey();
			if (instance.key.getId() >= _list.size())
				_list.resize(instance.key.getId() + 1);
			_list[instance.key.getId()] = instance;
			return instance.key;
		}

		void update(float time)
		{
			std::lock_guard<std::mutex> lock(_mutex); //dirty lock not definitive, to test purpose

			for (auto &e : _list)
			{
				e.update(time);
			}
		}

		std::vector<glm::mat4> &getBones(const Key<AnimationInstance> &key)
		{
			return _list[key.getId()].bindPoses;
		}

	private:
		std::vector<AGE::AnimationInstance> _list;
		std::mutex _mutex;
	};
}