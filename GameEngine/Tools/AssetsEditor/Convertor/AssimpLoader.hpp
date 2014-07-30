#pragma once

#include <assimp/Importer.hpp>
#include <assimp/types.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <glm/glm.hpp>

#include <iostream>

#include <Utils/File.hpp>

#include "AssetDataSet.hpp"

namespace AGE
{
	struct Skeleton;

	namespace AssimpLoader
	{
		static void replaceExtension(std::string &path, const std::string &extension)
		{
			auto lastindex = path.find_last_of(".");
			if (lastindex == std::string::npos)
				return;
			path = path.substr(0, lastindex);
			path += extension;
		}

		static glm::mat4 aiMat4ToGlm(const aiMatrix4x4 &m)
		{
			return glm::mat4(m.a1, m.b1, m.c1, m.d1, m.a2, m.b2, m.c2, m.d2, m.a3, m.b3, m.c3, m.d3, m.a4, m.b4, m.c4, m.d4);
		}

		static glm::vec4 aiColorToGlm(const aiColor4D &m)
		{
			return glm::vec4(m.r, m.g, m.b, m.a);
		}

		static glm::vec4 aiColorToGlm(const aiColor3D &m)
		{
			return glm::vec4(m.r, m.g, m.b, 1.0f);
		}

		static std::string aiStringToStd(const aiString &m)
		{
			return std::string(m.C_Str());
		}

		bool Load(AssetDataSet &dataSet)
		{
			auto path = dataSet.rawDirectory.path().string() + "\\" + dataSet.filePath.getFullName();
			if (!File(path).exists())
			{
				std::cerr << "File [" << path << "] does not exists." << std::endl;
				return false;
			}

			dataSet.assimpScene = const_cast<aiScene*>(dataSet.assimpImporter.ReadFile(
				path
				, aiProcess_Triangulate |
				aiProcess_CalcTangentSpace |
				aiProcess_JoinIdenticalVertices |
				aiProcess_SortByPType |
				aiProcess_ImproveCacheLocality |
				aiProcess_OptimizeMeshes));

			if (dataSet.assimpScene == nullptr)
			{
				std::cerr << "Assimp fail to load file [" << path << "] : " << dataSet.assimpImporter.GetErrorString() << std::endl;
				return false;
			}
			return true;
		}
	}
}