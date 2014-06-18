#ifndef STREAMGPU_H_
# define STREAMGPU_H_

#include <array>
#include <stdint.h>
#include <OpenGL/PixelBuffer.hh>

# define PINGPONG_SIZE 2

namespace OpenGLTools
{
	class Texture;

	class StreamGPU
	{
	public:
		StreamGPU();
		~StreamGPU();
		StreamGPU(StreamGPU &&move);
		StreamGPU &operator=(StreamGPU &&move);
	
		StreamGPU &upload(void const *data, Texture const *texture, uint32_t size);
		void *beginReadBack(Texture const *texture, uint32_t size);
		StreamGPU &endReadBack(void *data);
	private:
		std::array<PixelBufferPack, PINGPONG_SIZE> _pboPack;
		std::array<PixelBufferUnPack, PINGPONG_SIZE> _pboUnPack;
		uint8_t _currentPBOPack;
		uint8_t _currentPBOUnPack;

	};
}

#endif /*!STREAM_GPU_*/