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
			_list.emplace_back(instance);
			instance.key = Key<AGE::AnimationInstance>::createKey(_list.size() - 1);
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