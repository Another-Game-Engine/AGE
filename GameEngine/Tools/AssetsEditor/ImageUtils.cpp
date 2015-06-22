#include <ImageUtils.hh>
#include <glm/glm.hpp>

#include <iostream>

namespace AGE
{
#if USE_MICROSOFT_LIB
	// All this formats will be compressed in DXT5 (gradient alpha)
	static DXGI_FORMAT dxt5Compression[] =
	{
		DXGI_FORMAT_R32G32B32A32_TYPELESS,
		DXGI_FORMAT_R32G32B32A32_FLOAT,
		DXGI_FORMAT_R32G32B32A32_UINT,
		DXGI_FORMAT_R32G32B32A32_SINT,
		DXGI_FORMAT_R16G16B16A16_TYPELESS,
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		DXGI_FORMAT_R16G16B16A16_UNORM,
		DXGI_FORMAT_R16G16B16A16_UINT,
		DXGI_FORMAT_R16G16B16A16_SNORM,
		DXGI_FORMAT_R16G16B16A16_SINT,
		DXGI_FORMAT_R8G8B8A8_TYPELESS,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
		DXGI_FORMAT_R8G8B8A8_UINT,
		DXGI_FORMAT_R8G8B8A8_SNORM,
		DXGI_FORMAT_R8G8B8A8_SINT,
		DXGI_FORMAT_B8G8R8A8_TYPELESS,
		DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
		DXGI_FORMAT_B8G8R8X8_TYPELESS,
		DXGI_FORMAT_B8G8R8X8_UNORM_SRGB,
		DXGI_FORMAT_B4G4R4A4_UNORM,
		DXGI_FORMAT_BC3_TYPELESS,
		DXGI_FORMAT_BC3_UNORM,
		DXGI_FORMAT_BC3_UNORM_SRGB,
		DXGI_FORMAT_UNKNOWN
	};

	// All this formats will be compressed in DXT3 (sharp alpha)
	static DXGI_FORMAT dxt3Compression[] =
	{
		DXGI_FORMAT_R10G10B10A2_TYPELESS,
		DXGI_FORMAT_R10G10B10A2_UNORM,
		DXGI_FORMAT_R10G10B10A2_UINT,
		DXGI_FORMAT_BC2_TYPELESS,
		DXGI_FORMAT_BC2_UNORM,
		DXGI_FORMAT_BC2_UNORM_SRGB,
		DXGI_FORMAT_UNKNOWN
	};

	DXGI_FORMAT ImageUtils::getCompressionFormat(DXGI_FORMAT current)
	{
		for (int i = 0; dxt3Compression[i] != DXGI_FORMAT_UNKNOWN; ++i)
		{
			if (dxt3Compression[i] == current)
				return (DXGI_FORMAT_BC2_UNORM);
		}
		for (int i = 0; dxt5Compression[i] != DXGI_FORMAT_UNKNOWN; ++i)
		{
			if (dxt5Compression[i] == current)
				return (DXGI_FORMAT_BC3_UNORM);
		}
		return (DXGI_FORMAT_BC1_UNORM);
	}

	DirectX::ScratchImage *ImageUtils::loadFromFile(std::string const &path)
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
				std::cerr << "Material convertor error : DDS texture loading failed (\"" << path << "\")" << std::endl;
				return (NULL);
			}
		}
		else if (extension == ".tga")
		{
			loadResult = LoadFromTGAFile(widePath.c_str(), &info, *retImage);
			if (FAILED(loadResult))
			{
				std::cerr << "Material convertor error : TGA texture loading failed (\"" << path << "\")" << std::endl;
				return (NULL);
			}
		}
		else
		{
			loadResult = LoadFromWICFile(widePath.c_str(), DirectX::TEX_FILTER_DEFAULT, &info, *retImage);
			if (FAILED(loadResult))
			{
				std::cerr << "Material convertor error : WIC texture loading failed (\"" << path << "\")" << std::endl;
				return (NULL);
			}
		}
		return (retImage);
	}

