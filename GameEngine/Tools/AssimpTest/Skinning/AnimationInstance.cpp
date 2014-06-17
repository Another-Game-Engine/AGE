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
	//ATTENTION COPY TMP
	for (AnimationChannel &channel : animation->channels)
	{
		channel.getInterpolatedTransform(t, bindPoses[channel.boneIndex]);
	}
}