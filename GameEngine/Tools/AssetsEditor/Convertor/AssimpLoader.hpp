#pragma once

#include <assimp/Importer.hpp>
#include <assimp/types.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <iostream>

#include <Utils/File.hpp>

namespace AGE
{
	typedef const aiScene* AssimpFileData;

	namespace AssimpLoader
	{
		AssimpFileData LoadWithAssimp(const File &path)
		{
			if (!path.exists())
			{
				std::cerr << "File [" << path.getFullName() << "] does not exists." << std::endl;
				return nullptr;
			}
			Assimp::Importer importer;

			const aiScene *scene = importer.ReadFile(
				path.getFullName()
				, aiProcess_Triangulate |
				aiProcess_CalcTangentSpace |
				aiProcess_JoinIdenticalVertices |
				aiProcess_SortByPType |
				aiProcess_ImproveCacheLocality |
				aiProcess_OptimizeMeshes);

			if (scene == nullptr)
			{
				std::cerr << "Assimp fail to load file [" << path.getFullName() << "]" << std::endl;
				return nullptr;
			}
			return scene;
		}
	}
}