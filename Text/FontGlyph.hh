#pragma once

#include <array>
#include <cstddef>
#include <cereal/cereal.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/array.hpp>

// Maybe necessary ??
//#include <cereal/archives/binary.hpp>
//#include <cereal/archives/json.hpp>
//#include <cereal/types/base_class.hpp>
//#include <cereal/types/map.hpp>

template <uint8_t NBR_ATTRIBUTE> class Vertice;

#define ASCII_BEGIN 32   //space
#define ASCII_END   127  //DEL
#define GLYPH_PADDING 4

struct FontGlyph
{
	FontGlyph();
	FontGlyph(const FontGlyph &o);
	~FontGlyph();

	std::size_t index;
	std::size_t width;
	std::size_t height;
	std::array<float, 4> uvs; //-V112
	Vertice<2> *buffer;
	float ax;	// advance.x
	float ay;	// advance.y
	float bw;	// bitmap.width;
	float bh;	// bitmap.height;
	float bl;	// bitmap_left;
	float bt;	// bitmap_top;
	float tx;	// x offset of glyph in texture coordinates
	float ty;	// y offset of glyph in texture coordinates

	template <typename Archive>
	void serialize(Archive &ar)
	{
		ar(index, width, height, uvs, ax, ay, bw, bh, bl, bt, tx, ty);
	}

};