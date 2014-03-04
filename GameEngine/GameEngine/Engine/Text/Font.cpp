#include "Font.hh"

Font::Font()
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