#include <ImageUtils.hh>
#include <glm/glm.hpp>

#include <iostream>

namespace AGE
{
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

	uint8_t *ImageUtils::compressImage(fipImage &image, bool generateMipmaps, int compressionQuality,
		std::shared_ptr<TextureData> const &infos, uint32_t &compressedSize)
	{
		if (image.getBitsPerPixel() != 32)
			image.convertTo32Bits();
		crn_comp_params params;
		crn_mipmap_params mipmaps;

		params.m_height = infos->height;
		params.m_width = infos->width;

		bool flipVertical = image.flipVertical() != 0;
		assert(flipVertical);

		auto imgData = image.accessPixels();

		// set the compression params
		if (infos->format == RGBA_DXT5_FORMAT)
			params.m_format = crn_format::cCRNFmtDXT5;
		else
			params.m_format = crn_format::cCRNFmtDXT1;
		params.m_file_type = cCRNFileTypeDDS;
		params.m_alpha_component = 0;
		params.m_dxt_quality = (crn_dxt_quality)compressionQuality;
		if (infos->format == LUM_DXT1_FORMAT)
			params.set_flag(cCRNCompFlagGrayscaleSampling, true);
		params.set_flag(cCRNCompFlagPerceptual, false);
		params.m_pImages[0][0] = (crn_uint32*)imgData;

		// set the mipmaps params
		mipmaps.m_gamma_filtering = false;
		if (generateMipmaps)
			mipmaps.m_mode = cCRNMipModeGenerateMips;
		else
			mipmaps.m_mode = cCRNMipModeNoMips;

		uint8_t *compressedData = static_cast<uint8_t*>(crn_compress(params, mipmaps, compressedSize));

		assert(compressedData != NULL);

		// --- End of compression ---

		compressedSize -= sizeof(DDS_HEADER);

		DDS_HEADER *textureHeader = (DDS_HEADER*)compressedData;

		infos->mipmapNbr = textureHeader->dwMipMapCount;
		compressedData += sizeof(DDS_HEADER);
		return (compressedData);
	}
}