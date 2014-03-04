#include <iostream>
#include "FontManager.hh"

FontManager::FontManager()
{}

FontManager::~FontManager()
{}

bool FontManager::init()
{
	if (FT_Init_FreeType(&_library))
	{
		std::cerr << "Could not init freetype library" << std::endl;
		return false;
	}
	return true;
}