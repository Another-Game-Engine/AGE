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

		void updateSkinning(float time);
	private:
		void readNodeHierarchy(unsigned int boneID);
	};
}