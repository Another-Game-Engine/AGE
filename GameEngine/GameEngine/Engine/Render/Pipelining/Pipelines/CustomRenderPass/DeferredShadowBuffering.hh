#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>
#include <list>
#include <glm/glm.hpp>

namespace AGE
{
	class Texture2D;
	class TextureBuffer;

	class DeferredShadowBuffering : public FrameBufferRender
	{
	public:
		DeferredShadowBuffering(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager);
		virtual ~DeferredShadowBuffering() = default;
		virtual void init();
	protected:
		virtual void renderPass(const DRBCameraDrawableList &infos);

	private:
		std::vector<uint32_t> _depthPixels;
		std::list<std::shared_ptr<Texture2D>> _depthBuffers;

		std::shared_ptr<AGE::TextureBuffer> _positionBuffer = nullptr;
		static const std::size_t _maxMatrixInstancied = 1024;
		static const std::size_t _sizeofMatrix = sizeof(glm::mat4);
		static const std::size_t _maxInstanciedShadowCaster = _maxMatrixInstancied;
	};
}