#else
	void ImageUtils::convertBumpToNormal(fipImage &toConvert, float strength)
	{
		if (toConvert.getBitsPerPixel() < 24)
			toConvert.convertTo24Bits();
		const int w = toConvert.getWidth();
		const int h = toConvert.getHeight();

		for (int y = 0; y < h; ++y)
		{
			for (int x = 0; x < w; ++x)
			{
				RGBQUAD topLeft, top, topRight, left, right, bottomLeft, bottom, bottomRight;
				RGBQUAD current;

				toConvert.getPixelColor(glm::clamp(x - 1, 0, w - 1), glm::clamp(y - 1, 0, h - 1), &topLeft);
				toConvert.getPixelColor(x, glm::clamp(y - 1, 0, h - 1), &top);
				toConvert.getPixelColor(glm::clamp(x + 1, 0, w - 1), glm::clamp(y - 1, 0, h - 1), &topRight);
				toConvert.getPixelColor(glm::clamp(x - 1, 0, w - 1), y, &left);
				toConvert.getPixelColor(glm::clamp(x + 1, 0, w - 1), y, &right);
				toConvert.getPixelColor(glm::clamp(x - 1, 0, w - 1), glm::clamp(y + 1, 0, h - 1), &bottomLeft);
				toConvert.getPixelColor(x, glm::clamp(y + 1, 0, h - 1), &bottom);
				toConvert.getPixelColor(glm::clamp(x + 1, 0, w - 1), glm::clamp(y + 1, 0, h - 1), &bottomRight);

				toConvert.getPixelColor(x, y, &current);

				const float tl = intensity(topLeft);
				const float t = intensity(top);
				const float tr = intensity(topRight);
				const float l = intensity(left);
				const float r = intensity(right);
				const float bl = intensity(bottomLeft);
				const float b = intensity(bottom);
				const float br = intensity(bottomRight);

				glm::vec3 normal((tr + 2.0f * r + br) - (tl + 2.0f * l + bl),
					(bl + 2.0f * b + br) - (tl + 2.0f * t + tr),
					1.0f / strength);

				normal = (glm::normalize(normal) + 1.0f) * 0.5f * 255.0f;

				current.rgbRed = BYTE(normal.r);
				current.rgbGreen = BYTE(normal.g);
				current.rgbBlue = BYTE(normal.b);

				toConvert.setPixelColor(x, y, &current);
			}
		}
	}

	void ImageUtils::switchRedBlue(fipImage &image)
	{
		for (unsigned int y = 0; y < image.getHeight(); ++y)
		{
			for (unsigned int x = 0; x < image.getWidth(); ++x)
			{
				RGBQUAD color;

				image.getPixelColor(x, y, &color);
				BYTE r = color.rgbRed;
				color.rgbRed = color.rgbBlue;
				color.rgbBlue = r;
				image.setPixelColor(x, y, &color);
			}
		}

	}

	char *ImageUtils::compressImage(fipImage &image, bool generateMipmaps, int compressionQuality, size_t &compressedSize)
	{
		if (image.getBitsPerPixel() != 32)
			image.convertTo32Bits();

		crn_comp_params params;
		crn_mipmap_params mipmaps;

		params.m_width = image.getWidth();
		params.m_height = image.getHeight();

		auto imgData = image.accessPixels();

		// set the compression params
		params.m_format = crn_format::cCRNFmtDXT5;
		params.m_file_type = cCRNFileTypeDDS;
		params.m_alpha_component = 0;
		params.m_dxt_quality = (crn_dxt_quality)compressionQuality;
		params.set_flag(cCRNCompFlagPerceptual, false);
		params.m_pImages[0][0] = (crn_uint32*)imgData;

		// set the mipmaps params
		mipmaps.m_gamma_filtering = false;
		if (generateMipmaps)
			mipmaps.m_mode = cCRNMipModeGenerateMips;
		else
			mipmaps.m_mode = cCRNMipModeNoMips;

		uint32_t ddsSize = 0;

		char *compressedData = static_cast<char*>(crn_compress(params, mipmaps, ddsSize));

		compressedSize = ddsSize;

		assert(compressedData != NULL);
		return (compressedData);
	}

	char *ImageUtils::getDDSUncompressed(fipImage &image, size_t &dataSize)
	{
		if (image.getBitsPerPixel() != 32)
			image.convertTo32Bits();
		
		size_t imageSize = image.getWidth() * image.getHeight() * 4;
		dataSize = sizeof(uint32_t) + sizeof(DirectX::DDS_HEADER) + imageSize;
		char *ddsData = new char[dataSize];
		char *imageData = (char*)image.accessPixels();
		DirectX::DDS_HEADER *header;

		*(uint32_t*)ddsData = DirectX::DDS_MAGIC;
		header = (DirectX::DDS_HEADER*)(ddsData + sizeof(uint32_t));

		header->dwSize = sizeof(DirectX::DDS_HEADER);
		header->dwFlags = 0x1 | 0x2 | 0x4 | 0x8 | 0x1000;
		header->dwHeight = image.getHeight();
		header->dwWidth = image.getWidth();
		header->dwPitchOrLinearSize = image.getWidth() * 4;
		header->dwDepth = 1;
		header->dwMipMapCount = 1;
		for (int i = 0; i < 11; ++i)
			header->dwReserved1[i] = 0;
		header->ddspf = DirectX::DDSPF_A8R8G8B8;
		header->dwCaps = 0x1000;
		header->dwCaps2 = 0;
		header->dwCaps3 = 0;
		header->dwCaps4 = 0;
		header->dwReserved2 = 0;
		memcpy(ddsData + sizeof(uint32_t) + sizeof(DirectX::DDS_HEADER), imageData, imageSize);
		return (ddsData);
	}
#endif
}