#pragma once

#include "AssimpLoader.hpp"
#include <AssetManagement/Data/TextureData.hh>
#include <AssetManagement/Data/MaterialData.hh>
#include <ImageUtils.hh>
#include <algorithm>
#include <Utils/BitOperations.hpp>
#include "ConvertorStatusManager.hpp"
#include "CookingTask.hpp"
#if USE_MICROSOFT_LIB
#include <DirectXTex/DirectXTex/DirectXTex.h>
#else
#include <DirectXTex/DirectXTex/DDS.h>
#include <FreeImagePlus.h>
#endif
namespace AGE
{
	class ImageLoader
	{
	public:
		static bool save(std::shared_ptr<CookingTask> cookingTask)
		{
#if USE_MICROSOFT_LIB
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
				bool isNormalMap = false;
				auto t = cookingTask->textures.back();
				cookingTask->textures.pop_back();
				auto path = cookingTask->rawDirectory.path().string() + "\\" + t->rawPath;
				// Load texture
#if USE_MICROSOFT_LIB
				DirectX::ScratchImage *image;

				image = ImageUtils::loadFromFile(path);
				if (image == NULL)
					continue;
#else
				fipImage image;

				if (!image.load(path.c_str()))
				{
					continue;
				}
#endif
				// Handle the flipping if it is set by the user from the editor
#if USE_MICROSOFT_LIB
				{
					HRESULT flipResult;
					DirectX::ScratchImage *tmp = new DirectX::ScratchImage;
					DWORD dwflags = DirectX::TEX_FR_FLIP_VERTICAL;

					if (cookingTask->dataSet->flipH)
						dwflags |= DirectX::TEX_FR_FLIP_HORIZONTAL;
					if (cookingTask->dataSet->flipV)
						dwflags ^= DirectX::TEX_FR_FLIP_VERTICAL;

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
				// Check if the current texture is a normal map or a bump map to transform
				for (auto material : cookingTask->materials)
				{
					std::string textureName = OldFile(t->rawPath).getShortFileName();
					if (cookingTask->dataSet->bumpToNormal &&
						OldFile(material->bumpTexPath).getShortFileName() == textureName)
					{
						convertBump = true;
						material->normalTexPath = material->bumpTexPath;
						material->bumpTexPath.clear();
						break;
					}
					else if (OldFile(material->normalTexPath).getShortFileName() == textureName)
					{
						isNormalMap = true;
						break;
					}
				}
				// Convert from bump to normal texture
#if USE_MICROSOFT_LIB
				if (convertBump)
				{
					HRESULT convertBumpResult;
					DirectX::ScratchImage *tmp = new DirectX::ScratchImage;
					DXGI_FORMAT format;

					if (cookingTask->dataSet->compressNormalMap)
					{
						format = DXGI_FORMAT_BC1_UNORM;
					}
					else
					{
						format = DXGI_FORMAT_B8G8R8X8_UNORM;
					}

					convertBumpResult = DirectX::ComputeNormalMap(image->GetImages(), image->GetImageCount(), image->GetMetadata(),
						DirectX::CNMAP_CHANNEL_LUMINANCE | DirectX::CNMAP_MIRROR, cookingTask->dataSet->normalStrength,
						format, *tmp);

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
					ImageUtils::convertBumpToNormal(image, cookingTask->dataSet->normalStrength / 10.0f);
				}
#endif
				// Generate the mipmaps
#if USE_MICROSOFT_LIB
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
#endif

				char *ddsData;
				size_t ddsSize;

				// Compress the texture (only if it as not been already compressed by the normal map generation)
				// If the texture is a normal map, we check to see if we have to compress the normal maps
#if USE_MICROSOFT_LIB
				if (cookingTask->dataSet->compressTextures && convertBump == false &&
					(isNormalMap == false || cookingTask->dataSet->compressNormalMap))
				{
					HRESULT compressResult;
					DirectX::ScratchImage *tmp = new DirectX::ScratchImage;
					DXGI_FORMAT format;

					if (isNormalMap)
						format = DXGI_FORMAT_BC1_UNORM;
					else
						format = ImageUtils::getCompressionFormat(image->GetMetadata().format);

					compressResult = DirectX::Compress(image->GetImages(), image->GetImageCount(), image->GetMetadata(),
							format, DirectX::TEX_COMPRESS_PARALLEL, 0.5f, *tmp);

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
					ImageUtils::switchRedBlue(image);
					ddsData = ImageUtils::compressImage(image, cookingTask->dataSet->generateMipmap, 2, ddsSize);
				}
				else
					ddsData = ImageUtils::getDDSUncompressed(image, ddsSize);
#endif
				auto folderPath = std::tr2::sys::path(cookingTask->serializedDirectory.path().directory_string() + "\\" + OldFile(t->rawPath).getFolder());

				if (!std::tr2::sys::exists(folderPath) && !std::tr2::sys::create_directories(folderPath))
				{
					Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
					std::cerr << "Material convertor error : creating directory" << std::endl;
					return false;
				}
				auto name = cookingTask->serializedDirectory.path().directory_string() + "\\" + OldFile(t->rawPath).getFolder() + "\\" + OldFile(t->rawPath).getShortFileName() + ".dds";
				// Save the texture
#if USE_MICROSOFT_LIB
				DirectX::Blob blob;

				DirectX::SaveToDDSMemory(image->GetImages(), image->GetImageCount(), image->GetMetadata(), DirectX::DDS_FLAGS_NONE, blob);
				ddsData = static_cast<char*>(blob.GetBufferPointer());
				ddsSize = blob.GetBufferSize();
#else
#endif
				std::ofstream ofs(name, std::ios::trunc | std::ios::binary);
	
				ofs.write(ddsData, ddsSize);

				if (!ofs)
				{
					std::cerr << "Material convertor error : writing in file" << std::endl;
					return false;
				}
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