#include <iostream>
#include "FontManager.hh"

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/xml.hpp>
#include <OpenGL/Vertice.hh>
#include <OpenGL/VertexManager.hh>
#include <Core/Renderer.hh>
#include <Context/IRenderContext.hh>
#include <glm/gtc/matrix_transform.hpp>

FontManager::FontManager()
: _engine(nullptr)
{}

FontManager::~FontManager()
{}

bool FontManager::init(Engine *e)
{
	_engine = e;
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
//	srcBitmap += srcWidth * srcHeight;
    for (int i = 0; i < srcHeight; ++i)
    {
		auto d = dstBitmap;
		for (auto j = srcWidth - 1; j >= 0; --j)
		{
			*d = srcBitmap[j];
			++d;
		}
////        srcBitmap -= srcWidth;
//        memcpy(dstBitmap, (const void*)srcBitmap, srcWidth);
        dstBitmap += dstWidth;
		srcBitmap += srcWidth;
    }
}

// CODE LARGELY INSPIRED FROM : https://github.com/blackberry/GamePlay/blob/master/tools/encoder/src/TTFFontEncoder.cpp
bool FontManager::_convertFont(Font::FontSize &font, std::size_t size, FT_Face &face)
{
	unsigned int rowSize = 0;
	unsigned int actualfontHeight = 0;

	font._size = size;

	FT_GlyphSlot slot = NULL;
	FT_Int32 loadFlags = FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT;

	for (unsigned int requestedSize = font._size; requestedSize > 0; --requestedSize)
	{
		if (FT_Set_Pixel_Sizes(face, requestedSize, 0))
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
		int glyphWidth = slot->bitmap.width;
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

		font._map[i].ax = slot->advance.x >> 6;
		font._map[i].ay = slot->advance.y >> 6;
		font._map[i].bw = slot->bitmap.width;
		font._map[i].bh = slot->bitmap.rows;
		font._map[i].bl = slot->bitmap_left;
		font._map[i].bt = slot->bitmap_top;
		font._map[i].tx = penX / (float)font._texW;
		font._map[i].ty = penY / (float)font._texH;

		GLuint tid;
			glGenTextures(1, &tid);
			glBindTexture(GL_TEXTURE_2D, tid);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_ALPHA,
				slot->bitmap.width,
				slot->bitmap.rows,
				0,
				GL_ALPHA,
				GL_UNSIGNED_BYTE,
				slot->bitmap.buffer
				);

		// Draw the glyph to the bitmap with a one pixel padding.
		drawBitmap(font._textureDatas.data(), penX, penY, font._texW, glyphBuffer, glyphWidth, glyphHeight);

		// Move Y back to the top of the row.
		penY = row * rowSize;

		font._map[i].index = ascii;
		font._map[i].width = advance - GLYPH_PADDING;
		font._map[i].height = penY + rowSize - GLYPH_PADDING;

		if (i == 't')
			std::cout << "prout" << std::endl;

		// Generate UV coords.
		font._map[i].uvs[0] = (float)penX / (float)font._texW;
		font._map[i].uvs[1] = (float)penY / (float)font._texH;
		font._map[i].uvs[2] = (float)(penX + advance - GLYPH_PADDING) / (float)font._texW;
		font._map[i].uvs[3] = (float)(penY + rowSize - GLYPH_PADDING) / (float)font._texH;

		if (size == 70.0f)
			std::cout << "asdas" << std::endl;

		// Set the pen position for the next glyph
		penX += advance;
		i++;
	}
	return true;
}

bool FontManager::convertFont(const File &file,
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
		font.load();
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

	Font font;
	std::ifstream s(file.getFileName(), std::ios_base::binary);
	cereal::PortableBinaryInputArchive ar(s);
	ar(font);
	s.close();
	if (!font.load())
	{
		std::cerr << "Fail to load font: " << file.getFullName() << std::endl;
		return false;
	}
	_collection.insert(std::make_pair(font._name, font));

	return true;
}

bool FontManager::isLoaded(const std::string &name)
{
	return _collection.find(name) != std::end(_collection);
}

