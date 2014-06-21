#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Bone.hpp"


#include <Utils/GlmSerialization.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

namespace AGE
{
	struct AnimationInstance;
	struct Animation;

	struct Skeleton
	{
	public:
		std::string name;
		std::vector<Bone> bones;
		std::vector<AnimationInstance*> animations;
		std::uint32_t firstBone;
		glm::mat4 inverseGlobal;

		void updateSkinning();

	private:
		void readNodeHierarchy(unsigned int boneID);
	};

	template <class Archive>
	void serialize(Archive &ar, Skeleton &value)
	{
		ar(CEREAL_NVP(value.name));
		ar(CEREAL_NVP(value.bones));
		ar(CEREAL_NVP(value.firstBone));
		ar(CEREAL_NVP(value.inverseGlobal));
	}
}