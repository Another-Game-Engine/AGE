#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>
#include <glm\glm.hpp>

namespace AGE
{
	class Texture2D;

	class DeferredDebugBuffering : public FrameBufferRender
	{
	public:
		DeferredDebugBuffering(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
			std::shared_ptr<Texture2D> debugLights,
			std::shared_ptr<Texture2D> depth);
		virtual ~DeferredDebugBuffering() = default;

	protected:
		virtual void renderPass(std::list<BFCCullableObject*> const &meshs, RenderLightList &, CameraInfos const &);
	
	private:
		Key<Vertices> _quadVertices;
		std::shared_ptr<Painter> _quadPainter;
	};
}
