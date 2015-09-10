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
		AnimationManager();
		virtual ~AnimationManager();
		Key<AnimationInstance> createAnimationInstance(std::shared_ptr<Skeleton> skeleton, std::shared_ptr<AnimationData> animation);
		void update(float time);
		std::vector<glm::mat4> &getBones(const Key<AnimationInstance> &key);

	private:
		std::vector<std::shared_ptr<AGE::AnimationInstance>> _list;
		std::mutex _mutex;
	};
}