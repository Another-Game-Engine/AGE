#pragma once

#include <set>
#include <string>
#include <Utils/Containers/Vector.hpp>

#include <Utils/OldFile.hpp>
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
	struct AnimationData;
	struct MeshData;
	struct MaterialData;
	struct TextureData;

	struct AssetDataSet
	{
		~AssetDataSet()
		{
		}

		AssetDataSet() = delete;
		AssetDataSet(const std::string &path)
		{
			filePath = OldFile(path);
			isConverting = false;
		}

		std::atomic_bool isConverting;

		//Paths
		OldFile filePath = "";

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
		bool convex = true;
		bool concave = true;

		// Textures options
		bool compressTextures = false;
		int textureCompressionQuality = 2;
		bool generateMipmap = true;
		bool flipH = false;
		bool flipV = false;

		// Material options
		bool bumpToNormal = true;
		float normalStrength = 2;
	};
}