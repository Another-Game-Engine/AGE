#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>
#include <glm/glm.hpp>

namespace AGE
{
	class Texture2D;
	class Program;
	class IRenderingPipeline;

	class DeferredSpotLightning : public FrameBufferRender
	{
	public:
		DeferredSpotLightning(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
			std::shared_ptr<Texture2D> normal,
			std::shared_ptr<Texture2D> depth,
			std::shared_ptr<Texture2D> specular,
			std::shared_ptr<Texture2D> lightAccumulation,
			std::shared_ptr<Texture2D> shinyAccumulation,
			IRenderingPipeline *pipeline);
		virtual ~DeferredSpotLightning() = default;

	protected:
		virtual void renderPass(const DRBCameraDrawableList &infos);

	private:
		std::shared_ptr<Texture2D> _normalInput;
		std::shared_ptr<Texture2D> _depthInput;
		std::shared_ptr<Texture2D> _specularInput;
		Key<Vertices> _quad;
		Key<Painter> _quadPainter;
		IRenderingPipeline *_pipeline;
	};
}

