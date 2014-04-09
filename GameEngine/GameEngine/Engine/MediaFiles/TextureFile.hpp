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
#include <Utils/OpenGL.hh>

struct TextureFile : public MediaFile<TextureFile>
{
	TextureFile();
	virtual ~TextureFile();
	TextureFile(const TextureFile &o);
	TextureFile &operator=(const TextureFile &o);

	void save(cereal::PortableBinaryOutputArchive &ar) const;
	void load(cereal::PortableBinaryInputArchive &ar);

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
