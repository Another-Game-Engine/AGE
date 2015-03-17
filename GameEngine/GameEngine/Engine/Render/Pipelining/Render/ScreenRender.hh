#pragma once

# include <Render/Pipelining/Render/ARender.hh>

namespace AGE
{

	class ScreenRender : public ARender
	{
	public:
		ScreenRender(std::shared_ptr<PaintingManager> painterManager);
		ScreenRender(ScreenRender &&move);
		virtual ~ScreenRender() {}

	public:
		virtual IRender &render(RenderPipeline const &pipeline, RenderLightList const &lights, CameraInfos const &infos) override final;
	};
}