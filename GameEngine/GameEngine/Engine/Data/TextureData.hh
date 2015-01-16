#pragma once

#include <Utils/GlmSerialization.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

struct TextureData
{
public:
	std::string rawPath;
	std::uint32_t width;
	std::uint32_t height;
	std::uint8_t bpp;
	std::uint8_t colorNumber;
	AGE::Vector<unsigned char> data;

public:
	template <class Archive> void serialize(Archive &ar);
};

template <class Archive> 
void TextureData::serialize(Archive &ar)
{
	ar(rawPath, width, height, bpp, data, colorNumber);
}