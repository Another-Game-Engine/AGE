#include "FontGlyph.hh"

FontGlyph::FontGlyph()
: index(0)
, width(0)
, height(0)
, buffer(nullptr)
{
	uvs.fill(0);
}

FontGlyph::FontGlyph(const FontGlyph &o)
: index(o.index)
, width(o.width)
, height(o.height)
, uvs(o.uvs)
, buffer(o.buffer)
{
}

FontGlyph::~FontGlyph()
{}

template void FontGlyph::serialize<cereal::PortableBinaryInputArchive>(cereal::PortableBinaryInputArchive &);