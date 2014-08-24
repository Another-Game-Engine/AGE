#pragma once

#include <iostream>
#include <stdint.h>

namespace gl
{
	class Render;
	class Pipeline
	{
	public:
		Pipeline();
		~Pipeline();

		Pipeline &set(uint8_t time, Render *rendering);
		Pipeline &draw(uint8_t time);

	private:
		Pipeline(Pipeline const &copy) = delete;
		Pipeline &operator=(Pipeline const &p) = delete;

		uint8_t _min;
		uint8_t _max;
		uint8_t *_times;
		Render **_rendering;
		uint8_t _nbrRendering;
	};
}