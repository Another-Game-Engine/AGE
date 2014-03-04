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

bool FontManager::convertFont(const File &file, std::size_t size, const std::string &outputDirectory, const std::string &name)
{
	if (!file.exists())
	{
		std::cerr << "Font file " << file.getFullName() << " not found" << std::endl;
	}

	FT_Face face;

	if (FT_New_Face(_library, file.getFullName().c_str(), 0, &face)) {
		std::cerr << "Could not load font " << file.getFullName() << std::endl;
		return false;
	}

	std::string _name;
	if (name.empty())
	{
		_name = std::string(face->family_name) + "_" + std::string(face->style_name) + "_" + std::to_string(size);
	}
	else
		_name = name;

	Font font;
	font._size = size;
	font._name = name;

	std::size_t rowSize = 0;
	std::size_t actualFontH = 0;
	std::size_t glyphSize = 0;

	FT_GlyphSlot slot = nullptr;

	for (std::size_t possibleSize = size; possibleSize > 0; --possibleSize)
	{
		if (FT_Set_Char_Size(face, 0, possibleSize * 64, 0, 0))
		{
			std::cerr << "Error with FT_Set_Char_Size for font " << _name << std::endl;
			continue;
		}

		slot = face->glyph;
		rowSize = actualFontH = glyphSize = 0;

		for (auto c = ASCII_BEGIN; c < ASCII_END; ++c)
		{
			if (FT_Load_Char(face, c, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT))
			{
				std::cerr << "Error loading [" << (char)(c) << "] for font " << _name << std::endl;
				continue;
			}

			if (actualFontH < slot->bitmap.rows)
				actualFontH = slot->bitmap.rows;

			auto br = 0;
			if (slot->bitmap.rows > slot->bitmap_top)
				br = slot->bitmap.rows + slot->bitmap.rows - slot->bitmap_top;
			if (rowSize < br)
				rowSize = br;

		}
		if (rowSize < size)
		{
			glyphSize = rowSize;
			rowSize = size;
			break;
		}
	}

	if (slot == nullptr || glyphSize == 0)
	{
		std::cerr << "Error generating charset for " << _name << std::endl;
		return false;
	}

	std::size_t maxW = 0;
	std::size_t maxH = 0;

	for (auto c = ASCII_BEGIN; c < ASCII_END; ++c)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT))
		{
			std::cerr << "Error loading [" << (char)(c) << "] for font " << _name << std::endl;
			continue;
		}
		if (maxH < slot->bitmap.rows)
			maxH = slot->bitmap.rows;
		if (maxW < slot->bitmap.width)
			maxW = slot->bitmap.width;
	}


	auto nx = 12;
	auto ny = (ASCII_END - ASCII_BEGIN) / 12;
	font._textureDatas = std::unique_ptr<unsigned char>(new unsigned char(maxW * maxH * nx * ny));
	auto blop = maxW * maxH * nx * ny;
	font._texW = maxW * nx;
	font._texH = maxH * ny;
	for (auto c = ASCII_BEGIN; c < ASCII_END; ++c)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT))
		{
			std::cerr << "Error loading [" << (char)(c) << "] for font " << _name << std::endl;
			continue;
		}
		auto src = slot->bitmap.buffer;
		for (auto i = 0; i < slot->bitmap.rows; ++i)
		{
			auto dest = font._textureDatas.get();
			dest += ((c - ASCII_BEGIN) % nx) * maxW + (((c - ASCII_BEGIN) / nx) * font._texW) + i * font._texW;

			std::memcpy(dest, src, slot->bitmap.width);
			src += slot->bitmap.width;
			auto prout = dest - font._textureDatas.get();
			std::cout << prout << " " << blop << std::endl;
		}
//		dest = &a;

	}
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_ALPHA,
		font._texW,
		font._texH,
		0,
		GL_ALPHA,
		GL_UNSIGNED_BYTE,
		font._textureDatas.get()
		);
	std::cout << "lol" << std::endl;
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
