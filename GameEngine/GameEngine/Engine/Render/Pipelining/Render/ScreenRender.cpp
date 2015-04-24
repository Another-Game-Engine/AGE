#include <Render/Pipelining/Render/ScreenRender.hh>

namespace AGE
{

	ScreenRender::ScreenRender(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager) :
		ARender(painterManager),
		viewport(screenSize)
	{

	}

	ScreenRender::ScreenRender(ScreenRender &&move) :
		ARender(std::move(move))
	{

	}

	IRender & ScreenRender::render(RenderPipeline const &pipeline, RenderLightList const &lights, CameraInfos const &infos)
	{
		glViewport(0, 0, viewport.x, viewport.y);
		renderPass(pipeline, lights, infos);
		if (_nextPass != nullptr)
			_nextPass->render(pipeline, lights, infos);
		return (*this);
	}

}