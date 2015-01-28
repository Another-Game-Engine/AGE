#pragma once

#include <Utils/Containers/Vector.hpp>
#include <glm/glm.hpp>

#include "Bone.hpp"


#include <Utils/Serialization/MatrixSerialization.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/map.hpp>
#include <map>

namespace AGE
{
	struct AnimationInstance;
	struct Animation;

	struct Skeleton
	{
	public:
		Skeleton();
		std::string name;
		AGE::Vector<Bone> bones;
		AGE::Vector<AnimationInstance*> animations;
		std::uint32_t firstBone;
		glm::mat4 inverseGlobal;
		std::map<std::string, std::uint32_t> bonesReferences;

		void updateSkinning();

	private:
		void readNodeHierarchy(unsigned int boneID);
	};

	template <class Archive>
	void serialize(Archive &ar, Skeleton &value)
	{
		ar(cereal::make_nvp("name", value.name));
		ar(cereal::make_nvp("bones", value.bones));
		ar(cereal::make_nvp("firstBone", value.firstBone));
		ar(cereal::make_nvp("inverseGlobal", value.inverseGlobal));
		ar(cereal::make_nvp("bonesReferences", value.bonesReferences));
	}
}