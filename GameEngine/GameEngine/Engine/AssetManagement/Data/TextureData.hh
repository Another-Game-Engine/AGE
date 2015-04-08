#pragma once

#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <Utils/Containers/Vector.hpp>
#include <stdint.h>

namespace AGE
{
	enum SkyboxConfig
	{
		UP = 0,
		DOWN,
		LEFT,
		RIGHT,
		FRONT,
		BACK,
		TOTAL_FACES
	};

	enum ECompressedFormat
	{
		UNCOMPRESSED,
		LUM_DXT1_FORMAT,
		RGB_DXT1_FORMAT,
		RGBA_DXT5_FORMAT
	};

	struct SkyboxData {
		std::string rawPath[SkyboxConfig::TOTAL_FACES];

		template <class Archive>
		void serialize(Archive &ar, const std::uint32_t version)
		{
			ar(rawPath[UP], rawPath[DOWN], rawPath[LEFT], rawPath[RIGHT], rawPath[FRONT], rawPath[BACK]);
		}
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
		void serialize(Archive &ar, const std::uint32_t version)
		{
			ar(rawPath, width, height, bpp, colorNumber, mipmapNbr, format, data);
		}
	};
}

CEREAL_CLASS_VERSION(AGE::TextureData, 0)