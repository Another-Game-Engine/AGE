#pragma once

#include <Utils/Dependency.hpp>
#include <map>
#include <string>
#include <Utils/File.hpp>
#include "Font.hh"
#include <glm/glm.hpp>
#include <Core/Engine.hh>
#include <OpenGL/Vertice.hh>
#include <OpenGL/VertexManager.hh>
#include <Utils/PubSub.hpp>


class FontManager : public Dependency, public PubSub
{
public:
	FontManager(Engine *e);
	virtual ~FontManager();
	bool init();

	bool convertFont(const File &file,
		const std::vector<std::size_t> &sizes,
		const std::string &outputDirectory,
		const std::string &name = "");

	bool loadFont(const File &file, const std::string &name = "");
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

	std::vector<DrawStringSave> _toDraw;
	bool _convertFont(Font::FontSize &font, std::size_t size, FT_Face &face);
	std::map <std::string, Font> _collection;
	FT_Library  _library;
	Engine *_engine;
	std::unique_ptr<VertexManager<2>> _vertexManager;
	std::vector<glm::vec4>		_vertices;	// vertices positions
	std::vector<glm::vec2>		_uvs;		// texture coordinates
	std::vector<unsigned int>	_indices;	// indices

};