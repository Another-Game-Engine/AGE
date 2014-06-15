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
		readNodeHierarchy(0, glm::mat4(1), a->transformations, a->animation, time);
	}
}

void Skeleton::readNodeHierarchy(unsigned int boneID
	, const glm::mat4 &parentTrans
	, std::vector<glm::mat4> &trans
	, AGE::Animation *animation = nullptr
	, float time = 0.0f)
{
	glm::mat4 nodeT = bones[boneID].transformation;
	auto &bone = bones[boneID];

	if (animation)
	{
		auto localTime = std::fmodf(time, animation->duration);
		for (unsigned int i = 0; i < bone.animations[animation->id].size(); ++i)
		{
			bone.animations[animation->id][i]->getInterpolatedTransform(localTime, nodeT);
		}
	}

	glm::mat4 t = parentTrans * nodeT;
	trans[boneID] = t * bones[boneID].offset;

	for (unsigned int i = 0; i < bones[boneID].children.size(); ++i)
	{
		readNodeHierarchy(bones[boneID].children[i], t, trans, animation, time);
	}
}
