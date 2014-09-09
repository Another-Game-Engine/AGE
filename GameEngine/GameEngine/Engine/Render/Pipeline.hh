#pragma once

#include <stdint.h>
#include <Utils/Containers/Vector.hpp>
#include <Render/Render.hh>

namespace AGE { class Drawable; }

namespace gl
{

	enum DrawType
	{
		GLOBAL = 0,
		SEPARATE
	};

	class Render;

	class Pipeline
	{
	public:
		DrawType type;
		AGE::Vector<AGE::Drawable> const *toRender;

	public:
		Pipeline();
		~Pipeline();

		Pipeline &setDraw(AGE::Vector<AGE::Drawable> const &geo);
		Pipeline &setDraw(DrawType drawType);
		Pipeline &addRendering(uint8_t time, Render *rendering);
		uint8_t getMinTime() const;
		uint8_t getMaxTime() const;
		Pipeline &draw();
		Pipeline &draw(uint8_t time);
		Pipeline &draw(uint8_t time, size_t index);
	private:
		Pipeline(Pipeline const &copy) = delete;
		Pipeline &operator=(Pipeline const &p) = delete;
		Pipeline &draw(uint8_t time, size_t start, size_t end);

		uint8_t _min;
		uint8_t _max;

		std::map<size_t, Render *> _rendering;
	};
}