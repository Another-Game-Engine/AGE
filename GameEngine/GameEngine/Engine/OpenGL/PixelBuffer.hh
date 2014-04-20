#ifndef PIXELBUFFER_HH_
# define PIXELBUFFER_HH_

# include <stdint.h>
# include <Utils/OpenGL.hh>

namespace OpenGLTools
{
	class PixelBuffer
	{
	public:
		PixelBuffer();
		~PixelBuffer();
		PixelBuffer(PixelBuffer const &copy);
		PixelBuffer(PixelBuffer &&move);
		PixelBuffer &operator=(PixelBuffer const &p);
		PixelBuffer &operator=(PixelBuffer &&p);
	private:
		GLuint _id[2];
		uint8_t current;
	};
}

#endif /*!PIXELBUFFER_HH_*/