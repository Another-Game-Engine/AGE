#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>

namespace AGE
{
	class Texture2D;

	class DeferredBasicBuffering : public FrameBufferRender
	{
	public:
		DeferredBasicBuffering(std::shared_ptr<PaintingManager> painterManager,
								std::shared_ptr<Texture2D> diffuse,
								std::shared_ptr<Texture2D> normal,
								std::shared_ptr<Texture2D> specular,
								std::shared_ptr<Texture2D> depth,
								std::shared_ptr<Texture2D> occlusionDepth);
		virtual ~DeferredBasicBuffering() = default;

	protected:
		virtual void renderPass(RenderPipeline const &, RenderLightList const &, CameraInfos const &);
		std::shared_ptr<Texture2D> _depth;
		std::shared_ptr<Texture2D> _occlusionDepth;

		std::vector<uint32_t> _depthPixels;
	};
}
