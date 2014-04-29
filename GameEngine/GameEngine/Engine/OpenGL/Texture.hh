#ifndef TEXTURE_HH_
# define TEXTURE_HH_

# include <Utils/OpenGL.hh>
# include <OpenGL/PixelBuffer.hh>

namespace OpenGLTools
{
	class Texture
	{
	public:
		virtual ~Texture();

		virtual Texture const &bind() const = 0;
		virtual Texture const &unbind() const = 0;
		GLuint getId() const;

	protected:
		Texture();
		Texture(Texture const &copy);
		Texture(Texture &&copy);
		Texture &operator=(Texture const &t);
		Texture &operator=(Texture &&t);

		GLuint _id;
		PixelBufferPack _out;
		PixelBufferUnPack _in;

	};

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

	public:
		Texture2D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height);
		virtual ~Texture2D();
		Texture2D(Texture2D const &copy);
		Texture2D(Texture2D &&move);
		Texture2D &operator=(Texture2D const &t);
		Texture2D &operator=(Texture2D &&t);

		Texture2D const &wrap(GLint param) const;
		Texture2D const &filter(GLint param) const;
		Texture2D const &filter(GLint minFilter, GLint magFilter) const;
		Texture2D const &storage(GLint param) const;
		Texture2D const &storage(GLint pack, GLint unpack) const;
		Texture2D &setOptionTransfer(GLint level = 0, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);
		Texture2D const &generateMipMap() const;
		Texture2D const &write(void *write) const;
		void *read(void *read) const;

		virtual Texture const &bind() const;
		virtual Texture const &unbind() const;

	};

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
		TextureMultiSample(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocation = GL_FALSE);
		~TextureMultiSample();
		TextureMultiSample(TextureMultiSample &&move);
		TextureMultiSample &operator=(TextureMultiSample &&t);

		virtual Texture const &bind() const;
		virtual Texture const &unbind() const;
	};
}

#endif /*!TEXTURE_HH_*/