#include "Bone.hpp"
#include "Skeleton.hpp"
#include "Animation.hpp"
#include "AnimationChannel.hpp"

#include "AnimationInstance.hpp"

using namespace AGE;

AnimationInstance::AnimationInstance(std::shared_ptr<Skeleton> _skeleton, std::shared_ptr<Animation> _animation /* = nullptr */)
: skeleton(_skeleton)
, animation(_animation)
{
	_skeleton->animations.push_back(this);	
	bindPoses.resize(_skeleton->bones.size(), glm::mat4(1));
	transformations.resize(_skeleton->bones.size(), glm::mat4(1));
	if (!animation)
	{
		for (auto i = 0; i < transformations.size(); ++i)
		{
			transformations[i] = skeleton->bones[i].transformation;
		}
	}
}

void AnimationInstance::update(float t)
{
	if (!animation)
		return;
	auto localTime = std::fmodf(t, animation->duration);

	for (std::size_t i = 0; i < animation->channels.size(); ++i)
	{
		animation->channels[i].getInterpolatedTransform(localTime, bindPoses[animation->channels[i].boneIndex]);
	}
}