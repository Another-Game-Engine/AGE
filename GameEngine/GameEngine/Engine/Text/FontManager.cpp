#include <iostream>
#include "FontManager.hh"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>

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

static void drawBitmap(unsigned char* dstBitmap, int x, int y, int dstWidth, unsigned char* srcBitmap, int srcWidth, int srcHeight)
{
    // offset dst bitmap by x,y.
    dstBitmap +=  (x + (y * dstWidth));

    for (int i = 0; i < srcHeight; ++i)
    {
        memcpy(dstBitmap, (const void*)srcBitmap, srcWidth);
        srcBitmap += srcWidth;
        dstBitmap += dstWidth;
    }
}


// CODE LARGELY INSPIRED FROM : https://github.com/blackberry/GamePlay/blob/master/tools/encoder/src/TTFFontEncoder.cpp
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
	font._name = _name;

	unsigned int rowSize = 0;
	unsigned int actualfontHeight = 0;

	FT_GlyphSlot slot = NULL;
	FT_Int32 loadFlags = FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT;

	for (unsigned int requestedSize = font._size; requestedSize > 0; --requestedSize)
	{
		if (FT_Set_Char_Size(face, 0, requestedSize * 64, 0, 0))
		{
			//
			return false;
		}

		slot = face->glyph;

		rowSize = 0;
		font._glyphSize = 0;
		actualfontHeight = 0;

		for (unsigned char ascii = ASCII_BEGIN; ascii < ASCII_END; ++ascii)
		{
			if (FT_Load_Char(face, ascii, loadFlags))
			{
				std::cerr << "Error loading char [" << (unsigned char)(ascii) << "]" << std::endl;
				continue;
			}

			unsigned int bitmapRows = slot->bitmap.rows;
			actualfontHeight = (actualfontHeight < bitmapRows) ? bitmapRows : actualfontHeight;

			if (slot->bitmap.rows > slot->bitmap_top)
			{
				bitmapRows += (slot->bitmap.rows - slot->bitmap_top);
			}
			rowSize = (rowSize < bitmapRows) ? bitmapRows : rowSize;
		}

		if (rowSize <= font._size)
		{
			font._glyphSize = rowSize;
			rowSize = font._size;
			break;
		}
	}

	if (slot == NULL || font._glyphSize == 0)
	{
		std::cerr << "Error generating Charset of size " << font._size << "for font " << font._name << std::endl;
		return false;
	}

	// Include padding in the rowSize.
	rowSize += GLYPH_PADDING;

	// Initialize with padding.
	unsigned int penX = 0;
	unsigned int penY = 0;
	int row = 0;

	double powerOf2 = 2;
	bool textureSizeFound = false;

	int advance;
	int i;

	while (textureSizeFound == false)
	{
		font._texW = (std::size_t)pow(2.0, powerOf2);
		font._texH = (std::size_t)pow(2.0, powerOf2);
		penX = 0;
		penY = 0;
		row = 0;

		// Find out the squared texture size that would fit all the require font glyphs.
		i = 0;
		for (unsigned char ascii = ASCII_BEGIN; ascii < ASCII_END; ++ascii)
		{
			// Load glyph image into the slot (erase the previous one).
			
			if (FT_Load_Char(face, ascii, loadFlags))
			{
				std::cerr << "Error loading char [" << (unsigned char)(ascii) << "]" << std::endl;
			}
			// Glyph image.
			int glyphWidth = slot->bitmap.pitch;
			int glyphHeight = slot->bitmap.rows;

			advance = glyphWidth + GLYPH_PADDING;

			// If we reach the end of the image wrap aroud to the next row.
			if ((penX + advance) > font._texW)
			{
				penX = 0;
				row += 1;
				penY = row * rowSize;
				if (penY + rowSize > font._texH)
				{
					powerOf2++;
					break;
				}
			}

			// penY should include the glyph offsets.
			penY += (actualfontHeight - glyphHeight) + (glyphHeight - slot->bitmap_top);

			// Set the pen position for the next glyph
			penX += advance; // Move X to next glyph position
			// Move Y back to the top of the row.
			penY = row * rowSize;

			if (ascii == (ASCII_END - 1))
			{
				textureSizeFound = true;
			}
			i++;
		}
	}

	// Try further to find a tighter texture size.
	powerOf2 = 1;
	for (;;)
	{
		if ((penY + rowSize) >= pow(2.0, powerOf2))
		{
			powerOf2++;
		}
		else
		{
			font._texH = (std::size_t)pow(2.0, powerOf2);
			break;
		}
	}

	// Allocate temporary image buffer to draw the glyphs into.
	font._textureDatas.clear();
	font._textureDatas.assign(font._texW * font._texH, '\0');
	penX = 0;
	penY = 0;
	row = 0;
	i = 0;
	for (unsigned char ascii = ASCII_BEGIN; ascii < ASCII_END; ++ascii)
	{
		// Load glyph image into the slot (erase the previous one).		
		if (FT_Load_Char(face, ascii, loadFlags))
		{
			std::cerr << "Error loading char [" << (unsigned char)(ascii) << "]" << std::endl;
		}

		// Glyph image.
		unsigned char* glyphBuffer = slot->bitmap.buffer;
		int glyphWidth = slot->bitmap.pitch;
		int glyphHeight = slot->bitmap.rows;

		advance = glyphWidth + GLYPH_PADDING;

		// If we reach the end of the image wrap aroud to the next row.
		if ((penX + advance) > (int)font._texW)
		{
			penX = 0;
			row += 1;
			penY = row * rowSize;
			if (penY + rowSize > (int)font._texH)
			{
				std::cerr << "Image size exceeded" << std::endl;
				return false;
			}
		}

		// penY should include the glyph offsets.
		penY += (actualfontHeight - glyphHeight) + (glyphHeight - slot->bitmap_top);

		// Draw the glyph to the bitmap with a one pixel padding.
		drawBitmap(font._textureDatas.data(), penX, penY, font._texW, glyphBuffer, glyphWidth, glyphHeight);

		// Move Y back to the top of the row.
		penY = row * rowSize;

		font._map[i].index = ascii;
		font._map[i].width = advance - GLYPH_PADDING;

		// Generate UV coords.
		font._map[i].uvs[0] = (float)penX / (float)font._texW;
		font._map[i].uvs[1] = (float)penY / (float)font._texH;
		font._map[i].uvs[2] = (float)(penX + advance - GLYPH_PADDING) / (float)font._texW;
		font._map[i].uvs[3] = (float)(penY + rowSize - GLYPH_PADDING) / (float)font._texH;

		// Set the pen position for the next glyph
		penX += advance;
		i++;
	}
	std::ofstream s(outputDirectory + font._name + ".cpdFont", std::ios_base::binary);
	cereal::PortableBinaryOutputArchive ar(s);
	ar(font);
	s.close();
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