void FontManager::draw2DString(const std::string &text, const std::string &fontName, std::size_t size, const glm::ivec2 &position, const std::string &shader)
{
	auto s = _engine->getInstance<Renderer>()->getShader(shader);
	if (!s)
		return;
	s->use();

	auto font = _collection.find(fontName);
	if (font == std::end(_collection) || !font->second.isLoaded())
	{
		std::cerr << "Font not found : " << fontName << std::endl;
		return;
	}

	auto fontSize = font->second._sizes.find(size);
	if (fontSize == std::end(font->second._sizes) || !fontSize->second.isLoaded())
	{
		std::cerr << "Size not found [" << size << "] for font : " << fontName << std::endl;
		return;
	}
	auto &f = fontSize->second;

	std::vector<glm::vec4>		vertices;	// vertices positions
	std::vector<glm::vec4>		colors;		// vertices colors
	std::vector<glm::vec2>		uvs;		// texture coordinates
	std::vector<unsigned int>	indices;	// indices
	Vertice<3>					buffer;
	float lastX = position.x;
	for (auto i = 0; i < text.size(); ++i)
	{
		auto l = text[i] - ASCII_BEGIN;
		// IF SPACE
		if (text[i] == ' ')
		{
			lastX += 0.01f;
		}
		vertices.push_back(glm::vec4(lastX, position.y + size, 0, 1));
		vertices.push_back(glm::vec4(lastX + f._map[l].width, position.y + size, 0, 1));
		vertices.push_back(glm::vec4(lastX + f._map[l].width, position.y, 0, 1));
		vertices.push_back(glm::vec4(lastX, position.y, 0, 1));


		colors.push_back(glm::vec4(0, (float)(i) / 48.0f, 1, 1));
		colors.push_back(glm::vec4(0, (float)(i) / 48.0f, 1, 1));
		colors.push_back(glm::vec4(0, (float)(i) / 48.0f, 1, 1));
		colors.push_back(glm::vec4(0, (float)(i) / 48.0f, 1, 1));

		uvs.push_back(glm::vec2(f._map[l].uvs[2], f._map[l].uvs[3]));
		uvs.push_back(glm::vec2(f._map[l].uvs[0], f._map[l].uvs[3]));
		uvs.push_back(glm::vec2(f._map[l].uvs[0], f._map[l].uvs[1]));
		uvs.push_back(glm::vec2(f._map[l].uvs[2], f._map[l].uvs[1]));


		indices.push_back(i * 4);
		indices.push_back(i * 4 + 1);
		indices.push_back(i * 4 + 2);
		indices.push_back(i * 4 + 3);

		lastX += f._map[l].width * 3;
	}

	std::array<Data, 3> data =
	{
		Data(vertices.size() * 4 * sizeof(float), &vertices[0].x),
		Data(colors.size() * 4 * sizeof(float), &colors[0].x),
		Data(uvs.size() * 2 * sizeof(float), &uvs[0].x)
	};
	Data indicesData(indices.size() * sizeof(unsigned int), &indices[0]);
	buffer = Vertice<3>(vertices.size(), data, &indicesData);

	std::array<Attribute, 3> param =
	{
		Attribute(GL_FLOAT, sizeof(float), 4),
		Attribute(GL_FLOAT, sizeof(float), 4),
		Attribute(GL_FLOAT, sizeof(float), 2),
	};

	glUniform1i(glGetUniformLocation(s->getId(), "fTexture0"), 0);
	glm::ivec2 screen = _engine->getInstance<IRenderContext>()->getScreenSize();
	glm::mat4 Projection = glm::mat4(1);
	Projection *= glm::ortho(0.0f, (float)screen.x, (float)screen.y, 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(s->getId(), "transformation"), 1, GL_FALSE, glm::value_ptr(Projection));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, f._textureId);

	auto leak = new VertexManager<3>(param);
	leak->init();
	leak->addVertice(buffer);

	buffer.draw(GL_QUADS);
	delete leak;
}