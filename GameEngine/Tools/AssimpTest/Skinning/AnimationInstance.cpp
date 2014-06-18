#include "AnimationInstance.hpp"
#include "Skeleton.hpp"
#include "Animation.hpp"
#include "AnimationChannel.hpp"
#include "Bone.hpp"

using namespace AGE;

AnimationInstance::AnimationInstance(Skeleton *_skeleton, Animation *_animation /* = nullptr */)
: skeleton(_skeleton)
, animation(_animation)
{
	_skeleton->animations.push_back(this);	
	bindPoses.resize(_skeleton->bones.size(), glm::mat4(1));
	transformations.resize(_skeleton->bones.size(), glm::mat4(1));
}

void AnimationInstance::update(float t)
{
	if (!animation)
		return;
	auto localTime = std::fmodf(t, animation->duration);

	for (std::size_t i = 0; i < skeleton->bones.size(); ++i)
	{
		bindPoses[i] = skeleton->bones[i].transformation;
		transformations[i] = skeleton->bones[i].transformation;
	}


	//for (auto i = 0; i < transformations.size(); ++i)
	//	transformations[i] = glm::mat4(1);

	for (std::size_t i = 0; i < animation->channels.size(); ++i)
	{
		animation->channels[i].getInterpolatedTransform(localTime, bindPoses[animation->channels[i].boneIndex]);
	}
}