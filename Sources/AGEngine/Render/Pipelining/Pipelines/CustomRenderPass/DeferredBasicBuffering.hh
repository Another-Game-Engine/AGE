#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>
#include <glm\glm.hpp>

#include <Utils/Containers/LFQueue.hpp>

#include <Render/Pipelining/Prepare/MeshBufferingPrepare.hpp>

namespace AGE
{
	class Texture2D;

	class BFCBlockManagerFactory;
	class Frustum;
	class TextureBuffer;
	class Texture2D;

	class DeferredBasicBuffering : public FrameBufferRender
	{
	public:
		// hack for test
		static DeferredBasicBuffering *instance;
		virtual void init();

		DeferredBasicBuffering(glm::uvec2 const &screenSize, 
								std::shared_ptr<PaintingManager> painterManager,
								std::shared_ptr<Texture2D> diffuse,
								std::shared_ptr<Texture2D> normal,
								std::shared_ptr<Texture2D> specular,
								std::shared_ptr<Texture2D> depth);
		virtual ~DeferredBasicBuffering() = default;

		LFQueue<MeshBuffering::CullingOutput*>* getMeshResultQueue();
		typedef MeshBuffering::CullingOutput MeshOutput;
	protected:
		virtual void renderPass(const DRBCameraDrawableList &infos);
		std::shared_ptr<Texture2D> _depth;
		std::shared_ptr<Texture2D> _occlusionDepth;

		std::vector<uint32_t> _depthPixels;

		std::shared_ptr<AGE::TextureBuffer> _positionBuffer = nullptr;
		static const std::size_t _maxMatrixInstancied = 16384;
		static const std::size_t _sizeofMatrix = sizeof(glm::mat4);
		static const std::size_t _maxInstanciedShadowCaster = _maxMatrixInstancied;

		LFQueue<MeshBuffering::CullingOutput*>          _cullingResults;
	};
}
