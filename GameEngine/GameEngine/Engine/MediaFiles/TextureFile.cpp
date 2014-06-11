#include "TextureFile.hpp"
#include <math.h>
#include <algorithm>

TextureFile::TextureFile()
: MediaFile<TextureFile>()
, width(0)
, height(0)
, components(0)
, format(0)
, wrap(GL_REPEAT)
, minFilter(GL_LINEAR_MIPMAP_LINEAR)
, magFilter(GL_LINEAR)
{
	_type = TEXTURE;
}

TextureFile::~TextureFile()
{
}

TextureFile::TextureFile(const TextureFile &o)
: MediaFile<TextureFile>(o)
, width(0)
, height(0)
, components(0)
, format(0)
, wrap(GL_REPEAT)
, minFilter(GL_LINEAR_MIPMAP_LINEAR)
, magFilter(GL_LINEAR)
{
	width = o.width;
	height = o.height;
	components = o.components;
	format = o.format;
	wrap = o.wrap;
	minFilter = o.minFilter;
	magFilter = o.magFilter;
}

TextureFile &TextureFile::operator=(const TextureFile &o)
{
	if (&o != this)
	{
		width = o.width;
		height = o.height;
		components = o.components;
		format = o.format;
		wrap = o.wrap;
		minFilter = o.minFilter;
		magFilter = o.magFilter;
	}
	return *this;
}

TextureFile &TextureFile::operator=(gl::Texture2D &t)
{
	_texture = std::make_unique<gl::Texture2D>(t);
	return *this;
}

void TextureFile::save(cereal::PortableBinaryOutputArchive &ar) const
{
	ar(cereal::make_nvp("datas", datas), CEREAL_NVP(width), CEREAL_NVP(height), CEREAL_NVP(components), CEREAL_NVP(format));
}

void TextureFile::load(cereal::PortableBinaryInputArchive &ar)
{
	ar(cereal::make_nvp("datas", datas), CEREAL_NVP(width), CEREAL_NVP(height), CEREAL_NVP(components), CEREAL_NVP(format));
	_texture = std::make_unique<gl::Texture2D>(components, width, height, true);
	_texture->setOptionTransfer(0, format, GL_UNSIGNED_BYTE);
	_texture->write(datas.data());
	_texture->generateMipMap();
	_texture->filter(minFilter, magFilter).wrap(wrap);
	_texture->storage(1);
}

GLuint TextureFile::getId() const
{
	return _texture->getId();
}