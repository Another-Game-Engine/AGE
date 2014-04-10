#ifndef PIXELBUFFER_HH_
# define PIXELBUFFER_HH_

# include "Utils/OpenGL.hh"
# include <utility>
# include <glm/glm.hpp>

namespace OpenGLTools
{
	class PixelBuffer
	{
	public:
		PixelBuffer();
		~PixelBuffer();
		PixelBuffer(PixelBuffer const &copy);
		PixelBuffer &operator=(PixelBuffer const &other);

		void *getBuffer(glm::vec4 &&posDimension, GLenum format, GLenum component, GLenum buffer);
		void set(void *data);
	private:
		std::pair<GLuint, bool> _buffer1;
		std::pair<GLuint, bool> _buffer2;
	};
}

#endif /*!PIXELBUFFER_HH_*/