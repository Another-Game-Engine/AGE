#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>
#include <glm\glm.hpp>

namespace AGE
{
	class Texture2D;

	class DebugLightBillboards : public FrameBufferRender
	{
	public:
		DebugLightBillboards(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
			std::shared_ptr<Texture2D> diffuse,
			std::shared_ptr<Texture2D> depth);
		virtual ~DebugLightBillboards() = default;

	protected:
		virtual void renderPass(const DRBCameraDrawableList &infos);
	
	private:
		Key<Vertices> _quadVertices;
		std::shared_ptr<Painter> _quadPainter;
	};
}
