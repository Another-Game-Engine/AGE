#include <Render/Pipelining/Render/ScreenRender.hh>

namespace AGE
{

	ScreenRender::ScreenRender(std::shared_ptr<PaintingManager> painterManager) :
		ARender(painterManager)
	{

	}

	ScreenRender::ScreenRender(ScreenRender &&move) :
		ARender(std::move(move))
	{

	}

	IRender & ScreenRender::render(RenderPipeline const &pipeline, RenderLightList const &lights, CameraInfos const &infos)
	{
		renderPass(pipeline, lights, infos);
		if (_nextPass != nullptr)
			_nextPass->render(pipeline, lights, infos);
		return (*this);
	}

}