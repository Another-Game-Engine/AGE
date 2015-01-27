#pragma once

#include "AssimpLoader.hpp"
#include "SkeletonLoader.hpp"

#include <map>
#include <Skinning/Skeleton.hpp>
#include <Geometry/Mesh.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "ConvertorStatusManager.hpp"

namespace AGE
{
	class MeshLoader
	{
	public:
		static bool save(AssetDataSet &dataSet)
		{
			auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("MeshLoader : saving " + dataSet.filePath.getShortFileName());
			auto folderPath = std::tr2::sys::path(dataSet.serializedDirectory.path().directory_string() + "\\" + dataSet.filePath.getFolder());

			if (!std::tr2::sys::exists(folderPath) && !std::tr2::sys::create_directories(folderPath))
			{
				Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
				std::cerr << "Mesh convector error : creating directory" << std::endl;
				return false;
			}
			auto fileName = dataSet.skinName.empty() ? dataSet.filePath.getShortFileName() + ".sage" : dataSet.skinName + ".sage";
			auto name = dataSet.serializedDirectory.path().directory_string() + "\\" + dataSet.filePath.getFolder() + fileName;

			std::ofstream ofs(name, std::ios::trunc | std::ios::binary);
			cereal::PortableBinaryOutputArchive ar(ofs);
			ar(*dataSet.mesh);
			Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
			return true;
		}

		static bool load(AssetDataSet &dataSet)
		{
			auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("MeshLoader : loading " + dataSet.filePath.getShortFileName());

			dataSet.mesh = std::make_shared<MeshData>();

			dataSet.mesh->name = dataSet.skinName.empty() ? dataSet.filePath.getShortFileName() : dataSet.skinName;
			dataSet.mesh->subMeshs.resize(dataSet.assimpScene->mNumMeshes);

			auto &meshs = dataSet.mesh->subMeshs;

			// The meshes dont have bounding box (only the subMeshes)
//			AGE::Vector<AGE::AABoundingBox> subMeshBoundings;

			for (unsigned int meshIndex = 0; meshIndex < dataSet.assimpScene->mNumMeshes; ++meshIndex)
			{
				aiMesh *mesh = dataSet.assimpScene->mMeshes[meshIndex];

				std::uint32_t indice = 0;

				// init the bounding box to the first vertex
				meshs[meshIndex].boundingBox.maxPoint = glm::vec3(mesh->mVertices[0].x, mesh->mVertices[0].y, mesh->mVertices[0].z);
				meshs[meshIndex].boundingBox.minPoint = glm::vec3(mesh->mVertices[0].x, mesh->mVertices[0].y, mesh->mVertices[0].z);

				for (size_t i = 0; i < mesh->mNumVertices; i++)
				{
					if (mesh->HasPositions())
					{
						auto &aiPositions = mesh->mVertices[i];
						// set the bounding box
						meshs[meshIndex].boundingBox.maxPoint = glm::max(meshs[meshIndex].boundingBox.maxPoint,
							glm::vec3(aiPositions.x, aiPositions.y, aiPositions.z));
						meshs[meshIndex].boundingBox.minPoint = glm::min(meshs[meshIndex].boundingBox.minPoint,
							glm::vec3(aiPositions.x, aiPositions.y, aiPositions.z));

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
				// The meshes dont have bounding box (only the subMeshes)
//				subMeshBoundings.push_back(meshs[meshIndex].boundingBox);

				glm::vec3 dist = meshs[meshIndex].boundingBox.maxPoint - meshs[meshIndex].boundingBox.minPoint;
				meshs[meshIndex].boundingBox.center = meshs[meshIndex].boundingBox.minPoint + (dist / 2.0f);

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

			glm::vec3 min(std::numeric_limits<float>::max());
			glm::vec3 max(std::numeric_limits<float>::min());
			for (auto &e : dataSet.mesh->subMeshs)
			{
				min = glm::min(e.boundingBox.minPoint, min);
				max = glm::max(e.boundingBox.maxPoint, max);
			}
			auto dif = max - min;
			float t = dif.x > dif.y ? dif.x : dif.y;
			t = t > dif.z ? t : dif.z;
			auto center = ((max - min) / 2.0f);
			auto center4 = glm::vec4(center.x, center.y, center.z, 0.0f);
			for (auto &e : dataSet.mesh->subMeshs)
			{
				//e.boundingBox.minPoint += center;
				e.boundingBox.minPoint /= t;
				//e.boundingBox.maxPoint += center;
				e.boundingBox.maxPoint /= t;
				for (auto &f : e.positions)
				{
					//f += center4;
					f /= t;
					f.w = 1.0f;
				}
				glm::vec3 dist = e.boundingBox.maxPoint - e.boundingBox.minPoint;
				e.boundingBox.center = e.boundingBox.minPoint + (dist / 2.0f);
			}
			Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
			return true;
		}
	};
}