#pragma once

# include <Render/Pipelining/Render/IRender.hh>
# include <functional>
#include <vector>

namespace AGE
{

	class ARender : public IRender
	{
	public:
		virtual ~ARender() {}

	public:
		void setRenderFunction(std::function<void(RenderPipeline const &, RenderLightList const &, CameraInfos const &)> const &function);
		bool recompilePrograms();

	protected:
		ARender(std::shared_ptr<PaintingManager> painterManager);
		ARender(ARender &&move);

	protected:
		std::shared_ptr<PaintingManager> _painterManager;
		std::vector<std::shared_ptr<Program>> _programs;
		std::function<void(RenderPipeline const &, RenderLightList const &, CameraInfos const &)> _render_function;
	};

}