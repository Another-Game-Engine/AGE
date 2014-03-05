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
	if (_textureDatas.size() == 0)
		return false;
	if (isLoaded())
		return true;
	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_ALPHA,
		_texW,
		_texH,
		0,
		GL_ALPHA,
		GL_UNSIGNED_BYTE,
		_textureDatas.data()
		);
	return true;
}

bool Font::isLoaded()
{
	return _textureId != 0;
}