#pragma once

#include <array>
#include <stdint.h>
#include <Render/PixelBuffer.hh>

# define PINGPONG_SIZE 2

namespace gl
{
	class Texture;

	//!\file StreamGPU.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class StreamGPU
	//!\brief Handle Stream CPU memory through GPU memory and reverse
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
