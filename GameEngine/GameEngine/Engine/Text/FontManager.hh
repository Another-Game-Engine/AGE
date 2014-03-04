#pragma once

#include <Utils/Dependency.hpp>
#include <map>
#include <string>
#include <ft2build.h>
#include <Utils/File.hpp>

#include FT_FREETYPE_H

class FontManager : public Dependency
{
public:
	FontManager();
	virtual ~FontManager();
	bool init();
	bool loadFont(const File &file, const std::string &name = "");
	bool isLoaded(const std::string &name);
	std::map <std::string, FT_Face> _collection;
	FT_Library  _library;
};