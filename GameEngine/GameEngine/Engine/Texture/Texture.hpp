#pragma once

#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

namespace AGE
{
	struct TextureData
	{
		std::string rawPath;
		std::uint32_t width;
		std::uint32_t height;
		std::uint8_t bpp;
		std::uint8_t colorNumber;
		AGE::Vector<unsigned char> data;

		template <class Archive>
		void serialize(Archive &ar)
		{
			ar(rawPath, width, height, bpp, data, colorNumber);
		}
	};
}