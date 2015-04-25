#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>
#include <list>
#include <glm/glm.hpp>

namespace AGE
{
	class Texture2D;

	class DeferredShadowBuffering : public FrameBufferRender
	{
	public:
		DeferredShadowBuffering(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager);
		virtual ~DeferredShadowBuffering() = default;

	protected:
		virtual void renderPass(RenderPipeline const &, RenderLightList &, CameraInfos const &);

	private:
		std::vector<uint32_t> _depthPixels;
		std::list<std::shared_ptr<Texture2D>> _depthBuffers;
	};
}
