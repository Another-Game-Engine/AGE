#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>
#include <glm\glm.hpp>

namespace AGE
{
	class Texture2D;
	class Program;

	class DepthOfField : public FrameBufferRender
	{
	public:
		DepthOfField(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
			std::shared_ptr<Texture2D> depth,
			std::shared_ptr<Texture2D> blurred,
			std::shared_ptr<Texture2D> clean,
			std::shared_ptr<Texture2D> dst);
		virtual ~DepthOfField() = default;

	protected:
		virtual void renderPass(const DRBCameraDrawableList &infos);

		std::shared_ptr<Texture2D> _depth;
		std::shared_ptr<Texture2D> _blurred;
		std::shared_ptr<Texture2D> _clean;

		Key<Vertices> _quadVertices;
		std::shared_ptr<Painter> _quadPainter;
	};
}

