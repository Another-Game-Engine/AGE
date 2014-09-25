#pragma once

#include <set>
#include <string>
#include <Utils/Containers/Vector.hpp>

#include <Utils/File.hpp>
#include <assimp/Importer.hpp>

#include <filesystem>

namespace AGE
{
	struct Skeleton;
	struct Animation;
	struct MeshData;
	struct MaterialData;
	struct TextureData;

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

		//Ptrs
		Skeleton *skeleton = nullptr;
		AGE::Vector<Animation*> animations;
		MeshData *mesh;
		AGE::Vector<MaterialData*> materials;
		AGE::Vector<TextureData*> textures;

		//Assimp
		aiScene* assimpScene = nullptr;
		Assimp::Importer assimpImporter;
	};
}