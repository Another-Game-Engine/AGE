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
	
		virtual Texture &init() = 0;
		virtual void unload() const = 0;
		virtual Texture const &bind() const = 0;
		virtual Texture const &unbind() const = 0;
		virtual Texture &write(void *write) = 0;
		virtual Texture &writeFlush(void *write) = 0;
		virtual Texture &startRead(void **data) = 0;
		virtual Texture &stopRead(void **data) = 0;
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
		Texture2D const &filterMag(GLenum param) const;
		Texture2D const &filterMin(GLenum param) const;
		Texture2D const &storage(GLint param) const;
		Texture2D const &storagePack(GLint param) const;
		Texture2D const &storageUnPack(GLint param) const;
		Texture2D &setOptionReadWrite(GLint level = 0, GLenum format = GL_RGBA, GLenum type = GL_UNSIGNED_BYTE);
		Texture2D const &generateMipMap() const;

		virtual Texture &init();
		virtual void unload() const;
		virtual Texture const &bind() const;
		virtual Texture const &unbind() const;
		virtual Texture &write(void *write);
		virtual Texture &writeFlush(void *write);
		virtual Texture &startRead(void **read);
		virtual Texture &stopRead(void **read);
	};
}

#endif /*!TEXTURE_HH_*/