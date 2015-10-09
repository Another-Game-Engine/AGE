#include "Skeleton.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <AssetManagement/Instance/AnimationInstance.hh>
#include <Utils/Profiler.hpp>

using namespace AGE;

Skeleton::Skeleton(const char *_path /*= nullptr*/)
: name("noname")
, firstBone(0)
, inverseGlobal(glm::mat4(1))
, path(path)
{}

void Skeleton::updateSkinning(std::shared_ptr<AnimationInstance> animInstance)
{
	SCOPE_profile_cpu_function("Animations");

	readNodeHierarchy(this->firstBone, animInstance);

	auto &transformations = animInstance->transformations;
	for (std::size_t i = 0; i < bones.size(); ++i)
	{
		transformations[i] *= bones[i].offset;
	}
}

void Skeleton::readNodeHierarchy(
	unsigned int boneID
	, std::shared_ptr<AnimationInstance> animInstance)
{
	auto &animation = *(animInstance.get());
	auto &bone = bones[boneID];

	glm::mat4 nodeT = animation.bindPoses[boneID];
	if (boneID == firstBone)
		animation.transformations[boneID] = nodeT;
	else
		animation.transformations[boneID] = animation.transformations[bone.parent] * nodeT;

	for (unsigned int i = 0; i < bones[boneID].children.size(); ++i)
	{
		readNodeHierarchy(bones[boneID].children[i], animInstance);
	}
}
