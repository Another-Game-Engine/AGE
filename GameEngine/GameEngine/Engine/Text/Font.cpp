#include "Font.hh"

Font::Font()
: _name("")
, _isLoaded(false)
{
}

Font::~Font()
{
}

bool Font::load(std::unique_ptr<VertexManager<2>> &vm)
{
	if (_isLoaded)
		return true;
	for (auto &e : _sizes)
	{
		if (!e.second.load(vm))
			return false;
	}
	_isLoaded = true;
	return true;
}

bool Font::isLoaded()
{
	return _isLoaded;
}

Font::FontSize::FontSize()
: _size(0)
, _glyphSize(0)
, _textureId(0)
, _texW(0)
, _texH(0)
{}

bool Font::FontSize::load(std::unique_ptr<VertexManager<2>> &vm)
{
	if (_textureDatas.size() == 0)
		return false;
	if (isLoaded())
		return true;

	std::vector<glm::vec4>		vertices;	// vertices positions
	std::vector<glm::vec2>		uvs;		// texture coordinates
	std::vector<unsigned int>	indices;	// indices

	vertices.resize(4);
	uvs.resize(4);
	indices.resize(4);
	indices = { 0, 1, 2, 3 };
	for (auto &glyph : _map)
	{
		auto glyphWidth = (float)glyph.width;
		vertices[0] = glm::vec4(0, 0, 0, 1);
		vertices[1] = glm::vec4(glyphWidth, 0, 0, 1);
		vertices[2] = glm::vec4(glyphWidth, (float)(_size), 0, 1);
		vertices[3] = glm::vec4(0, (float)(_size), 0, 1);

		uvs[0] = glm::vec2(glyph.uvs[0], glyph.uvs[1]);
		uvs[1] = glm::vec2(glyph.uvs[2], glyph.uvs[1]);
		uvs[2] = glm::vec2(glyph.uvs[2], glyph.uvs[3]);
		uvs[3] = glm::vec2(glyph.uvs[0], glyph.uvs[3]);

		std::array<Data, 2> data =
		{
			Data(vertices.size() * 4 * sizeof(float), &vertices[0].x),
			Data(uvs.size() * 2 * sizeof(float), &uvs[0].x)
		};
		Data indicesData(indices.size() * sizeof(unsigned int), &indices[0]);
		glyph.buffer = new Vertice<2>(vertices.size(), data, &indicesData);
		vm->addVertice(*(glyph.buffer));
	}

	glGenTextures(1, &_textureId);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_ALPHA,
		static_cast<GLsizei>(_texW),
		static_cast<GLsizei>(_texH),
		0,
		GL_ALPHA,
		GL_UNSIGNED_BYTE,
		_textureDatas.data()
		);
	return true;
}

bool Font::FontSize::isLoaded()
{
	return _textureId != 0;
}


template void Font::serialize<cereal::PortableBinaryInputArchive>(cereal::PortableBinaryInputArchive &);
template void Font::FontSize::serialize<cereal::PortableBinaryInputArchive>(cereal::PortableBinaryInputArchive &);