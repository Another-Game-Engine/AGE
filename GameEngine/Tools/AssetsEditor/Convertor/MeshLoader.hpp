#pragma once

#include "AssimpLoader.hpp"
#include "SkeletonLoader.hpp"

#include <map>
#include <Skinning/Skeleton.hpp>
#include <Geometry/Mesh.hpp>

namespace AGE
{
	class MeshLoader
	{
	public:
		static bool save(AssetDataSet &dataSet)
		{
			if (dataSet.meshLoaded == false)
				return false;
			auto folderPath = std::tr2::sys::path(dataSet.serializedDirectory.path().directory_string() + "\\" + dataSet.filePath.getFolder());

			if (!std::tr2::sys::exists(folderPath) && !std::tr2::sys::create_directories(folderPath))
			{
					std::cerr << "Mesh convertor error : creating directory" << std::endl;
					return false;
			}
			auto fileName = dataSet.skinName.empty() ? dataSet.filePath.getShortFileName() + ".sage" : dataSet.skinName + ".sage";
			auto name = dataSet.serializedDirectory.path().directory_string() + "\\" + dataSet.filePath.getFolder() + fileName;

			std::ofstream ofs(name, std::ios::trunc | std::ios::binary);
			cereal::PortableBinaryOutputArchive ar(ofs);
			ar(*dataSet.mesh);
			return true;
		}

		static bool load(AssetDataSet &dataSet)
		{
			if (!dataSet.assimpScene)
			{
				if (!AssimpLoader::Load(dataSet))
					return false;
			}
			//if (!dataSet.skeletonLoaded)
			//{
			//	if (!SkeletonLoader::load(dataSet))
			//		return false;
			//}
			if (!dataSet.assimpScene->HasMeshes())
				return true;
			dataSet.meshLoaded = false;

			dataSet.mesh = new MeshData();

			dataSet.mesh->name = dataSet.skinName.empty() ? dataSet.filePath.getShortFileName() : dataSet.skinName;
			dataSet.mesh->subMeshs.resize(dataSet.assimpScene->mNumMeshes);

			auto &meshs = dataSet.mesh->subMeshs;

			AGE::Vector<BoundingInfos> subMeshBoundings;

			for (unsigned int meshIndex = 0; meshIndex < dataSet.assimpScene->mNumMeshes; ++meshIndex)
			{
				aiMesh *mesh = dataSet.assimpScene->mMeshes[meshIndex];

				std::uint32_t indice = 0;

				for (size_t i = 0; i < mesh->mNumVertices; i++)
				{
					if (mesh->HasPositions())
					{
						auto &aiPositions = mesh->mVertices[i];
						meshs[meshIndex].boundingInfos.addPosition(glm::vec3(aiPositions.x, aiPositions.y, aiPositions.z));
						meshs[meshIndex].positions.push_back(glm::vec4(aiPositions.x, aiPositions.y, aiPositions.z, 1));
						meshs[meshIndex].infos.set(MeshInfos::Positions);
					}
					if (mesh->HasNormals())
					{
						auto &aiNormals = mesh->mNormals[i];
						meshs[meshIndex].normals.push_back(glm::vec4(aiNormals.x, aiNormals.y, aiNormals.z, 1));
						meshs[meshIndex].infos.set(MeshInfos::Normals);
					}
					for (unsigned int texCoordIndex = 0; texCoordIndex < 1 /*AI_MAX_NUMBER_OF_TEXTURECOORDS*/; ++texCoordIndex)
					{
						if (mesh->HasTextureCoords(texCoordIndex))
						{
							meshs[meshIndex].uvs.resize(texCoordIndex + 1);
							auto &aiUvs = mesh->mTextureCoords[texCoordIndex][i];
							meshs[meshIndex].uvs[texCoordIndex].push_back(glm::vec2(aiUvs.x, aiUvs.y));
							meshs[meshIndex].infos.set(MeshInfos::Uvs);
						}
					}
					if (mesh->HasTangentsAndBitangents())
					{
						auto &aiTangents = mesh->mTangents[i];
						meshs[meshIndex].tangents.push_back(glm::vec4(aiTangents.x, aiTangents.y, aiTangents.z, 1));
						auto &aiBiTangents = mesh->mBitangents[i];
						meshs[meshIndex].biTangents.push_back(glm::vec4(aiBiTangents.x, aiBiTangents.y, aiBiTangents.z, 1));
						meshs[meshIndex].infos.set(MeshInfos::Tangents);
						meshs[meshIndex].infos.set(MeshInfos::BiTangents);
					}
				}

				subMeshBoundings.push_back(meshs[meshIndex].boundingInfos);

				unsigned int meshFacesNbr = mesh->mNumFaces;
				for (unsigned int faceIndex = 0; faceIndex < meshFacesNbr; ++faceIndex)
				{
					const aiFace &face = mesh->mFaces[faceIndex];
					for (unsigned int k = 0; k < 3; ++k)
					{
						meshs[meshIndex].indices.push_back(face.mIndices[k]);
					}
				}

				std::map<std::string, unsigned int> bonesIndices;
				unsigned int numBone = 0;

				meshs[meshIndex].weights.resize(meshs[meshIndex].positions.size(), glm::vec4(0));
				meshs[meshIndex].boneIndices.resize(meshs[meshIndex].positions.size(), glm::vec4(0));
				meshs[meshIndex].name = mesh->mName.C_Str();
				meshs[meshIndex].defaultMaterialIndex = mesh->mMaterialIndex > 0 ? mesh->mMaterialIndex : std::uint16_t(-1);
				for (unsigned int i = 0; i < mesh->mNumBones; ++i)
				{
					unsigned int boneIndex = dataSet.skeleton->bonesReferences.find(mesh->mBones[i]->mName.data)->second;

					meshs[meshIndex].infos.set(MeshInfos::Weights);
					meshs[meshIndex].infos.set(MeshInfos::BoneIndices);

					for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; ++j)
					{
						float weight = mesh->mBones[i]->mWeights[j].mWeight;
						float vid = mesh->mBones[i]->mWeights[j].mVertexId;
						if (meshs[meshIndex].weights[vid].r == 0.0f)
						{
							meshs[meshIndex].weights[vid].r = weight;
							meshs[meshIndex].boneIndices[vid].r = boneIndex;
						}
						else if (meshs[meshIndex].weights[vid].g == 0.0f)
						{
							meshs[meshIndex].weights[vid].g = weight;
							meshs[meshIndex].boneIndices[vid].g = boneIndex;
						}
						else if (meshs[meshIndex].weights[vid].b == 0.0f)
						{
							meshs[meshIndex].weights[vid].b = weight;
							meshs[meshIndex].boneIndices[vid].b = boneIndex;
						}
						else if (meshs[meshIndex].weights[vid].a == 0.0f)
						{
							meshs[meshIndex].weights[vid].a = weight;
							meshs[meshIndex].boneIndices[vid].a = boneIndex;
						}
					}
				}
			}
			dataSet.mesh->boundingInfos.initFromList(subMeshBoundings);
			dataSet.meshLoaded = true;
			return true;
		}
	};
}