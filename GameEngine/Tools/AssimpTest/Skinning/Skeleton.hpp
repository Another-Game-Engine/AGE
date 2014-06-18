#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace AGE
{
	struct AnimationInstance;
	struct Bone;
	struct Animation;

	struct Skeleton
	{
	public:
		std::vector<Bone> bones;
		std::vector<AnimationInstance*> animations;
		unsigned int firstBone;

		void updateSkinning();
	private:
		void readNodeHierarchy(unsigned int boneID);
	};
}