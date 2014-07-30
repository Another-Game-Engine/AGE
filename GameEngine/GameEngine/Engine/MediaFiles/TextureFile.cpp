#include "TextureFile.hpp"
#include <math.h>
#include <algorithm>
#include <Render/ShadingManager.hh>
#include <Render/Texture.hh>

TextureFile::TextureFile()
: MediaFile<TextureFile>(),
  width(0),
  height(0),
  components(0),
  format(0),
  wrap(GL_REPEAT),
  minFilter(GL_LINEAR_MIPMAP_LINEAR),
  magFilter(GL_LINEAR)
{
	_type = TEXTURE;
}

TextureFile::~TextureFile()
{
}

TextureFile::TextureFile(const TextureFile &o)
: MediaFile<TextureFile>(o),
  width(0),
  height(0),
  components(0),
  format(0),
  wrap(GL_REPEAT),
  minFilter(GL_LINEAR_MIPMAP_LINEAR),
  magFilter(GL_LINEAR),
  key(KEY_DESTROY)
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
		key = o.key;
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
	auto manager = _dependencyManager.lock()->getInstance<gl::ShadingManager>();
	key = manager->addTexture2D(components, width, height, true);
	manager->setOptionTransferTexture2D(key, 0, format, GL_UNSIGNED_BYTE);
	manager->writeTexture(key, datas.data());
	manager->generateMipMapTexture2D(key);
	manager->filterTexture2D(key, minFilter, magFilter);
	manager->wrapTexture2D(key, wrap);
	manager->storageTexture2D(key, 1);
}

gl::Key<gl::Texture> const &TextureFile::getTexture() const
{
	return (key);
}