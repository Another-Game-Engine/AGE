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

	FT_Face face;
 
	if (FT_New_Face(_library, file.getFullName().c_str(), 0, &face)) {
		std::cerr << "Could not load font " << file.getFullName() << std::endl;
		return false;
	}

	if (_collection.find(face->family_name) != std::end(_collection))
	{
		FT_Done_Face(face);
		return true;
	}

	std::string _name;
	if (name.empty())
	{
		_name = std::string(face->family_name) + "_" + std::string(face->style_name);
	}
	else
		_name = name;
	_collection.insert(std::make_pair(name, face));

	return true;
}

bool FontManager::isLoaded(const std::string &name)
{
	return _collection.find(name) != std::end(_collection);
}
