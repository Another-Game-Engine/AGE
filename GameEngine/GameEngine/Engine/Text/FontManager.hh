#pragma once

#include <Utils/Dependency.hpp>
#include <map>
#include <string>
#include <ft2build.h>

#include FT_FREETYPE_H

class FontManager : public Dependency
{
public:
	FontManager();
	virtual ~FontManager();
	bool init();
	std::map <std::string, FT_Face> _collection;
	FT_Library  _library;
};