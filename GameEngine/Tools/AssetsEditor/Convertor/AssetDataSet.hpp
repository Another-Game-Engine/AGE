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

<<<<<<< HEAD
struct MeshData;
struct MaterialData;
struct TextureData;
=======
class btBvhTriangleMeshShape;
class btConvexHullShape;
class btTriangleMesh;
>>>>>>> prod_graphic

namespace AGE
{
	struct Skeleton;
	struct Animation;

	struct AssetDataSet
	{
		~AssetDataSet()
		{
		}

		AssetDataSet() = delete;
		AssetDataSet(const std::string &path)
		{
			filePath = File(path);
			isConverting = false;
		}

		std::atomic_bool isConverting;

		//Paths
		File filePath = "";

		//Configurations
		bool loadSkeleton = true;
		bool loadAnimations = true;
		bool loadMesh = true;
		bool loadMaterials = true;
		bool loadTextures = true;
		bool loadPhysic = true;

		//Mesh Options
		bool normalize = true;
		float maxSideLength = 1.0f;
		bool positions = true;
		bool normals = true;
		bool bonesInfos = true;
		bool uvs = true;
		bool tangents = true;
		bool biTangents = true;

		//Physic Options
		bool staticConcave = true;
		bool dynamicConcave = true;

		// Textures options
		bool compressTextures = true;
		bool generateMipmap = true;
		int mipmapLevels = 4;

		// Material options
		bool bumpToNormal = true;
		int normalStrength = 5;
	};
}