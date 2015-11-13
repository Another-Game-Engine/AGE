#pragma once

#include <map>
#include <list>
#include <memory>
#include <mutex>

#include <Utils/Dependency.hpp>
#include <Utils/Containers/Vector.hpp>
#include <AssetManagement/Instance/AnimationInstance.hh>

namespace AGE
{
	struct AnimationData;
	struct Skeleton;

	class AnimationManager : public Dependency < AnimationManager >
	{
	public:
		AnimationManager();
		virtual ~AnimationManager();
		// if shared arg is true, the AnimationInstance will be a shared one (can be usefull for crowd for example)
		std::shared_ptr<AnimationInstance> createAnimationInstance(std::shared_ptr<Skeleton> skeleton, std::shared_ptr<AnimationData> animation, bool shared);
		void deleteAnimationInstance(std::shared_ptr<AnimationInstance> animation);
		void update(float time);

	private:
		std::mutex _mutex;
		std::map<std::shared_ptr<Skeleton>, std::list<std::shared_ptr<AnimationInstance>>> _animations;
		std::vector<glm::mat4> _bonesBuffers[2];
		std::uint8_t _currentBonesBufferIndex;
		std::size_t  _bonesBufferSize = 0;
	};
}