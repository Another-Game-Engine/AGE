#pragma once

#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <Utils/Containers/Vector.hpp>

namespace AGE
{
	enum ECompressedFormat
	{
		UNCOMPRESSED,
		LUM_DXT1_FORMAT,
		RGB_DXT1_FORMAT,
		RGBA_DXT5_FORMAT
	};

	struct TextureData
	{
		std::string rawPath;
		uint32_t width;
		uint32_t height;
		uint32_t bpp;
		uint32_t colorNumber;
		uint32_t mipmapNbr;
		uint32_t format;
		AGE::Vector<unsigned char> data;

		template <class Archive>
		void serialize(Archive &ar)
		{
			ar(rawPath, width, height, bpp, colorNumber, mipmapNbr, format, data);
		}
	};
}