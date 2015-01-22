#pragma once

#include "AssimpLoader.hpp"
#include <AssetManagement/Data/TextureData.hh>
#include <FreeImagePlus.h>
#include <Utils/BitOperations.hpp>

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
				dataSet.textures.pop_back();
				auto path = dataSet.rawDirectory.path().string() + "\\" + t->rawPath;

				fipImage image;

				if (!image.load(path.c_str()))
				{
					continue;
				}

				t->width = image.getWidth();
				t->height = image.getHeight();
				auto colorType = image.getColorType();
				t->bpp = image.getBitsPerPixel();

				bool toResize = false;
				if (!isPowerOfTwo(t->width))
				{
					t->width = roundToHighestPowerOfTwo(t->width);
					toResize = true;
				}
				if (!isPowerOfTwo(t->height))
				{
					t->height = roundToHighestPowerOfTwo(t->height);
					toResize = true;
				}
				if (toResize)
				{
					if (!image.rescale(t->width, t->height, FILTER_BICUBIC));
					{
						char a[2];
						a[30000] = '1';
					}
					std::cout << "Texture : " << path << " resized !" << std::endl;
				}

				t->colorNumber = 0;
				if (colorType == FIC_RGB)
				{
					t->colorNumber = 3;
					if (t->bpp > 24)
						t->colorNumber = 4;
				}
				else if (colorType == FIC_RGBALPHA)
				{
					t->colorNumber = 4;
					if (t->bpp < 32)
						t->colorNumber = 3;
				}
				else if (colorType == FIC_MINISBLACK || colorType == FIC_MINISWHITE)
				{
					t->colorNumber = 1;
				}
				else
					assert(false);
				auto imgData = FreeImage_GetBits(image);
				t->data.assign(imgData, imgData + sizeof(unsigned char) * t->width * t->height * t->colorNumber);

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