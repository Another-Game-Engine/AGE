#pragma once

#include "AssimpLoader.hpp"

#include <map>

namespace AGE
{
		struct MaterialData
		{
			std::string name;

			glm::vec4 diffuse;
			glm::vec4 ambient;
			glm::vec4 emissive;
			glm::vec4 relfective;
			glm::vec4 specular;

			std::string diffuseTexPath;
			std::string ambientTexPath;
			std::string emissiveTexPath;
			std::string reflectiveTexPath;
			std::string specularTexPath;
			std::string normalTexPath;
		};

	class MaterialLoader
	{
	public:
		static bool load(AssetDataSet &dataSet)
		{
			dataSet.materialsLoaded = false;
			if (!dataSet.assimpScene)
			{
				if (!AssimpLoader::Load(dataSet))
					return false;
			}

			if (!dataSet.assimpScene->HasMaterials())
				return false;

			for (auto materialIndex = 0; materialIndex < dataSet.assimpScene->mNumMaterials; ++materialIndex)
			{
				auto &aiMat = dataSet.assimpScene->mMaterials[materialIndex];

				aiColor4D diffuse(1.0f,1.0f,1.0f,1.0f);
				aiColor4D ambient(1.0f, 1.0f, 1.0f, 1.0f);
				aiColor4D emissive(1.0f, 1.0f, 1.0f, 1.0f);
				aiColor4D reflective(1.0f, 1.0f, 1.0f, 1.0f);
				aiColor4D specular(1.0f, 1.0f, 1.0f, 1.0f);

				aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
				aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_AMBIENT, &ambient);
				aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_EMISSIVE, &emissive);
				aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_REFLECTIVE, &reflective);
				aiGetMaterialColor(aiMat, AI_MATKEY_COLOR_SPECULAR, &specular);

				aiString diffuseTexPath("__DEFAULT");
				aiString ambientTexPath("__DEFAULT");
				aiString emissiveTexPath("__DEFAULT");
				aiString reflexionTexPath("__DEFAULT");
				aiString specularTexPath("__DEFAULT");
				aiString normalTexPath("__DEFAULT");

				aiMat->GetTexture(aiTextureType_DIFFUSE, 0, &diffuseTexPath);
				aiMat->GetTexture(aiTextureType_AMBIENT, 0, &ambientTexPath);
				aiMat->GetTexture(aiTextureType_EMISSIVE, 0, &emissiveTexPath);
				aiMat->GetTexture(aiTextureType_REFLECTION, 0, &reflexionTexPath);
				aiMat->GetTexture(aiTextureType_SPECULAR, 0, &specularTexPath);
				aiMat->GetTexture(aiTextureType_NORMALS, 0, &normalTexPath);

				auto material = new MaterialData();

				material->diffuse = AssimpLoader::aiColorToGlm(diffuse);
				material->ambient = AssimpLoader::aiColorToGlm(ambient);
				material->emissive = AssimpLoader::aiColorToGlm(emissive);
				material->relfective = AssimpLoader::aiColorToGlm(reflective);
				material->specular = AssimpLoader::aiColorToGlm(specular);

				material->diffuseTexPath = AssimpLoader::aiStringToStd(diffuseTexPath);
				material->ambientTexPath = AssimpLoader::aiStringToStd(ambientTexPath);
				material->emissiveTexPath = AssimpLoader::aiStringToStd(emissiveTexPath);
				material->reflectiveTexPath = AssimpLoader::aiStringToStd(reflexionTexPath);
				material->specularTexPath = AssimpLoader::aiStringToStd(specularTexPath);
				material->normalTexPath = AssimpLoader::aiStringToStd(normalTexPath);

				dataSet.materials.push_back(material);
            }

			if (!dataSet.materials.size() == 0)
			{
				std::cerr << "AnimationLoader : Skeleton has not been loaded" << std::endl;
				return false;
			}
			dataSet.materialsLoaded = true;
			return true;
		}
	};
}