#pragma once

# include <Utils/OpenGL.hh>
# include <stdint.h>
# include <glm/glm.hpp>

namespace gl
{
	class Storage
	{
	public:
		~Storage();

		virtual GLenum getType() const = 0;
		virtual GLuint getId() const = 0;
	protected:
		Storage(GLsizei width, GLsizei height, GLenum internalFormat);

		GLuint _id;
		GLsizei _width;
		GLsizei _height;
		GLenum _internalFormat;
	};

	class Texture : public Storage
	{
	public:
		virtual ~Texture();

		virtual Texture const &bind() const = 0;
		virtual Texture const &unbind() const = 0;
		virtual Texture const &generateMipMap() const = 0;
		virtual Texture const &download(GLint level, GLenum format, GLenum type, GLvoid *img) const = 0;
		virtual Texture const &upload(GLint level, GLenum format, GLenum type, GLvoid *data) const = 0;
		virtual Texture const &parameter(GLenum pname, GLint param) const = 0;

		uint8_t getMaxLevelMipMap() const;

	protected:
		Texture(GLsizei width, GLsizei height, GLenum internalFormat, bool mipMapping);

		uint8_t _mipMapLevels;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(GLsizei width, GLsizei height, GLenum internalFormat, bool mipmapping = true);
		virtual ~Texture2D();

		virtual GLenum getType() const;
		virtual GLuint getId() const;
		virtual Texture const &bind() const;
		virtual Texture const &unbind() const;
		virtual Texture const &download(GLint level, GLenum format, GLenum type, GLvoid *img) const;
		virtual Texture const &upload(GLint level, GLenum format, GLenum type, GLvoid *data) const;
		virtual Texture const &parameter(GLenum pname, GLint param) const;
		virtual Texture const &generateMipMap() const;

		Texture2D &configUpload(glm::ivec4 const &rect);

	private:
		glm::vec4 _rect;
	};
}