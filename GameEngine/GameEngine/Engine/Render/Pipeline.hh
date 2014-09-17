#pragma once

#include <stdint.h>
#include <Utils/Containers/Vector.hpp>
#include <Render/Render.hh>

namespace AGE { struct Drawable; }

namespace gl
{

	enum DrawType
	{
		ALL_OBJECT = 0,
		EACH_FOLLOWING_OBJECT,
		NONE_OBJECT,
		SIZE
	};

	class Render;

	class Pipeline
	{
	public:
		Pipeline();
		~Pipeline();
		Pipeline(Pipeline const &copy);

		Pipeline &update(AGE::Vector<AGE::Drawable> const &geo);
		Pipeline &config(DrawType drawType);
		Pipeline &pushRender(Render *render);
		Pipeline &pushRenderPass(RenderPass *renderPass);
		Pipeline &draw();
	private:
		void drawAllObject();
		void drawEachFollowObject();
		void drawNoneObject();

		Pipeline &operator=(Pipeline const &p) = delete;

		DrawType _type;
		AGE::Vector<AGE::Drawable> const *_toRender;
		AGE::Vector<Render *> _render;
		AGE::Vector<RenderPass *> _renderPass;
		void (Pipeline::*_drawFunc[DrawType::SIZE])();
	};
}