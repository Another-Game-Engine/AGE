#pragma once

#include <Render/Pipelining/Render/ScreenRender.hh>

#include <glm/glm.hpp>

namespace AGE
{
	class Texture2D;
	class Program;

	class DeferredMergingDebug : public ScreenRender
	{
	public:
		DeferredMergingDebug(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
			std::shared_ptr<Texture2D> debugLightRender);
		virtual ~DeferredMergingDebug() = default;

	protected:
		virtual void renderPass(RenderPipeline const &, RenderLightList &, CameraInfos const &);

	private:
		std::shared_ptr<Texture2D> _debugLightRender;
		Key<Vertices> _quadVertices;
		std::shared_ptr<Painter> _quadPainter;
		std::shared_ptr<Painter> _lines;
	};
}