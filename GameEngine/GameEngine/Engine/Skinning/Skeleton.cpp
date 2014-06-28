#include "Skeleton.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "AnimationInstance.hpp"

using namespace AGE;

Skeleton::Skeleton()
: name("noname")
, firstBone(0)
, inverseGlobal(glm::mat4(1))
{}

void Skeleton::updateSkinning()
{
	if (this->animations.size() == 0)
		return;
	readNodeHierarchy(this->firstBone);

	for (std::size_t j = 0; j < this->animations.size(); ++j)
	{
		if (this->animations[j]->animation)
		{
			for (std::size_t i = 0; i < bones.size(); ++i)
			{
				this->animations[j]->transformations[i] *= this->bones[i].offset;
			}
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
		if (boneID == firstBone)
			this->animations[i]->transformations[boneID] = this->inverseGlobal * nodeT;
		else
			this->animations[i]->transformations[boneID] = this->animations[i]->transformations[bone.parent] * nodeT;
	}

	for (unsigned int i = 0; i < bones[boneID].children.size(); ++i)
	{
		readNodeHierarchy(bones[boneID].children[i]);
	}
}
