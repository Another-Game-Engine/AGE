#pragma once

#include <set>
#include <string>
#include <Utils/Containers/Vector.hpp>

#include <Utils/File.hpp>
#include <assimp/Importer.hpp>

#include <filesystem>
#include <memory>
#include "ConvertorStatusManager.hpp"

#include <atomic>

class btBvhTriangleMeshShape;
class btConvexHullShape;
class btTriangleMesh;

namespace AGE
{
	struct Skeleton;
	struct Animation;
	struct MeshData;
	struct MaterialData;
	struct TextureData;

	struct AssetDataSet
	{
		~AssetDataSet()
		{
			auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("Releasing memory for : " + filePath.getShortFileName());
			assimpImporter.FreeScene();
			animations.clear();
			materials.clear();
			textures.clear();
			Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
			Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(taskId);
		}

		AssetDataSet() = delete;
		AssetDataSet(const std::string &path)
		{
			filePath = File(path);
			taskId = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("In active conversion : " + filePath.getShortFileName());
		}

		std::size_t taskId;

		//Configurations
		bool loadSkeleton = true;
		bool loadAnimations = true;
		bool loadMesh = true;
		bool loadMaterials = true;
		bool loadTextures = true;
		bool loadPhysic = true;

		//Directory
		std::tr2::sys::directory_entry rawDirectory;
		std::tr2::sys::directory_entry serializedDirectory;

		//Paths
		File filePath = "";

		std::set<std::string> texturesPath;

		std::string animationName = ""; //if empty -> same name as file (fbx, collada)
		std::string skinName = ""; //if empty -> same name as file (fbx, collada)
		std::string skeletonName = ""; //if empty -> same name as file (fbx, collada)
		std::string materialName = ""; //if empty -> same name as file (fbx, collada)
		std::string physicName = "";

		//Ptrs
		std::shared_ptr<Skeleton> skeleton = nullptr;
		AGE::Vector<std::shared_ptr<Animation>> animations;
		std::shared_ptr<MeshData> mesh = nullptr;
		AGE::Vector<std::shared_ptr<MaterialData>> materials;
		AGE::Vector<std::shared_ptr<TextureData>> textures;
		std::shared_ptr<btBvhTriangleMeshShape> staticShape;
		std::shared_ptr<btTriangleMesh> staticTriangleMesh;
		std::shared_ptr<btConvexHullShape> dynamicShape;

		//Assimp
		aiScene* assimpScene = nullptr;
		Assimp::Importer assimpImporter;


		//Mesh Options
		bool normalize = true;
		float maxSideLength = 1.0f;
		bool positions = true;
		bool normals = true;
		bool bonesInfos = false;
		bool uvs = true;
		bool tangents = true;
		bool biTangents = true;

		//Physic Options
		bool staticConcave = true;
		bool dynamicConcave = true;
	};
}