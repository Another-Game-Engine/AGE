#pragma once

#include <Utils/Dependency.hpp>
#include <Utils/Containers/Vector.hpp>
#include <memory>
#include <AssetManagement/Data/AnimationData.hpp>
#include <AssetManagement/Instance/AnimationInstance.hh>
#include <mutex>
#include <Skinning/Skeleton.hpp>

namespace AGE
{
	class AnimationManager : public Dependency < AnimationManager >
	{
	public:
		AnimationManager()
		{}

		virtual ~AnimationManager()
		{}

		Key<AnimationInstance> createAnimationInstance(std::shared_ptr<Skeleton> skeleton, std::shared_ptr<AnimationData> animation)
		{
			std::lock_guard<std::mutex> lock(_mutex); //dirty lock not definitive, to test purpose

			auto instance = std::make_shared<AnimationInstance>(skeleton, animation);
			_list.push_back(instance);
			instance->key = Key<AnimationInstance>::createKey(_list.size() - 1);
			return instance->key;
		}

		void update(float time)
		{
			std::lock_guard<std::mutex> lock(_mutex); //dirty lock not definitive, to test purpose

			for (auto &e : _list)
			{
				e->update(time);
			}
		}

		std::vector<glm::mat4> &getBones(const Key<AnimationInstance> &key)
		{
			return _list[key.getId()]->transformations;
		}

	private:
		std::vector<std::shared_ptr<AGE::AnimationInstance>> _list;
		std::mutex _mutex;
	};
}