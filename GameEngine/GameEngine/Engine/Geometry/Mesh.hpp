#pragma once

#include <Utils/Containers/Vector.hpp>
#include <bitset>
#include <glm/glm.hpp>

#include <Render/GeometryManager.hh>

#include <Utils/BoundingInfos.hpp>

#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/bitset.hpp>

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
		BoundingInfos boundingInfos;
		MaterialIndex defaultMaterialIndex;

		template <class Archive>
		void serialize(Archive &ar)
		{
			ar(name, infos, positions, normals, tangents, biTangents, uvs, indices, weights, boneIndices, colors, boundingInfos, defaultMaterialIndex);
		}
	};

	struct MeshData
	{
		std::string name;
		AGE::Vector<SubMeshData> subMeshs;
		AGE::BoundingInfos boundingInfos;

		template <class Archive>
		void serialize(Archive &ar)
		{
			ar(name, subMeshs, boundingInfos);
		}
	};

	struct SubMeshInstance
	{
		gl::Key<gl::Indices> indices;
		gl::Key<gl::Vertices> vertices;
		gl::Key<gl::VertexPool> vertexPool;
		gl::Key<gl::IndexPool> indexPool;
		AGE::BoundingInfos bounding;
//		std::string name;
		MaterialIndex defaultMaterialIndex;
	};

	struct MeshInstance
	{
		std::string name;
		AGE::Vector<SubMeshInstance> subMeshs;
	};

}