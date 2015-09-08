#include "MeshLoader.hpp"

#include "AssimpLoader.hpp"
#include "SkeletonLoader.hpp"
#include <map>
#include <Skinning/Skeleton.hpp>
#include <AssetManagement/Data/MeshData.hh>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "ConvertorStatusManager.hpp"
#include "CookingTask.hpp"

namespace AGE
{
	bool MeshLoader::save(std::shared_ptr<CookingTask> cookingTask)
	{
		if (!cookingTask->dataSet->loadMesh)
			return true;
		auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("MeshLoader : saving " + cookingTask->dataSet->filePath.getShortFileName());
		auto folderPath = std::tr2::sys::path(cookingTask->serializedDirectory.path().directory_string() + "\\" + cookingTask->dataSet->filePath.getFolder());

		if (!std::tr2::sys::exists(folderPath) && !std::tr2::sys::create_directories(folderPath))
		{
			Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
			std::cerr << "Mesh convector error : creating directory" << std::endl;
			return false;
		}
		auto fileName = cookingTask->dataSet->filePath.getShortFileName() + ".sage";
		auto name = cookingTask->serializedDirectory.path().directory_string() + "\\" + cookingTask->dataSet->filePath.getFolder() + fileName;

		std::ofstream ofs(name, std::ios::trunc | std::ios::binary);
		cereal::PortableBinaryOutputArchive ar(ofs);
		ar(*cookingTask->mesh);
		Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
		return true;
	}

