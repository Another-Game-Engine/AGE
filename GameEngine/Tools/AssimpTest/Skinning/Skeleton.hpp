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
		glm::mat4 inverseGlobal;
		void updateSkinning();
	private:
		void readNodeHierarchy(unsigned int boneID);
	};
}