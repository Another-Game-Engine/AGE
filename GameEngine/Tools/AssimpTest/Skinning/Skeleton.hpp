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
		void readNodeHierarchy(unsigned int boneID
			, const glm::mat4 &parentTrans
			, std::vector<glm::mat4> &trans
			, AGE::Animation *animation = nullptr
			, float time = 0.0f);
	};
}