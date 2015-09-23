#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>
#include <glm\glm.hpp>

namespace AGE
{
	class Texture2D;
	class Program;

	class GaussianBlur : public FrameBufferRender
	{
	public:
		GaussianBlur(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
					std::shared_ptr<Texture2D> src, std::shared_ptr<Texture2D> dst, bool horizontal);
		virtual ~GaussianBlur() = default;

	protected:
		virtual void renderPass(const DRBCameraDrawableList &infos);

		glm::vec2 _inverseSourceSize;
		std::shared_ptr<Texture2D> _source;

		Key<Vertices> _quadVertices;
		std::shared_ptr<Painter> _quadPainter;
	};
}

