#pragma once

#include "FontGlyph.hh"
#include <Utils/OpenGL.hh>
#include <vector>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>

class Font
{
public:
	Font();
	~Font();
	bool load();
	bool isLoaded();

	template <class Archive>
	void serialize(Archive &ar)
	{
		ar(_size, _glyphSize, _textureDatas, _name, _texW, _texH, _map);
	}

private:
	std::size_t _size;
	std::size_t _glyphSize;
	std::vector<unsigned char> _textureDatas;
	std::string _name;
	GLuint _textureId;
	std::size_t _texW;
	std::size_t _texH;
	std::array<FontGlyph, ASCII_END - ASCII_BEGIN> _map;
	friend class FontManager;
};