#include <AssetManagement/Instance/AnimationInstance.hh>
#include <AssetManagement/Data/AnimationData.hpp>

#include <Skinning/Bone.hpp>
#include <Skinning/Skeleton.hpp>
#include <Skinning/AnimationChannel.hpp>

#include <Utils/Profiler.hpp>

using namespace AGE;

AnimationInstance::AnimationInstance(std::shared_ptr<Skeleton> pSkeleton, std::shared_ptr<AnimationData> pAnimation)
	: skeleton(pSkeleton)
	, animationData(pAnimation)
	, _tranformationBufferSize(pSkeleton->bones.size())
{
	bindPoses.resize(skeleton->bones.size(), glm::mat4(1));
	if (!animationData)
	{
		//for (auto i = 0; i < _tranformationBufferSize; ++i)
		//{
		//	_tranformationBuffer[i] = skeleton->bones[i].transformation;
		//}
	}
}

void AnimationInstance::update(float t)
{
	SCOPE_profile_cpu_function("Animations");

	if (!animationData)
		return;
	auto localTime = std::fmodf(t, animationData->duration);

	for (std::size_t i = 0; i < animationData->channels.size(); ++i)
	{
		animationData->channels[i].getInterpolatedTransform(localTime, bindPoses[animationData->channels[i].boneIndex]);
	}
}