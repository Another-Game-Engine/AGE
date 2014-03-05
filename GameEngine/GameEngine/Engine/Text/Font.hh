#pragma once

#include "FontGlyph.hh"
#include <Utils/OpenGL.hh>
#include <vector>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>

class Font
{
public:
	Font();
	~Font();
	bool load();
	bool isLoaded();

	template <class Archive>
	void serialize(Archive &ar)
	{
		ar(_name, _sizes);
	}

private:

	struct FontSize
	{
		std::size_t _size;
		std::size_t _glyphSize;
		std::vector<unsigned char> _textureDatas;
		GLuint _textureId;
		std::size_t _texW;
		std::size_t _texH;
		std::array<FontGlyph, ASCII_END - ASCII_BEGIN> _map;

		FontSize()
			: _size(0)
			, _glyphSize(0)
			, _textureId(0)
			, _texW(0)
			, _texH(0)
		{}

		bool load()
		{
			if (_textureDatas.size() == 0)
				return false;
			if (isLoaded())
				return true;
			glGenTextures(1, &_textureId);
			glBindTexture(GL_TEXTURE_2D, _textureId);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_ALPHA,
				_texW,
				_texH,
				0,
				GL_ALPHA,
				GL_UNSIGNED_BYTE,
				_textureDatas.data()
				);
			return true;			
		}

		bool isLoaded()
		{
			return _textureId != 0;
		}

		template <class Archive>
		void serialize(Archive &ar)
		{
			ar(_size, _glyphSize, _textureDatas, _texW, _texH, _map);
		}

	};
	std::string _name;
	bool _isLoaded;
	std::map<std::size_t, FontSize> _sizes;
	friend class FontManager;
};