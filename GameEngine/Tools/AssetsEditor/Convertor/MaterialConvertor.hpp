#pragma once

#include "AssimpLoader.hpp"

#include <map>

namespace AGE
{
	class MaterialConvertor
	{
	public:
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
				auto aiMat = dataSet.assimpScene->mMaterials[materialIndex];
			}

			if (!dataSet.materialsLoaded)
			{
				std::cerr << "AnimationLoader : Skeleton has not been loaded" << std::endl;
				return false;
			}
			dataSet.materialsLoaded = true;
			return true;
		}
	};
}