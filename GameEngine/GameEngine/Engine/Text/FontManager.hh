#pragma once

#include <Utils/Dependency.hpp>
#include <map>
#include <string>
#include <Utils/File.hpp>
#include "Font.hh"
#include <glm/glm.hpp>
#include <Core/Engine.hh>

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
	void drawString(const std::string &s, const std::string &fontName, std::size_t size, const glm::fvec2 &position, const std::string &shader);
private:
	bool _convertFont(Font::FontSize &font, std::size_t size, FT_Face &face);
	std::map <std::string, Font> _collection;
	FT_Library  _library;
	Engine *_engine;
};