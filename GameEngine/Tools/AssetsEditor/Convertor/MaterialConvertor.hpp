#pragma once

#include "AssimpLoader.hpp"
#include "ImageLoader.hpp"
#include <map>

#include <Utils/GlmSerialization.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

namespace AGE
{
		struct MaterialData
		{
			std::string name;

			glm::vec4 diffuse;
			glm::vec4 ambient;
			glm::vec4 emissive;
			glm::vec4 reflective;
			glm::vec4 specular;

			std::string diffuseTexPath;
			std::string ambientTexPath;
			std::string emissiveTexPath;
			std::string reflectiveTexPath;
			std::string specularTexPath;
			std::string normalTexPath;

			template <class Archive>
			void serialize(Archive &ar)
			{
				ar(name, diffuse, ambient, emissive, reflective, specular, diffuseTexPath, ambientTexPath, emissiveTexPath, reflectiveTexPath, specularTexPath, normalTexPath);
			}
		};

		struct MaterialDataSet
		{
			std::vector<MaterialData> collection;

			template <class Archive>
			void serialize(Archive &ar)
			{
				ar(collection);
			}
		};

	class MaterialLoader
	{
	public:
		static bool save(AssetDataSet &dataSet)
		{
			if (dataSet.materialsLoaded == false)
				return false;
			auto name = dataSet.destination.getFullName() + "/" + dataSet.filePath.getFolder() + dataSet.filePath.getShortFileName() + ".mage";
			MaterialDataSet materialDataSet;
			for (auto &m : dataSet.materials)
			{
				materialDataSet.collection.push_back(*m);
			}
			std::ofstream ofs(name, std::ios::trunc | std::ios::binary);
			cereal::PortableBinaryOutputArchive ar(ofs);
			ar(materialDataSet);			
		}
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
				material->reflective = AssimpLoader::aiColorToGlm(reflective);
				material->specular = AssimpLoader::aiColorToGlm(specular);

				material->diffuseTexPath = dataSet.filePath.getFolder() + "/" + AssimpLoader::aiStringToStd(diffuseTexPath);
				material->ambientTexPath = dataSet.filePath.getFolder() + "/" + AssimpLoader::aiStringToStd(ambientTexPath);
				material->emissiveTexPath = dataSet.filePath.getFolder() + "/" + AssimpLoader::aiStringToStd(emissiveTexPath);
				material->reflectiveTexPath = dataSet.filePath.getFolder() + "/" + AssimpLoader::aiStringToStd(reflexionTexPath);
				material->specularTexPath = dataSet.filePath.getFolder() + "/" + AssimpLoader::aiStringToStd(specularTexPath);
				material->normalTexPath = dataSet.filePath.getFolder() + "/" + AssimpLoader::aiStringToStd(normalTexPath);

				dataSet.texturesPath.push_back(material->diffuseTexPath);
				dataSet.texturesPath.push_back(material->ambientTexPath);
				dataSet.texturesPath.push_back(material->emissiveTexPath);
				dataSet.texturesPath.push_back(material->reflectiveTexPath);
				dataSet.texturesPath.push_back(material->specularTexPath);
				dataSet.texturesPath.push_back(material->normalTexPath);

				dataSet.materials.push_back(material);
            }

			if (dataSet.materials.size() == 0)
			{
				std::cerr << "MaterialLoader : Materials has not been loaded" << std::endl;
				return false;
			}
			dataSet.materialsLoaded = true;
			return true;
		}
	};
}