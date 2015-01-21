#pragma once

#include <Utils/Containers/Vector.hpp>
#include <bitset>
#include <glm/fwd.hpp>
#include <Utils/AABoundingBox.hh>
#include <Render/Pool.hh>
#include <Utils/Serialization/SerializationMacros.hpp>

SERIALIZATION_ARCHIVE_FORWARD_DECLARATION();

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

		SERIALIZATION_SERIALIZE_METHOD_DECLARATION();
	};

	struct MeshData
	{
		std::string name;
		AGE::Vector<SubMeshData> subMeshs;

		SERIALIZATION_SERIALIZE_METHOD_DECLARATION();
	};

	struct SubMeshInstance
	{
		gl::Key<gl::Indices> indices;
		gl::Key<gl::Vertices> vertices;
		
		AGE::AABoundingBox boundingBox;
//		std::string name;

		gl::Key<gl::IndexPool> indexPool;
		gl::Key<gl::VertexPool> vertexPool;

		MaterialIndex defaultMaterialIndex;
	};

	struct MeshInstance
	{
		std::string name;
		std::string path;
		AGE::Vector<SubMeshInstance> subMeshs;
	};
}
