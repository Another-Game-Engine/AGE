#pragma once

#include <AssetManagement/Data/TextureData.hh>

#include <cstdint>
#include <FreeImagePlus.h>
#include <crunch/inc/crnlib.h>

namespace AGE
{
	class ImageUtils
	{
	public:
		struct DDS_PIXELFORMAT
		{
			uint32_t dwSize;
			uint32_t dwFlags;
			uint32_t dwFourCC;
			uint32_t dwRGBBitCount;
			uint32_t dwRBitMask;
			uint32_t dwGBitMask;
			uint32_t dwBBitMask;
			uint32_t dwABitMask;
		};

		struct DDS_HEADER
		{
			uint32_t		magicNbr;
			uint32_t        dwSize;
			uint32_t        dwFlags;
			uint32_t        dwHeight;
			uint32_t        dwWidth;
			uint32_t        dwPitchOrLinearSize;
			uint32_t        dwDepth;
			uint32_t        dwMipMapCount;
			uint32_t        dwReserved1[11];
			DDS_PIXELFORMAT	ddspf;
			uint32_t        dwCaps;
			uint32_t        dwCaps2;
			uint32_t        dwCaps3;
			uint32_t        dwCaps4;
			uint32_t        dwReserved2;
		};

		static void convertBumpToNormal(fipImage &toConvert, float strength = 2.0f);
		static void switchRedBlue(fipImage &image);
		static uint8_t *compressImage(fipImage &image, bool generateMipmaps, int compressionQuality,
			std::shared_ptr<TextureData> const &infos, uint32_t &compressedSize);

	private:
		ImageUtils();
		~ImageUtils();

		static inline float intensity(RGBQUAD const &pixelColor)
		{
			return (float(pixelColor.rgbRed + pixelColor.rgbGreen + pixelColor.rgbBlue) / (255.0f * 3.0f));
		}

	};

}