	bool MeshLoader::load(std::shared_ptr<CookingTask> cookingTask)
	{
		if (!cookingTask->dataSet->loadMesh)
			return true;
		auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("MeshLoader : loading " + cookingTask->dataSet->filePath.getShortFileName());

		cookingTask->mesh = std::make_shared<AGE::MeshData>();

		cookingTask->mesh->name = cookingTask->dataSet->filePath.getShortFileName();
		cookingTask->mesh->subMeshs.resize(cookingTask->assimpScene->mNumMeshes);

		auto &meshs = cookingTask->mesh->subMeshs;

		// The meshes dont have bounding box (only the subMeshes)
		//			AGE::Vector<AGE::AABoundingBox> subMeshBoundings;

		for (unsigned int meshIndex = 0; meshIndex < cookingTask->assimpScene->mNumMeshes; ++meshIndex)
		{
			aiMesh *mesh = cookingTask->assimpScene->mMeshes[meshIndex];

			std::uint32_t indice = 0;

			// init the bounding box to the first vertex
			meshs[meshIndex].boundingBox.maxPoint = glm::vec3(mesh->mVertices[0].x, mesh->mVertices[0].y, mesh->mVertices[0].z);
			meshs[meshIndex].boundingBox.minPoint = glm::vec3(mesh->mVertices[0].x, mesh->mVertices[0].y, mesh->mVertices[0].z);

			for (size_t i = 0; i < mesh->mNumVertices; i++)
			{
				if (mesh->HasPositions() && cookingTask->dataSet->positions)
				{
					auto &aiPositions = mesh->mVertices[i];
					// set the bounding box
					meshs[meshIndex].boundingBox.maxPoint = glm::max(meshs[meshIndex].boundingBox.maxPoint,
						glm::vec3(aiPositions.x, aiPositions.y, aiPositions.z));
					meshs[meshIndex].boundingBox.minPoint = glm::min(meshs[meshIndex].boundingBox.minPoint,
						glm::vec3(aiPositions.x, aiPositions.y, aiPositions.z));

					meshs[meshIndex].positions.push_back(glm::vec3(aiPositions.x, aiPositions.y, aiPositions.z));
					meshs[meshIndex].infos.set(MeshInfos::Positions);
				}
				if (mesh->HasNormals() && cookingTask->dataSet->normals)
				{
					auto &aiNormals = mesh->mNormals[i];
					meshs[meshIndex].normals.push_back(glm::vec3(aiNormals.x, aiNormals.y, aiNormals.z));
					meshs[meshIndex].infos.set(MeshInfos::Normals);
				}
				for (unsigned int texCoordIndex = 0; texCoordIndex < 1 /*AI_MAX_NUMBER_OF_TEXTURECOORDS*/; ++texCoordIndex)
				{
					if (mesh->HasTextureCoords(texCoordIndex) && cookingTask->dataSet->uvs)
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
					if (cookingTask->dataSet->tangents)
						meshs[meshIndex].tangents.push_back(glm::vec3(aiTangents.x, aiTangents.y, aiTangents.z));
					auto &aiBiTangents = mesh->mBitangents[i];
					if (cookingTask->dataSet->biTangents)
						meshs[meshIndex].biTangents.push_back(glm::vec3(aiBiTangents.x, aiBiTangents.y, aiBiTangents.z));
					if (cookingTask->dataSet->tangents)
						meshs[meshIndex].infos.set(MeshInfos::Tangents);
					if (cookingTask->dataSet->biTangents)
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
			if (cookingTask->dataSet->bonesInfos)
			{
				meshs[meshIndex].weights.resize(meshs[meshIndex].positions.size(), glm::vec4(0));
				meshs[meshIndex].boneIndices.resize(meshs[meshIndex].positions.size(), glm::vec4(0));
				meshs[meshIndex].name = mesh->mName.C_Str();
				// if MaterialIndex == 0, it's invalid
				meshs[meshIndex].defaultMaterialIndex = mesh->mMaterialIndex - 1;

				for (unsigned int i = 0; i < mesh->mNumBones; ++i)
				{
					unsigned int boneIndex = cookingTask->skeleton->bonesReferences.find(mesh->mBones[i]->mName.data)->second;

					meshs[meshIndex].infos.set(MeshInfos::Weights);
					meshs[meshIndex].infos.set(MeshInfos::BoneIndices);

					for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; ++j)
					{
						float weight = mesh->mBones[i]->mWeights[j].mWeight;
						unsigned int vid = mesh->mBones[i]->mWeights[j].mVertexId;
						if (meshs[meshIndex].weights[vid].r == 0.0f)
						{
							meshs[meshIndex].weights[vid].r = weight;
							meshs[meshIndex].boneIndices[vid].r = static_cast<float>(boneIndex);
						}
						else if (meshs[meshIndex].weights[vid].g == 0.0f)
						{
							meshs[meshIndex].weights[vid].g = weight;
							meshs[meshIndex].boneIndices[vid].g = static_cast<float>(boneIndex);
						}
						else if (meshs[meshIndex].weights[vid].b == 0.0f)
						{
							meshs[meshIndex].weights[vid].b = weight;
							meshs[meshIndex].boneIndices[vid].b = static_cast<float>(boneIndex);
						}
						else if (meshs[meshIndex].weights[vid].a == 0.0f)
						{
							meshs[meshIndex].weights[vid].a = weight;
							meshs[meshIndex].boneIndices[vid].a = static_cast<float>(boneIndex);
						}
					}
				}
			}
		}

		if (cookingTask->dataSet->normalize)
		{
			glm::vec3 min = cookingTask->mesh->subMeshs[0].boundingBox.minPoint;
			glm::vec3 max = cookingTask->mesh->subMeshs[0].boundingBox.maxPoint;
			for (auto &e : cookingTask->mesh->subMeshs)
			{
				min = glm::min(e.boundingBox.minPoint, min);
				max = glm::max(e.boundingBox.maxPoint, max);
			}
			auto dif = max - min;
			glm::vec3 decalage = (max - (dif / 2.0f)) * -1.0f;

			float t = dif.x > dif.y ? dif.x : dif.y;
			t = t > dif.z ? t : dif.z;

			decalage /= t;

			for (auto &e : cookingTask->mesh->subMeshs)
			{
				e.boundingBox.minPoint /= t;
				e.boundingBox.minPoint *= cookingTask->dataSet->maxSideLength;
				e.boundingBox.maxPoint /= t;
				e.boundingBox.maxPoint *= cookingTask->dataSet->maxSideLength;
				glm::vec3 dist = e.boundingBox.maxPoint - e.boundingBox.minPoint;
				e.boundingBox.center = e.boundingBox.minPoint + (dist / 2.0f);

				for (auto &f : e.positions)
				{
					f /= t;
					f *= cookingTask->dataSet->maxSideLength;
					f += decalage;
				}
			}
		}
		Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
		return true;
	}
}