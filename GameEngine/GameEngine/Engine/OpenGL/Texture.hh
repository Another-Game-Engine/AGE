#pragma once

# include <Utils/OpenGL.hh>
# include <OpenGL/PixelBuffer.hh>

namespace gl
{
	//!\file Texture.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class Texture
	//!\brief Handle generic Texture
	class Texture
	{
	public:
		virtual ~Texture();

		virtual void *read(void *data) const = 0;
		virtual void write(void *data) const = 0;
		virtual Texture const &bind() const = 0;
		virtual Texture const &unbind() const = 0;
		virtual GLenum getType() const = 0;

		GLuint getId() const;

		// avoid copylian form
		Texture(Texture const &copy) = delete;
		Texture(Texture &&copy) = delete;
		Texture &operator=(Texture const &t) = delete;
		Texture &operator=(Texture &&t) = delete;
	protected:
		Texture();

		GLuint _id;
	};

	//!\file Texture.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class Texture2D
	//!\brief Handle Texture 2D
	class Texture2D : public Texture
	{
	private:
		GLsizei _levels;
		GLint _level;
		GLsizei _width;
		GLsizei _height;
		GLenum _internalFormat;
		GLenum _format;
		GLenum _type;

		// avoid operator =
		Texture2D &operator=(Texture2D const &t) = delete;
		Texture2D &operator=(Texture2D &&t) = delete;
	public:
		Texture2D(GLenum internalFormat, GLsizei width, GLsizei height, bool mipmapping = true);
		virtual ~Texture2D();
		Texture2D(Texture2D const &copy);
		Texture2D(Texture2D &&move);

		Texture2D const &wrap(GLint param) const;
		Texture2D const &filter(GLint param) const;
		Texture2D const &filter(GLint minFilter, GLint magFilter) const;
		Texture2D const &storage(GLint param) const;
		Texture2D const &storage(GLint pack, GLint unpack) const;
		Texture2D &setOptionTransfer(GLint level = 0, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);
		Texture2D const &generateMipMap() const;
		std::uint8_t getMaxLevelMipMap() const;
		
		virtual void write(void *write) const;
		virtual void *read(void *read) const;
		virtual Texture const &bind() const;
		virtual Texture const &unbind() const;
		virtual GLenum getType() const;

	};

	//!\file Texture.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class TextureMultiSample
	//!\brief Handle Texture 2D multi sample
	class TextureMultiSample : public Texture
	{
	private:
		GLsizei _samples;
		GLsizei _width;
		GLsizei _height;
		GLenum _internalFormat;
		GLenum _format;
		GLenum _type;
		GLboolean _fixedSampleLocation;
	public:
		TextureMultiSample(GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocation = GL_FALSE);
		virtual ~TextureMultiSample();
		TextureMultiSample(TextureMultiSample &&move);
		TextureMultiSample &operator=(TextureMultiSample &&t);

		virtual void write(void *write) const;
		virtual void *read(void *read) const;
		virtual Texture const &bind() const;
		virtual Texture const &unbind() const;
		virtual GLenum getType() const;
	};
}