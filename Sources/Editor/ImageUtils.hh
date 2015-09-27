#pragma once

#define USE_MICROSOFT_LIB 1

#include <AssetManagement/Data/TextureData.hh>

#include <cstdint>
#include <FreeImagePlus.h>
#if USE_MICROSOFT_LIB
#include <DirectXTex/DirectXTex/DirectXTex.h>
#else
#include <crunch/inc/crnlib.h>
#include <DirectXTex/DirectXTex/DDS.h>
#endif

namespace AGE
{
	class ImageUtils
	{
	public:
#if USE_MICROSOFT_LIB
		static DXGI_FORMAT getCompressionFormat(DXGI_FORMAT current);
		static DirectX::ScratchImage *loadFromFile(std::string const &path);
#else
		static void convertBumpToNormal(fipImage &toConvert, float strength = 2.0f);
		static void switchRedBlue(fipImage &image);
		static char *compressImage(fipImage &image, bool generateMipmaps, int compressionQuality, size_t &compressedSize);
		static char *getDDSUncompressed(fipImage &image, size_t &dataSize);
#endif

	private:
		ImageUtils();
		~ImageUtils();

		static inline float intensity(RGBQUAD const &pixelColor)
		{
			return (float(pixelColor.rgbRed + pixelColor.rgbGreen + pixelColor.rgbBlue) / (255.0f * 3.0f));
		}

	};

}