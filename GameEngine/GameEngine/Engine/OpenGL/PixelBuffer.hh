#ifndef PIXELBUFFER_HH_
# define PIXELBUFFER_HH_

# include <cstdint>
# include <Utils/OpenGL.hh>

namespace OpenGLTools
{
	class PixelBuffer
	{
	public:
		virtual ~PixelBuffer();
		PixelBuffer &operator=(PixelBuffer &&p);

		virtual PixelBuffer &bind() = 0;
		virtual PixelBuffer &unbind() = 0;
		GLint getId() const;

	protected:
		PixelBuffer();
		PixelBuffer(PixelBuffer &&move);

		GLuint _id;
	};

	class PixelBufferPack : public PixelBuffer
	{
	public:
		PixelBufferPack();
		virtual ~PixelBufferPack();
		PixelBufferPack(PixelBufferPack &&move);

		virtual PixelBuffer &bind();
		virtual PixelBuffer &unbind();
		
	};

	class PixelBufferUnPack : public PixelBuffer
	{
	public:
		PixelBufferUnPack();
		virtual ~PixelBufferUnPack();
		PixelBufferUnPack(PixelBufferUnPack &&move);

		virtual PixelBuffer &bind();
		virtual PixelBuffer &unbind();
	};
}

#endif /*!PIXELBUFFER_HH_*/