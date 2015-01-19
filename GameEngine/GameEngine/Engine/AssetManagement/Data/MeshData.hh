#pragma once

# include <string>
# include <vector>
# include <glm/glm.hpp>
# include <bitset>
# include <Utils/AABoundingBox.hh>
# include <Utils/OpenGL.hh>

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
	std::vector<glm::vec4> positions;
	std::vector<glm::vec4> normals;
	std::vector<glm::vec4> tangents;
	std::vector<glm::vec4> biTangents;
	std::vector<std::vector<glm::vec2>> uvs;
	std::vector<std::uint32_t> indices;
	std::vector<glm::vec4> weights;
	std::vector<glm::vec4> boneIndices;
	std::vector<glm::vec4> colors;
	AGE::AABoundingBox boundingBox;
	uint16_t defaultMaterialIndex;

public:
	template <class Archive> void serialize(Archive &ar);
};

struct MeshData
{
public:
	std::string name;
	std::vector<SubMeshData> subMeshs;

public:
	template <class Archive> void serialize(Archive &ar);
};

template <class Archive>
void SubMeshData::serialize(Archive &ar)
{
	ar(name, infos, positions, normals, tangents, biTangents, uvs, indices, weights, boneIndices, colors, boundingBox, defaultMaterialIndex);
}

template <class Archive>
void MeshData::serialize(Archive &ar)
{
	ar(name, subMeshs);
}