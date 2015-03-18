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
		bool recompilePrograms();

	protected:
		ARender(std::shared_ptr<PaintingManager> painterManager);
		ARender(ARender &&move);
		virtual void renderPass(RenderPipeline const &, RenderLightList const &, CameraInfos const &) = 0;

	protected:
		std::shared_ptr<PaintingManager> _painterManager;
		std::vector<std::shared_ptr<Program>> _programs;
	};

}