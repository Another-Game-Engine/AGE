#pragma once

#include <vector>
#include <bitset>
#include <glm/glm.hpp>

#include <OpenGL/GeometryManager.hh>

#include <Utils/GlmSerialization.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/bitset.hpp>

namespace AGE
{
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
		std::vector<glm::vec4> positions;
		std::vector<glm::vec4> normals;
		std::vector<glm::vec4> tangents;
		std::vector<glm::vec4> biTangents;
		std::vector<std::vector<glm::vec2>> uvs;
		std::vector<std::uint32_t> indices;
		std::vector<glm::vec4> weights;
		std::vector<glm::vec4> boneIndices;
		std::vector<glm::vec4> colors;

		template <class Archive>
		void serialize(Archive &ar)
		{
			ar(name, infos, positions, normals, tangents, biTangents, uvs, indices, weights, boneIndices, colors);
		}
	};

	struct MeshData
	{
		std::string name;
		std::vector<SubMeshData> subMeshs;

		template <class Archive>
		void serialize(Archive &ar)
		{
			ar(name, subMeshs);
		}
	};

	struct SubMeshInstance
	{
		gl::Key<gl::Indices> indices;
		gl::Key<gl::Vertices> vertices;
		std::string name;
	};

	struct MeshInstance
	{
		std::string name;
		std::vector<SubMeshInstance> subMeshs;
	};

}