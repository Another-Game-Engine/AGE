#pragma once

#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <Utils/Containers/Vector.hpp>
#include <stdint.h>

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
		enum RepeatType
		{
			NoRepeat = 0,
			Repeat,
			MirrorRepeat,
			ClampToEdge,
			ClampToBorder
		};

		std::string rawPath;
//		uint32_t width;
//		uint32_t height;
//		uint32_t bpp;
//		uint32_t colorNumber;
//		uint32_t mipmapNbr;
//		uint32_t format;
//		AGE::Vector<unsigned char> data;
		RepeatType repeatX = NoRepeat;
		RepeatType repeatY = NoRepeat;

//		template <class Archive>
//		void serialize(Archive &ar, const std::uint32_t version)
//		{
//			if (version < 1)
//			{
//				ar(rawPath, width, height, bpp, colorNumber, mipmapNbr, format, data);
//			}
//			else
//			{
//				ar(rawPath, width, height, bpp, colorNumber, mipmapNbr, format, data, repeatX, repeatY);
//			}
//		}
	};
}

CEREAL_CLASS_VERSION(AGE::TextureData, 0)