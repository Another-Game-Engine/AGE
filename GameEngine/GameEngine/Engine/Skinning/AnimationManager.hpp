#pragma once

#include <Utils/Dependency.hpp>
#include <list>
#include <memory>
#include <Skinning/AnimationInstance.hpp>

namespace AGE
{
	class AnimationManager : public Dependency < AnimationManager >
	{
	public:
		AnimationManager()
		{}

		virtual ~AnimationManager()
		{}

		std::shared_ptr<AGE::AnimationInstance> createAnimationInstance(std::shared_ptr<AGE::Skeleton> skeleton, std::shared_ptr<AGE::Animation> animation)
		{
			auto instance = std::make_shared<AGE::AnimationInstance>(skeleton, animation);
			_list.push_back(instance);
			return instance;
		}

		void update(float time)
		{
			for (auto &e : _list)
			{
				e->update(time);
			}
		}

	private:
		std::list<std::shared_ptr<AGE::AnimationInstance>> _list;
	};
}