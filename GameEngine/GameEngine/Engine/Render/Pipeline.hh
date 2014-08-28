#pragma once

#include <stdint.h>
#include <Utils/Containers/Vector.hpp>
#include <Render/Render.hh>

namespace AGE { class Drawable; }

namespace gl
{
	class Render;
	class Pipeline
	{
	public:
		Pipeline();
		~Pipeline();

		Pipeline &setToRender(AGE::Vector<AGE::Drawable> const &geo);
		Pipeline &setRendering(uint8_t time, Render *rendering);
		uint8_t getMinTime() const;
		uint8_t getMaxTime() const;
		Pipeline &draw(uint8_t time);

	private:
		Pipeline(Pipeline const &copy) = delete;
		Pipeline &operator=(Pipeline const &p) = delete;

		uint8_t _min;
		uint8_t _max;
		uint8_t *_times;
		Render **_rendering;
		uint8_t _nbrRendering;
		AGE::Vector<AGE::Drawable> const *_toRender;
	};
}