#pragma once

#include <assimp/Importer.hpp>
#include <assimp/types.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <glm/glm.hpp>

#include <iostream>

#include <Utils/File.hpp>

namespace AGE
{
	struct Skeleton;

	struct AssetDataSet
	{
		aiScene* assimpScene = nullptr;
		Skeleton * skeleton = nullptr;
		File filePath = "";
		std::string name = "";
		Assimp::Importer assimpImporter;
	};

	namespace AssimpLoader
	{
		static glm::mat4 aiMat4ToGlm(const aiMatrix4x4 &m)
		{
			return glm::mat4(m.a1, m.b1, m.c1, m.d1, m.a2, m.b2, m.c2, m.d2, m.a3, m.b3, m.c3, m.d3, m.a4, m.b4, m.c4, m.d4);
		}

		bool Load(AssetDataSet &dataSet)
		{
			if (!dataSet.filePath.exists())
			{
				std::cerr << "File [" << dataSet.filePath.getFullName() << "] does not exists." << std::endl;
				return false;
			}

			dataSet.assimpScene = const_cast<aiScene*>(dataSet.assimpImporter.ReadFile(
				dataSet.filePath.getFullName()
				, aiProcess_Triangulate |
				aiProcess_CalcTangentSpace |
				aiProcess_JoinIdenticalVertices |
				aiProcess_SortByPType |
				aiProcess_ImproveCacheLocality |
				aiProcess_OptimizeMeshes));

			if (dataSet.assimpScene == nullptr)
			{
				std::cerr << "Assimp fail to load file [" << dataSet.filePath.getFullName() << "] : " << dataSet.assimpImporter.GetErrorString() << std::endl;
				return false;
			}
			return true;
		}
	}
}