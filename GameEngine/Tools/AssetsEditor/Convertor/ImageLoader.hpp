#pragma once

#include "AssimpLoader.hpp"
#include <Texture/TextureData.hpp>
#include <SOIL.h>

namespace AGE
{
	class ImageLoader
	{
	public:
		static bool save(AssetDataSet &dataSet)
		{
			if (dataSet.texturesLoaded == false)
				return false;
			for (auto &m : dataSet.textures)
			{
				auto name = dataSet.destination.getFullName() + "/" + File(m->rawPath).getFolder() + "/" + File(m->rawPath).getShortFileName() + ".tage";
				std::ofstream ofs(name, std::ios::trunc | std::ios::binary);
				cereal::PortableBinaryOutputArchive ar(ofs);
				ar(*m);
			}
		}

		static bool load(AssetDataSet &dataSet)
		{
			dataSet.texturesLoaded = false;
			if (!dataSet.assimpScene)
			{
				if (!AssimpLoader::Load(dataSet))
					return false;
			}

			if (dataSet.assimpScene->HasTextures())
			{
				for (auto textureIndex = 0; textureIndex < dataSet.assimpScene->mNumTextures; ++textureIndex)
				{
					auto &aiText = dataSet.assimpScene->mTextures[textureIndex];
					unsigned int i = 0;
				}
			}

			for (auto &e : dataSet.texturesPath)
			{
				bool found = false;
				for (auto &f : dataSet.textures)
				{
					if (f->rawPath == e)
					{
						found = true;
						break;
					}
				}
				if (found)
					continue;
				auto path = e;
				int width, height, channels;
				auto imgData = SOIL_load_image(path.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
				if (imgData)
				{
					auto t = new TextureData();
					dataSet.textures.push_back(t);
					t->width = width;
					t->height = height;
					t->rawPath = path;
					t->data.assign(imgData, imgData + (sizeof(unsigned char) * width * height * channels));
					SOIL_free_image_data(imgData);
				}
			}
			dataSet.texturesPath.clear();
			if (dataSet.textures.size() == 0)
			{
				std::cerr << "ImageLoader : Image has not been loaded" << std::endl;
				return false;
			}
			dataSet.texturesLoaded = true;
			return true;
		}
	};
}