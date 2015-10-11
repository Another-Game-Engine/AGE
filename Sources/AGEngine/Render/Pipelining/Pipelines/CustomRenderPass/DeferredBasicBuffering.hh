#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>
#include <glm\glm.hpp>

#include <concurrentqueue/concurrentqueue.h>

namespace AGE
{
	class Texture2D;

	class BFCBlockManagerFactory;
	class Frustum;
	class MeshCasterResult;
	class MeshCasterBFCCallback;
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
		MeshCasterResult *prepareRender(glm::mat4, BFCBlockManagerFactory *bf, Frustum frustum, std::atomic_size_t *counter);
	protected:
		virtual void renderPass(const DRBCameraDrawableList &infos);
		std::shared_ptr<Texture2D> _depth;
		std::shared_ptr<Texture2D> _occlusionDepth;

		std::vector<uint32_t> _depthPixels;

		std::shared_ptr<AGE::TextureBuffer> _positionBuffer = nullptr;
		static const std::size_t _maxMatrixInstancied = 16384;
		static const std::size_t _sizeofMatrix = sizeof(glm::mat4);
		static const std::size_t _maxInstanciedShadowCaster = _maxMatrixInstancied;

		moodycamel::ConcurrentQueue<MeshCasterResult*>       _cullingResultsPool;
		moodycamel::ConcurrentQueue<MeshCasterBFCCallback*>    _cullerPool;
	};
}
