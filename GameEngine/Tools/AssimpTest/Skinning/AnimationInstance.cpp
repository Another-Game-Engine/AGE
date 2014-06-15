#include "AnimationInstance.hpp"
#include "Skeleton.hpp"
#include "Animation.hpp"

using namespace AGE;

AnimationInstance::AnimationInstance(Skeleton *_skeleton, Animation *_animation /* = nullptr */)
: skeleton(_skeleton)
, animation(_animation)
{
	_skeleton->animations.push_back(this);
}

