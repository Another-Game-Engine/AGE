#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>

#include <glm/glm.hpp>

namespace AGE
{
	class Texture2D;
	class Program;

	class DeferredMergingDebug : public FrameBufferRender
	{
	public:
		DeferredMergingDebug(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
			std::shared_ptr<Texture2D> debugLightRender,
			std::shared_ptr<Texture2D> diffuse, std::shared_ptr<Texture2D> depthStencil);
		virtual ~DeferredMergingDebug() = default;

	protected:
		virtual void renderPass(const DRBCameraDrawableList &infos);

	private:
		std::shared_ptr<Texture2D> _debugLightRender;
		Key<Vertices> _quadVertices;
		std::shared_ptr<Painter> _quadPainter;
		std::shared_ptr<Painter> _lines;
	};
}