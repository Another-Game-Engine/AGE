#pragma once

#include <string>
#include <vector>

#include <Utils/File.hpp>
#include <assimp/Importer.hpp>

namespace AGE
{
	struct Skeleton;
	struct Animation;
	struct MeshData;
	struct MaterialData;

	struct AssetDataSet
	{
		//Configurations
		bool loadSkeleton = false;
		bool loadAnimations = false;
		bool loadMesh = false;
		bool loadMaterials = false;
		bool loadTextures = false;

		//Results
		bool skeletonLoaded = false;
		bool animationLoaded = false;
		bool meshLoaded = false;
		bool materialsLoaded = false;
		bool texturesLoaded = false;

		//Paths
		File filePath = "";
		std::vector<File> texturesPath;

		std::string name = "";

		//Ptrs
		Skeleton *skeleton = nullptr;
		std::vector<Animation*> animations;
		MeshData *mesh;
		std::vector<MaterialData*> materials;
		//std::vector<Material*> material;
		//std::vector<Texture*> textures

		//Assimp
		aiScene* assimpScene = nullptr;
		Assimp::Importer assimpImporter;
	};
}