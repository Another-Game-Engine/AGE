#include "FontManager.hh"

#if ACTIVATE

#include <Utils/PubSub.hpp>
#include <Text/Font.hh>
#include <cereal/archives/xml.hpp>
#include <Core/Renderer.hh>
#include <Context/IRenderContext.hh>
#include <glm/gtc/matrix_transform.hpp>

FontManager::FontManager()
: Dependency()
{}

FontManager::~FontManager()
{}

bool FontManager::init()
{
	_pubSub = std::make_unique<PubSub>(_dependencyManager->getInstance<PubSub::Manager>());
	_pubSub->globalSub(std::string("endOfFrame"), [&](){
		_drawList();
	});

//	std::array<Attribute, 2> param =
//	{
//		Attribute(GL_FLOAT, sizeof(float), 4), //-V112
//		Attribute(GL_FLOAT, sizeof(float), 2),
//	};
//	_vertexManager = std::make_unique<VertexManager<2>>(param);
//	return _vertexManager->init();
	return (true);
}

bool FontManager::loadFont(const OldFile &file, const std::string &name)
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

	std::ifstream s(file.getFullName(), std::ios_base::binary);
	if (!s.is_open())
	{
		std::cerr << "File " << file.getFullName() << " is not open." << std::endl;
		return false;
	}

	cereal::PortableBinaryInputArchive ar(s);
	ar(font);
	s.close();

	//if (!font.load(_vertexManager))
	//{
	//	std::cerr << "Fail to load font: " << file.getFullName() << std::endl;
	//	return false;
	//}
	_collection.insert(std::make_pair(font._name, font));

	return true;
}

bool FontManager::isLoaded(const std::string &name)
{
	return _collection.find(name) != std::end(_collection);
}

void FontManager::_drawList()
{
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);

	for (auto &e : _toDraw)
	{
		_draw2DString(e.str, e.fontName, e.size, e.position, e.color, e.shader);
	}
	_toDraw.clear();
	glDisable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
}

void FontManager::_draw2DString(const std::string &text,
	const std::string &fontName,
	std::size_t size,
	const glm::ivec2 &position,
	const glm::vec4 &color,
	const std::string &shader)
{
	auto s = _dependencyManager->getInstance<Renderer>()->getShader(shader);
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
	float sz = static_cast<float>(f._size);
	if (sz != static_cast<float>(size))
		sz = static_cast<float>(size);
	glm::mat4 transformation(1);
	transformation = glm::translate(transformation, glm::vec3(position.x, position.y, 0));

	glUniform1i(glGetUniformLocation(s->getId(), "fTexture0"), 0);
	glUniform4f(glGetUniformLocation(s->getId(), "color"), color.x, color.y, color.z, color.a);
	auto screen = _dependencyManager->getInstance<IRenderContext>()->getScreenSize();
	glm::mat4 Projection = glm::mat4(1);
	Projection *= glm::ortho(0.0f, (float)screen.x, (float)screen.y, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(s->getId(), "projection"), 1, GL_FALSE, glm::value_ptr(Projection));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, f._textureId);
	auto transformationID = glGetUniformLocation(s->getId(), "transformation");
	auto glyphWidth = 0.0f;
	float lastX = static_cast<float>(position.x);
	float lineWidth = 0.0f;
	for (std::size_t i = 0; i < text.size(); ++i)
	{
		std::size_t l = std::size_t(text[i] - ASCII_BEGIN);
		if (l > ASCII_END)
			glyphWidth = static_cast<float>(size);
		else
			glyphWidth = f._size != size ? ((float)f._map[l].width / (float)f._size) * sz : f._map[l].width;
		if (text[i] == ' ')
		{
			lastX = sz / 3.0f;
		}
		else if (text[i] == '\n')
		{
			transformation = glm::translate(transformation, glm::vec3(-lineWidth, f._glyphSize, 0));
			lineWidth = 0;
			continue;
		}
		else
		{
			lastX = glyphWidth;
		}
		lineWidth += lastX;
		glUniformMatrix4fv(transformationID, 1, GL_FALSE, glm::value_ptr(transformation));
	//	f._map[l].buffer->draw(GL_QUADS);
		transformation = glm::translate(transformation, glm::vec3(lastX, 0, 0));
	}
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

FontManager::DrawStringSave::DrawStringSave(
	const std::string &_str
	, const std::string &_fontName
	, std::size_t _size
	, const glm::ivec2 &_position
	, const glm::vec4 &_color
	, const std::string &_shader
	) : str(_str)
	, fontName(_fontName)
	, size(_size)
	, position(_position)
	, color(_color)
	, shader(_shader))
{
}

#endif