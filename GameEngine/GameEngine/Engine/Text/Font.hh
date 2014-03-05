#pragma once

#include "FontGlyph.hh"
#include <Utils/OpenGL.hh>
#include <vector>

class Font
{
public:
	Font();
	~Font();
	bool load();
	bool isLoaded();
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