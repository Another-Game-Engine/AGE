#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>
#include <list>
#include <glm/glm.hpp>

#include <concurrentqueue/concurrentqueue.h>
#include <Utils/Containers/LFQueue.hpp>
#include <Render\Pipelining\Prepare\MeshBufferingPrepare.hpp>

namespace AGE
{
	class Texture2D;
	class TextureBuffer;
	class IRenderingPipeline;

	class DeferredShadowBuffering : public FrameBufferRender
	{
	public:
		DeferredShadowBuffering(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager, IRenderingPipeline *pipeline);
		virtual ~DeferredShadowBuffering() = default;
		virtual void init();
	protected:
		virtual void renderPass(const DRBCameraDrawableList &infos);

	private:
		std::shared_ptr<AGE::TextureBuffer> _positionBuffer = nullptr;
		static const std::size_t _maxMatrixInstancied = 4096;
		static const std::size_t _sizeofMatrix = sizeof(glm::mat4);
		static const std::size_t _maxInstanciedShadowCaster = _maxMatrixInstancied;
		IRenderingPipeline *_pipeline;
	};
}
