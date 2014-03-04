#pragma once

#define ASCII_BEGIN 32   //space
#define ASCII_END   127  //DEL

#include <array>
#include <cstddef>
#include <ft2build.h>
#include FT_FREETYPE_H

struct FontGlyph
{
	std::size_t index;
	std::size_t width;
	std::size_t height;
	std::array<float, 4> uvs;
};