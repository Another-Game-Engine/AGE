#pragma once

#include <TMQ/message.hpp>

namespace AGE
{
	class Engine;
	struct DrawableCollection;
	class Painter;
	class Vertices;

	namespace Tasks
	{

		class Render
		{
		public:
			struct CreateRenderContext : TMQ::FutureData < bool >
			{
				CreateRenderContext(Engine *_engine);
				Engine *engine;
			};

			struct InitRenderPipelines: TMQ::FutureData < bool >
			{
				InitRenderPipelines(Engine *_engine);
				Engine *engine;
			};

			struct ReloadShaders
			{
				std::size_t notEmpty;
			};

			struct ContextGrabMouse
			{
				uint32_t grabMouse;

				ContextGrabMouse(bool _grabMouse) :
					grabMouse(_grabMouse == true ? 1 : 0)
				{ }
			};

		};
	
	}
}