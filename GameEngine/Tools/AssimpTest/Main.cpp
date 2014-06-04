#include <assimp/Importer.hpp>
#include <assimp/types.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <glm.hpp>
#include <vector>

namespace AGE
{
	struct Mesh
	{
		std::vector<glm::vec4> positions;
		std::vector<glm::vec4> normals;
		std::vector<std::vector<glm::vec2>> uvs;
	};
}

void main(void)
{
	Assimp::Importer importer;

	const aiScene *scene = importer.ReadFile("../../Assets/catwoman/atk close front 6.fbx"
		, aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType |
		aiProcess_ImproveCacheLocality |
		aiProcess_OptimizeMeshes);

	if (!scene)
	{
		std::cerr << "Scene load error" << std::endl;
		return;
	}

	std::vector<AGE::Mesh> meshs;

	meshs.resize(scene->mNumMeshes);

	for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
	{
		aiMesh *mesh = scene->mMeshes[meshIndex];

		unsigned int meshFacesNbr = mesh->mNumFaces;
		for (unsigned int faceIndex = 0; faceIndex < meshFacesNbr; ++faceIndex)
		{
			const aiFace &face = mesh->mFaces[faceIndex];
			if (mesh->HasPositions())
			{
				for (unsigned int k = 0; k < 3; ++k)
				{
					auto &aiPositions = mesh->mVertices[face.mIndices[k]];
					meshs[meshIndex].positions.push_back(glm::vec4(aiPositions.x, aiPositions.y, aiPositions.y, 1));
				}
			}
			if (mesh->HasNormals())
			{
				for (unsigned int k = 0; k < 3; ++k)
				{
					auto &aiNormals = mesh->mNormals[face.mIndices[k]];
					meshs[meshIndex].normals.push_back(glm::vec4(aiNormals.x, aiNormals.y, aiNormals.y, 1));
				}
			}
			for (unsigned int texCoordIndex = 0; texCoordIndex < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++texCoordIndex)
			{
				if (mesh->HasTextureCoords(texCoordIndex))
				{
					meshs[meshIndex].uvs.resize(texCoordIndex + 1);
					for (unsigned int k = 0; k < 3; ++k)
					{
						auto &aiUvs = mesh->mTextureCoords[texCoordIndex][face.mIndices[k]];
						meshs[meshIndex].uvs[texCoordIndex].push_back(glm::vec2(aiUvs.x, aiUvs.y));
					}
				}
			}
		}
	}
}