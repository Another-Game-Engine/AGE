#pragma once

#include "AssimpLoader.hpp"
#include <AssetManagement/Data/TextureData.hh>
#include <AssetManagement/Data/MaterialData.hh>
//#include <ImageUtils.hh>
//#include <FreeImagePlus.h>
#include <algorithm>
#include <Utils/BitOperations.hpp>
#include "ConvertorStatusManager.hpp"
#include "CookingTask.hpp"
#include <DirectXTex/DirectXTex/DirectXTex.h>

namespace AGE
{
	class ImageLoader
	{
	public:
		static DirectX::ScratchImage *loadFromFile(std::string const &path)
		{
			HRESULT loadResult;
			DirectX::TexMetadata info;
			DirectX::ScratchImage *retImage = new DirectX::ScratchImage;
			std::string extension = path.substr(path.size() - 4, 4);
			std::wstring widePath = std::wstring(path.begin(), path.end());

			std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
			if (extension == ".dds")
			{
				loadResult = LoadFromDDSFile(widePath.c_str(), DirectX::DDS_FLAGS_NONE, &info, *retImage);
				if (FAILED(loadResult))
				{
					wprintf(L" FAILED (%x)\n", loadResult);
					return (NULL);
				}
			}
			else if (extension == ".tga")
			{
				loadResult = LoadFromTGAFile(widePath.c_str(), &info, *retImage);
				if (FAILED(loadResult))
				{
					wprintf(L" FAILED (%x)\n", loadResult);
					return (NULL);
				}
			}
			else
			{
				loadResult = LoadFromWICFile(widePath.c_str(), DirectX::TEX_FILTER_DEFAULT, &info, *retImage);
				if (FAILED(loadResult))
				{
					wprintf(L" FAILED (%x)\n", loadResult);
					assert(false);
					return (NULL);
				}
			}
			return (retImage);
		}

