#pragma once

#include <Managers/AssetsConvertorManager.hh>
#include <Utils/Dependency.hpp>
#include <Text/Font.hh>
#include <Text/FontGlyph.hh>
#include <ft2build.h>
#include <iostream>

#include FT_FREETYPE_H

static void drawBitmap(unsigned char* dstBitmap,
	std::size_t x,
	std::size_t y,
	std::size_t dstWidth,
	unsigned char* srcBitmap,
	std::size_t srcWidth,
	std::size_t srcHeight)
{
    dstBitmap +=  (x + (y * dstWidth));
    for (std::size_t i = 0; i < srcHeight; ++i)
    {
        memcpy(dstBitmap, (const void*)srcBitmap, srcWidth);
        dstBitmap += dstWidth;
		srcBitmap += srcWidth;
    }
}

class FontConvertor : public Dependency<FontConvertor>
{
public:
	FontConvertor()
	{
	}

	virtual ~FontConvertor()
	{}

	bool init()
	{
		if (FT_Init_FreeType(&_library))
		{
			std::cerr << "Could not init freetype library" << std::endl;
			return false;
		}
		return true;
	}


	bool convertFont(const File &file,
		const std::vector<std::size_t> &sizes,
		const std::string &outputDirectory,
		const std::string &name)
	{
		if (!file.exists())
		{
			std::cerr << "Font file " << file.getFullName() << " not found" << std::endl;
			return false;
		}

		FT_Face face;

		if (FT_New_Face(_library, file.getFullName().c_str(), 0, &face)) {
			std::cerr << "Could not load font " << file.getFullName() << std::endl;
			return false;
		}

		std::string _name;
		if (name.empty())
		{
			_name = std::string(face->family_name) + "_" + std::string(face->style_name);
		}
		else
			_name = name;

		Font font;
		font._name = _name;

		for (auto &size : sizes)
		{
			if (font._sizes.find(size) != std::end(font._sizes))
				continue;
			Font::FontSize f;
			if (!_convertFont(f, size, face))
				continue;
			font._sizes.insert(std::make_pair(size, f));
		}

		std::ofstream s(outputDirectory + font._name + ".cpdFont", std::ios_base::binary);
		cereal::PortableBinaryOutputArchive ar(s);
		ar(font);
		s.close();
		return true;
	}


private:
	AssetsConvertorManager *_manager;
	FT_Library  _library;

	// CODE LARGELY INSPIRED FROM : https://github.com/blackberry/GamePlay/blob/master/tools/encoder/src/TTFFontEncoder.cpp
	bool _convertFont(Font::FontSize &font, std::size_t size, FT_Face &face)
	{
		std::size_t rowSize = 0;
		std::size_t actualfontHeight = 0;

		font._size = size;

		FT_GlyphSlot slot = NULL;
		FT_Int32 loadFlags = FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT;

		for (std::size_t requestedSize = font._size; requestedSize > 0; --requestedSize)
		{
			if (FT_Set_Pixel_Sizes(face, static_cast<FT_UInt>(requestedSize), 0))
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
			std::cerr << "Error generating Charset of size " << font._size << "for font " << face->family_name << std::endl;
			return false;
		}

		// Include padding in the rowSize.
		rowSize += GLYPH_PADDING;

		// Initialize with padding.
		std::size_t penX = 0;
		std::size_t penY = 0;
		std::size_t row = 0;

		double powerOf2 = 2;
		bool textureSizeFound = false;

		std::size_t advance;
		std::size_t i;

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
				//penY += (actualfontHeight - glyphHeight) + (glyphHeight - slot->bitmap_top);

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
			if ((penY + rowSize) >= (std::size_t)(pow(2.0, powerOf2)))
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
			std::size_t glyphWidth(slot->bitmap.width);
			std::size_t glyphHeight(slot->bitmap.rows);

			advance = glyphWidth + static_cast<std::size_t>(GLYPH_PADDING);

			// If we reach the end of the image wrap aroud to the next row.
			if ((penX + advance) > font._texW)
			{
				penX = 0;
				row += 1;
				penY = row * rowSize;
				if (penY + rowSize > font._texH)
				{
					std::cerr << "Image size exceeded" << std::endl;
					return false;
				}
			}

			// penY should include the glyph offsets.
			penY += (actualfontHeight - glyphHeight) + (glyphHeight - std::size_t(slot->bitmap_top));

			// Draw the glyph to the bitmap with a one pixel padding.
			drawBitmap(font._textureDatas.data(), penX, penY, font._texW, glyphBuffer, glyphWidth, glyphHeight);

			// Move Y back to the top of the row.
			penY = row * rowSize;

			font._map[i].index = ascii;
			font._map[i].width = advance - GLYPH_PADDING;
			font._map[i].height = penY + rowSize - GLYPH_PADDING;

			// Generate UV coords.
			font._map[i].uvs[0] = (float)penX / (float)font._texW;
			font._map[i].uvs[1] = (float)penY / (float)font._texH;
			font._map[i].uvs[2] = (float)(penX + advance - GLYPH_PADDING) / (float)font._texW;
			font._map[i].uvs[3] = (float)(penY + rowSize - GLYPH_PADDING) / (float)font._texH;

			// Set the pen position for the next glyph
			penX += advance;
			i++;
		}
		return true;
	}
};