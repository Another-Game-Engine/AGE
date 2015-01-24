#pragma once

#include "AssimpLoader.hpp"
#include <Texture/Texture.hpp>
#include <FreeImagePlus.h>
#include <Utils/BitOperations.hpp>
#include <squish/squish.h>
#include <crunch/inc/crnlib.h>

#include <thread>
#include <mutex>

static std::mutex convertImageMutex;

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
				auto t = dataSet.textures.back();
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
					// for the moment, we wont handle monochromatic images
					t->colorNumber = 1;
				}
				else
					assert(false);
				
				// Convert image to RGBA and the to DXT
#if 1
				convertImageMutex.lock();

				crn_comp_params params;
				crn_mipmap_params mipmaps;

				params.m_height = t->height;
				params.m_width = t->width;

				assert(image.convertTo32Bits());
				auto imgData = FreeImage_GetBits(image);

				int	compressionFlag;

				if (t->colorNumber == 4)
					params.m_format = crn_format::cCRNFmtDXT5;
				else
					params.m_format = crn_format::cCRNFmtDXT5;

				params.set_flag(cCRNCompFlagPerceptual, false);
				params.m_file_type = cCRNFileTypeDDS;

				params.m_pImages[0][0] = (crn_uint32*)imgData;

				mipmaps.m_gamma_filtering = false;
				mipmaps.m_mode = cCRNMipModeGenerateMips;

				uint32_t compressedSize;

				uint8_t *compressedData = static_cast<uint8_t*>(crn_compress(params, mipmaps, compressedSize));

				assert(compressedData != NULL);

				convertImageMutex.unlock();
				// --- End of compression ---

				t->data.assign(compressedData, compressedData + compressedSize);

#else
				auto imgData = FreeImage_GetBits(image);

				t->data.assign(imgData, imgData + t->height * t->width * t->colorNumber);
#endif

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

				auto t = std::make_shared<TextureData>();
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