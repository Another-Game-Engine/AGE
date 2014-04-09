#include "TextureFile.hpp"

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
	glDeleteTextures(1, &id);
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


void TextureFile::save(cereal::PortableBinaryOutputArchive &ar) const
{
	ar(cereal::make_nvp("datas", datas), CEREAL_NVP(width), CEREAL_NVP(height), CEREAL_NVP(components), CEREAL_NVP(format));
}

void TextureFile::load(cereal::PortableBinaryInputArchive &ar)
{
	ar(cereal::make_nvp("datas", datas), CEREAL_NVP(width), CEREAL_NVP(height), CEREAL_NVP(components), CEREAL_NVP(format));
	glGenTextures(1, &id);
	assert(id != 0 && "Error generating texture.");
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, components, width, height, 0, format, GL_UNSIGNED_BYTE, datas.data());

	if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
		minFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minFilter == GL_NEAREST_MIPMAP_NEAREST)
		glGenerateMipmap(GL_TEXTURE_2D);
}