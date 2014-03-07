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


class FontManager : public Dependency
{
public:
	FontManager();
	virtual ~FontManager();
	bool init(Engine *e);

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
	bool _convertFont(Font::FontSize &font, std::size_t size, FT_Face &face);
	std::map <std::string, Font> _collection;
	FT_Library  _library;
	Engine *_engine;
	std::unique_ptr<VertexManager<2>> _vertexManager;
	std::vector<glm::vec4>		_vertices;	// vertices positions
	std::vector<glm::vec2>		_uvs;		// texture coordinates
	std::vector<unsigned int>	_indices;	// indices

};