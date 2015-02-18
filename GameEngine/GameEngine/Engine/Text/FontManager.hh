#pragma once

#define ACTIVATE 0

#if ACTIVATE

#include <Utils/OpenGL.hh>
#include <Utils/Dependency.hpp>
#include <iostream>
#include <memory>
#include <Utils/Containers/Vector.hpp>
#include <map>
#include <string>
#include <glm/glm.hpp>

class OldFile;
class Font;
class PubSub;


#if !TEST_NEW_VERTEXMANAGER
#include <Render/GeometryManager.hh>
template <uint8_t NBR_ATTRIBUTE> class VertexManager;
#endif

class FontManager : public Dependency<FontManager>
{
public:
	FontManager();
	virtual ~FontManager();
	bool init();

	bool loadFont(const OldFile &file, const std::string &name = "");
	bool isLoaded(const std::string &name);
	void draw2DString(const std::string &s,
		const std::string &fontName,
		std::size_t size,
		const glm::ivec2 &position,
		const glm::vec4 &color,
		const std::string &shader);
private:
	void _drawList();
	void _draw2DString(const std::string &s,
		const std::string &fontName,
		std::size_t size,
		const glm::ivec2 &position,
		const glm::vec4 &color,
		const std::string &shader);

	struct DrawStringSave
	{
		DrawStringSave(
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
			, shader(_shader)
		{}

		std::string str;
		std::string fontName;
		std::size_t size;
		glm::ivec2 position;
		glm::vec4 color;
		std::string shader;
	};

	AGE::Vector<DrawStringSave> _toDraw;
	std::map <std::string, Font> _collection;
//	std::unique_ptr<VertexManager<2>> _vertexManager;
	std::unique_ptr<PubSub> _pubSub;
};

#endif