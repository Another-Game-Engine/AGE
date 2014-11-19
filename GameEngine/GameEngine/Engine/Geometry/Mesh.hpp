#pragma once

#include <Utils/Containers/Vector.hpp>
#include <bitset>
#include <glm/glm.hpp>

#include <Utils/AABoundingBox.hh>

#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/bitset.hpp>
#include <Render/Pool.hh>
#include <Render/Key.hh>

class Vertices;
class Indices;

namespace AGE
{
	typedef std::uint16_t MaterialIndex;

	enum MeshInfos
	{
		Positions = 0
		, Normals
		, Tangents
		, BiTangents
		, Uvs
		, Weights
		, BoneIndices
		, Colors
		, END
	};

	struct SubMeshData
	{
		std::string name;
		std::bitset<MeshInfos::END> infos;
		AGE::Vector<glm::vec4> positions;
		AGE::Vector<glm::vec4> normals;
		AGE::Vector<glm::vec4> tangents;
		AGE::Vector<glm::vec4> biTangents;
		AGE::Vector<AGE::Vector<glm::vec2>> uvs;
		AGE::Vector<std::uint32_t> indices;
		AGE::Vector<glm::vec4> weights;
		AGE::Vector<glm::vec4> boneIndices;
		AGE::Vector<glm::vec4> colors;
		AGE::AABoundingBox boundingBox;
		MaterialIndex defaultMaterialIndex;

		template <class Archive>
		void serialize(Archive &ar)
		{
			ar(name, infos, positions, normals, tangents, biTangents, uvs, indices, weights, boneIndices, colors, boundingBox, defaultMaterialIndex);
		}
	};

	struct MeshData
	{
		std::string name;
		AGE::Vector<SubMeshData> subMeshs;

		template <class Archive>
		void serialize(Archive &ar)
		{
			ar(name, subMeshs);
		}
	};

	struct SubMeshInstance
	{
		Key<Vertices> vertices;
		AGE::AABoundingBox boundingBox;
		MaterialIndex defaultMaterialIndex;
	};

	struct MeshInstance
	{
		std::string name;
		std::string path;
		AGE::Vector<SubMeshInstance> subMeshs;
	};

}