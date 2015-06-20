#pragma once

#define USE_MICROSOFT_LIB

#include "AssimpLoader.hpp"
#include <AssetManagement/Data/TextureData.hh>
#include <AssetManagement/Data/MaterialData.hh>
//#include <ImageUtils.hh>
#include <algorithm>
#include <Utils/BitOperations.hpp>
#include "ConvertorStatusManager.hpp"
#include "CookingTask.hpp"
#ifdef USE_MICROSOFT_LIB
#include <DirectXTex/DirectXTex/DirectXTex.h>
#else
#include <FreeImagePlus.h>
#endif
namespace AGE
{
	class ImageLoader
	{
	public:
#ifdef USE_MICROSOFT_LIB
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
					return (NULL);
				}
			}
			return (retImage);
		}
#endif

		static bool save(std::shared_ptr<CookingTask> cookingTask)
		{
#ifdef USE_MICROSOFT_LIB
			// Initialize COM (needed for WIC)
			HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
			if (FAILED(hr))
			{
				wprintf(L"Failed to initialize COM (%08X)\n", hr);
				return 1;
			}
#endif

			if (!cookingTask->dataSet->loadTextures)
				return true;
			auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("ImageLoader : load and save " + cookingTask->dataSet->filePath.getShortFileName());
			while (!cookingTask->textures.empty())
			{
				bool convertBump = false;
				auto t = cookingTask->textures.back();
				cookingTask->textures.pop_back();
				auto path = cookingTask->rawDirectory.path().string() + "\\" + t->rawPath;

#ifdef USE_MICROSOFT_LIB
				DirectX::ScratchImage *image;

				image = loadFromFile(path);
				if (image == NULL)
					continue;
//				fipImage loadImg;
//				DirectX::Image imageData;
//				DirectX::ScratchImage *image = NULL;
//
//				if (!loadImg.load(path.c_str()))
//				{
//					continue;
//				}
//				if (loadImg.getBitsPerPixel() != 32 &&
//					!loadImg.convertTo32Bits())
//					assert(!"Convert to 32 bits failed");
//				imageData.format = DXGI_FORMAT_R8G8B8A8_UNORM;
//				imageData.width = loadImg.getWidth();
//				imageData.height = loadImg.getHeight();
//				imageData.pixels = loadImg.accessPixels();
//				DirectX::ComputePitch(imageData.format, imageData.width, imageData.height, imageData.rowPitch, imageData.slicePitch);
#else
				fipImage image;

				if (!image.load(path.c_str()))
				{
					continue;
				}
#endif
#ifdef USE_MICROSOFT_LIB
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
#else
				if (cookingTask->dataSet->flipH)
				{
					image.flipHorizontal();
				}
				if (cookingTask->dataSet->flipV)
				{
					image.flipVertical();
				}
#endif

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

#ifdef  USE_MICROSOFT_LIB
				if (convertBump)
				{
					HRESULT convertBumpResult;
					DirectX::ScratchImage *tmp = new DirectX::ScratchImage;

					convertBumpResult = DirectX::ComputeNormalMap(image->GetImages(), image->GetImageCount(), image->GetMetadata(),
						DirectX::CNMAP_CHANNEL_LUMINANCE | DirectX::CNMAP_MIRROR, cookingTask->dataSet->normalStrength,
						DXGI_FORMAT_R8G8B8A8_UNORM, *tmp);

					if (FAILED(convertBumpResult))
					{
						wprintf(L" FAILED (%x)\n", convertBumpResult);
						assert(false);
						continue;
					}
					delete image;
					image = tmp;
				}
#else
				if (convertBump)
				{
					if (t->colorNumber < 3)
						t->colorNumber = 3;
					dataSize = t->width * t->height * t->colorNumber;
					ImageUtils::convertBumpToNormal(image, cookingTask->dataSet->normalStrength);
				}
#endif

#ifdef USE_MICROSOFT_LIB
				if (cookingTask->dataSet->generateMipmap)
				{
					HRESULT mipmapResult;
					DirectX::ScratchImage *tmp = new DirectX::ScratchImage;

					mipmapResult = DirectX::GenerateMipMaps(image->GetImages(), image->GetImageCount(), image->GetMetadata(), DirectX::TEX_FILTER_DEFAULT, 0, *tmp);

					if (FAILED(mipmapResult))
					{
						wprintf(L" FAILED (%x)\n", mipmapResult);
						assert(false);
						continue;
					}
					delete image;
					image = tmp;
				}
#else
#endif

#ifdef USE_MICROSOFT_LIB
				if (cookingTask->dataSet->compressTextures)
				{
					HRESULT compressResult;
					DirectX::ScratchImage *tmp = new DirectX::ScratchImage;

					compressResult = DirectX::Compress(image->GetImages(), image->GetImageCount(), image->GetMetadata(),
							DXGI_FORMAT_BC3_UNORM, DirectX::TEX_COMPRESS_PARALLEL, 0.5f, *tmp);

					if (FAILED(compressResult))
					{
						wprintf(L" FAILED (%x)\n", compressResult);
						assert(false);
						continue;
					}
					delete image;
					image = tmp;
				}
#else
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



				if (cookingTask->dataSet->compressTextures)
				{
					ImageUtils::switchRedBlue(image);
					imgData = ImageUtils::compressImage(image, cookingTask->dataSet->generateMipmap,
						cookingTask->dataSet->textureCompressionQuality, t, dataSize);
				}
				else
					imgData = FreeImage_GetBits(image);
				t->data.assign(imgData, imgData + dataSize);
#endif


				auto folderPath = std::tr2::sys::path(cookingTask->serializedDirectory.path().directory_string() + "\\" + OldFile(t->rawPath).getFolder());

				if (!std::tr2::sys::exists(folderPath) && !std::tr2::sys::create_directories(folderPath))
				{
					Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
					std::cerr << "Material convertor error : creating directory" << std::endl;
					return false;
				}
				auto name = cookingTask->serializedDirectory.path().directory_string() + "\\" + OldFile(t->rawPath).getFolder() + "\\" + OldFile(t->rawPath).getShortFileName() + ".dds";
				char *ddsData;
				size_t ddsSize;

#ifdef USE_MICROSOFT_LIB
				DirectX::Blob blob;

				DirectX::SaveToDDSMemory(image->GetImages(), image->GetImageCount(), image->GetMetadata(), DirectX::DDS_FLAGS_NONE, blob);
				ddsData = static_cast<char*>(blob.GetBufferPointer());
				ddsSize = blob.GetBufferSize();
#else
#endif
				std::ofstream ofs(name, std::ios::trunc | std::ios::binary);
	
				ofs.write(ddsData, ddsSize);
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