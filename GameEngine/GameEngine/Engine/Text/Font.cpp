#include "Font.hh"

Font::Font()
: _size(0)
, _glyphSize(0)
, _name("")
, _textureId(0)
, _texW(0)
, _texH(0)
{
}

Font::~Font()
{
}

bool Font::load()
{
	return true;
}

bool Font::isLoaded()
{
	return _textureId != 0;
}