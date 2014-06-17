#include "Skeleton.hpp"
#include <glm/glm.hpp>

#include "Bone.hpp"
#include "AnimationInstance.hpp"

using namespace AGE;

void Skeleton::updateSkinning(float time)
{
	readNodeHierarchy(0);

	for (std::size_t j = 0; j < this->animations.size(); ++j)
	{
		for (std::size_t i = 0; i < bones.size(); ++i)
		{
			this->animations[j]->transformations[i] = this->animations[j]->transformations[i] * this->bones[i].offset;
		}
	}
}

void Skeleton::readNodeHierarchy(
	unsigned int boneID)
{
	glm::mat4 nodeT = bones[boneID].transformation;
	auto &bone = bones[boneID];

	for (unsigned int i = 0; i < this->animations.size(); ++i)
	{
		nodeT = this->animations[i]->bindPoses[boneID];
		if (boneID == 0)
			continue;
		this->animations[i]->transformations[boneID] = this->animations[i]->transformations[bone.parent] * nodeT;
	}

	for (unsigned int i = 0; i < bones[boneID].children.size(); ++i)
	{
		readNodeHierarchy(bones[boneID].children[i]);
	}
}
