#pragma once

# include <Utils/OpenGL.hh>
# include <stdint.h>
# include <glm/glm.hpp>

namespace gl
{
	class Framebuffer;

	class Storage
	{
	public:
		~Storage();

		virtual GLuint getId() const;

		virtual GLenum getType() const = 0;
		virtual Storage const &attachement(Framebuffer const &fbo, GLenum attach) const = 0;
	protected:
		Storage(size_t width, size_t height, GLenum internalFormat);

		GLuint _id;
		size_t _width;
		size_t _height;
		GLenum _internalFormat;
	};

	class Texture : public Storage
	{
	public:
		virtual ~Texture();

		virtual Texture const &bind() const = 0;
		virtual Texture const &unbind() const = 0;
		virtual Texture const &generateMipMap() const = 0;
		virtual Texture const &download(GLenum format, GLenum type, GLvoid *img) const = 0;
		virtual Texture const &upload(GLenum format, GLenum type, GLvoid *data) const = 0;
		virtual Texture const &parameter(GLenum pname, GLint param) const = 0;
		
		Texture &setLevelTarget(uint8_t target);
		uint8_t getMaxLevelMipMap() const;

	protected:
		Texture(size_t width, size_t height, GLenum internalFormat, bool mipMapping);

		uint8_t _mipMapLevels;
		uint8_t _levelTarget;
	};

	class Texture2D : public Texture
	{
	public:
		Texture2D(size_t width, size_t height, GLenum internalFormat, bool mipmapping = true);
		virtual ~Texture2D();

		virtual GLenum getType() const;
		virtual Texture const &bind() const;
		virtual Texture const &unbind() const;
		virtual Texture const &download(GLenum format, GLenum type, GLvoid *img) const;
		virtual Texture const &upload(GLenum format, GLenum type, GLvoid *data) const;
		virtual Texture const &parameter(GLenum pname, GLint param) const;
		virtual Texture const &generateMipMap() const;
		virtual Storage const &attachement(Framebuffer const &fbo, GLenum attach) const;

		Texture2D &configUpload(glm::ivec4 const &rect);

	private:
		glm::ivec4 _rect;
	};

	class RenderBuffer : public Storage
	{
	public:
		RenderBuffer(size_t width, size_t height, GLenum internalFormat);
		virtual ~RenderBuffer();

	private:
		virtual GLenum getType() const;
		virtual Storage const &attachement(Framebuffer const &fbo, GLenum attach) const;

	};
}