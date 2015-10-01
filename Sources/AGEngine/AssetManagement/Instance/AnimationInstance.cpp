#include <AssetManagement/Instance/AnimationInstance.hh>
#include <AssetManagement/Data/AnimationData.hpp>
#include <Skinning/Bone.hpp>
#include <Skinning/Skeleton.hpp>
#include <Skinning/AnimationChannel.hpp>

using namespace AGE;

AnimationInstance::AnimationInstance(std::shared_ptr<Skeleton> pSkeleton, std::shared_ptr<AnimationData> pAnimation)
	: skeleton(pSkeleton)
	, animationData(pAnimation)
{
	skeleton->animations.push_back(this);	
	bindPoses.resize(skeleton->bones.size(), glm::mat4(1));
	transformations.resize(skeleton->bones.size(), glm::mat4(1));
	if (!animationData)
	{
		for (auto i = 0; i < transformations.size(); ++i)
		{
			transformations[i] = skeleton->bones[i].transformation;
		}
	}
}

void AnimationInstance::update(float t)
{
	if (!animationData)
		return;
	auto localTime = std::fmodf(t, animationData->duration);

	for (std::size_t i = 0; i < animationData->channels.size(); ++i)
	{
		auto &test1 = animationData->channels[i];
		auto &test2 = test1.boneIndex;
		auto &test3 = bindPoses[test2];
		animationData->channels[i].getInterpolatedTransform(localTime, bindPoses[animationData->channels[i].boneIndex]);
	}
}