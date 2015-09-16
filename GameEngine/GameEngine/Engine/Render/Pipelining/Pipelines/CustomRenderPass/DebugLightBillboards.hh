#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>
#include <glm\glm.hpp>

namespace AGE
{
	class Texture2D;
	class TextureBuffer;

	class DebugLightBillboards : public FrameBufferRender
	{
	public:
		DebugLightBillboards(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager,
			std::shared_ptr<Texture2D> diffuse,
			std::shared_ptr<Texture2D> depth);
		virtual ~DebugLightBillboards() = default;
		virtual void init();

	protected:
		virtual void renderPass(const DRBCameraDrawableList &infos);
	
	private:
		Key<Vertices> _quadVertices;
		std::shared_ptr<Painter> _quadPainter;

		std::shared_ptr<AGE::TextureBuffer> _positionBuffer = nullptr;
		static const std::size_t _maxMatrixInstancied = 256;
		static const std::size_t _sizeofMatrix = sizeof(glm::mat4);
		static const std::size_t _maxInstanciedBillboard = _maxMatrixInstancied;

	};
}
