#include <iostream>
#include "FontManager.hh"

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <Core/Renderer.hh>
#include <Context/IRenderContext.hh>
#include <glm/gtc/matrix_transform.hpp>

FontManager::FontManager(Engine *engine)
: PubSub(engine->getInstance<PubSub::Manager>())
, _engine(engine)
{}

FontManager::~FontManager()
{}

bool FontManager::init()
{
	globalSub(std::string("endOfFrame"), [&](){
		_drawList();
	});

	std::array<Attribute, 2> param =
	{
		Attribute(GL_FLOAT, sizeof(float), 4),
		Attribute(GL_FLOAT, sizeof(float), 2),
	};
	_vertexManager = std::make_unique<VertexManager<2>>(param);
	return _vertexManager->init();
}

bool FontManager::loadFont(const File &file, const std::string &name)
{
	if (!name.empty())
	{
		if (_collection.find(name) != std::end(_collection))
			return true;
	}

	if (!file.exists())
	{
		std::cerr << "Font file " << file.getFullName() << " not found" << std::endl;
	}

	Font font;
	std::ifstream s(file.getFileName(), std::ios_base::binary);
	cereal::PortableBinaryInputArchive ar(s);
	ar(font);
	s.close();
 
	if (!font.load(_vertexManager))
	{
		std::cerr << "Fail to load font: " << file.getFullName() << std::endl;
		return false;
	}
	_collection.insert(std::make_pair(font._name, font));

	return true;
}

bool FontManager::isLoaded(const std::string &name)
{
	return _collection.find(name) != std::end(_collection);
}

void FontManager::_drawList()
{
	for (auto &e : _toDraw)
	{
		_draw2DString(e.str, e.fontName, e.size, e.position, e.color, e.shader);
	}
	_toDraw.clear();
}

void FontManager::_draw2DString(const std::string &text,
	const std::string &fontName,
	std::size_t size,
	const glm::ivec2 &position,
	const glm::vec4 &color,
	const std::string &shader)
{
	auto s = _engine->getInstance<Renderer>()->getShader(shader);
	if (!s)
		return;
	s->use();

	auto font = _collection.find(fontName);
	if (font == std::end(_collection) || !font->second.isLoaded())
	{
		std::cerr << "Font not found : " << fontName << std::endl;
		return;
	}

	auto fontSize = font->second._sizes.find(size);
	if (fontSize == std::end(font->second._sizes))
	{
		fontSize = font->second._sizes.upper_bound(size);
		if (fontSize == std::end(font->second._sizes))
		{
			fontSize = --font->second._sizes.end();
		}
	}
	if (!fontSize->second.isLoaded())
	{
		std::cerr << "Size [" << size << "] for font " << fontName << " is not loaded." << std::endl;
		return;
	}
	auto &f = fontSize->second;
	float sz = f._size;
	if (sz != (float)size)
		sz = size;

	//_vertices.clear();
	//_uvs.clear();
	//_indices.clear();
	//float lastX = position.x;
	//for (auto i = 0; i < text.size(); ++i)
	//{
	//	auto l = text[i] - ASCII_BEGIN;
	//	// IF SPACE
	//	if (text[i] == ' ')
	//	{
	//		lastX += sz / 3.0f;
	//	}
	//	auto glyphWidth = f._size != size ? ((float)f._map[l].width / (float)f._size) * sz : f._map[l].width;
	//	_vertices.emplace_back(lastX, position.y, 0, 1);
	//	_vertices.emplace_back(lastX + glyphWidth, position.y, 0, 1);
	//	_vertices.emplace_back(lastX + glyphWidth, position.y + sz, 0, 1);
	//	_vertices.emplace_back(lastX, position.y + sz, 0, 1);

	//	_uvs.emplace_back(f._map[l].uvs[0], f._map[l].uvs[1]);
	//	_uvs.emplace_back(f._map[l].uvs[2], f._map[l].uvs[1]);
	//	_uvs.emplace_back(f._map[l].uvs[2], f._map[l].uvs[3]);
	//	_uvs.emplace_back(f._map[l].uvs[0], f._map[l].uvs[3]);


	//	_indices.push_back(i * 4);
	//	_indices.push_back(i * 4 + 1);
	//	_indices.push_back(i * 4 + 2);
	//	_indices.push_back(i * 4 + 3);

	//	lastX += glyphWidth;
	//}

	//std::array<Data, 2> data =
	//{
	//	Data(_vertices.size() * 4 * sizeof(float), &_vertices[0].x),
	//	Data(_uvs.size() * 2 * sizeof(float), &_uvs[0].x)
	//};
	//Data indicesData(_indices.size() * sizeof(unsigned int), &_indices[0]);

	//auto buffer = Vertice<2>(_vertices.size(), data, &indicesData);

	glUniform1i(glGetUniformLocation(s->getId(), "fTexture0"), 0);
	glUniform4f(glGetUniformLocation(s->getId(), "color"), color.x, color.y, color.z, color.a);
	glm::ivec2 screen = _engine->getInstance<IRenderContext>()->getScreenSize();
	glm::mat4 Projection = glm::mat4(1);
	Projection *= glm::ortho(0.0f, (float)screen.x, (float)screen.y, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(s->getId(), "projection"), 1, GL_FALSE, glm::value_ptr(Projection));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, f._textureId);



	//std::array<Attribute, 2> param =
	//{
	//	Attribute(GL_FLOAT, sizeof(float), 4),
	//	Attribute(GL_FLOAT, sizeof(float), 2),
	//};

	//_vertexManager->addVertice(buffer);
	//buffer.draw(GL_QUADS);
}

void FontManager::draw2DString(const std::string &text,
	const std::string &fontName,
	std::size_t size,
	const glm::ivec2 &position,
	const glm::vec4 &color,
	const std::string &shader)
{
	_toDraw.emplace_back(text, fontName, size, position, color, shader);
}