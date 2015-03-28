#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>

namespace AGE
{
	class Texture2D;

	class DeferredDebugBuffering : public FrameBufferRender
	{
	public:
		DeferredDebugBuffering(std::shared_ptr<PaintingManager> painterManager,
			std::shared_ptr<Texture2D> debugLights,
			std::shared_ptr<Texture2D> depth);
		virtual ~DeferredDebugBuffering() = default;

	protected:
		virtual void renderPass(RenderPipeline const &, RenderLightList const &, CameraInfos const &);
	
	private:
		Key<Vertices> _quadVertices;
		std::shared_ptr<Painter> _quadPainter;
	};
}
