#include "Skeleton.hpp"
#include <glm/glm.hpp>

#include "Animation.hpp"
#include "Bone.hpp"
#include "AnimationInstance.hpp"

using namespace AGE;

void Skeleton::updateSkinning(float time)
{
	for (auto &a : animations)
	{
		readNodeHierarchy(0);
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
		glm::mat4 t = this->animations[i]->transformations[bone.parent] * nodeT;
		this->animations[i]->transformations[boneID]
			= t * bone.offset * this->animations[i]->transformations[boneID];
	}

	for (unsigned int i = 0; i < bones[boneID].children.size(); ++i)
	{
		readNodeHierarchy(bones[boneID].children[i]);
	}
}
