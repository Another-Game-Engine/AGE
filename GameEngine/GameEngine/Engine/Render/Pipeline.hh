#pragma once

#include <stdint.h>
#include <Utils/Containers/Vector.hpp>
#include <Render/Render.hh>

namespace AGE { class Drawable; }

namespace gl
{

	enum DrawType
	{
		NON = 0,
		GLOBAL,
		SEPARATE
	};

	class Render;

	class Pipeline
	{
	public:
		Pipeline(uint8_t priority);
		~Pipeline();

		Pipeline &update(AGE::Vector<AGE::Drawable> const &geo);
		Pipeline &config(DrawType drawType);
		Pipeline &pushRender(Render *render);
		Pipeline &pushRenderPass(RenderPass *renderPass);
		bool operator<(Pipeline const &p);
		bool operator>(Pipeline const &p);
		bool operator<=(Pipeline const &p);
		bool operator>=(Pipeline const &p);
		bool operator==(Pipeline const &p);
		bool operator!=(Pipeline const &p);
	private:
		Pipeline(Pipeline const &copy) = delete;
		Pipeline &operator=(Pipeline const &p) = delete;

		uint8_t _priority;
		DrawType _type;
		AGE::Vector<AGE::Drawable> const *_toRender;
		AGE::Vector<Render *> _render;
		AGE::Vector<RenderPass *> _renderPass;
	};
}