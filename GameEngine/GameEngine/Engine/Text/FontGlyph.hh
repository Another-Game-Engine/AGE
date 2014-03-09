#pragma once

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/array.hpp>
#include <OpenGL/VertexManager.hh>
#include <OpenGL/Vertice.hh>

#define ASCII_BEGIN 32   //space
#define ASCII_END   127  //DEL
#define GLYPH_PADDING 4

#include <array>
#include <cstddef>

struct FontGlyph
{
	FontGlyph();
	FontGlyph(const FontGlyph &o);
	~FontGlyph();

	std::size_t index;
	std::size_t width;
	std::size_t height;
	std::array<float, 4> uvs;
	Vertice<2> *buffer;

	template <typename Archive>
	void serialize(Archive &ar)
	{
		ar(index, width, height, uvs);
	}

};