		static bool save(std::shared_ptr<CookingTask> cookingTask)
		{
			if (!cookingTask->dataSet->loadTextures)
				return true;
			auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("ImageLoader : load and save " + cookingTask->dataSet->filePath.getShortFileName());
			while (!cookingTask->textures.empty())
			{
				bool convertBump = false;
				auto t = cookingTask->textures.back();
				cookingTask->textures.pop_back();
				auto path = cookingTask->rawDirectory.path().string() + "\\" + t->rawPath;

				DirectX::ScratchImage *image;

				image = loadFromFile(path);
				if (image == NULL)
				{
					continue;
				}

				// Handle the flipping if it is set by the user from the editor
				if (cookingTask->dataSet->flipH || cookingTask->dataSet->flipV)
				{
					HRESULT flipResult;
					DirectX::ScratchImage *tmp = new DirectX::ScratchImage;
					DWORD dwflags = 0;

					if (cookingTask->dataSet->flipH)
						dwflags |= DirectX::TEX_FR_FLIP_HORIZONTAL;
					if (cookingTask->dataSet->flipV)
						dwflags |= DirectX::TEX_FR_FLIP_VERTICAL;

					flipResult = DirectX::FlipRotate(image->GetImages(), image->GetImageCount(), image->GetMetadata(), dwflags, *tmp);

					if (FAILED(flipResult))
					{
						wprintf(L" FAILED (%x)\n", flipResult);
						assert(false);
						continue;
					}
					delete image;
					image = tmp;
				}

				if (cookingTask->dataSet->bumpToNormal)
				{
					for (auto material : cookingTask->materials)
					{
						if (!t->rawPath.empty() &&
							OldFile(material->bumpTexPath).getShortFileName() == OldFile(t->rawPath).getShortFileName())
						{
							convertBump = true;
							break;
						}
					}
				}

				if (convertBump)
				{
					HRESULT convertBumpResult;
					DirectX::ScratchImage *tmp = new DirectX::ScratchImage;

					if (FAILED(convertBumpResult))
					{
						wprintf(L" FAILED (%x)\n", convertBumpResult);
						assert(false);
						continue;
					}
				}

//				t->width = image.getWidth();
//				t->height = image.getHeight();
//				t->bpp = image.getBitsPerPixel();
//				t->colorNumber = t->bpp / 8;
//
				//				bool toResize = false;
				//				if (!Bits::isPowerOfTwo(t->width))
				//				{
				//					t->width = Bits::roundToHighestPowerOfTwo(t->width);
				//					toResize = true;
				//				}
				//				if (!Bits::isPowerOfTwo(t->height))
				//				{
				//					t->height = Bits::roundToHighestPowerOfTwo(t->height);
				//					toResize = true;
				//				}
				//				if (toResize)
				//				{
				//					if (!image.rescale(t->width, t->height, FILTER_BICUBIC));
				//					{
				//						//ERROR PAUL WILL FIXE ALL THAT PART :D
				//					}
				//					std::cout << "Texture : " << path << " resized !" << std::endl;
				//				}

				if (cookingTask->dataSet->compressTextures)
				{
					if (t->colorNumber == 1)
						t->format = ECompressedFormat::LUM_DXT1_FORMAT;
					else if (t->colorNumber == 3)
						t->format = ECompressedFormat::RGB_DXT1_FORMAT;
					else if (t->colorNumber == 4)
						t->format = ECompressedFormat::RGBA_DXT5_FORMAT;
				}
				else
					t->format = ECompressedFormat::UNCOMPRESSED;

				uint8_t *imgData;
				uint32_t dataSize = t->width * t->height * t->colorNumber;

				if (convertBump)
				{
					if (t->colorNumber < 3)
						t->colorNumber = 3;
					dataSize = t->width * t->height * t->colorNumber;
					ImageUtils::convertBumpToNormal(image, cookingTask->dataSet->normalStrength);
				}

				if (cookingTask->dataSet->compressTextures)
				{
					ImageUtils::switchRedBlue(image);
					imgData = ImageUtils::compressImage(image, cookingTask->dataSet->generateMipmap,
						cookingTask->dataSet->textureCompressionQuality, t, dataSize);
				}
				else
					imgData = FreeImage_GetBits(image);

				t->data.assign(imgData, imgData + dataSize);

				auto folderPath = std::tr2::sys::path(cookingTask->serializedDirectory.path().directory_string() + "\\" + OldFile(t->rawPath).getFolder());

				if (!std::tr2::sys::exists(folderPath) && !std::tr2::sys::create_directories(folderPath))
				{
					Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
					std::cerr << "Material convertor error : creating directory" << std::endl;
					return false;
				}
				auto name = cookingTask->serializedDirectory.path().directory_string() + "\\" + OldFile(t->rawPath).getFolder() + "\\" + OldFile(t->rawPath).getShortFileName() + ".tage";
				std::ofstream ofs(name, std::ios::trunc | std::ios::binary);
				cereal::PortableBinaryOutputArchive ar(ofs);
				ar(*t);
			}
			Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
			return true;
		}

		static bool load(std::shared_ptr<CookingTask> cookingTask)
		{
			if (!cookingTask->dataSet->loadTextures)
				return true;
			if (cookingTask->assimpScene && cookingTask->assimpScene->HasTextures())
			{
				for (unsigned int textureIndex = 0; textureIndex < cookingTask->assimpScene->mNumTextures; ++textureIndex)
				{
					auto &aiText = cookingTask->assimpScene->mTextures[textureIndex];
					unsigned int i = 0;
				}
			}

			for (auto &e : cookingTask->texturesPath)
			{
				bool found = false;
				for (auto &f : cookingTask->textures)
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
				cookingTask->textures.push_back(t);
				t->rawPath = e;
			}
			cookingTask->texturesPath.clear();
			if (cookingTask->textures.size() == 0)
			{
				std::cerr << "ImageLoader : Image has not been loaded" << std::endl;
				return false;
			}
			return true;
		}
	};
}