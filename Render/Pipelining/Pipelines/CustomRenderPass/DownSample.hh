#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>
#include <glm\glm.hpp>

namespace AGE
{
	class Texture2D;
	class Program;

	class DownSample : public FrameBufferRender
	{
	public:
		DownSample(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
					std::shared_ptr<Texture2D> src, std::shared_ptr<Texture2D> dst);
		virtual ~DownSample() = default;

	protected:
		virtual void renderPass(const DRBCameraDrawableList &infos);

		std::shared_ptr<Texture2D> _source;
		glm::vec2 _inverseSourceSize;

		Key<Vertices> _quadVertices;
		std::shared_ptr<Painter> _quadPainter;
	};
}

