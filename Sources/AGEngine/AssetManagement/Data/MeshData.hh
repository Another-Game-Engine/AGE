#pragma once

# include <cereal/archives/portable_binary.hpp>
# include <cereal/types/vector.hpp>
# include <cereal/types/bitset.hpp>
# include <string>
# include <vector>
# include <glm/glm.hpp>
# include <bitset>
# include <Utils/AABoundingBox.hh>
#include <stdint.h>

namespace AGE
{

	enum MeshInfos
	{
		Positions = 0,
		Normals,
		Tangents,
		BiTangents,
		Uvs,
		Weights,
		BoneIndices,
		Colors,
		END
	};

	struct SubMeshData
	{
	public:
		std::string name;
		std::bitset<MeshInfos::END> infos;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec3> tangents;
		std::vector<glm::vec3> biTangents;
		std::vector<std::vector<glm::vec2>> uvs;
		std::vector<std::uint32_t> indices;
		std::vector<glm::vec4> weights;
		std::vector<glm::vec4> boneIndices;
		std::vector<glm::vec4> colors;
		AGE::AABoundingBox boundingBox;
		uint16_t defaultMaterialIndex;

	public:
		template <class Archive> void serialize(Archive &ar, const std::uint32_t version);
	};

	struct MeshData
	{
	public:
		std::string name;
		std::vector<SubMeshData> subMeshs;
		AGE::AABoundingBox boundingBox;

	public:
		template <class Archive> void serialize(Archive &ar, const std::uint32_t version);
	};

	template <class Archive>
	void SubMeshData::serialize(Archive &ar, const std::uint32_t version)
	{
		ar(name, infos, positions, normals, tangents, biTangents, uvs, indices, weights, boneIndices, colors, boundingBox, defaultMaterialIndex);
	}

	template <class Archive>
	void MeshData::serialize(Archive &ar, const std::uint32_t version)
	{
		ar(name, subMeshs);
		if (version > 0)
		{
			ar(boundingBox);
		}
	}
}

CEREAL_CLASS_VERSION(AGE::MeshData, 1)
CEREAL_CLASS_VERSION(AGE::SubMeshData, 0)