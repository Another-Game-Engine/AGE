#pragma once

#include <Utils/Dependency.hpp>
#include <map>
#include <string>
#include <Utils/File.hpp>
#include "Font.hh"

class FontManager : public Dependency
{
public:
	FontManager();
	virtual ~FontManager();
	bool init();

	bool convertFont(const File &file,
		const std::vector<std::size_t> &sizes,
		const std::string &outputDirectory,
		const std::string &name = "");

	bool loadFont(const File &file, const std::string &name = "");
	bool isLoaded(const std::string &name);

private:
	bool _convertFont(Font::FontSize &font, std::size_t size, FT_Face &face);
	std::map <std::string, FT_Face> _collection;
	FT_Library  _library;
};