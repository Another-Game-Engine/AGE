#ifndef  __TEXTURE_FILE_HPP__
# define __TEXTURE_FILE_HPP__

#include <MediaFiles/MediaFile.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <Utils/GlmSerialization.hpp>
#include <OpenGL/include/SDL/SDL_opengl.h>

struct TextureFile : public MediaFile<TextureFile>
{
	TextureFile()
		: MediaFile<TextureFile>()
		, width(0)
		, height(0)
		, components(0)
		, format(0)
		, wrap(GL_REPEAT)
		, minFilter(GL_LINEAR_MIPMAP_LINEAR)
		, magFilter(GL_LINEAR)
	{
		_type = TEXTURE;
	}

	virtual ~TextureFile()
	{
		glDeleteTextures(1, &id);
	}

	template <typename Archive>
	void save(Archive &ar) const
	{
		ar(cereal::make_nvp("datas", datas), CEREAL_NVP(width), CEREAL_NVP(height), CEREAL_NVP(components), CEREAL_NVP(format));
	}

	template <typename Archive>
	void load(Archive &ar)
	{
		ar(cereal::make_nvp("datas", datas), CEREAL_NVP(width), CEREAL_NVP(height), CEREAL_NVP(components), CEREAL_NVP(format));
		glGenTextures(1, &id);
		assert(id != 0 && "Error generating texture.");
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, components, width, height, 0, format, GL_UNSIGNED_BYTE, datas.data());

		if (minFilter == GL_LINEAR_MIPMAP_LINEAR ||
			minFilter == GL_LINEAR_MIPMAP_NEAREST ||
			minFilter == GL_NEAREST_MIPMAP_LINEAR ||
			minFilter == GL_NEAREST_MIPMAP_NEAREST)
			glGenerateMipmap(GL_TEXTURE_2D);
	}

	inline const GLuint			getId() const
	{
		return id;
	}

	std::vector<GLbyte> datas;
	GLint		width, height;
	GLint		components;
	GLenum		format;
	GLuint      id;
	GLenum		wrap;
	GLenum		minFilter;
	GLenum		magFilter;
};

#endif   //__TEXTURE_FILE_HPP__