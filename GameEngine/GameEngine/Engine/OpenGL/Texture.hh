#ifndef TEXTURE_HH_
# define TEXTURE_HH_

# include <Utils/OpenGL.hh>

namespace OpenGLTools
{
	class Texture
	{
	public:
		~Texture();
	protected:
		Texture();
		Texture(Texture const &copy);
		Texture(Texture &&copy);
		Texture &operator=(Texture const &t);
		Texture &operator=(Texture &&t);

		virtual inline void bind() const = 0;
		virtual inline void unbind() const = 0;
		virtual inline void write() const = 0;

		GLuint getId() const;

		GLuint _id;

	};

	class Texture2D : public Texture
	{
	private:
		GLsizei _levels;
		GLsizei _width;
		GLsizei _height;
		GLenum _internalFormat;
	public:
		Texture2D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height);
		~Texture2D();
		Texture2D(Texture2D const &copy);
		Texture2D(Texture2D &&move);
		Texture2D &operator=(Texture2D const &t);
		Texture2D &operator=(Texture2D &&t);

		inline void wrap(GLint param) const;
		inline void filter(GLint param) const;
		virtual inline void bind() const;
		virtual inline void unbind() const;
	};
}

#endif /*!TEXTURE_HH_*/