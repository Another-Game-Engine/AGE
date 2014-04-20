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
	
		virtual inline Texture const &bind() const = 0;
		virtual inline Texture const &unbind() const = 0;
		virtual inline Texture &write(void *write) = 0;
		virtual inline Texture &writeFlush(void *write) = 0;
		virtual inline Texture &startRead(void **data) = 0;
		virtual inline Texture &stopRead(void **data) = 0;

	protected:
		Texture();
		Texture(Texture const &copy);
		Texture(Texture &&copy);
		Texture &operator=(Texture const &t);
		Texture &operator=(Texture &&t);

		GLuint getId() const;

		GLuint _id;
		PixelBufferPack _out;
		PixelBufferUnPack _in;

	};

	class Texture2D : public Texture
	{
	private:
		GLsizei _levels;
		GLsizei _level;
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

		inline Texture2D const &wrap(GLint param) const;
		inline Texture2D const &filter(GLint param) const;
		inline Texture2D const &setOptionReadWrite(GLenum level = 0, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);

		virtual inline Texture const &bind() const;
		virtual inline Texture const &unbind() const;
		virtual inline Texture &write(void *write);
		virtual inline Texture &writeFlush(void *write);
		virtual inline Texture &startRead(void **read);
		virtual inline Texture &stopRead(void **read);
	};
}

#endif /*!TEXTURE_HH_*/