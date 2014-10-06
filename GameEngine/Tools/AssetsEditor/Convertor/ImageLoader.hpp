#pragma once

#include "AssimpLoader.hpp"
#include <Texture/Texture.hpp>
#include <FreeImagePlus.h>

namespace AGE
{
	class ImageLoader
	{
	public:
		static bool save(AssetDataSet &dataSet)
		{
			if (dataSet.texturesLoaded == false)
				return false;
			while (!dataSet.textures.empty())
			{
				auto &t = dataSet.textures.back();

				auto path = dataSet.rawDirectory.path().string() + "\\" + t->rawPath;

				fipImage image;

				if (!image.load(path.c_str()))
				{
					std::cout << "coucou";
					return false;
				}

				if (!image.convertTo32Bits())
				{
					std::cout << "coucou";
					return false;
				}


				t->width = image.getWidth();
				t->height = image.getHeight();
				auto colorType = image.getColorType();
				t->bpp = image.getBitsPerPixel();

					auto colNumber = 8;
					if (t->bpp == 8)
						colNumber = 3;
					else if (t->bpp == 16)
						colNumber = 3;
					else
						colNumber = 4;
					auto imgData = FreeImage_GetBits(image);
					t->data.assign(imgData, imgData + sizeof(unsigned char) * t->width * t->height * colNumber);


				dataSet.textures.pop_back();
				auto folderPath = std::tr2::sys::path(dataSet.serializedDirectory.path().directory_string() + "\\" + File(t->rawPath).getFolder());

				if (!std::tr2::sys::exists(folderPath) && !std::tr2::sys::create_directories(folderPath))
				{
					std::cerr << "Material convertor error : creating directory" << std::endl;
					return false;
				}
				auto name = dataSet.serializedDirectory.path().directory_string() + "\\" + File(t->rawPath).getFolder() + "\\" + File(t->rawPath).getShortFileName() + ".tage";
				std::ofstream ofs(name, std::ios::trunc | std::ios::binary);
				cereal::PortableBinaryOutputArchive ar(ofs);
				ar(*t);
			}
			return true;
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

				auto t = new TextureData();
				dataSet.textures.push_back(t);
				t->rawPath = e;
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