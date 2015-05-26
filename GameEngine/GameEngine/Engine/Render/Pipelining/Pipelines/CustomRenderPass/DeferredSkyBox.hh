#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>
#include <glm\glm.hpp>

namespace AGE
{
	class Texture2D;

	class DeferredSkyBox : public FrameBufferRender
	{
	public:
		DeferredSkyBox(glm::uvec2 const &screenSize,
			std::shared_ptr<PaintingManager> painterManager,
			std::shared_ptr<Texture2D> diffuse);
		virtual ~DeferredSkyBox() = default;

	protected:
		virtual void renderPass(RenderPipeline const &, RenderLightList &, CameraInfos const &);
	};
}
