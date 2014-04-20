#ifndef PIXELBUFFER_HH_
# define PIXELBUFFER_HH_

# include <cstdint>
# include <Utils/OpenGL.hh>

# define NBR_BUFFER 2

namespace OpenGLTools
{
	class PixelBuffer
	{
	public:
		virtual ~PixelBuffer();
		PixelBuffer &operator=(PixelBuffer const &p);
		PixelBuffer &operator=(PixelBuffer &&p);

		virtual inline PixelBuffer &bind() = 0;
		virtual inline PixelBuffer &unbind() = 0;

	protected:
		PixelBuffer();
		PixelBuffer(PixelBuffer const &copy);
		PixelBuffer(PixelBuffer &&move);

		GLuint _id[NBR_BUFFER];
		std::uint32_t _current;
	};

	class PixelBufferPack : public PixelBuffer
	{
	public:
		PixelBufferPack();
		virtual ~PixelBufferPack();
		PixelBufferPack(PixelBufferPack const &copy);
		PixelBufferPack(PixelBufferPack &&move);

		virtual inline PixelBuffer &bind();
		virtual inline PixelBuffer &unbind();
		
		inline PixelBuffer &mapBuffer(void **data);
		inline PixelBuffer &unmapBuffer(void **data);
	};

	class PixelBufferUnPack : public PixelBuffer
	{
	private:
		std::uint32_t _size;

	public:
		PixelBufferUnPack();
		virtual ~PixelBufferUnPack();
		PixelBufferUnPack(PixelBufferUnPack const &copy);
		PixelBufferUnPack(PixelBufferUnPack &&move);

		virtual inline PixelBuffer &bind();
		virtual inline PixelBuffer &unbind();
		
		inline PixelBuffer &setBuffer(void *data, std::uint32_t size_data = 0);
	};
}

#endif /*!PIXELBUFFER_HH_*/