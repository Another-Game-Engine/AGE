#pragma once

#include "AssimpLoader.hpp"
#include "ImageLoader.hpp"
#include <map>

#include <Geometry/Material.hpp>

namespace AGE
{
	class MaterialLoader
	{
	public:
		static bool save(AssetDataSet &dataSet)
		{
			if (dataSet.materialsLoaded == false)
				return false;
			auto folderPath = std::tr2::sys::path(dataSet.serializedDirectory.path().directory_string() + "\\" + dataSet.filePath.getFolder());

			if (!std::tr2::sys::exists(folderPath) && !std::tr2::sys::create_directories(folderPath))
			{
					std::cerr << "Material convertor error : creating directory" << std::endl;
					return false;
			}
			auto fileName = dataSet.materialName.empty() ? dataSet.filePath.getShortFileName() + ".mage" : dataSet.materialName + ".mage";
			auto name = dataSet.serializedDirectory.path().directory_string() + "\\" + dataSet.filePath.getFolder() + fileName;
			MaterialDataSet materialDataSet;
			for (auto &m : dataSet.materials)
			{
				materialDataSet.collection.push_back(*m);
			}
			std::ofstream ofs(name, std::ios::trunc | std::ios::binary);
			cereal::PortableBinaryOutputArchive ar(ofs);
			ar(materialDataSet);
			return true;
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

				aiString diffuseTexPath;
				aiString ambientTexPath;
				aiString emissiveTexPath;
				aiString reflexionTexPath;
				aiString specularTexPath;
				aiString normalTexPath;

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

				material->diffuseTexPath = diffuseTexPath.length > 0 ? dataSet.filePath.getFolder() + "/" + AssimpLoader::aiStringToStd(diffuseTexPath) : "default.jpg";
				material->ambientTexPath = ambientTexPath.length > 0 ? dataSet.filePath.getFolder() + "/" + AssimpLoader::aiStringToStd(ambientTexPath) : "default.jpg";
				material->emissiveTexPath = emissiveTexPath.length > 0 ? dataSet.filePath.getFolder() + "/" + AssimpLoader::aiStringToStd(emissiveTexPath) : "default.jpg";
				material->reflectiveTexPath = reflexionTexPath.length > 0 ? dataSet.filePath.getFolder() + "/" + AssimpLoader::aiStringToStd(reflexionTexPath) : "default.jpg";
				material->specularTexPath = specularTexPath.length > 0 ? dataSet.filePath.getFolder() + "/" + AssimpLoader::aiStringToStd(specularTexPath) : "default.jpg";
				material->normalTexPath = normalTexPath.length > 0 ? dataSet.filePath.getFolder() + "/" + AssimpLoader::aiStringToStd(normalTexPath) : "default.jpg";

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